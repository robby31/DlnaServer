#include "dlnayoutubevideo.h"

DlnaYouTubeVideo::DlnaYouTubeVideo(Logger *log, QUrl url, QString host, int port, QObject *parent) :
    DlnaVideoItem(log, host, port, parent),
    m_url(url),
    m_title(),
    m_durationInMs(-1),
    m_resolution(),
    m_bitrate(-1),
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
        {
            m_title = process.readAll().trimmed();
        }
        else
        {
            logError("ERROR: unable to get title.");
            process.kill();
            process.waitForFinished();
        }
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

    if (res) {
        QString answer(ffmpeg.readAllStandardError());
//        qWarning() << answer;

        QRegExp duration_bitrate("Duration:\\s*(\\S*),\\s*start:\\s*(\\S*),\\s*bitrate:\\s*(\\S*)");
        if (duration_bitrate.indexIn(answer) != -1) {
            QString duration(duration_bitrate.cap(1));
            QTime time = QTime::fromString(duration, "hh:mm:ss.z");
            m_durationInMs = (time.hour()*3600+time.minute()*60+time.second())*1000+time.msec();
        }

        QRegExp video("Stream #\\d+:\\d+\\(\\w+\\): Video:([^,]+), ([^,]+), ([^,\\s]+)\\s?([^,]*), ([^,]+), ([^,]+)");
        if (video.indexIn(answer) != -1) {
            m_resolution = video.cap(3);
//            qWarning() << "VIDEO INFO" << video.capturedTexts();
        }
    }
    else
    {
        logError("ERROR: unable to get video info.");
        ffmpeg.kill();
        ffmpeg.waitForFinished();
    }

    youtube.kill();
    youtube.waitForFinished();
}

TranscodeProcess *DlnaYouTubeVideo::getTranscodeProcess()
{
    FfmpegTranscoding* transcodeProcess = new FfmpegTranscoding(log());

    // set the url for transcoding
    QProcess process;
    process.setProgram("/usr/local/bin/youtube-dl");
    QStringList argList;
    argList << "--max-quality" << "18" << "-g" << getSystemName();
    process.setArguments(argList);

    if (process.open())
    {
        if (process.waitForFinished())
        {
            QString url = process.readAllStandardOutput().trimmed();
            if (url.startsWith("http"))
                transcodeProcess->setUrl(url);
        }
        else
        {
            logError("ERROR: unable to get url.");
            process.kill();
            process.waitForFinished();
        }
    }

    transcodeProcess->setLengthInSeconds(getLengthInSeconds());
    transcodeProcess->setFormat(transcodeFormat);
    transcodeProcess->setBitrate(bitrate());
    transcodeProcess->setAudioLanguages(audioLanguages());
    transcodeProcess->setSubtitleLanguages(subtitleLanguages());
    transcodeProcess->setFrameRate(framerate());
    transcodeProcess->setAudioChannelCount(channelCount());
    transcodeProcess->setAudioSampleRate(samplerate());
    return transcodeProcess;
}
