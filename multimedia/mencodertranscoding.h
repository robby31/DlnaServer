#ifndef MENCODERTRANSCODING_H
#define MENCODERTRANSCODING_H

#include "transcodeprocess.h"

class MencoderTranscoding : public TranscodeProcess
{
    Q_OBJECT

public:
    explicit MencoderTranscoding(Logger* log, QObject *parent = 0);

    // initialize program pathname and arguments of the QProcess
    virtual bool initialize(HttpRange* range, long timeseek_start, long timeseek_end, QString filePath, int lengthInSeconds, TranscodeFormatAvailable transcodeFormat, int bitrate, QStringList audioLanguages, QStringList subtitleLanguages, QString framerate);

signals:

public slots:

};

#endif // MENCODERTRANSCODING_H
