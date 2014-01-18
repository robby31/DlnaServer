#include "transcodeprocess.h"

const QString TranscodeProcess::CRLF = "\r\n";

TranscodeProcess::TranscodeProcess(QObject *parent) :
    QProcess(parent),
    killTranscodeProcess(false)
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
    transcodeLog.append(CRLF+"KILL transcoding process."+CRLF);
    killTranscodeProcess = true;
    kill();
}

bool TranscodeProcess::pause() {
    QStringList arguments;
    arguments << "-STOP" << QString("%1").arg(pid());
    QProcess pauseTrancodeProcess;
    pauseTrancodeProcess.setProgram("kill");
    pauseTrancodeProcess.setArguments(arguments);
    pauseTrancodeProcess.start();
    return pauseTrancodeProcess.waitForFinished();
}

bool TranscodeProcess::resume() {
    QStringList arguments;
    arguments << "-CONT" << QString("%1").arg(pid());
    QProcess continueTrancodeProcess;
    continueTrancodeProcess.setProgram("kill");
    continueTrancodeProcess.setArguments(arguments);
    continueTrancodeProcess.start();
    return continueTrancodeProcess.waitForFinished();
}
