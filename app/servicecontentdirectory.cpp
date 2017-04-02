#include "servicecontentdirectory.h"

ServiceContentDirectory::ServiceContentDirectory(Logger *log, QString host, int port, QObject *parent):
    QObject(parent),
    rootFolder(log, host, port)
{
    connect(this, SIGNAL(folderAdded(QString)), this, SLOT(folderAddedSlot(QString)));

    connect(this, SIGNAL(addFolderSignal(QString)), &rootFolder, SLOT(addFolderSlot(QString)));
    connect(&rootFolder, SIGNAL(folderAddedSignal(QString)), this, SIGNAL(folderAdded(QString)));
    connect(&rootFolder, SIGNAL(error_addFolder(QString)), this, SIGNAL(error_addFolder(QString)));

    connect(this, SIGNAL(addNetworkLinkSignal(QString)), &rootFolder, SLOT(addNetworkLink(QString)));
    connect(&rootFolder, SIGNAL(linkAdded(QString)), this, SIGNAL(linkAdded(QString)));
    connect(&rootFolder, SIGNAL(error_addNetworkLink(QString)), this, SIGNAL(error_addNetworkLink(QString)));

    connect(this, SIGNAL(reloadLibrarySignal(QStringList)), &rootFolder, SLOT(reloadLibrary(QStringList)));

    connect(this, SIGNAL(updateMediaFromId(int,QHash<QString,QVariant>)), &rootFolder, SLOT(updateLibraryFromId(int,QHash<QString,QVariant>)));

    connect(this, SIGNAL(getDLNAResourcesSignal(QObject*, QString,bool,int,int,QString)), &rootFolder, SLOT(requestDlnaResources(QObject*, QString,bool,int,int,QString)));
    connect(&rootFolder, SIGNAL(dlnaResources(QObject*,QList<DlnaResource*>)), this, SIGNAL(dlnaResources(QObject*,QList<DlnaResource*>)));

}

void ServiceContentDirectory::setNetworkAccessManager(QNetworkAccessManager *nam)
{
    rootFolder.setNetworkAccessManager(nam);
}

void ServiceContentDirectory::_addFolder(const QString &folder)
{
    if (QFileInfo(folder).isDir())
        emit addFolderSignal(folder);
    else
        emit error_addFolder(folder);   // error folder is not a directory
}

void ServiceContentDirectory::folderAddedSlot(QString folder)
{
    listFolderAdded << folder;
}

void ServiceContentDirectory::reloadLibrary()
{
    emit reloadLibrarySignal(listFolderAdded);
}

void ServiceContentDirectory::requestDLNAResourcesSignal(QString objectId, bool returnChildren, int start, int count, QString searchStr)
{
    emit getDLNAResourcesSignal(sender(), objectId, returnChildren, start, count, searchStr);
}

void ServiceContentDirectory::reply(HttpRequest *request)
{
    if (request->operation() == QNetworkAccessManager::PostOperation && request->url().toString() == "/upnp/control/content_directory")
    {
        SoapAction action(request->requestData());

        QString soapaction = request->header("SOAPACTION");

        if (!action.isValid() or soapaction != action.soapaction())
        {
            qCritical() << "invalid action" << soapaction << action.soapaction() << request->requestData().size();
            UpnpError error(UpnpError::INVALID_ACTION);
            request->replyError(error);
        }
        else
        {
            if (action.actionName() == "GetSystemUpdateID")
            {
                SoapActionResponse response(action.serviceType(), action.actionName());
                response.addArgument("Id", "1");
                request->replyAction(response);
            }
            else if (action.actionName() == "GetSortCapabilities")
            {
                SoapActionResponse response(action.serviceType(), action.actionName());
                response.addArgument("SortCaps", "");
                request->replyAction(response);
            }
            else if (action.actionName() == "GetSearchCapabilities")
            {
                SoapActionResponse response(action.serviceType(), action.actionName());
                response.addArgument("SearchCaps", "");
                request->replyAction(response);
            }
            else if (action.actionName() ==  "Browse")
            {
                QString objectID = action.argumentValue("ObjectID");
                if (!objectID.isEmpty())
                {
                    QString browseFlag = action.argumentValue("BrowseFlag");
                    QString startingIndex = action.argumentValue("StartingIndex");
                    QString requestedCount = action.argumentValue("RequestedCount");
                    QString filter = action.argumentValue("Filter");
                    QString sortCriteria = action.argumentValue("SortCriteria");

                    if (!browseFlag.isEmpty() && !startingIndex.isEmpty() && !requestedCount.isEmpty() && !filter.isEmpty())
                    {
                        qDebug() << "Browse" << objectID << startingIndex << requestedCount << browseFlag << filter << sortCriteria;

                        QObject context;
                        QList<DlnaResource*> l_dlna = rootFolder.getDLNAResources(objectID,
                                                                                  browseFlag == "BrowseDirectChildren",
                                                                                  startingIndex.toInt(),
                                                                                  requestedCount.toInt(),
                                                                                  QString(),
                                                                                  &context);

                        qDebug() << "returned DLNA resources" << l_dlna.size();

                        DidlLite didlDoc;

                        foreach (DlnaResource* resource, l_dlna)
                        {
                            didlDoc.addElement(resource->getXmlContentDirectory(&didlDoc, filter.split(",")));
                        }

                        qDebug() << didlDoc.toString();

                        SoapActionResponse response(action.serviceType(), action.actionName());
                        response.addArgument("Result", didlDoc.toString(-1));
                        response.addArgument("NumberReturned", QString("%1").arg(l_dlna.size()));

                        if (browseFlag == "BrowseMetaData")
                        {
                            response.addArgument("TotalMatches", QString("1"));
                        }
                        else
                        {
                            DlnaResource *parent = 0;
                            if (l_dlna.size() > 0)
                                parent = l_dlna.at(0)->getDlnaParent();

                            if (parent)
                            {
                                response.addArgument("TotalMatches", QString("%1").arg(parent->getChildrenSize()));
                            }
                            else
                            {
                                response.addArgument("TotalMatches", QString("%1").arg(l_dlna.size()));
                            }
                        }

                        request->replyAction(response);
                    }
                    else
                    {
                        qCritical() << "invalid argument in Browse" << objectID << startingIndex << requestedCount << browseFlag << filter << sortCriteria;

                        UpnpError error(UpnpError::INVALID_ARGS);
                        request->replyError(error);
                    }
                }
                else
                {
                    UpnpError error(UpnpError::INVALID_ARGS);
                    request->replyError(error);
                }

            }
//            else if (action.actionName() == "Search")
//            {

//            }
            else
            {
                qCritical() << "unknwon action" << action.actionName();
                UpnpError error(UpnpError::INVALID_ACTION);
                request->replyError(error);
            }
        }
    }
    else if (request->url().toString().startsWith("/get/"))
    {
        qDebug() << this << request->operationString() << request->url();

        QStringList l_path = request->url().toString().split("/");
        if (l_path.size() > 1)
        {
            QString objectID = l_path.at(l_path.size()-2);

            QObject context;
            QList<DlnaResource*> l_dlna = rootFolder.getDLNAResources(objectID, false, 0, 0, "", &context);

            if (l_dlna.size() == 1)
            {
                DlnaItem *dlna = qobject_cast<DlnaItem*>(l_dlna.at(0));

                if (dlna)
                {
                    if (request->url().fileName().startsWith("thumbnail0000"))
                    {
                        request->replyData(dlna->getByteAlbumArt(), "image/jpeg");
                    }
                    else
                    {
                        HttpRange *range = 0;
                        if (!request->header("RANGE").isEmpty())
                        {
                            range = new HttpRange(QString("RANGE: %1").arg(request->header("RANGE")));
                            if (range->isNull())
                            {
                                // invalid range, ignore it
                                delete range;
                                range = 0;
                                qCritical() << "Invalid range in request: " + request->header("RANGE");
                            }
                            else
                            {
                                // update range with size of dlna object
                                range->setSize(dlna->size());
                            }
                        }

                        qint64 timeSeekRangeStart = -1;
                        qint64 timeSeekRangeEnd = -1;
                        if (!request->header("TIMESEEKRANGE.DLNA.ORG").isEmpty())
                        {
                            readTimeSeekRange(QString("TIMESEEKRANGE.DLNA.ORG: %1").arg(request->header("TIMESEEKRANGE.DLNA.ORG")), &timeSeekRangeStart, &timeSeekRangeEnd);
                            qWarning() << "TIMESEEKRANGE.DLNA.ORG" << request->header("TIMESEEKRANGE.DLNA.ORG") << timeSeekRangeStart << timeSeekRangeEnd;
                        }

                        QStringList m_header;
                        m_header << QString("Content-Type: %1").arg(dlna->mimeType());

                        if (!request->header("GETCONTENTFEATURES.DLNA.ORG").isEmpty())
                            m_header << QString("contentFeatures.dlna.org: %1").arg(dlna->getDlnaContentFeatures());

                        if (!request->header("transferMode.dlna.org").isEmpty())
                            m_header << QString("transferMode.dlna.org: %1").arg(request->header("transferMode.dlna.org"));

                        if (!request->header("GETMEDIAINFO.SEC").isEmpty())
                            m_header << QString("MediaInfo.sec: SEC_Duration=%1;").arg(dlna->getLengthInMilliSeconds());

                        if (dlna->getdlnaOrgOpFlags().at(1) == '1')
                            m_header << QString("Accept-Ranges: bytes");

                        if (request->version() != "HTTP/1.0")
                            m_header << QString("Connection: keep-alive");

                        m_header << QString("SERVER: %1").arg(request->serverName());

                        m_header << QString("Content-Length: %1").arg(dlna->size());

                        if (range)
                            m_header << QString("Content-Range: bytes %1-%2/%3").arg(range->getStartByte()).arg(range->getEndByte()).arg(dlna->size());

                        if (timeSeekRangeStart >= 0 && dlna->getLengthInMilliSeconds() > 0)
                        {
                            QTime start_time(0, 0, 0);
                            start_time = start_time.addSecs(timeSeekRangeStart);

                            QTime end_time(0, 0, 0);
                            if (timeSeekRangeEnd != -1)
                            {
                                end_time = end_time.addSecs(timeSeekRangeEnd);
                            } else {
                                end_time = end_time.addMSecs(dlna->getLengthInMilliSeconds());
                            }

                            QTime length_time(0, 0, 0);
                            length_time = length_time.addMSecs(dlna->getLengthInMilliSeconds());

                            m_header << QString("TimeSeekRange.dlna.org: npt=%1-%2/%3").arg(start_time.toString("hh:mm:ss,z")).arg(end_time.toString("hh:mm:ss,z")).arg(length_time.toString("hh:mm:ss,z"));
                            m_header << QString("X-Seek-Range: npt=%1-%2/%3").arg(start_time.toString("hh:mm:ss,z")).arg(end_time.toString("hh:mm:ss,z")).arg(length_time.toString("hh:mm:ss,z"));
                            m_header << QString("X-AvailableSeekRange: 1 npt=%1-%2").arg(0).arg(dlna->getLengthInSeconds());
                        }

                        if (request->sendHeader(m_header))
                        {
                            if (request->operation() == QNetworkAccessManager::GetOperation)
                            {
                                request->logMessage(QString("%1 bytes to send in %2.").arg(dlna->size()).arg(QTime(0, 0).addMSecs(dlna->getLengthInMilliSeconds()).toString("hh:mm:ss.zzz")));

                                QString mediaFilename = dlna->getSystemName();
                                request->setRequestedResource(mediaFilename);
                                emit servingSignal(request->peerAddress().toString(), mediaFilename, 0);

                                // recover resume time
                                qint64 resume = dlna->getResumeTime();
                                if (resume>0) {
                                    timeSeekRangeStart = resume/1000;
                                    request->setClockSending(timeSeekRangeStart*1000);
                                }

                                Device *streamContent = dlna->getStream(range, timeSeekRangeStart, timeSeekRangeEnd);

                                QTcpSocket *socket = request->tcpSocket();

                                if (!streamContent or !socket)
                                {
                                    // No inputStream indicates that transcoding / remuxing probably crashed.
                                    QString message = QString("There is no inputstream to return for %1.").arg(dlna->getDisplayName());
                                    request->logMessage(message);
                                    request->setError(message);
                                    request->close();

                                    if (streamContent)
                                        streamContent->deleteLater();
                                }
                                else
                                {
                                    streamContent->setParent(socket);
                                    connect(socket, SIGNAL(disconnected()), streamContent, SLOT(deleteLater()));

                                    if (dlna->bitrate()>0)
                                        streamContent->setBitrate(dlna->bitrate());

                                    request->setMaxBufferSize(streamContent->maxBufferSize());

                                    connect(streamContent, SIGNAL(openedSignal()), request, SLOT(streamOpened()));
                                    connect(streamContent, SIGNAL(status(QString)), request, SLOT(streamingStatus(QString)));
                                    connect(streamContent, SIGNAL(LogMessage(QString)), request, SLOT(logMessage(QString)));
                                    connect(streamContent, SIGNAL(errorRaised(QString)), request, SLOT(streamError(QString)));
                                    connect(streamContent, SIGNAL(endReached()), request, SLOT(streamingCompleted()));
//                                    connect(request, SIGNAL(networkPaused()), streamContent, SLOT(networkPaused()));

                                    connect(request, SIGNAL(bytesSent(qint64,qint64)), streamContent, SLOT(bytesSent(qint64,qint64)));
                                    connect(streamContent, SIGNAL(sendDataToClientSignal(QByteArray)), request, SLOT(sendPartialData(QByteArray)));

                                    if (streamContent->open())
                                    {
                                        connect(request, SIGNAL(servingFinishedSignal(QString,QString,int)), this, SIGNAL(servingFinishedSignal(QString,QString,int)));

                                        request->logMessage(QString("Streaming started, %1 bytes to send.").arg(streamContent->size()));
                                        emit servingRendererSignal(request->peerAddress().toString(), dlna->getDisplayName());

                                        streamContent->startRequestData();
                                        streamContent->requestData();
                                    }
                                    else
                                    {
                                        request->logMessage(QString("Streaming failed."));
                                        request->setError("Streaming failed.");
                                        request->close();
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    request->setError(QString("invalid DLNA resource %1, %2 results found.").arg(objectID).arg(l_dlna.size()));
                    request->close();
                }
            }
            else
            {
                request->setError(QString("DLNA resource not found %1, %2 results found.").arg(objectID).arg(l_dlna.size()));
                request->close();
            }
        }
    }
    else
    {
        request->setError(QString("unknown request to reply : %1 %2").arg(request->operationString()).arg(request->url().toString()));
        request->close();
    }
}

void ServiceContentDirectory::readTimeSeekRange(const QString &data, qint64 *start, qint64 *end)
{
    QRegularExpression pattern("timeseekrange\\.dlna\\.org:\\s*npt\\s*=\\s*(\\d+)\\-?(\\d*)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = pattern.match(data);

    if (match.hasMatch())
    {
        if (!match.captured(1).isEmpty())
            *start = match.captured(1).toLong();

        if (!match.captured(2).isEmpty())
            *end = match.captured(2).toLong();
    }
    else
    {
        qCritical() << "invalid TimeSeekRange" << data;
    }
}
