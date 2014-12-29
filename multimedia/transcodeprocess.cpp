#include "transcodeprocess.h"

const QString TranscodeProcess::CRLF = "\r\n";

TranscodeProcess::TranscodeProcess(Logger *log, QObject *parent) :
    Device(log, parent),
    m_process(this),
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
    m_durationBuffer(10),
    m_lengthInSeconds(-1),
    m_format(UNKNOWN),
    m_audioLanguages(),
    m_subtitleLanguages(),
    m_frameRate(""),
    m_audioChannelCount(-1),
    m_audioSampleRate(-1)
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

    connect(&processPauseResume, SIGNAL(error(QProcess::ProcessError)), this, SLOT(_pause_resume_error(QProcess::ProcessError)));
}

TranscodeProcess::~TranscodeProcess()
{
    if (processPauseResume.state() == QProcess::Running)
    {
        processPauseResume.kill();
        if (!processPauseResume.waitForFinished(1000))
            logError("Unable to stop pause/resume for TranscodeProcess.");
    }

    if (m_process.state() == QProcess::Running)
    {
        killProcess();
        if (!m_process.waitForFinished(1000))
            logError("Unable to stop TranscodeProcess.");
    }
}

void TranscodeProcess::_open(const QIODevice::OpenMode &open)
{
    updateArguments();

//    if (range())
//    {
//        if (range()->getStartByte() >= 0)
//            m_pos = range()->getStartByte();
//        else
//            m_pos = range()->getEndByte() + 1;   // invalid range
//    }

    m_process.open(open);
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

    if (m_opened)
        emit readyRead();
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
//    if (range() && range()->getEndByte()!=-1)
//        return pos() > range()->getEndByte();

    if (!isOpen() or bytesAvailable()>0 or m_process.state() == QProcess::Running)
        return false;
    else
        return m_process.atEnd();
}

QByteArray TranscodeProcess::read(qint64 maxlen)
{
    QByteArray data;
     if (m_opened)
         data = m_process.read(maxlen);
     m_pos += data.size();

    if (m_paused && m_process.state() != QProcess::NotRunning && bytesAvailable() < (maxBufferSize()*0.5))
        resume();

    if (m_paused)
        emit status(QString("Transcoding paused (%1%)").arg(progress()));
    else
        emit status(QString("Transcoding (%1%)").arg(progress()));

    return data;
}

void TranscodeProcess::appendTranscodingLogMessage() {
    // incoming log message
    QByteArray msg(m_process.readAllStandardError());
    appendLog(msg);
}

void TranscodeProcess::errorTrancodedData(const QProcess::ProcessError &error)
{
    // trancoding failed
    if (killTranscodeProcess == false) {
        // an error occured
        appendLog(QString("%2: ERROR Transcoding at %4% : error n°%3 - %1."+CRLF).arg(m_process.errorString()).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(error).arg(progress()));
        if (progress()<95)
            emit errorRaised(m_process.errorString());
        else
            appendLog(QString("%1: ERROR ignored."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
    }
}

void TranscodeProcess::finishedTranscodeData(const int &exitCode) {
    appendLog(QString("%2: TRANSCODING FINISHED with exitCode %1."+CRLF).arg(exitCode).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
    appendLog(QString("%2: %3% TRANSCODING DONE in %1 ms."+CRLF).arg(QTime(0, 0).addMSecs(transcodeClock.elapsed()).toString("hh:mm:ss")).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(progress()));

    if (!m_opened)
    {
        m_opened = true;
        emit openedSignal();
        emit readyRead();
    }

    m_paused = false;

    if (isLogLevel(DEBG))
        appendLog(QString("%1: finished transcoding, %2 remaining bytes."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(bytesAvailable()));

    if (isKilled() == false)
    {
        if (exitCode != 0)
        {
            // trancoding failed
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
    if (!m_paused && m_process.state() != QProcess::NotRunning && pid > 0)
    {
        logDebug(QString("Pause transcoding (pid: %1)").arg(pid));
        if (isLogLevel(DEBG))
            appendLog(QString("%1: PAUSE TRANSCODING"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

        QStringList arguments;
        arguments << "-STOP" << QString("%1").arg(pid);

        processPauseResume.start("kill", arguments);
        if (processPauseResume.waitForFinished())
        {
            m_paused = true;
            emit status(QString("Transcoding paused (%1%)").arg(progress()));
        }
        else
        {
            logError("ERROR: unable to pause transcoding process.");
        }
    }
    else
    {
        logError("ERROR: unable to pause transcoding process.");
    }
}

void TranscodeProcess::_resume()
{
    qint64 pid = m_process.processId();
    if (m_paused && m_process.state() != QProcess::NotRunning && pid > 0)
    {
        logDebug(QString("Restart transcoding (pid: %1)").arg(pid));
        if (isLogLevel(DEBG))
            appendLog(QString("%1: RESUME TRANSCODING"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

        QStringList arguments;
        arguments << "-CONT" << QString("%1").arg(pid);

        processPauseResume.start("kill", arguments);
        if (processPauseResume.waitForFinished())
        {
            m_paused = false;
            emit status(QString("Transcoding (%1%)").arg(progress()));
        }
        else
        {
            logError("ERROR: unable to resume transcoding process.");
        }
    }
    else
    {
        logError("ERROR: unable to resume transcoding process.");
    }
}

void TranscodeProcess::_pause_resume_error(const QProcess::ProcessError &error)
{
    Q_UNUSED(error)

    logError(processPauseResume.errorString());
}

