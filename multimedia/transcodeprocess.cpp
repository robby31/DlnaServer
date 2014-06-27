#include "transcodeprocess.h"

const QString TranscodeProcess::CRLF = "\r\n";

TranscodeProcess::TranscodeProcess(QObject *parent) :
    QProcess(parent),
    transcodeClock(),
    transcodeLog(),
    killTranscodeProcess(false),
    m_paused(false)
{
    connect(this, SIGNAL(readyReadStandardError()), this, SLOT(receivedTranscodingLogMessage()));
    connect(this, SIGNAL(error(QProcess::ProcessError)), this, SLOT(errorTrancodedData(QProcess::ProcessError)));
    connect(this, SIGNAL(finished(int)), this, SLOT(finishedTranscodeData(int)));
}

void TranscodeProcess::receivedTranscodingLogMessage() {
    // incoming log message
    transcodeLog.append(this->readAllStandardError());
}

void TranscodeProcess::errorTrancodedData(QProcess::ProcessError error) {
    Q_UNUSED(error);

    // trancoding failed
    if (killTranscodeProcess == false) {
        // an error occured
        transcodeLog.append(QString("ERROR Transcoding: %1."+CRLF).arg(errorString()));
    }
}

void TranscodeProcess::finishedTranscodeData(int exitCode) {
    transcodeLog.append(QString("TRANSCODE FINISHED with exitCode %1."+CRLF).arg(exitCode));
    transcodeLog.append(QString("TRANCODING done in %1 ms."+CRLF).arg(transcodeClock.elapsed()));
}

void TranscodeProcess::launch() {
    transcodeLog.append(program()+' ');
    transcodeLog.append(arguments().join(' ')+CRLF);

    transcodeClock.start();

    start();
}

void TranscodeProcess::killProcess() {
    if (state() != QProcess::NotRunning) {
        transcodeLog.append(CRLF+"KILL transcoding process."+CRLF);
        killTranscodeProcess = true;
        kill();
    }
}

bool TranscodeProcess::pause() {
    if (!m_paused) {
        qWarning() << QString("%1 pause transcoding").arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")) << state();

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
    if (m_paused) {
        qWarning() << QString("%1 restart transcoding").arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")) << state();

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
