#ifndef TRANSCODEPROCESS_H
#define TRANSCODEPROCESS_H

#include <QProcess>
#include <QElapsedTimer>
#include <QDebug>
#include <QDateTime>

#include "device.h"

// Format available for transcoding
enum TranscodeFormatAvailable {UNKNOWN, MP3, LPCM, AAC, ALAC, WAV, MPEG2_AC3, H264_AAC, H264_AC3};

class TranscodeProcess : public Device
{
    Q_OBJECT

public:
    explicit TranscodeProcess(Logger* log, QObject *parent = 0);
    virtual ~TranscodeProcess();

    bool waitForFinished(int msecs = 30000) { return m_process->waitForFinished(msecs); }
    int exitCode()  const { return m_process->exitCode(); }
    bool isKilled() const { return killTranscodeProcess; }

    QString url() const { return m_url; }

    void setSize(const qint64 size);
    virtual qint64 size() const Q_DECL_OVERRIDE;

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

    void setVolumeInfo(const QHash<QString, double> info);
    QHash<QString, double> volumeInfo() const { return m_volumeInfo; }

    virtual bool atEnd() const Q_DECL_OVERRIDE;
    virtual qint64 bytesAvailable() const Q_DECL_OVERRIDE  { return m_process->bytesAvailable(); }
    virtual qint64 pos() const  Q_DECL_OVERRIDE            { return m_pos; }    // position in bytes of read data

    virtual bool open() Q_DECL_OVERRIDE;
    virtual bool isOpen() const Q_DECL_OVERRIDE { return m_opened; }

    virtual bool isReadyToOpen() const Q_DECL_OVERRIDE;

protected:
    void setProgram(const QString &program)         { m_process->setProgram(program);        }
    void setArguments(const QStringList &arguments) { m_process->setArguments(arguments);    }

private:
    virtual QByteArray read(qint64 maxlen) Q_DECL_OVERRIDE;

    qint64 transcodedPos() const { return pos() + bytesAvailable(); }  // position in bytes of transcoded data
    qint64 transcodedProgress() const;

signals:
    void openSignal(const QIODevice::OpenMode &open);

public slots:
    void setUrl(const QString &url);
    
private slots:
    void _open(const QIODevice::OpenMode &open);
    void processStarted();
    void dataAvailable();
    void errorTrancodedData(const QProcess::ProcessError &error);
    void appendTranscodingLogMessage();
    void finishedTranscodeData(const int &exitCode, const QProcess::ExitStatus &exitStatus);
    void killProcess();

    void pause();
    void resume();


private:
    // Carriage return and line feed.
    static const QString CRLF;

    QProcess *m_process;
    bool m_opened;
    QString m_url;

    qint64 m_pos;
    qint64 m_size;

    QElapsedTimer transcodeClock;
    bool killTranscodeProcess;  // true if the application aborts the transcoding
    bool m_paused;              // true if the transcoding has been paused

    qint64 m_lengthInSeconds;
    TranscodeFormatAvailable m_format;

    QStringList m_audioLanguages;
    QStringList m_subtitleLanguages;
    QString m_frameRate;
    int m_audioChannelCount;
    int m_audioSampleRate;
    QHash<QString, double> m_volumeInfo;
};

#endif // TRANSCODEPROCESS_H
