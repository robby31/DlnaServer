#ifndef FFMPEGTRANSCODING_H
#define FFMPEGTRANSCODING_H

#include <QDir>
#include "transcodeprocess.h"

class FfmpegTranscoding : public TranscodeProcess
{
    Q_OBJECT

public:
    explicit FfmpegTranscoding(Logger* log, QObject *parent = 0);

    static void setDirPath(const QString &folder);

private:
    virtual void updateArguments();

signals:

public slots:

private:
    static QString EXE_DIRPATH;
    double audioVolumeTarget;
};

#endif // FFMPEGTRANSCODING_H
