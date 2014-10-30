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

    bool isKilled() const { return killTranscodeProcess; }
    void killProcess();

    virtual int transcodeExitCode() const { return exitCode(); }
    void setBufferSize(const qint64 &size) { if (size>0) maxBufferSize = size; }

    void setUrl(const QString &url)                             { m_url = url;                                  updateArguments(); }
    QString url() const { return m_url; }

    void setRange(HttpRange *range);
    HttpRange * range() const { return m_range; }

    void setTimeSeek(qint64 start, qint64 end)                  { timeseek_start = start; timeseek_end = end;   updateArguments();}
    qint64 timeSeekStart() const { return timeseek_start; }
    qint64 timeSeekEnd() const { return timeseek_end; }

    virtual bool atEnd() const;

    virtual qint64 pos() const { return m_pos; }
    virtual qint64 readData(char *data, qint64 maxlen);


private:
    bool isLogLevel(const LogLevel &level) const { return m_log ? m_log->isLevel(level) : false; }
    void logError(const QString &message)  const { if (m_log) m_log->Error(message); }
    void logDebug(const QString &message)  const { if (m_log) m_log->Debug(message); }
    void logInfo(const QString &message)   const { if (m_log) m_log->Info(message); }
    void logTrace(const QString &message)  const { if (m_log) m_log->Trace(message); }

    virtual void updateArguments() = 0;

    bool pause();
    bool resume();

signals:
    void LogMessage(const QString &msg);
    void status(const QString &status);
    void errorRaised(const QString &errorString);

private slots:
    void _logDestroyed() { m_log = 0; }

    void processStarted();
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

    QString m_url;
    HttpRange *m_range;
    qint64 timeseek_start;
    qint64 timeseek_end;

    qint64 m_pos;

    QElapsedTimer transcodeClock;
    bool killTranscodeProcess;  // true if the application aborts the transcoding
    bool m_paused;              // true if the transcoding has been paused
    qint64 maxBufferSize;
};

#endif // TRANSCODEPROCESS_H
