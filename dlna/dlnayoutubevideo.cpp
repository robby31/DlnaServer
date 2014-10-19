#include "dlnayoutubevideo.h"

DlnaYouTubeVideo::DlnaYouTubeVideo(Logger *log, QUrl url, QString host, int port, QObject *parent) :
    DlnaVideoItem(log, host, port, parent),
    m_url(url),
    m_title(),
    m_durationInMs(-1),
    m_resolution(),
    programYouTube("/usr/local/bin/youtube-dl"),
    programFfmpeg("/Users/doudou/workspaceQT/DLNA_server/exe/ffmpeg")
{
    requestTitle();
    requestVideoInfo();
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
//        qWarning() << answer;

        QRegExp duration_bitrate("Duration:\\s*(\\S*),\\s*start:\\s*(\\S*),\\s*bitrate:\\s*(\\S*)");
        if (duration_bitrate.indexIn(answer) != -1) {
            QString duration(duration_bitrate.cap(1));
            QString bitrate(duration_bitrate.cap(3));
            QTime time = QTime::fromString(duration, "hh:mm:ss.z");
            m_durationInMs = (time.hour()*3600+time.minute()*60+time.second())*1000+time.msec();
        }

        QRegExp video("Stream #\\d+:\\d+\\(\\w+\\): Video:([^,]+), ([^,]+), ([^,\\s]+)\\s?([^,]*), ([^,]+), ([^,]+)");
        if (video.indexIn(answer) != -1) {
            m_resolution = video.cap(3);
//            qWarning() << "VIDEO INFO" << video.capturedTexts();
        }
    }
}
