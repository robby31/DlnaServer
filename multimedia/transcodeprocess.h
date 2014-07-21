#ifndef TRANSCODEPROCESS_H
#define TRANSCODEPROCESS_H

#include <QProcess>
#include <QElapsedTimer>
#include <QDebug>
#include <QDateTime>

#include "logger.h"
#include "httprange.h"

// Format available for transcoding
enum TranscodeFormatAvailable {UNKNOWN, MP3, LPCM, MPEG2_AC3};

class TranscodeProcess : public QProcess
{
    Q_OBJECT

public:
    explicit TranscodeProcess(Logger* log, QObject *parent = 0);

    virtual void launch();
    bool pause();
    bool resume();

    QString getTranscodeLog() const { return transcodeLog; }
    void appendLog(const QString &msg) { transcodeLog.append(msg+CRLF); }

    bool isKilled() const { return killTranscodeProcess; }
    void killProcess();

signals:

public slots:
    void errorTrancodedData(const QProcess::ProcessError &error);
    void receivedTranscodingLogMessage();
    void finishedTranscodeData(const int &exitCode);

private:
    // Carriage return and line feed.
    static const QString CRLF;

    Logger *m_log;
    QElapsedTimer transcodeClock;
    QString transcodeLog;
    bool killTranscodeProcess;  // true if the application aborts the transcoding
    bool m_paused;   // true if the transcoding has been paused
};

#endif // TRANSCODEPROCESS_H
