#ifndef YOUTUBETRANSCODING_H
#define YOUTUBETRANSCODING_H

#include "transcodeprocess.h"

class YouTubeTranscoding : public TranscodeProcess
{
    Q_OBJECT

public:
    explicit YouTubeTranscoding(Logger* log, QObject *parent = 0);

private:
    virtual void updateArguments();

signals:

public slots:

};

#endif // YOUTUBETRANSCODING_H
