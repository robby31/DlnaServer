#include "replydlnaitemcontent.h"

// call updateStatus function every second
const int ReplyDlnaItemContent::UPDATE_STATUS_PERIOD = 1000;

ReplyDlnaItemContent::ReplyDlnaItemContent(Logger *log, QThread *streamWorker, const bool &http10, const QString &method, const QString &argument, const QHash<QString, QString> &paramsHeader, const QString &content, HttpRange *range, const int &timeSeekRangeStart, const int &timeSeekRangeEnd, QString uuid, QString servername, QString host, int port, QObject *parent):
    Reply(log, http10, method, argument, paramsHeader, content, range, timeSeekRangeStart, timeSeekRangeEnd, uuid, servername, host, port, parent),
    streamWorker(streamWorker),
    m_closed(false),
    timerStatus(this),
    bytesToWrite(0),
    networkBytesSent(0),
    lastNetBytesSent(-1),
    clockSending(),
    clockUpdateStatus(),
    m_requestResource(false),
    requestFilename(),
    mediaFilename(),
    m_streamingCompleted(false),
    streamingWithErrors(false),
    m_maxBufferSize(1024*1024*10)   // 10 MBytes by default when bitrate is unknown
{
    timerStatus.setTimerType(Qt::PreciseTimer);
    timerStatus.setInterval(UPDATE_STATUS_PERIOD);
    connect(&timerStatus, SIGNAL(timeout()), this, SLOT(updateStatus()));

    clockUpdateStatus.invalidate();
}

void ReplyDlnaItemContent::streamOpened()
{
    // Start timer to update periodically the status on streaming
    timerStatus.start();
    clockUpdateStatus.start();
    emit logTextSignal(QString("%1: periodic timer started? %3, %2."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(UPDATE_STATUS_PERIOD).arg(timerStatus.isActive()));

    if (!clockSending.isValid())
        clockSending.start();   // start clock to measure time taken for streaming

    sendHeader();
}

void ReplyDlnaItemContent::_run()
{
    // prepare data and send answer
    logTrace("ANSWER: " + requestMethod() + " " + requestArgument());

    if ((requestMethod() == "GET" || requestMethod() == "HEAD") && requestArgument().startsWith("get/"))
    {
        // Request to retrieve a file

        /**
        * Skip the leading "get/" and extract the resource ID from the first path element
        * e.g. "get/0$1$5$3$4/Foo.mp4" -> "0$1$5$3$4"
        */
        QRegExp rxId("get/([\\d$]+)/(.+)");
        QString id;
        if (rxId.indexIn(requestArgument()) != -1) {
            id = rxId.capturedTexts().at(1);
            requestFilename = rxId.capturedTexts().at(2);
        }

        // Some clients escape the separators in their request: unescape them.
        id = id.replace("%24", "$");

        if (!getRequestTransferMode().isNull())
            setParamHeader("TransferMode.dlna.org", getRequestTransferMode());

        // Retrieve the DLNAresource itself.
        m_requestResource = true;
        emit getDLNAResourcesSignal(id, false, 0, 0, "");
    }
    else
    {
        requestFilename.clear();
        logError("Unkown answer for: " + requestMethod() + " " + requestArgument());
        emit replyStatusSignal("KO");
        close();
    }
}

void ReplyDlnaItemContent::updateStatus()
{
    if (clockUpdateStatus.isValid()) {
        int delta = clockUpdateStatus.restart() - UPDATE_STATUS_PERIOD;

        if (qAbs(delta) > UPDATE_STATUS_PERIOD/2)
        {
            QString msg = QString("%1: UPDATE STATUS delta %3 <%2>").arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(mediaFilename).arg(delta);
            logInfo(msg);
        }
    } else {
        clockUpdateStatus.start();
    }

    if (isLogLevel(DEBG))
        emit logTextSignal(QString("%1: UPDATE STATUS, bytes sent?%2 bytes to send?%3"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(networkBytesSent).arg(bytesToWrite));

    if (!mediaFilename.isNull()) {
        if (clockSending.isValid())
            emit servingSignal(mediaFilename, clockSending.elapsedFromBeginning());
        else
            emit servingSignal(mediaFilename, 0);

        if (clockSending.isValid() && m_maxBufferSize != 0) {
            // display the network buffer and network speed
            int networkSpeed = int((double(networkBytesSent)/1024.0)/(double(clockSending.elapsed())/1000.0));
            int bufferTime = 0;
            if (networkSpeed!=0)
                bufferTime = m_maxBufferSize/(networkSpeed*1024);
            emit networkStatusSignal(QString("Time: %5 Buffer: %1% (%4 KB - %3 seconds), Speed: %2 KB/s").arg(int(100.0*double(bytesToWrite)/double(m_maxBufferSize))).arg(networkSpeed).arg(bufferTime).arg(m_maxBufferSize/1024).arg(QTime(0, 0).addMSecs(clockSending.elapsedFromBeginning()).toString("hh:mm:ss")));
        }


        if (lastNetBytesSent!=-1 && lastNetBytesSent==networkBytesSent)
            clockSending.pause();
        else if (networkBytesSent > 0)
            clockSending.start();

        lastNetBytesSent = networkBytesSent;
    }
}

void ReplyDlnaItemContent::close()
{
    if (m_closed)
        return;

    m_closed = true;

    if (isLogLevel(DEBG))
        emit logTextSignal(QString("%1: Close reply."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

    timerStatus.stop();

    if (clockSending.isValid())
        emit logTextSignal(QString("%3: Close Reply: %1 bytes sent, %2 taken to send data."+CRLF).arg(networkBytesSent).arg(QTime(0, 0).addMSecs(clockSending.elapsed()).toString("hh:mm:ss.zzz")).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

    if (!requestFilename.isEmpty())
    {
        if (m_streamingCompleted && !streamingWithErrors && bytesToWrite == 0)
        {
            emit replyStatusSignal("Streaming finished.");
            emit servingFinishedSignal(mediaFilename, 0);
            emit logTextSignal(QString("%1: Streaming finished."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
        }
        else
        {
            if (!m_streamingCompleted)
                emit logTextSignal(QString("Streaming not completed.")+CRLF);
            if (streamingWithErrors)
                emit logTextSignal(QString("Streaming with errors.")+CRLF);
            if (bytesToWrite != 0)
                emit logTextSignal(QString("Streaming: remaining bytes to write %1 bytes.").arg(bytesToWrite)+CRLF);

            emit replyStatusSignal("Streaming aborted.");
            emit servingFinishedSignal(mediaFilename, 1);
            emit logTextSignal(QString("%1: Streaming aborted."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
        }
    }

    emit stopServingRendererSignal();

    emit logTextSignal(QString("%1: Reply closed."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

    if (clockSending.isValid())
    {
        int networkSpeed = int((double(networkBytesSent)/1024.0)/(double(clockSending.elapsed())/1000.0));
        emit logTextSignal(QString("%1: network speed %2 KB/s, %3 b/s"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(networkSpeed).arg(networkSpeed*1024*8));
    }

    // invalidate clock
    clockSending.invalidate();
    clockUpdateStatus.invalidate();

    emit logTextSignal(QString("%2: Remaining data to send: %1"+CRLF).arg(bytesToWrite).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

    emit closeClientSignal();

    emit logTextSignal(QString("%1: Reply finished."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

    if (m_requestResource)
        logError(QString("%1: reply not finished, resource is expected.").arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
    else
        emit finishedSignal();
}

void ReplyDlnaItemContent::dlnaResources(QObject *requestor, QList<DlnaResource *> resources)
{
    if (requestor != this)
        return;

    foreach (DlnaResource *item, resources)
        setDlnaResourceParent(item);

    m_requestResource = false;

    if (m_closed)
    {
        logWarning(QString("%1: resource received but reply closed.").arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
        emit finishedSignal();
    }

    if (resources.size() == 1)
    {
        // DLNAresource was found.
        DlnaItem *dlna = qobject_cast<DlnaItem*>(resources.at(0));
        dlna->setUserAgent(userAgent());

        HttpRange *range = requestRange();
        if (range != 0) {
            // update range with size of dlna object
            range->setSize(dlna->size());
        }

        if (requestFilename.startsWith("thumbnail0000")) {
            // This is a request for a thumbnail file.
            setParamHeader("Content-Type", "image/jpeg");
            setParamHeader("Accept-Ranges", "bytes");
//                setParamHeader("Expires", getFUTUREDATE() + " GMT");
            if (!isHttp10())
                setParamHeader("Connection", "keep-alive");
            setParamHeader("Server", servername());

            QByteArray answerContent = dlna->getByteAlbumArt();
            if (answerContent.isNull()) {
                logError("Unable to get thumbnail: " + dlna->getDisplayName());
                requestFilename.clear();
                emit replyStatusSignal("KO");
                close();
            } else {
                requestFilename.clear();
                sendAnswer(answerContent);
                m_streamingCompleted = true;
                emit replyStatusSignal("OK");
            }

        } else if (requestFilename.indexOf("subtitle0000") > -1) {
            // This is a request for a subtitle file
            /*output(output, "Content-Type: text/plain");
            output(output, "Expires: " + getFUTUREDATE() + " GMT");
            List<DLNAMediaSubtitle> subs = dlna.getMedia().getSubtitleTracksList();

            if (subs != null && !subs.isEmpty()) {
                // TODO: maybe loop subs to get the requested subtitle type instead of using the first one
                DLNAMediaSubtitle sub = subs.get(0);

                try {
                    // XXX external file is null if the first subtitle track is embedded:
                    // http://www.ps3mediaserver.org/forum/viewtopic.php?f=3&t=15805&p=75534#p75534
                    if (sub.isExternal()) {
                        inputStream = new java.io.FileInputStream(sub.getExternalFile());
                    }
                } catch (NullPointerException npe) {
                    LOGGER.trace("Could not find external subtitles: " + sub);
                }
            }*/
        } else {
            // This is a request for a regular file.
            setParamHeader("Content-Type", dlna->mimeType());

            if (!getRequestContentFeatures().isNull())
                setParamHeader("contentFeatures.dlna.org", dlna->getDlnaContentFeatures());

            if (!getRequestMediaInfoSec().isNull())
                setParamHeader("MediaInfo.sec", QString("SEC_Duration=%1").arg(dlna->getLengthInMilliSeconds()));

            if (dlna->getdlnaOrgOpFlags().at(1) == '1')
                setParamHeader("Accept-Ranges", "bytes");

            if (!isHttp10())
                setParamHeader("Connection", "keep-alive");

            setParamHeader("Server", servername());

            if (requestTimeSeekRangeStart() >= 0 && dlna->getLengthInMilliSeconds() > 0) {
                QTime start_time(0, 0, 0);
                start_time = start_time.addSecs(requestTimeSeekRangeStart());

                QTime end_time(0, 0, 0);
                if (requestTimeSeekRangeEnd() != -1) {
                    end_time = end_time.addSecs(requestTimeSeekRangeEnd());
                } else {
                    end_time = end_time.addMSecs(dlna->getLengthInMilliSeconds());
                }

                QTime length_time(0, 0, 0);
                length_time = length_time.addMSecs(dlna->getLengthInMilliSeconds());

                setParamHeader("TimeSeekRange.dlna.org", QString("npt=%1-%2/%3").arg(start_time.toString("hh:mm:ss,z")).arg(end_time.toString("hh:mm:ss,z")).arg(length_time.toString("hh:mm:ss,z")));
                setParamHeader("X-Seek-Range", QString("npt=%1-%2/%3").arg(start_time.toString("hh:mm:ss,z")).arg(end_time.toString("hh:mm:ss,z")).arg(length_time.toString("hh:mm:ss,z")));
                setParamHeader("X-AvailableSeekRange", QString("1 npt=%1-%2").arg(0).arg(dlna->getLengthInSeconds()));
            }

            if (dlna->size() > 0)
            {
                setParamHeader("Content-Length", QString("%1").arg(dlna->size()));
                if (range)
                    setParamHeader("Content-Range", QString("bytes %1-%2/%3").arg(range->getStartByte()).arg(range->getEndByte()).arg(dlna->size()));
            }

            if (requestMethod() == "HEAD")
            {
                requestFilename.clear();
                sendAnswer(QByteArray());
                emit replyStatusSignal("OK");

            } else {
                emit logTextSignal(QString("%3: %1 bytes to send in %2."+CRLF).arg(dlna->size()).arg(QTime(0, 0).addMSecs(dlna->getLengthInMilliSeconds()).toString("hh:mm:ss.zzz")).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

                mediaFilename = dlna->getSystemName();
                emit servingSignal(mediaFilename, 0);

                // recover resume time
                qint64 timeSeekRangeStart = requestTimeSeekRangeStart();
                qint64 timeSeekRangeEnd = requestTimeSeekRangeEnd();
                qint64 resume = dlna->getResumeTime();
                if (resume>0) {
                    timeSeekRangeStart = resume/1000;
                    clockSending.addMSec(timeSeekRangeStart*1000);
                }

                // get stream file
                Device *streamContent = dlna->getStream(range, timeSeekRangeStart, timeSeekRangeEnd);              

                if (!streamContent)
                {
                    // No inputStream indicates that transcoding / remuxing probably crashed.
                    logError("There is no inputstream to return for " + dlna->getDisplayName());
                    requestFilename.clear();
                    emit replyStatusSignal("KO");
                    close();
                }
                else
                {
                    if (dlna->bitrate()>0)
                        streamContent->setBitrate(dlna->bitrate());

                    m_maxBufferSize = streamContent->maxBufferSize();

                    connect(streamContent, SIGNAL(destroyed()), this, SLOT(streamContentDestroyed()));
                    connect(this, SIGNAL(destroyed()), streamContent, SLOT(deleteLater()));
                    connect(streamContent, SIGNAL(openedSignal()), this, SLOT(streamOpened()));
                    connect(streamContent, SIGNAL(status(QString)), this, SIGNAL(replyStatusSignal(QString)));
                    connect(streamContent, SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
                    connect(streamContent, SIGNAL(errorRaised(QString)), this, SLOT(streamingError(QString)));
                    connect(streamContent, SIGNAL(endReached()), this, SLOT(streamingCompleted()));

                    if (getRequest())
                    {
                        connect(getRequest(), SIGNAL(headerSent()), streamContent, SLOT(startRequestData()));
                        connect(getRequest(), SIGNAL(bytesSent(qint64,qint64)), streamContent, SLOT(bytesSent(qint64,qint64)));
                        connect(streamContent, SIGNAL(sendDataToClientSignal(QByteArray)), getRequest(), SIGNAL(sendData(QByteArray)));
                    }

                    if (streamContent->open())
                    {
                        emit logTextSignal(QString("%1: Streaming started, %2 bytes to send."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(streamContent->size()));
                        emit startServingRendererSignal(dlna->getDisplayName());
                    }

                    if (streamWorker)
                    {
                        streamContent->moveToThread(streamWorker);
                        connect(streamWorker, SIGNAL(finished()), streamContent, SLOT(deleteLater()));
                    }
                }
            }
        }
    }
    else
    {
        logError(QString("%1 media found").arg(resources.size()));
        requestFilename.clear();
        emit replyStatusSignal("KO");
        close();
    }
}

void ReplyDlnaItemContent::bytesSentSlot(const qint64 &size, const qint64 &towrite)
{
//    qWarning() << this << QString("%1: %2 bytes sent, %3 bytes to send.").arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(size).arg(towrite);

    networkBytesSent += size;
    bytesToWrite = towrite;
    if (towrite == 0 && m_streamingCompleted)
    {
        close();
    }
}
