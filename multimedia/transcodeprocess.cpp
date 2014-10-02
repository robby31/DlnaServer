#include "transcodeprocess.h"

const QString TranscodeProcess::CRLF = "\r\n";

TranscodeProcess::TranscodeProcess(Logger *log, QObject *parent) :
    QProcess(parent),
    m_log(log != 0 ? log : new Logger(this)),
    transcodeClock(),
    transcodeLog(),
    killTranscodeProcess(false),
    m_paused(false)
{
    connect(this, SIGNAL(readyReadStandardError()), this, SLOT(appendTranscodingLogMessage()));
    connect(this, SIGNAL(error(QProcess::ProcessError)), this, SLOT(errorTrancodedData(QProcess::ProcessError)));
    connect(this, SIGNAL(finished(int)), this, SLOT(finishedTranscodeData(int)));
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
    }
}

void TranscodeProcess::finishedTranscodeData(const int &exitCode) {
    appendLog(QString("%2: TRANSCODE FINISHED with exitCode %1."+CRLF).arg(exitCode).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
    appendLog(QString("%2: TRANSCODING DONE in %1 ms."+CRLF).arg(QTime(0, 0).addMSecs(transcodeClock.elapsed()).toString("hh:mm:ss")).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
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
