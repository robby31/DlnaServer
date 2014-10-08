#ifndef YOUTUBETRANSCODING_H
#define YOUTUBETRANSCODING_H

#include "transcodeprocess.h"

class YouTubeTranscoding : public TranscodeProcess
{
    Q_OBJECT

public:
    explicit YouTubeTranscoding(Logger* log, QObject *parent = 0);

    // initialize program pathname and arguments of the QProcess
    bool initialize(HttpRange* range, const long &timeseek_start, const long &timeseek_end,
                    const QString &filePath, const int &lengthInSeconds, const TranscodeFormatAvailable &transcodeFormat, const int &bitrate,
                    const QStringList &audioLanguages, const QStringList &subtitleLanguages, const QString &framerate);

signals:

public slots:

private:
    QString url;
};

#endif // YOUTUBETRANSCODING_H
