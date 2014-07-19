#ifndef MENCODERTRANSCODING_H
#define MENCODERTRANSCODING_H

#include "transcodeprocess.h"

class MencoderTranscoding : public TranscodeProcess
{
    Q_OBJECT

public:
    explicit MencoderTranscoding(Logger* log, QObject *parent = 0);

    // initialize program pathname and arguments of the QProcess
    virtual bool initialize(HttpRange* range, const long &timeseek_start, const long &timeseek_end,
                            const QString &filePath, const int &lengthInSeconds, const TranscodeFormatAvailable &transcodeFormat, const int &bitrate,
                            const QStringList &audioLanguages, const QStringList &subtitleLanguages, const QString &framerate);

signals:

public slots:

};

#endif // MENCODERTRANSCODING_H
