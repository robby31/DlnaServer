#include "youtubetranscoding.h"

YouTubeTranscoding::YouTubeTranscoding(Logger *log, QObject *parent) :
    TranscodeProcess(log, parent),
    url()
{
    setProgram("/usr/local/bin/youtube-dl");
}

bool YouTubeTranscoding::initialize(HttpRange *range, const long &timeseek_start, const long &timeseek_end, const QString &filePath, const int &lengthInSeconds, const TranscodeFormatAvailable &transcodeFormat, const int &bitrate, const QStringList &audioLanguages, const QStringList &subtitleLanguages, const QString &framerate)
{
    url = filePath;

    QStringList argYoutube;
    argYoutube << "-o" << "-";
    argYoutube << "--max-quality" << "18";
    argYoutube << url;
    setArguments(argYoutube);

    return true;
}
