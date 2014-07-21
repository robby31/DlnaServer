#ifndef YOUTUBETRANSCODING_H
#define YOUTUBETRANSCODING_H

#include "mencodertranscoding.h"

class YouTubeTranscoding : public MencoderTranscoding
{
    Q_OBJECT
public:
    explicit YouTubeTranscoding(Logger* log, QObject *parent = 0);

    // initialize program pathname and arguments of the QProcess
    virtual bool initialize(HttpRange* range, const long &timeseek_start, const long &timeseek_end,
                            const QString &filePath, const int &lengthInSeconds, const TranscodeFormatAvailable &transcodeFormat, const int &bitrate,
                            const QStringList &audioLanguages, const QStringList &subtitleLanguages, const QString &framerate);

    virtual void launch();

signals:

public slots:

private:
    QProcess youtubeStreaming;
};

#endif // YOUTUBETRANSCODING_H
