#ifndef FFMPEGTRANSCODING_H
#define FFMPEGTRANSCODING_H

#include "transcodeprocess.h"

class FfmpegTranscoding : public TranscodeProcess
{
    Q_OBJECT

public:
    explicit FfmpegTranscoding(Logger* log, QObject *parent = 0);

    // initialize program pathname and arguments of the QProcess
    virtual bool initialize(HttpRange* range, const long &timeseek_start, const long &timeseek_end,
                            const QString &filePath, const int &lengthInSeconds, const TranscodeFormatAvailable &transcodeFormat, const int &bitrate);

signals:

public slots:

};

#endif // FFMPEGTRANSCODING_H
