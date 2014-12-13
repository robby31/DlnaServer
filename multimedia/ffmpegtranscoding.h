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

private:
    virtual void updateArguments();

signals:

public slots:

private:
    static const QString PROGRAM;

    qint64 m_lengthInSeconds;
    TranscodeFormatAvailable m_format;
};

#endif // FFMPEGTRANSCODING_H
