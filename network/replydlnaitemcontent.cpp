#include "replydlnaitemcontent.h"

// call updateStatus function every second
const int ReplyDlnaItemContent::UPDATE_STATUS_PERIOD = 1000;

ReplyDlnaItemContent::ReplyDlnaItemContent(Logger *log, Request *request, DlnaRootFolder *rootFolder, MediaRendererModel *renderersModel, QObject *parent):
    Reply(log, request, rootFolder, renderersModel, parent),
    timerStatus(this),
    networkBytesSent(0),
    lastNetBytesSent(-1),
    streamContent(0),
    transcodeProcess(0),
    maxBufferSize(1024*1024*100)  // 100 MBytes
{
    connect(request->getClient(), SIGNAL(bytesWritten(qint64)), this, SLOT(bytesSent(qint64)));
    connect(request->getClient(), SIGNAL(disconnected()), this, SLOT(close()));

    connect(&timerStatus, SIGNAL(timeout()), this, SLOT(updateStatus()));

    clockUpdateStatus.invalidate();
}

ReplyDlnaItemContent::~ReplyDlnaItemContent() {
    if (transcodeProcess)
    {
        delete transcodeProcess;
        transcodeProcess = 0;
    }

    if (streamContent)
    {
        delete streamContent;
        streamContent = 0;
    }
}

void ReplyDlnaItemContent::run() {
    // prepare data and send answer
    HttpRange *range = m_request->getRange();

    m_log->Trace("ANSWER: " + m_request->getMethod() + " " + m_request->getArgument());

    if ((m_request->getMethod() == "GET" || m_request->getMethod() == "HEAD") && m_request->getArgument().startsWith("get/"))
    {
        // Request to retrieve a file
        QStringList answerHeader;

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

        if (!m_request->getTransferMode().isNull()) {
            answerHeader << "TransferMode.DLNA.ORG: " + m_request->getTransferMode();
            headerAnswerToSend << "TransferMode.DLNA.ORG: " + m_request->getTransferMode();
        }

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
                answerHeader << "Content-Type: image/jpeg";
                answerHeader << "Accept-Ranges: bytes";
//                answerHeader << "Expires: " + getFUTUREDATE() + " GMT";
                if (!m_request->isHttp10())
                    answerHeader << "Connection: keep-alive";
                answerHeader << "Server: " + m_request->getServername();

                QByteArray answerContent = dlna->getByteAlbumArt();
                if (answerContent.isNull()) {
                    m_log->Error("Unable to get thumbnail: " + dlna->getDisplayName());
                    m_request->setStatus("KO");
                } else {
                    sendAnswer(answerHeader, answerContent);
                    m_request->setStatus("OK");
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
                answerHeader << "Content-Type: " + dlna->mimeType();
                headerAnswerToSend << "Content-Type: " + dlna->mimeType();

                if (!m_request->getContentFeatures().isNull()) {
                    answerHeader << "contentFeatures.dlna.org: " + dlna->getDlnaContentFeatures();
                    headerAnswerToSend << "contentFeatures.dlna.org: " + dlna->getDlnaContentFeatures();
                }

                if (!m_request->getMediaInfoSec().isNull()) {
                    answerHeader << QString("MediaInfo.sec: SEC_Duration=%1").arg(dlna->getLengthInMilliSeconds());
                    headerAnswerToSend << QString("MediaInfo.sec: SEC_Duration=%1").arg(dlna->getLengthInMilliSeconds());
                }

                if (dlna->getdlnaOrgOpFlags().at(1) == '1') {
                    answerHeader << "Accept-Ranges: bytes";
                    headerAnswerToSend << "Accept-Ranges: bytes";
                }

                if (!m_request->isHttp10())
                {
                    answerHeader << "Connection: keep-alive";
                    headerAnswerToSend << "Connection: keep-alive";
                }

                answerHeader << "Server: " + m_request->getServername();
                headerAnswerToSend << "Server: " + m_request->getServername();

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

                    headerAnswerToSend << QString("TimeSeekRange.dlna.org: npt=%1-%2/%3").arg(start_time.toString("hh:mm:ss,z")).arg(end_time.toString("hh:mm:ss,z")).arg(length_time.toString("hh:mm:ss,z"));
                    headerAnswerToSend << QString("X-Seek-Range: npt=%1-%2/%3").arg(start_time.toString("hh:mm:ss,z")).arg(end_time.toString("hh:mm:ss,z")).arg(length_time.toString("hh:mm:ss,z"));
                    headerAnswerToSend << QString("X-AvailableSeekRange: 1 npt=%1-%2").arg(0).arg(dlna->getLengthInSeconds());
                }

                if (m_request->getMethod() == "HEAD") {
                    sendAnswer(answerHeader, QByteArray(), dlna->size());
                    m_request->setStatus("OK");

                } else {
                    m_log->Debug(QString("%1 bytes to send, %2 to send.").arg(dlna->size()).arg(QTime(0, 0).addMSecs(dlna->getLengthInMilliSeconds()).toString("hh:mm:ss.zzz")));

                    mediaFilename = dlna->getSystemName();
                    emit serving(mediaFilename, 0);

                    if (!dlna->toTranscode()) {

                        // stream file
                        runStreaming(dlna);

                        if (streamContent != 0) {
                            // TODO: header shall be sent with the first part of the content in the same time
                            sendAnswer(answerHeader, QByteArray(), dlna->size());
                        } else
                            m_log->Error(QString("Streaming not started : %1").arg(dlna->getSystemName()));

                    } else {
                        if (dlna->size() != -1) {
                            headerAnswerToSend << "Content-Length: " + QString("%1").arg(dlna->size());
                        }

                        // transcode file
                        runTranscoding(dlna);
                    }
                }
            }
        }
        else
        {
            m_log->Error(QString("%1 media found: %2 %3").arg(files.size()).arg(m_request->getMethod()).arg(m_request->getArgument()));
            m_request->setStatus("KO");
        }

    }
    else
    {
        m_log->Error("Unkown answer for: " + m_request->getMethod() + " " + m_request->getArgument());
        m_request->setStatus("KO");
    }

}

void ReplyDlnaItemContent::runStreaming(DlnaItem* dlna) {
    if (streamContent == 0) {
        //set buffer size
        if (dlna->bitrate()>0) {
            // set buffer size to 10 seconds transcoding
            maxBufferSize = dlna->bitrate()/8*10;
        }

        // recover resume time
        long timeSeekRangeStart = m_request->getTimeSeekRangeStart();
        long timeSeekRangeEnd = m_request->getTimeSeekRangeEnd();
        qint64 resume = dlna->getResumeTime();
        if (resume>0) {
            timeSeekRangeStart = resume/1000 - 10;
            clockSending.addMSec(timeSeekRangeStart*1000);
        }

        // get stream file
        streamContent = dlna->getStream(m_request->getRange(), timeSeekRangeStart, timeSeekRangeEnd, this);

        if (streamContent == 0) {
            // No inputStream indicates that transcoding / remuxing probably crashed.
            m_log->Error("There is no inputstream to return for " + dlna->getDisplayName());
            m_request->setStatus("KO");
        } else {
            m_renderersModel->serving(m_request->getpeerAddress(), dlna->getDisplayName());

            // Start timer to update periodically the status on streaming or transcoding
            timerStatus.start(UPDATE_STATUS_PERIOD);
        }
    } else {
        m_log->Error(QString("Streaming already in progress"));
        m_request->setStatus("KO");
    }
}

void ReplyDlnaItemContent::runTranscoding(DlnaItem* dlna) {
    if (transcodeProcess == 0) {
        //set buffer size
        if (dlna->bitrate()>0) {
            // set buffer size to 10 seconds transcoding
            maxBufferSize = dlna->bitrate()/8*10;
        }

        // recover resume time
        long timeSeekRangeStart = m_request->getTimeSeekRangeStart();
        long timeSeekRangeEnd = m_request->getTimeSeekRangeEnd();
        qint64 resume = dlna->getResumeTime();
        if (resume>0) {
            timeSeekRangeStart = resume/1000 - 10;
            clockSending.addMSec(timeSeekRangeStart*1000);
        }

        // get transcoding process
        transcodeProcess = dlna->getTranscodeProcess(m_request->getRange(), timeSeekRangeStart, timeSeekRangeEnd, this);

        if (transcodeProcess == 0) {
            m_log->Error(QString("Cannot create transcoding process"));
            m_request->setStatus("KO");

        } else {
            connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
            connect(transcodeProcess, SIGNAL(receivedTranscodingLogMessage(QString)), this, SLOT(receivedTranscodingLogMessage(QString)));
            connect(transcodeProcess, SIGNAL(finished(int)), this, SLOT(finishedTranscodeData(int)));

            transcodeProcess->launch();
            if (!transcodeProcess->waitForStarted()) {
                m_log->Error(transcodeProcess->errorString());
                m_request->setStatus("KO");
            } else {
                // transcoding process started
                m_renderersModel->serving(m_request->getpeerAddress(), dlna->getDisplayName());
                m_request->setStatus("Transcoding");

                // Start timer to update periodically the status on streaming or transcoding
                timerStatus.start(UPDATE_STATUS_PERIOD);
            }
        }
    } else {
        m_log->Error(QString("Transcoding already in progress"));
        m_request->setStatus("KO");
    }
}

void ReplyDlnaItemContent::waitTranscodingFinished() {
    if (transcodeProcess != 0) {
        // wait until process is finished
        transcodeProcess->waitForFinished(-1);
    }
}

void ReplyDlnaItemContent::receivedTranscodingLogMessage(const QString &msg) {
    m_request->appendLog(msg);
}

void ReplyDlnaItemContent::receivedTranscodedData() {
    if (transcodeProcess != 0) {
        if (transcodeProcess->bytesAvailable() > 0) {
            if (client != 0) {
                if (!headerAnswerToSend.isEmpty()) {
                    // send the header
                    sendAnswer(headerAnswerToSend);

                    // header sent
                    headerAnswerToSend.clear();
                }

                // send data to client
                if (client->write(transcodeProcess->readAllStandardOutput()) == -1) {
                    m_log->Error("Unable to send transcoded data to client.");
                } else if (transcodeProcess->state()==QProcess::Running) {
                    // data sent and transcoding is in progress
                    if (client->bytesToWrite() > maxBufferSize) {
                        // pause transcoding process
                        if (transcodeProcess->pause() == false)
                            m_log->Error(QString("Unable to pause transcoding: pid=%1").arg(transcodeProcess->pid()));
                    }
                }
            } else {
                m_log->Error("Unable to send transcoded data to client (client deleted).");
            }
        }
    }
}

void ReplyDlnaItemContent::finishedTranscodeData(const int &exitCode) {
    if (transcodeProcess != 0) {
        if (transcodeProcess->bytesAvailable() > 0) {
            // send last transcoded data to client
            if (client && client->write(transcodeProcess->readAllStandardOutput()) == -1)
                m_log->Error("Unable to send last transcoded data to client.");
        }


        if (transcodeProcess->isKilled() == false) {
            if (exitCode != 0) {
                // trancoding failed
                m_request->setStatus("Transcoding failed.");
            } else {
                m_request->setStatus("Transcoding finished.");
            }
        } else {
            m_request->setStatus("Transcoding aborted.");
        }

        closeClient();
    }
}

void ReplyDlnaItemContent::updateStatus()
{
    if (clockUpdateStatus.isValid()) {
        int delta = clockUpdateStatus.restart() - UPDATE_STATUS_PERIOD;

        if (qAbs(delta) > UPDATE_STATUS_PERIOD/10) {
            if (streamContent)
                m_log->Info(QString("UPDATE STATUS delta %2 streaming <%1>").arg(mediaFilename).arg(delta));
            else if (transcodeProcess)
                m_log->Info(QString("UPDATE STATUS delta %2 transcoding <%1>").arg(mediaFilename).arg(delta));
            else
                m_log->Info(QString("UPDATE STATUS delta %2 <%1>").arg(mediaFilename).arg(delta));
        }
    } else {
        clockUpdateStatus.start();
    }

    if (streamContent)
        m_request->setStatus(QString("Streaming (%1%)").arg(int(100.0*double(streamContent->pos())/double(streamContent->size()))));

    if (!mediaFilename.isNull()) {
        if (clockSending.isValid())
            emit serving(mediaFilename, clockSending.elapsedFromBeginning());
        else
            emit serving(mediaFilename, 0);

        if (client && clockSending.isValid() && maxBufferSize != 0) {
            // display the network buffer and network speed
            int networkSpeed = int((double(networkBytesSent)/1024.0)/(double(clockSending.elapsed())/1000.0));
            double bufferTime = double(maxBufferSize)/double(networkSpeed*1024);
            m_request->setNetworkStatus(QString("Time: %5 Buffer: %4 bytes %1% %3 seconds, Speed: %2 Ko/s").arg(int(100.0*double(client->bytesToWrite())/double(maxBufferSize))).arg(networkSpeed).arg(bufferTime).arg(maxBufferSize).arg(QTime(0, 0).addMSecs(clockSending.elapsedFromBeginning()).toString("hh:mm:ss")));
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
    networkBytesSent += size;

    if (!clockSending.isValid()) {
        // start clock to measure time taken to stream or transcode
        clockSending.start();
    }

    if (m_log->isLevel(TRA) and transcodeProcess != 0) {
        if (client != 0)
            appendTrancodeProcessLog(QString("%2: bytes sent %3 (%1)").arg(client->socketDescriptor()).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(size));
        else
            appendTrancodeProcessLog(QString("%1: bytes sent %2 (client deleted)").arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(size));
    }

    if (streamContent) {
        if (client == 0) {
            m_log->Error("HTTP Request: Unable to send content (client deleted).");

            close();

            m_request->setStatus("KO");
        } else {
            int bytesToRead = maxBufferSize - client->bytesToWrite();
            if (bytesToRead > 0) {
                // read the stream
                char bytesToSend[bytesToRead];
                qint64 bytes = streamContent->read(bytesToSend, sizeof(bytesToSend));

                if (bytes == -1) {
                    m_log->Error("HTTP Request: Unable to send content.");

                    close();

                    m_request->setStatus("KO");

                } else if (bytes > 0) {
                    if (client->write(bytesToSend, bytes)== -1) {
                        m_log->Error("HTTP Request: Unable to send content.");

                        close();

                        m_request->setStatus("KO");
                    }
                }
            }
        }
    }

    if (transcodeProcess && transcodeProcess->state() != QProcess::NotRunning && client && client->bytesToWrite() < (maxBufferSize*0.5)) {
        // restart transcoding process
        if (transcodeProcess->resume() == false)
            m_log->Error(QString("Unable to restart transcoding: pid=%1").arg(transcodeProcess->pid()));
    }
}

void ReplyDlnaItemContent::close() {
    appendTrancodeProcessLog(QString("CLOSE REPLY"+CRLF));
    timerStatus.stop();

    if (clockSending.isValid())
        m_log->Debug(QString("REQUEST CLOSED: %1 bytes sent, %2 taken to send data.").arg(networkBytesSent).arg(QTime(0, 0).addMSecs(clockSending.elapsed()).toString("hh:mm:ss.zzz")));

    if (streamContent) {
        if (streamContent->atEnd() && client && client->bytesToWrite() == 0) {
            m_request->setStatus("Streaming finished.");
            emit servingFinished(mediaFilename, 0);
        } else {
            m_request->setStatus("Streaming aborted.");
            emit servingFinished(mediaFilename, 1);
        }

        delete streamContent;
        streamContent = 0;

        m_renderersModel->stopServing(m_request->getpeerAddress());
    }

    if (transcodeProcess) {
        if (transcodeProcess->state() != QProcess::NotRunning) {
            m_request->setStatus("Transcoding aborted.");
            transcodeProcess->killProcess();
            if (!transcodeProcess->waitForFinished(1000))
                m_log->Warning("Unable to kill process before process deletion.");
        }

        if (!transcodeProcess->isKilled() && transcodeProcess->transcodeExitCode()==0 && client && client->bytesToWrite() == 0)
            emit servingFinished(mediaFilename, 0);
        else
            emit servingFinished(mediaFilename, 1);

        delete transcodeProcess;
        transcodeProcess = 0;

        m_renderersModel->stopServing(m_request->getpeerAddress());
    }

    // invalidate clock
    clockSending.invalidate();
    clockUpdateStatus.invalidate();

    if (client) {
        if (clockSending.isValid())
            m_log->Debug(QString("remaining data to send: %1").arg(client->bytesToWrite()));
    }

    emit finished();
}

void ReplyDlnaItemContent::closeClient() {
    if (m_request->isHttp10() or m_request->getHttpConnection().toLower() == "close")
    {
        if (m_log->isLevel(DEBG) and transcodeProcess)
            appendTrancodeProcessLog(QString("%3: closeclient, state transcodeprocess %1, client valid? %2").arg(transcodeProcess->state()).arg(client != 0).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

        if (!keepSocketOpened and (transcodeProcess == 0 or transcodeProcess->state() != QProcess::Running) and (streamContent == 0 or streamContent->atEnd())) {
            // No streaming or transcoding in progress
            m_log->Trace("Close client connection in request");
            appendTrancodeProcessLog("CLose Client."+CRLF);
            if (client != 0) {
                if (m_log->isLevel(DEBG) and transcodeProcess)
                    appendTrancodeProcessLog(QString("%2: CLOSE CLIENT (%1)").arg(client->socketDescriptor()).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

                client->disconnectFromHost();
            } else
                m_log->Debug("Unable to close client (client deleted).");
        }
    }

    if (!keepSocketOpened and transcodeProcess == 0 and streamContent == 0)
        emit finished();
}
