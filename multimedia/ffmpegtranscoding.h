#ifndef FFMPEGTRANSCODING_H
#define FFMPEGTRANSCODING_H

#include "transcodeprocess.h"

class FfmpegTranscoding : public TranscodeProcess
{
    Q_OBJECT

public:
    explicit FfmpegTranscoding(Logger* log, QObject *parent = 0);

private:
    virtual void updateArguments();

signals:

public slots:

private:
    static const QString PROGRAM;
};

#endif // FFMPEGTRANSCODING_H
