#include "dlnayoutubevideo.h"

DlnaYouTubeVideo::DlnaYouTubeVideo(Logger *log, QUrl url, QString host, int port, QObject *parent) :
    DlnaVideoItem(log, host, port, parent),
    m_url(url),
    m_title(),
    m_durationInMs(-1),
    m_resolution(),
    m_bitrate(-1),
    programYouTube("/usr/local/bin/youtube-dl"),
    programFfmpeg("/opt/local/bin/ffprobe")
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
    argFfmpeg << "-show_format";
    argFfmpeg << "-show_entries" << "stream";
    argFfmpeg << "-of" << "xml";

    youtube.start(programYouTube, argYoutube);
    ffmpeg.start(programFfmpeg, argFfmpeg);

    bool res = ffmpeg.waitForFinished();

    if (res)
    {
        QDomDocument xml_res;
        xml_res.setContent(ffmpeg.readAll());
//        qWarning() << xml_res.toString();

        if (xml_res.elementsByTagName("format").size() == 1)
        {
            QDomNode format = xml_res.elementsByTagName("format").at(0);
            m_durationInMs = QVariant::fromValue(format.attributes().namedItem("duration").nodeValue()).toDouble()*1000;

            QDomNodeList streams = xml_res.elementsByTagName("stream");
            for (int i = 0; i<streams.size(); ++i)
            {
                QDomNode stream = streams.at(i);
                if (stream.attributes().namedItem("codec_type").nodeValue() == "video")
                {
                    if (stream.attributes().contains("width") && stream.attributes().contains("height"))
                    {
                        m_resolution = QString("%1x%2").arg(stream.attributes().namedItem("width").nodeValue()).arg(stream.attributes().namedItem("height").nodeValue());
                    }
                }
            }
        }
        else
        {
            logError("ERROR: unable to get video info.");
            ffmpeg.kill();
            ffmpeg.waitForFinished();
        }
    }
    else
    {
        logError("ERROR: unable to get video info.");
        ffmpeg.kill();
        ffmpeg.waitForFinished();
    }

    ffmpeg.kill();
    youtube.kill();
    youtube.waitForFinished();
}

TranscodeProcess *DlnaYouTubeVideo::getTranscodeProcess()
{
    FfmpegTranscoding* transcodeProcess = new FfmpegTranscoding(log());

    // set the url for transcoding
    QProcess process;
    process.setProgram(programYouTube);
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
