#ifndef TRANSCODEPROCESS_H
#define TRANSCODEPROCESS_H

#include <QProcess>
#include <QElapsedTimer>
#include <QDebug>
#include <QDateTime>

#include "device.h"

// Format available for transcoding
enum TranscodeFormatAvailable {UNKNOWN, MP3, LPCM, AAC, MPEG2_AC3};

class TranscodeProcess : public Device
{
    Q_OBJECT

public:
    explicit TranscodeProcess(Logger* log, QObject *parent = 0);
    virtual ~TranscodeProcess();

    bool waitForFinished(int msecs = 30000) { return m_process.waitForFinished(msecs); }
    int exitCode()  const { return m_process.exitCode(); }
    bool isKilled() const { return killTranscodeProcess; }

    void setBitrate(const qint64 &bitrate)                  { m_bitrate = bitrate;
                                                              setMaxBufferSize(m_bitrate/8*m_durationBuffer);
                                                             }
    qint64 bitrate() const { return m_bitrate; }

    qint64 maxBufferSize() { return m_maxBufferSize; }

    void setUrl(const QString &url)                             { m_url = url; }
    QString url() const { return m_url; }

    void setSize(const qint64 size) { m_size = size; }
    virtual qint64 size() const;

    qint64 lengthInSeconds() const                          { return m_lengthInSeconds; }
    void setLengthInSeconds(const qint64 length)            { m_lengthInSeconds = length; }

    TranscodeFormatAvailable format() const                 { return m_format; }
    void setFormat(const TranscodeFormatAvailable &format)  { m_format = format; }

    QStringList audioLanguages() const                      { return m_audioLanguages; }
    void setAudioLanguages(const QStringList &languages)    { m_audioLanguages = languages; }

    QStringList subtitleLanguages() const                   { return m_subtitleLanguages; }
    void setSubtitleLanguages(const QStringList &languages) { m_subtitleLanguages = languages; }

    QString frameRate() const                           { return m_frameRate; }
    void setFrameRate(const QString &rate)              { m_frameRate = rate; }

    int audioChannelCount() const                       { return m_audioChannelCount; }
    void setAudioChannelCount(const int &channel)       { m_audioChannelCount = channel; }

    int audioSampleRate() const                         { return m_audioSampleRate; }
    void setAudioSampleRate(const int &rate)            { m_audioSampleRate = rate; }

    virtual bool atEnd() const;
    virtual qint64 bytesAvailable() const   { return m_process.bytesAvailable(); }
    virtual qint64 pos() const              { return m_pos; }

    virtual bool open()         { emit openSignal(QIODevice::ReadOnly); return true; }
    virtual bool isOpen() const { return m_opened; }

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
    void readyRead();

private slots:
    void _open(const QIODevice::OpenMode &open);
    void processStarted();
    void dataAvailable();
    void errorTrancodedData(const QProcess::ProcessError &error);
    void appendTranscodingLogMessage();
    void finishedTranscodeData(const int &exitCode);
    void killProcess();

    void _pause();
    void _resume();
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

    qint64 m_lengthInSeconds;
    TranscodeFormatAvailable m_format;

    QStringList m_audioLanguages;
    QStringList m_subtitleLanguages;
    QString m_frameRate;
    int m_audioChannelCount;
    int m_audioSampleRate;
};

#endif // TRANSCODEPROCESS_H
