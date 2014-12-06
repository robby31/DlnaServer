#include "transcodeprocess.h"

const QString TranscodeProcess::CRLF = "\r\n";

TranscodeProcess::TranscodeProcess(Logger *log, QObject *parent) :
    Device(log, parent),
    m_process(parent),
    m_opened(false),
    m_url(),
    m_bitrate(-1),
    m_pos(0),
    m_size(-1),
    processPauseResume(this),
    transcodeClock(),
    killTranscodeProcess(false),
    m_paused(false),
    m_maxBufferSize(1024*1024*10),   // 10 MBytes by default when bitrate is unknown
    m_durationBuffer(10)
{
    connect(&m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(dataAvailable()));
    connect(&m_process, SIGNAL(readyReadStandardError()), this, SLOT(appendTranscodingLogMessage()));
    connect(&m_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(errorTrancodedData(QProcess::ProcessError)));
    qRegisterMetaType<QIODevice::OpenMode>("OpenMode");
    connect(this, SIGNAL(openSignal(QIODevice::OpenMode)), this, SLOT(_open(QIODevice::OpenMode)));
    connect(&m_process, SIGNAL(started()), this, SLOT(processStarted()));
    connect(&m_process, SIGNAL(finished(int)), this, SLOT(finishedTranscodeData(int)));
    connect(this, SIGNAL(pauseSignal()), this, SLOT(_pause()));
    connect(this, SIGNAL(resumeSignal()), this, SLOT(_resume()));

    connect(&processPauseResume, SIGNAL(finished(int)), this, SLOT(_pause_resume_done(int)));
    connect(&processPauseResume, SIGNAL(error(QProcess::ProcessError)), this, SLOT(_pause_resume_error(QProcess::ProcessError)));
}

TranscodeProcess::~TranscodeProcess()
{
    if (m_process.state() == QProcess::Running)
    {
        killProcess();
        if (!m_process.waitForFinished(1000))
            logError("Unable to stop TranscodeProcess.");
    }
}

void TranscodeProcess::dataAvailable()
{
    if (isLogLevel(DEBG))
        appendLog(QString("%1: received %2 bytes transcoding data."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(bytesAvailable()));

    if (!m_opened && bytesAvailable() >= maxBufferSize())
    {
        m_opened = true;
        emit openedSignal();
    }

    // manage buffer
    if (m_process.state() == QProcess::Running) {
        if (bytesAvailable() > maxBufferSize() && !m_paused)
            pause();
    }
}

qint64 TranscodeProcess::size() const
{
    if (range() && range()->getLength()!=-1)
        return range()->getLength();

    if (m_size!=-1)
        return m_size;

    return m_process.size();
}

bool TranscodeProcess::atEnd() const
{
//    if (m_range && m_range->getEndByte()!=-1)
//        return pos() > m_range->getEndByte();

    if (bytesAvailable()>0)
        return false;

    if (m_paused or m_process.state() == QProcess::Running)
        return false;
    else
        return m_process.atEnd();
}

QByteArray TranscodeProcess::read(qint64 maxlen)
{
    //    if (m_range && m_range->getEndByte()>0)
    //    {
    //        int bytesToRead = m_range->getEndByte() - pos() + 1;
    //        if (bytesToRead>=0 && bytesToRead<maxSize)
    //            return QFile::read(bytesToRead);
    //    }

    QByteArray data;
     if (m_opened)
         data = m_process.read(maxlen);
    m_pos += data.size();
    emit status(QString("Transcoding (%1%)").arg(progress()));

    if (m_paused && m_process.state() != QProcess::NotRunning && bytesAvailable() < (maxBufferSize()*0.5))
        resume();

    return data;
}

void TranscodeProcess::appendTranscodingLogMessage() {
    // incoming log message
    QByteArray msg(m_process.readAllStandardError());
    appendLog(msg);
}

void TranscodeProcess::errorTrancodedData(const QProcess::ProcessError &error) {
    Q_UNUSED(error);

    // trancoding failed
    if (killTranscodeProcess == false) {
        // an error occured
        appendLog(QString("%2: ERROR Transcoding: %1."+CRLF).arg(m_process.errorString()).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
//        m_process.setErrorString(m_process.errorString());
        emit errorRaised(m_process.errorString());
    }
}

void TranscodeProcess::finishedTranscodeData(const int &exitCode) {
    appendLog(QString("%2: TRANSCODE FINISHED with exitCode %1."+CRLF).arg(exitCode).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
    appendLog(QString("%2: TRANSCODING DONE in %1 ms."+CRLF).arg(QTime(0, 0).addMSecs(transcodeClock.elapsed()).toString("hh:mm:ss")).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

    if (!m_opened)
    {
        m_opened = true;
        emit openedSignal();
    }

    m_paused = false;

    if (isLogLevel(DEBG))
        appendLog(QString("%1: finished transcoding, %2 remaining bytes."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(bytesAvailable()));

    if (isKilled() == false)
    {
        if (exitCode != 0)
        {
            // trancoding failed
//            m_process.setErrorString(QString("Transcoding finished with status %1").arg(exitCode));
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

void TranscodeProcess::processStarted()
{
    logDebug(QString("Transcoding process %1 %2").arg(m_process.program()).arg(m_process.arguments().join(' ')));
    appendLog(m_process.program()+' ');
    appendLog(m_process.arguments().join(' ')+CRLF);

    transcodeClock.start();
}

void TranscodeProcess::killProcess()
{
    if (m_process.state() != QProcess::NotRunning) {
        appendLog(QString("%1: KILL transcoding process."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
        killTranscodeProcess = true;
        m_process.kill();
    }
}

void TranscodeProcess::_pause()
{
    qint64 pid = m_process.processId();
    if (!m_paused && m_process.state() != QProcess::NotRunning && pid > 0) {
        if (processPauseResume.arguments().isEmpty())
        {
            logDebug(QString("Pause transcoding (pid: %1)").arg(pid));
            if (isLogLevel(DEBG))
                appendLog(QString("%1: PAUSE TRANSCODING"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

            QStringList arguments;
            arguments << "-STOP" << QString("%1").arg(pid);

            processPauseResume.start("kill", arguments);
        }
    }
}

void TranscodeProcess::_resume()
{
    qint64 pid = m_process.processId();
    if (m_paused && m_process.state() != QProcess::NotRunning && pid > 0) {
        if (processPauseResume.arguments().isEmpty())
        {
            logDebug(QString("Restart transcoding (pid: %1)").arg(pid));
            if (isLogLevel(DEBG))
                appendLog(QString("%1: RESUME TRANSCODING"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

            QStringList arguments;
            arguments << "-CONT" << QString("%1").arg(pid);

            processPauseResume.start("kill", arguments);
        }
    }
}

void TranscodeProcess::_pause_resume_done(const int &exitCode)
{
    Q_UNUSED(exitCode)

    if (processPauseResume.exitStatus() == QProcess::NormalExit)
    {
        if (processPauseResume.arguments().at(0) == "-STOP")
            m_paused = true;
        else if (processPauseResume.arguments().at(0) == "-CONT")
            m_paused = false;
        else
            logError("unknown arguments for pause or resume process");
    }
    else
    {
        logError("process pause or resume crashed");
    }
    processPauseResume.setArguments(QStringList());
}

void TranscodeProcess::_pause_resume_error(const QProcess::ProcessError &error)
{
    Q_UNUSED(error)

    logError(processPauseResume.errorString());
}
