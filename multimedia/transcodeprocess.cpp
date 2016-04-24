#include "transcodeprocess.h"

const QString TranscodeProcess::CRLF = "\r\n";

TranscodeProcess::TranscodeProcess(Logger *log, QObject *parent) :
    Device(log, parent),
    m_process(this),
    m_opened(false),
    m_url(),
    m_pos(0),
    m_size(-1),
    processPauseResume(this),
    transcodeClock(),
    killTranscodeProcess(false),
    m_paused(false),
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
    connect(&m_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finishedTranscodeData(int,QProcess::ExitStatus)));

    connect(&processPauseResume, SIGNAL(error(QProcess::ProcessError)), this, SLOT(_pause_resume_error(QProcess::ProcessError)));
}

TranscodeProcess::~TranscodeProcess()
{
    QString msg = QString("DESTROY TranscodeProcess, bytes available:%1, state:%2, paused?%3, durationBuffer:%4, maxBufferSize:%5").arg(bytesAvailable()).arg(m_process.state()).arg(m_paused).arg(durationBuffer()).arg(maxBufferSize());
    logDebug(msg);

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
            logError(QString("Unable to stop TranscodeProcess (Pause flag %1).").arg(m_paused));
    }
}

void TranscodeProcess::_open(const QIODevice::OpenMode &open)
{
    updateArguments();

    m_process.open(open);
}

void TranscodeProcess::dataAvailable()
{
    if (isLogLevel(DEBG))
        appendLog(QString("%1: received %2 bytes transcoding data."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(bytesAvailable()));

    if (!m_opened && bytesAvailable() > 0)
    {
        m_opened = true;
        emit openedSignal();
    }

    // manage buffer
    if (m_process.state() == QProcess::Running)
    {
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

void TranscodeProcess::setSize(const qint64 size)
{
    if (size > 0)
        m_size = size;
    else
        qWarning() << "unable to set size" << size;
}

bool TranscodeProcess::atEnd() const
{
    if (!isOpen() or bytesAvailable()>0 or m_process.state() == QProcess::Running)
        return false;
    else
        return m_process.atEnd();
}

QByteArray TranscodeProcess::read(qint64 maxlen)
{
    qint64 oldProgress = progress();

    QByteArray data;
     if (m_opened)
         data = m_process.read(maxlen);
     m_pos += data.size();

    if (m_paused && m_process.state() != QProcess::NotRunning && bytesAvailable() < (maxBufferSize()*0.75))
        resume();

    qint64 newProgress = progress();

    if (oldProgress != newProgress)
    {
        if (m_paused)
            emit status(QString("Transcoding paused (%1%)").arg(newProgress));
        else
            emit status(QString("Transcoding (%1%)").arg(newProgress));
    }

    return data;
}

void TranscodeProcess::appendTranscodingLogMessage()
{
    // incoming log message
    QByteArray msg(m_process.readAllStandardError());
    appendLog(msg);
}

void TranscodeProcess::errorTrancodedData(const QProcess::ProcessError &error)
{
    // trancoding failed
    if (killTranscodeProcess == false)
    {
        // an error occured
        appendLog(QString("%2: ERROR Transcoding at %4% : error n°%3 - %1."+CRLF).arg(m_process.errorString()).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(error).arg(transcodedProgress()));
        emit errorRaised(m_process.errorString());
    }
}

void TranscodeProcess::finishedTranscodeData(const int &exitCode, const QProcess::ExitStatus &exitStatus)
{
    if (exitStatus == QProcess::NormalExit)
        appendLog(QString("%2: TRANSCODING FINISHED with exitCode %1."+CRLF).arg(exitCode).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
    else
        appendLog(QString("%1: TRANSCODING CRASHED."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
    appendLog(QString("%2: %3% TRANSCODING DONE in %1 seconds."+CRLF).arg(QTime(0, 0).addMSecs(transcodeClock.elapsed()).toString("hh:mm:ss")).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(transcodedProgress()));

    if (!m_opened)
    {
        m_opened = true;
        emit openedSignal();
        emit readyRead();
    }

    m_paused = false;

    if (atEnd())
        emit endReached();

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

void TranscodeProcess::pause()
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
        if (processPauseResume.waitForFinished(1000))
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

void TranscodeProcess::resume()
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
        if (processPauseResume.waitForFinished(1000))
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

qint64 TranscodeProcess::transcodedProgress() const
{
    if (size()==0)
        return 0;
    else
        return qint64(100.0*double(transcodedPos())/double(size()));
}
