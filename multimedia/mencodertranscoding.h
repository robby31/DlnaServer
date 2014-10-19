#ifndef MENCODERTRANSCODING_H
#define MENCODERTRANSCODING_H

#include "transcodeprocess.h"

class MencoderTranscoding : public TranscodeProcess
{
    Q_OBJECT

public:
    explicit MencoderTranscoding(Logger* log, QObject *parent = 0);

    void setLengthInSeconds(const qint64 length)            { m_lengthInSeconds = length;       updateArguments(); }
    void setFormat(const TranscodeFormatAvailable &format)  { m_format = format;                updateArguments(); }
    void setBitrate(const qint64 &bitrate)                  { m_bitrate = bitrate;              updateArguments(); }
    void setAudioLanguages(const QStringList &languages)    { m_audioLanguages = languages;     updateArguments(); }
    void setSubtitleLanguages(const QStringList &languages) { m_subtitleLanguages = languages;  updateArguments(); }
    void setFrameRate(const QString &rate)                  { m_frameRate = rate;               updateArguments(); }

private:
    virtual void updateArguments();

signals:

public slots:

private:
    qint64 m_lengthInSeconds;
    TranscodeFormatAvailable m_format;
    qint64 m_bitrate;
    QStringList m_audioLanguages;
    QStringList m_subtitleLanguages;
    QString m_frameRate;
};

#endif // MENCODERTRANSCODING_H
