#ifndef FFMPEGTRANSCODING_H
#define FFMPEGTRANSCODING_H

#include "transcodeprocess.h"

class FfmpegTranscoding : public TranscodeProcess
{
    Q_OBJECT

public:
    explicit FfmpegTranscoding(Logger* log, QObject *parent = 0);

    void setLengthInSeconds(const qint64 length)            { m_lengthInSeconds = length;   updateArguments(); }
    void setFormat(const TranscodeFormatAvailable &format)  { m_format = format;            updateArguments(); }
    void setBitrate(const qint64 &bitrate)                  { m_bitrate = bitrate;          updateArguments(); }

    static const QString PROGRAM;

private:
    virtual void updateArguments();

signals:

public slots:

private:
    qint64 m_lengthInSeconds;
    TranscodeFormatAvailable m_format;
    qint64 m_bitrate;
};

#endif // FFMPEGTRANSCODING_H
