#ifndef TRANSCODEPROCESS_H
#define TRANSCODEPROCESS_H

#include <QProcess>
#include <QElapsedTimer>
#include <QDebug>
#include <QDateTime>

#include "device.h"

// Format available for transcoding
enum TranscodeFormatAvailable {UNKNOWN, MP3, LPCM, MPEG2_AC3};

class TranscodeProcess : public Device
{
    Q_OBJECT

public:
    explicit TranscodeProcess(Logger* log, QObject *parent = 0);
    virtual ~TranscodeProcess();

    bool isKilled() const { return killTranscodeProcess; }

    void setBitrate(const qint64 &bitrate)                  { m_bitrate = bitrate;
                                                              setMaxBufferSize(m_bitrate/8*m_durationBuffer);
                                                              updateArguments(); }
    qint64 bitrate() const { return m_bitrate; }

    qint64 maxBufferSize() { return m_maxBufferSize; }

    void setUrl(const QString &url)                             { m_url = url;                                  updateArguments(); }
    QString url() const { return m_url; }

    void setSize(const qint64 size) { m_size = size; }
    virtual qint64 size() const;

    virtual bool atEnd() const;
    virtual qint64 bytesAvailable() const   { return m_process.bytesAvailable(); }
    virtual qint64 pos() const              { return m_pos; }

    virtual bool open()         { emit openSignal(QIODevice::ReadOnly); return true; }
    virtual bool isOpen() const { return m_process.isOpen(); }

    virtual QByteArray read(qint64 maxlen);


protected:
    void setProgram(const QString &program)         { m_process.setProgram(program);        }
    void setArguments(const QStringList &arguments) { m_process.setArguments(arguments);    }

private:
    void setMaxBufferSize(const qint64 &size)       { if (size>0) m_maxBufferSize = size;   }

    void pause()    { emit pauseSignal();  }
    void resume()   { emit resumeSignal(); }

signals:
    void openSignal(const QIODevice::OpenMode &open);
    void pauseSignal();
    void resumeSignal();

private slots:
    void _open(const QIODevice::OpenMode &open) { m_process.open(open); }
    void processStarted();
    void dataAvailable();
    void errorTrancodedData(const QProcess::ProcessError &error);
    void appendTranscodingLogMessage();
    void finishedTranscodeData(const int &exitCode);
    void killProcess();

    void _pause();
    void _resume();
    void _pause_resume_done(const int &exitCode);
    void _pause_resume_error(const QProcess::ProcessError &error);


private:
    // Carriage return and line feed.
    static const QString CRLF;

    QProcess m_process;
    bool m_opened;
    QString m_url;

    qint64 m_bitrate;

    qint64 m_pos;
    qint64 m_size;

    QProcess processPauseResume;
    QElapsedTimer transcodeClock;
    bool killTranscodeProcess;  // true if the application aborts the transcoding
    bool m_paused;              // true if the transcoding has been paused
    qint64 m_maxBufferSize;
    int m_durationBuffer;       // when bitrate is known, m_maxBufferSize is set to m_durationBuffer seconds of streaming
};

#endif // TRANSCODEPROCESS_H
