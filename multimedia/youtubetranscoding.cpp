#include "youtubetranscoding.h"

YouTubeTranscoding::YouTubeTranscoding(Logger *log, QObject *parent) :
    TranscodeProcess(log, parent)
{
    setProgram("/usr/local/bin/youtube-dl");
}

void YouTubeTranscoding::updateArguments()
{
    QStringList argYoutube;
    argYoutube << "-o" << "-";
    argYoutube << "--max-quality" << "18";
    argYoutube << url();
    setArguments(argYoutube);
}
