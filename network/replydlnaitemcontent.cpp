#include "replydlnaitemcontent.h"

// call updateStatus function every second
const int ReplyDlnaItemContent::UPDATE_STATUS_PERIOD = 1000;

ReplyDlnaItemContent::ReplyDlnaItemContent(Logger *log, Request *request, QThread *streamThread, QObject *parent):
    Reply(log, request, parent),
    m_closed(false),
    timerStatus(this),
    bytesToWrite(0),
    networkBytesSent(0),
    lastNetBytesSent(-1),
    streamThread(streamThread),
    streamContent(0),
    streamingWithErrors(false),
    m_maxBufferSize(1024*1024*10),   // 10 MBytes by default when bitrate is unknown
    durationBuffer(10),
    counter_sendDataToClient(0),
    timeElapsed_sendDataToClient(0)
{
    timerStatus.setTimerType(Qt::PreciseTimer);
    timerStatus.setInterval(UPDATE_STATUS_PERIOD);
    connect(&timerStatus, SIGNAL(timeout()), this, SLOT(updateStatus()));

    clockUpdateStatus.invalidate();
}

ReplyDlnaItemContent::~ReplyDlnaItemContent()
{
    close();
}

void ReplyDlnaItemContent::streamOpened()
{
    // Start timer to update periodically the status on streaming
    timerStatus.start();
    clockUpdateStatus.start();
    emit logTextSignal(QString("%1: periodic timer started? %3, %2."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(UPDATE_STATUS_PERIOD).arg(timerStatus.isActive()));

    // start sending when device is opened
    sendDataToClient();
}

void ReplyDlnaItemContent::_run(const QString &method, const QString &argument)
{
    if (!m_request)
        return;

    // prepare data and send answer
    logTrace("ANSWER: " + method + " " + argument);

    if ((method == "GET" || method == "HEAD") && argument.startsWith("get/"))
    {
        // Request to retrieve a file

        /**
        * Skip the leading "get/" and extract the resource ID from the first path element
        * e.g. "get/0$1$5$3$4/Foo.mp4" -> "0$1$5$3$4"
        */
        QRegExp rxId("get/([\\d$]+)/(.+)");
        QString id;
        if (rxId.indexIn(argument) != -1) {
            id = rxId.capturedTexts().at(1);
            requestFilename = rxId.capturedTexts().at(2);
        }

        // Some clients escape the separators in their request: unescape them.
        id = id.replace("%24", "$");

        if (!m_request->getTransferMode().isNull())
            setParamHeader("TransferMode.dlna.org", m_request->getTransferMode());

        // Retrieve the DLNAresource itself.
        emit getDLNAResourcesSignal(id, false, 0, 0, "");
    }
    else
    {
        logError("Unkown answer for: " + method + " " + argument);
        emit replyStatusSignal("KO");
        close();
    }
}

void ReplyDlnaItemContent::sendDataToClient()
{
    QElapsedTimer timer;
    timer.start();

    counter_sendDataToClient++;

    if (streamContent && !streamContent->atEnd())
    {
        if (streamContent->bytesAvailable() > 0)
        {        
            if (!clockSending.isValid())
                clockSending.start();   // start clock to measure time taken for streaming

            sendHeader();

            int bytesToRead = 0;
            bytesToRead = maxBufferSize() - bytesToWrite;
            if (bytesToRead > 0) {
                // read the stream
                QByteArray bytesToSend = streamContent->read(bytesToRead);
                if (!bytesToSend.isEmpty())
                    emit sendDataToClientSignal(bytesToSend);
            }

            if (streamContent->atEnd())
                emit closeClientSignal();
        }
        else
        {
            if (isLogLevel(DEBG))
                emit logTextSignal(QString("%1: sendDataToClient no data available"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
        }
    }

    timeElapsed_sendDataToClient += timer.elapsed();
}

void ReplyDlnaItemContent::updateStatus()
{
    if (clockUpdateStatus.isValid()) {
        int delta = clockUpdateStatus.restart() - UPDATE_STATUS_PERIOD;

        if (qAbs(delta) > UPDATE_STATUS_PERIOD/10) {
            QString msg;
            if (streamContent)
                msg = QString("UPDATE STATUS delta %2 <%1>").arg(mediaFilename).arg(delta);
            else
                msg = QString("UPDATE STATUS delta %2 no streaming no transcoding <%1>").arg(mediaFilename).arg(delta);

            logInfo(msg);
            emit logTextSignal(QString("%1: %2").arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(msg)+CRLF);
        }
    } else {
        clockUpdateStatus.start();
    }

    if (isLogLevel(DEBG) && streamContent)
        emit logTextSignal(QString("%1: UPDATE STATUS, bytes available?%2 bytes sent?%3 bytes to send?%4"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(streamContent->bytesAvailable()).arg(networkBytesSent).arg(bytesToWrite));

    if (streamContent && streamContent->isOpen())
    {
        if (!streamContent->atEnd() && int(100.0*double(bytesToWrite)/double(maxBufferSize())) < 50)
            sendDataToClient();
    }

    if (!mediaFilename.isNull()) {
        if (clockSending.isValid())
            emit servingSignal(mediaFilename, clockSending.elapsedFromBeginning());
        else
            emit servingSignal(mediaFilename, 0);

        if (clockSending.isValid() && maxBufferSize() != 0) {
            // display the network buffer and network speed
            int networkSpeed = int((double(networkBytesSent)/1024.0)/(double(clockSending.elapsed())/1000.0));
            int bufferTime = 0;
            if (networkSpeed!=0)
                bufferTime = maxBufferSize()/(networkSpeed*1024);
            emit networkStatusSignal(QString("Time: %5 Buffer: %1% (%4 KB - %3 seconds), Speed: %2 KB/s").arg(int(100.0*double(bytesToWrite)/double(maxBufferSize()))).arg(networkSpeed).arg(bufferTime).arg(maxBufferSize()/1024).arg(QTime(0, 0).addMSecs(clockSending.elapsedFromBeginning()).toString("hh:mm:ss")));
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

    if (isLogLevel(DEBG))
        emit logTextSignal(QString("%1: Close reply."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

    timerStatus.stop();

    if (clockSending.isValid())
        emit logTextSignal(QString("%3: Close Reply: %1 bytes sent, %2 taken to send data."+CRLF).arg(networkBytesSent).arg(QTime(0, 0).addMSecs(clockSending.elapsed()).toString("hh:mm:ss.zzz")).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

    if (streamContent)
    {
        if (streamContent->atEnd() && !streamingWithErrors && bytesToWrite == 0) {
            emit replyStatusSignal("Streaming finished.");
            emit servingFinishedSignal(mediaFilename, 0);
            emit logTextSignal(QString("%1: Streaming finished."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
        } else {
            emit replyStatusSignal("Streaming aborted.");
            emit servingFinishedSignal(mediaFilename, 1);
            emit logTextSignal(QString("%1: Streaming aborted."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
        }

        emit stopServingRendererSignal();
    }

    emit logTextSignal(QString("%1: Reply closed, %2 call to sendDataToClient."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(counter_sendDataToClient));
    if (counter_sendDataToClient!=0)
        emit logTextSignal(QString("%1: sendDataToClient called every %2 ms, total duration %3 ms."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(clockSending.elapsed()/counter_sendDataToClient).arg(timeElapsed_sendDataToClient));

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

    emit finishedSignal();

    m_closed = true;
}

void ReplyDlnaItemContent::dlnaResources(QObject *requestor, QList<DlnaResource *> resources)
{
    if (requestor != this)
        return;

    if (resources.size() == 1)
    {
        // DLNAresource was found.
        DlnaItem *dlna = qobject_cast<DlnaItem*>(resources.at(0));

        HttpRange *range = m_request->getRange();
        if (range != 0) {
            // update range with size of dlna object
            range->setSize(dlna->size());
        }

        if (requestFilename.startsWith("thumbnail0000")) {
            // This is a request for a thumbnail file.
            setParamHeader("Content-Type", "image/jpeg");
            setParamHeader("Accept-Ranges", "bytes");
//                setParamHeader("Expires", getFUTUREDATE() + " GMT");
            if (!m_request->isHttp10())
                setParamHeader("Connection", "keep-alive");
            setParamHeader("Server", m_request->getServername());

            QByteArray answerContent = dlna->getByteAlbumArt();
            if (answerContent.isNull()) {
                logError("Unable to get thumbnail: " + dlna->getDisplayName());
                emit replyStatusSignal("KO");
                close();
            } else {
                sendAnswer(answerContent);
                emit replyStatusSignal("OK");
                close();
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
                emit replyStatusSignal("OK");
                close();

            } else {
                emit logTextSignal(QString("%3: %1 bytes to send in %2."+CRLF).arg(dlna->size()).arg(QTime(0, 0).addMSecs(dlna->getLengthInMilliSeconds()).toString("hh:mm:ss.zzz")).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

                mediaFilename = dlna->getSystemName();
                emit servingSignal(mediaFilename, 0);

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
                    streamContent = dlna->getStream(range, timeSeekRangeStart, timeSeekRangeEnd);
//                        streamContent->moveToThread(streamThread);

                    if (!streamContent)
                    {
                        // No inputStream indicates that transcoding / remuxing probably crashed.
                        logError("There is no inputstream to return for " + dlna->getDisplayName());
                        emit replyStatusSignal("KO");
                        close();
                    }
                    else
                    {
                        if (dlna->bitrate()>0)
                            setMaxBufferSize(dlna->bitrate()/8*durationBuffer);  // set Max buffer size if bitrate is available

                        connect(streamContent, SIGNAL(destroyed()), this, SLOT(streamContentDestroyed()));
                        connect(this, SIGNAL(destroyed()), streamContent, SLOT(deleteLater()));
                        connect(streamContent, SIGNAL(openedSignal()), this, SLOT(streamOpened()));
                        connect(streamContent, SIGNAL(status(QString)), this, SIGNAL(replyStatusSignal(QString)));
                        connect(streamContent, SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
                        connect(streamContent, SIGNAL(errorRaised(QString)), this, SLOT(streamingError(QString)));

                        if (streamContent->open())
                        {
                            emit logTextSignal(QString("%1: Streaming started, %2 bytes to send."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(streamContent->size()));
                            emit startServingRendererSignal(dlna->getDisplayName());
                        }
                    }
                } else {
                    logError(QString("Streaming already in progress"));
                    emit replyStatusSignal("KO");
                    close();
                }
            }
        }
    }
    else
    {
        logError(QString("%1 media found").arg(resources.size()));
        emit replyStatusSignal("KO");
        close();
    }
}
