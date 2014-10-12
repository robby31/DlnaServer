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
    virtual ~TranscodeProcess();

    virtual void launch();

    bool isKilled() const { return killTranscodeProcess; }
    void killProcess();

    virtual int transcodeExitCode() const { return exitCode(); }
    void setBufferSize(const qint64 &size) { if (size>0) maxBufferSize = size; }

    virtual bool atEnd() const;

    virtual qint64 pos() const { return m_pos; }
    virtual qint64 readData(char *data, qint64 maxlen);


private:
    bool pause();
    bool resume();

signals:
    void LogMessage(const QString &msg);
    void status(const QString &status);
    void errorRaised(const QString &errorString);

private slots:
    void dataAvailable();
    void errorTrancodedData(const QProcess::ProcessError &error);
    void appendTranscodingLogMessage();
    void finishedTranscodeData(const int &exitCode);

protected:
    void appendLog(const QString &msg) { emit LogMessage(msg+CRLF); }

    Logger *m_log;

private:
    // Carriage return and line feed.
    static const QString CRLF;

    qint64 m_pos;

    QElapsedTimer transcodeClock;
    bool killTranscodeProcess;  // true if the application aborts the transcoding
    bool m_paused;              // true if the transcoding has been paused
    qint64 maxBufferSize;
};

#endif // TRANSCODEPROCESS_H
