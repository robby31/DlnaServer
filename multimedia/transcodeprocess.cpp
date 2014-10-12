#include "transcodeprocess.h"

const QString TranscodeProcess::CRLF = "\r\n";

TranscodeProcess::TranscodeProcess(Logger *log, QObject *parent) :
    QProcess(parent),
    m_log(log != 0 ? log : new Logger(this)),
    m_pos(0),
    transcodeClock(),
    killTranscodeProcess(false),
    m_paused(false),
    maxBufferSize(1024*1024*100)  // 100 MBytes
{
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(dataAvailable()));
    connect(this, SIGNAL(readyReadStandardError()), this, SLOT(appendTranscodingLogMessage()));
    connect(this, SIGNAL(error(QProcess::ProcessError)), this, SLOT(errorTrancodedData(QProcess::ProcessError)));
    connect(this, SIGNAL(finished(int)), this, SLOT(finishedTranscodeData(int)));
}

TranscodeProcess::~TranscodeProcess()
{
    if (state() == QProcess::Running)
    {
        kill();
        waitForFinished(1000);
    }
}

void TranscodeProcess::dataAvailable()
{
    if (m_log->isLevel(DEBG))
        appendLog(QString("%1: received %2 bytes transcoding data."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(bytesAvailable()));

    // manage buffer
    if (state() == QProcess::Running) {
        if (bytesAvailable() > maxBufferSize && !m_paused)
        {
            // pause transcoding process
            if (pause() == false)
                m_log->Error(QString("Unable to pause transcoding: pid=%1").arg(pid()));

            if (m_log->isLevel(DEBG))
                appendLog(QString("%1: PAUSE TRANSCODING").arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
        }
    }
}

bool TranscodeProcess::atEnd() const
{
//    if (m_range && m_range->getEndByte()!=-1)
//        return pos() > m_range->getEndByte();

    if (state() == QProcess::Running)
        return false;
    else
        return QProcess::atEnd();
}

qint64 TranscodeProcess::readData(char *data, qint64 maxlen)
{
    //    if (m_range && m_range->getEndByte()>0)
    //    {
    //        int bytesToRead = m_range->getEndByte() - pos() + 1;
    //        if (bytesToRead>=0 && bytesToRead<maxSize)
    //            return QFile::read(bytesToRead);
    //    }

    qint64 bytes = QProcess::readData(data, maxlen);
    m_pos += bytes;

    if (m_paused && state() != QProcess::NotRunning && bytesAvailable() < (maxBufferSize*0.5))
    {
        if (resume() == false)   // restart transcoding process
            m_log->Error(QString("Unable to restart transcoding: pid=%1").arg(pid()));

        if (m_log->isLevel(DEBG))
            appendLog(QString("%1: RESUME TRANSCODING").arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
    }

    return bytes;
}

void TranscodeProcess::appendTranscodingLogMessage() {
    // incoming log message
    QByteArray msg(this->readAllStandardError());
    appendLog(msg);
}

void TranscodeProcess::errorTrancodedData(const ProcessError &error) {
    Q_UNUSED(error);

    // trancoding failed
    if (killTranscodeProcess == false) {
        // an error occured
        appendLog(QString("%2: ERROR Transcoding: %1."+CRLF).arg(errorString()).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
        setErrorString(errorString());
        emit errorRaised(errorString());
    }
}

void TranscodeProcess::finishedTranscodeData(const int &exitCode) {
    appendLog(QString("%2: TRANSCODE FINISHED with exitCode %1."+CRLF).arg(exitCode).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
    appendLog(QString("%2: TRANSCODING DONE in %1 ms."+CRLF).arg(QTime(0, 0).addMSecs(transcodeClock.elapsed()).toString("hh:mm:ss")).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

    if (m_log->isLevel(DEBG))
        appendLog(QString("%1: finished transcoding, %2 remaining bytes."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(bytesAvailable()));

    if (isKilled() == false)
    {
        if (exitCode != 0)
        {
            // trancoding failed
            setErrorString(QString("Transcoding finished with status %1").arg(exitCode));
            emit errorRaised(QString("Transcoding finished with status %1").arg(exitCode));
            emit status("Transcoding failed.");
        }
        else
        {
            emit status("Transcoding finished.");
        }
    }
    else
    {
        emit status("Transcoding aborted.");
    }
}

void TranscodeProcess::launch() {
    appendLog(program()+' ');
    appendLog(arguments().join(' ')+CRLF);

    transcodeClock.start();

    start();
}

void TranscodeProcess::killProcess() {
    if (state() != QProcess::NotRunning) {
        appendLog(QString("%1: KILL transcoding process."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
        killTranscodeProcess = true;
        kill();
    }
}

bool TranscodeProcess::pause() {
    if (!m_paused && state() != QProcess::NotRunning) {
        m_log->Debug(QString("Pause transcoding"));

        QStringList arguments;
        arguments << "-STOP" << QString("%1").arg(pid());
        QProcess pauseTrancodeProcess;
        pauseTrancodeProcess.setProgram("kill");
        pauseTrancodeProcess.setArguments(arguments);
        pauseTrancodeProcess.start();
        m_paused = pauseTrancodeProcess.waitForFinished();
    }
    return m_paused;
}

bool TranscodeProcess::resume() {
    if (m_paused && state() != QProcess::NotRunning) {
        m_log->Debug(QString("Restart transcoding"));

        QStringList arguments;
        arguments << "-CONT" << QString("%1").arg(pid());
        QProcess continueTrancodeProcess;
        continueTrancodeProcess.setProgram("kill");
        continueTrancodeProcess.setArguments(arguments);
        continueTrancodeProcess.start();
        m_paused = !continueTrancodeProcess.waitForFinished();
    }
    return !m_paused;
}
