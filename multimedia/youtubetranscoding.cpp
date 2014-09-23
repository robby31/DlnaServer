#include "youtubetranscoding.h"

YouTubeTranscoding::YouTubeTranscoding(Logger *log, QObject *parent) :
    MencoderTranscoding(log, parent),
    youtubeStreaming(this),
    url()
{
    connect(&youtubeStreaming, SIGNAL(finished(int)), this, SLOT(youtubeFinished(int)));
    connect(&youtubeStreaming, SIGNAL(readyReadStandardError()), this, SLOT(youtubeReadyReadStandardError()));
    youtubeStreaming.setProgram("/usr/local/bin/youtube-dl");
}

YouTubeTranscoding::~YouTubeTranscoding() {
    if (!youtubeStreaming.waitForFinished(1000))
        m_log->Error("YouTubeTranscoding not finished");
}

bool YouTubeTranscoding::initialize(HttpRange *range, const long &timeseek_start, const long &timeseek_end, const QString &filePath, const int &lengthInSeconds, const TranscodeFormatAvailable &transcodeFormat, const int &bitrate, const QStringList &audioLanguages, const QStringList &subtitleLanguages, const QString &framerate)
{
    url = filePath;

    QStringList argYoutube;
    argYoutube << "-o" << "-";
    argYoutube << "--max-quality" << "18";
    argYoutube << url;
    youtubeStreaming.setArguments(argYoutube);
    youtubeStreaming.setStandardOutputProcess(this);

    return MencoderTranscoding::initialize(range, timeseek_start, timeseek_start, "-", lengthInSeconds, transcodeFormat, bitrate, audioLanguages, subtitleLanguages, framerate);
}

void YouTubeTranscoding::launch()
{
    appendLog(QString("%1 %2").arg(youtubeStreaming.program()).arg(youtubeStreaming.arguments().join(" ")));

    youtubeStreaming.start();
    MencoderTranscoding::launch();
}

void YouTubeTranscoding::youtubeFinished(int status)
{
    appendLog(QString("YOUTUBE %1 finished with status %2").arg(url).arg(status));
}

void YouTubeTranscoding::youtubeReadyReadStandardError()
{
    appendLog(youtubeStreaming.readAllStandardError());
}

int YouTubeTranscoding::transcodeExitCode() const
{
    if (youtubeStreaming.exitCode()==0)
        return MencoderTranscoding::exitCode();
    else
        return youtubeStreaming.exitCode();
}
