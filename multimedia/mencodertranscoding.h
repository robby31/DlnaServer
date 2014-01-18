#ifndef MENCODERTRANSCODING_H
#define MENCODERTRANSCODING_H

#include "transcodeprocess.h"

class MencoderTranscoding : public TranscodeProcess
{
    Q_OBJECT

public:
    explicit MencoderTranscoding(QObject *parent = 0);

    // initialize program pathname and arguments of the QProcess
    virtual bool initialize(HttpRange* range, QString filePath, int lengthInSeconds, TranscodeFormatAvailable transcodeFormat, int bitrate);

signals:

public slots:

};

#endif // MENCODERTRANSCODING_H
