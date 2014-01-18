#ifndef FFMPEGTRANSCODING_H
#define FFMPEGTRANSCODING_H

#include "transcodeprocess.h"

class FfmpegTranscoding : public TranscodeProcess
{
    Q_OBJECT

public:
    explicit FfmpegTranscoding(QObject *parent = 0);

    // initialize program pathname and arguments of the QProcess
    virtual bool initialize(HttpRange* range, QString filePath, int lengthInSeconds, TranscodeFormatAvailable transcodeFormat, int bitrate, QStringList audioLanguages, QStringList subtitleLanguages, QString framerate);

signals:

public slots:

};

#endif // FFMPEGTRANSCODING_H
