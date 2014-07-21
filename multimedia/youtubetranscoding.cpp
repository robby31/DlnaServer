#include "youtubetranscoding.h"

YouTubeTranscoding::YouTubeTranscoding(Logger *log, QObject *parent) :
    MencoderTranscoding(log, parent),
    youtubeStreaming(this)
{
    youtubeStreaming.setProgram("/usr/local/bin/youtube-dl");
}

bool YouTubeTranscoding::initialize(HttpRange *range, const long &timeseek_start, const long &timeseek_end, const QString &filePath, const int &lengthInSeconds, const TranscodeFormatAvailable &transcodeFormat, const int &bitrate, const QStringList &audioLanguages, const QStringList &subtitleLanguages, const QString &framerate)
{
    QStringList argYoutube;
    argYoutube << "-o";
    argYoutube << "-";
    argYoutube << filePath;
    youtubeStreaming.setArguments(argYoutube);

    youtubeStreaming.setStandardOutputProcess(this);

    return MencoderTranscoding::initialize(range, timeseek_start, timeseek_start, "-", lengthInSeconds, transcodeFormat, bitrate, audioLanguages, subtitleLanguages, framerate);
}

void YouTubeTranscoding::launch()
{
    youtubeStreaming.start();
    TranscodeProcess::launch();
}
