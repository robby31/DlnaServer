#include "dlnayoutubevideo.h"

DlnaYouTubeVideo::DlnaYouTubeVideo(Logger *log, QUrl url, QString host, int port, QObject *parent) :
    DlnaVideoItem(log, host, port, parent),
    m_url(url),
    m_title(),
    programYouTube("/usr/local/bin/youtube-dl"),
    programFfmpeg("/Users/doudou/workspaceQT/DLNA_server/exe/ffmpeg")
{
    requestTitle();
}

void DlnaYouTubeVideo::requestTitle()
{
    if (m_title.isNull()) {
        QProcess process;
        QStringList arg;
        arg << "--get-title";
        arg << m_url.toString();
        process.start(programYouTube, arg);
        if (process.waitForFinished())
            m_title = process.readAll().trimmed();
    }
}

void DlnaYouTubeVideo::requestVideoInfo()
{
    QProcess youtube;
    QStringList argYoutube;
    argYoutube << "-o";
    argYoutube << "-";
    argYoutube << m_url.toString();

    QProcess ffmpeg;
    youtube.setStandardOutputProcess(&ffmpeg);
    QStringList argFfmpeg;
    argFfmpeg << "-i";
    argFfmpeg << "-";

    youtube.start(programYouTube, argYoutube);
    ffmpeg.start(programFfmpeg, argFfmpeg);

    bool res = ffmpeg.waitForFinished();
    youtube.kill();
    youtube.waitForFinished();

    if (res) {
        QString answer(ffmpeg.readAllStandardError());
        qWarning() << answer;

        QRegExp duration_bitrate("Duration:\\s*(\\S*),\\s*start:\\s*(\\S*),\\s*bitrate:\\s*(\\S*)");
        if (duration_bitrate.indexIn(answer) != -1) {
            QString duration(duration_bitrate.cap(1));
            QString bitrate(duration_bitrate.cap(2));
            qWarning() << duration;
            qWarning() << bitrate;
        }

        QRegExp video("Stream #\\d+:\\d+\\(\\w+\\): Video:(.*)");
        if (video.indexIn(answer) != -1) {
            qWarning() << video.capturedTexts();
        }
    }
}

YouTubeTranscoding *DlnaYouTubeVideo::getTranscodeProcess(HttpRange *range, long timeseek_start, long timeseek_end, QObject *parent) {
    if (!toTranscode()) {

        // use getStream instead of transcoding
        return 0;

    } else {

        YouTubeTranscoding* transcodeProcess = new YouTubeTranscoding(log, parent != 0 ? parent : this);

        if (transcodeProcess->initialize(range, timeseek_start, timeseek_end, getSystemName(), getLengthInSeconds(), transcodeFormat, bitrate(), audioLanguages(), subtitleLanguages(), framerate())) {

            log->Debug(QString("YouTube Video Transcoding process %1 %2").arg(transcodeProcess->program()).arg(transcodeProcess->arguments().join(' ')));
            return transcodeProcess;

        } else {

            return 0;

        }
    }
}
