#include "replydlnaitemcontent.h"

// call updateStatus function every second
const int ReplyDlnaItemContent::UPDATE_STATUS_PERIOD = 1000;

ReplyDlnaItemContent::ReplyDlnaItemContent(Logger *log, Request *request, DlnaRootFolder *rootFolder, QObject *parent):
    Reply(log, request, rootFolder, parent),
    timerStatus(this),
    networkBytesSent(0),
    lastNetBytesSent(-1),
    streamContent(0),
    streamingWithErrors(false),
    maxBufferSize(1024*1024*100),  // 100 MBytes
    counter_bytesSent(0),
    counter_sendDataToClient(0)
{
    connect(client, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesSent(qint64)));
    connect(client, SIGNAL(disconnected()), this, SLOT(close()));

    timerStatus.setTimerType(Qt::PreciseTimer);
    timerStatus.setInterval(UPDATE_STATUS_PERIOD);
    connect(&timerStatus, SIGNAL(timeout()), this, SLOT(updateStatus()));

    clockUpdateStatus.invalidate();
}

ReplyDlnaItemContent::~ReplyDlnaItemContent()
{
}

void ReplyDlnaItemContent::run()
{
    // prepare data and send answer
    HttpRange *range = m_request->getRange();

    m_log->Trace("ANSWER: " + m_request->getMethod() + " " + m_request->getArgument());

    if (m_rootFolder && (m_request->getMethod() == "GET" || m_request->getMethod() == "HEAD") && m_request->getArgument().startsWith("get/"))
    {
        // Request to retrieve a file

        /**
        * Skip the leading "get/" and extract the resource ID from the first path element
        * e.g. "get/0$1$5$3$4/Foo.mp4" -> "0$1$5$3$4"
        */
        QRegExp rxId("get/([\\d$]+)/(.+)");
        QString id;
        QString fileName;
        if (rxId.indexIn(m_request->getArgument()) != -1) {
            id = rxId.capturedTexts().at(1);
            fileName = rxId.capturedTexts().at(2);
        }

        // Some clients escape the separators in their request: unescape them.
        id = id.replace("%24", "$");

        // Retrieve the DLNAresource itself.
        QObject context_object;
        QList<DlnaResource*> files = m_rootFolder->getDLNAResources(id, false, 0, 0, "", &context_object);

        if (!m_request->getTransferMode().isNull())
            setParamHeader("TransferMode.dlna.org", m_request->getTransferMode());

        if (files.size() == 1)
        {
            // DLNAresource was found.
            DlnaItem *dlna = static_cast<DlnaItem*>(files.at(0));

            if (range != 0) {
                // update range with size of dlna object
                range->setSize(dlna->size());
            }

            if (fileName.startsWith("thumbnail0000")) {
                // This is a request for a thumbnail file.
                setParamHeader("Content-Type", "image/jpeg");
                setParamHeader("Accept-Ranges", "bytes");
//                setParamHeader("Expires", getFUTUREDATE() + " GMT");
                if (!m_request->isHttp10())
                    setParamHeader("Connection", "keep-alive");
                setParamHeader("Server", m_request->getServername());

                QByteArray answerContent = dlna->getByteAlbumArt();
                if (answerContent.isNull()) {
                    m_log->Error("Unable to get thumbnail: " + dlna->getDisplayName());
                    emit replyStatus("KO");
                } else {
                    sendAnswer(answerContent);
                    emit replyStatus("OK");
                }

            } else if (fileName.indexOf("subtitle0000") > -1) {
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

                if (!m_request->getContentFeatures().isNull())
                    setParamHeader("contentFeatures.dlna.org", dlna->getDlnaContentFeatures());

                if (!m_request->getMediaInfoSec().isNull())
                    setParamHeader("MediaInfo.sec", QString("SEC_Duration=%1").arg(dlna->getLengthInMilliSeconds()));

                if (dlna->getdlnaOrgOpFlags().at(1) == '1')
                    setParamHeader("Accept-Ranges", "bytes");

                if (!m_request->isHttp10())
                    setParamHeader("Connection", "keep-alive");

                setParamHeader("Server", m_request->getServername());

                if (m_request->getTimeSeekRangeStart() >= 0 && dlna->getLengthInMilliSeconds() > 0) {
                    QTime start_time(0, 0, 0);
                    start_time = start_time.addSecs(m_request->getTimeSeekRangeStart());

                    QTime end_time(0, 0, 0);
                    if (m_request->getTimeSeekRangeEnd() != -1) {
                        end_time = end_time.addSecs(m_request->getTimeSeekRangeEnd());
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

                if (m_request->getMethod() == "HEAD") {
                    sendAnswer(QByteArray());
                    emit replyStatus("OK");

                } else {
                    emit logText(QString("%3: %1 bytes to send in %2."+CRLF).arg(dlna->size()).arg(QTime(0, 0).addMSecs(dlna->getLengthInMilliSeconds()).toString("hh:mm:ss.zzz")).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

                    mediaFilename = dlna->getSystemName();
                    emit serving(mediaFilename, 0);

                    if (streamContent == 0) {
                        // recover resume time
                        qint64 timeSeekRangeStart = m_request->getTimeSeekRangeStart();
                        qint64 timeSeekRangeEnd = m_request->getTimeSeekRangeEnd();
                        qint64 resume = dlna->getResumeTime();
                        if (resume>0) {
                            timeSeekRangeStart = resume/1000 - 10;
                            clockSending.addMSec(timeSeekRangeStart*1000);
                        }

                        // get stream file
                        streamContent = dlna->getStream(m_request->getRange(), timeSeekRangeStart, timeSeekRangeEnd, this);

                        if (!streamContent or !streamContent->open(QIODevice::ReadOnly))
                        {
                            if (streamContent)
                                streamContent->deleteLater();

                            // No inputStream indicates that transcoding / remuxing probably crashed.
                            m_log->Error("There is no inputstream to return for " + dlna->getDisplayName());
                            emit replyStatus("KO");
                        }
                        else
                        {
                            //set buffer size
                            if (dlna->bitrate()>0) {
                                // set buffer size to 10 seconds transcoding
                                maxBufferSize = dlna->bitrate()/8*10;
                            }

                            connect(streamContent, SIGNAL(destroyed()), this, SLOT(streamContentDestroyed()));
                            connect(streamContent, SIGNAL(status(QString)), this, SIGNAL(replyStatus(QString)));
                            connect(streamContent, SIGNAL(readyRead()), this, SLOT(sendDataToClient()));
                            connect(streamContent, SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
                            connect(streamContent, SIGNAL(errorRaised(QString)), this, SLOT(streamingError(QString)));

                            emit logText(QString("%1: Streaming started."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
                            emit servingRenderer(m_request->getpeerAddress(), dlna->getDisplayName());
                            emit replyStatus("Streaming");

                            // Start timer to update periodically the status on streaming
                            timerStatus.start(UPDATE_STATUS_PERIOD);

                            sendDataToClient();
                        }
                    } else {
                        m_log->Error(QString("Streaming already in progress"));
                        emit replyStatus("KO");
                    }
                }
            }
        }
        else
        {
            m_log->Error(QString("%1 media found: %2 %3").arg(files.size()).arg(m_request->getMethod()).arg(m_request->getArgument()));
            emit replyStatus("KO");
        }

    }
    else
    {
        m_log->Error("Unkown answer for: " + m_request->getMethod() + " " + m_request->getArgument());
        emit replyStatus("KO");
    }

}

void ReplyDlnaItemContent::sendDataToClient()
{
    counter_sendDataToClient++;

    if (streamContent)
    {
        if (client == 0)
        {
            m_log->Error("HTTP Request: Unable to send content (client deleted).");

            close();

            emit replyStatus("KO");
        }
        else if (streamContent->bytesAvailable() > 0)
        {
            int bytesToRead = maxBufferSize - client->bytesToWrite();
            if (bytesToRead > 0) {
                // read the stream
                QByteArray bytesToSend = streamContent->read(bytesToRead);
                if (!bytesToSend.isEmpty()) {
                    sendHeader();
                    if (client->write(bytesToSend)== -1)
                        m_log->Error("HTTP Request: Unable to send content.");
                }
            }

            if (streamContent->atEnd())
                closeClient();
        }
    }
}

void ReplyDlnaItemContent::updateStatus()
{
    if (m_log->isLevel(DEBG))
        emit logText(QString("%1: update status"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

    if (clockUpdateStatus.isValid()) {
        int delta = clockUpdateStatus.restart() - UPDATE_STATUS_PERIOD;

        if (qAbs(delta) > UPDATE_STATUS_PERIOD/10) {
            if (streamContent)
                m_log->Info(QString("UPDATE STATUS delta %2 <%1>").arg(mediaFilename).arg(delta));
            else
                m_log->Info(QString("UPDATE STATUS delta %2 no streaming no transcoding <%1>").arg(mediaFilename).arg(delta));
        }
    } else {
        clockUpdateStatus.start();
    }

    if (streamContent)
        emit replyStatus(QString("Streaming (%1%)").arg(int(100.0*double(streamContent->pos())/double(streamContent->size()))));

    if (!mediaFilename.isNull()) {
        if (clockSending.isValid())
            emit serving(mediaFilename, clockSending.elapsedFromBeginning());
        else
            emit serving(mediaFilename, 0);

        if (client && clockSending.isValid() && maxBufferSize != 0) {
            // display the network buffer and network speed
            int networkSpeed = int((double(networkBytesSent)/1024.0)/(double(clockSending.elapsed())/1000.0));
            int bufferTime = 0;
            if (networkSpeed!=0)
                bufferTime = maxBufferSize/(networkSpeed*1024);
            m_request->setNetworkStatus(QString("Time: %5 Buffer: %1% (%4 KB - %3 seconds), Speed: %2 KB/s").arg(int(100.0*double(client->bytesToWrite())/double(maxBufferSize))).arg(networkSpeed).arg(bufferTime).arg(maxBufferSize/1024).arg(QTime(0, 0).addMSecs(clockSending.elapsedFromBeginning()).toString("hh:mm:ss")));
        }

        if (lastNetBytesSent!=-1 && lastNetBytesSent==networkBytesSent)
            clockSending.pause();
        else if (networkBytesSent > 0)
            clockSending.start();

        lastNetBytesSent = networkBytesSent;
    }
}

void ReplyDlnaItemContent::bytesSent(const qint64 &size)
{
    counter_bytesSent++;

    if (m_log->isLevel(DEBG))
    {
        if (client)
            emit logText(QString("%2: %3 bytes sent (socket %1)"+CRLF).arg(client->socketDescriptor()).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(size));
        else
            emit logText(QString("%1: %2 bytes sent (client deleted)"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(size));
    }

    networkBytesSent += size;

    if (!clockSending.isValid())
        clockSending.start();   // start clock to measure time taken for streaming

    if (streamContent)
        sendDataToClient();
}

void ReplyDlnaItemContent::close()
{
    if (m_log->isLevel(DEBG))
        emit logText(QString("%1: Close reply."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

    timerStatus.stop();

    if (clockSending.isValid())
        emit logText(QString("%3: Close Reply: %1 bytes sent, %2 taken to send data."+CRLF).arg(networkBytesSent).arg(QTime(0, 0).addMSecs(clockSending.elapsed()).toString("hh:mm:ss.zzz")).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

    if (streamContent) {
        if (streamContent->atEnd() && !streamingWithErrors && client && client->bytesToWrite() == 0) {
            emit replyStatus("Streaming finished.");
            emit servingFinished(mediaFilename, 0);
        } else {
            emit replyStatus("Streaming aborted.");
            emit servingFinished(mediaFilename, 1);
        }

        streamContent->disconnect(this);
        streamContent->deleteLater();
        streamContent = 0;

        emit stopServingRenderer(m_request->getpeerAddress());
    }

    emit logText(QString("%1: Reply closed, %2 call to bytesSent, %3 call to sendDataToClient."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(counter_bytesSent).arg(counter_sendDataToClient));
    if (counter_bytesSent!=0)
        emit logText(QString("%1: bytesSent called every %2 milliseconds."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(clockSending.elapsed()/counter_bytesSent));
    if (counter_sendDataToClient!=0)
        emit logText(QString("%1: sendDataToClient called every %2 milliseconds."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(clockSending.elapsed()/counter_sendDataToClient));

    // invalidate clock
    clockSending.invalidate();
    clockUpdateStatus.invalidate();

    if (client)
        emit logText(QString("%2: Remaining data to send: %1"+CRLF).arg(client->bytesToWrite()).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

    closeClient();
}

void ReplyDlnaItemContent::closeClient()
{
    if (m_request->isHttp10() or m_request->getHttpConnection().toLower() == "close")
    {
        if (!keepSocketOpened and (streamContent == 0 or streamContent->atEnd())) {
            // No streaming in progress
            if (client != 0) {
                emit logText(QString("%2: Close client (%1)"+CRLF).arg(client->socketDescriptor()).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

                client->disconnectFromHost();
            } else
                emit logText(QString("%1: Unable to close client (client deleted)."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
        }
    }

    if (!keepSocketOpened and streamContent == 0)
        emit finished();
}
