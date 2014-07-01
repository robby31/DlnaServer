#ifndef FFMPEGTRANSCODING_H
#define FFMPEGTRANSCODING_H

#include "transcodeprocess.h"

class FfmpegTranscoding : public TranscodeProcess
{
    Q_OBJECT

public:
    explicit FfmpegTranscoding(Logger* log, QObject *parent = 0);

    // initialize program pathname and arguments of the QProcess
    virtual bool initialize(HttpRange* range, long timeseek_start, long timeseek_end, QString filePath, int lengthInSeconds, TranscodeFormatAvailable transcodeFormat, int bitrate);

signals:

public slots:

};

#endif // FFMPEGTRANSCODING_H
