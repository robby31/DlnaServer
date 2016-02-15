#include "dlnayoutubevideo.h"

DlnaYouTubeVideo::DlnaYouTubeVideo(Logger *log, QString host, int port, QObject *parent) :
    DlnaVideoItem(log, host, port, parent),
    m_url(),
    m_analyzeStream(true),
    m_videoUrlInProgress(false),
    m_unavailableMessage(),
    m_title(),
    m_streamUrl(),
    m_format(),
    m_srcSize(-1),
    m_durationInMs(-1),
    m_resolution(),
    m_framerate(),
    m_bitrate(-1),
    m_samplerate(-1),
    m_channelcount(-1),
    programFfmpeg("/opt/local/bin/ffprobe"),
    m_youtube(0)
{
    m_youtube = new YouTube();
    connect(this, SIGNAL(destroyed()), m_youtube, SLOT(deleteLater()));
    connect(this, SIGNAL(getVideoUrl(QString)), m_youtube, SLOT(getVideoUrl(QString)));
    connect(m_youtube, SIGNAL(videoNotAvailable(QString)), this, SLOT(videoNotAvailable(QString)));
    connect(m_youtube, SIGNAL(gotVideoTitle(QString)), this, SLOT(videoTitle(QString)));
    connect(m_youtube, SIGNAL(gotVideoUrl(QString)), this, SLOT(videoUrl(QString)));
    connect(m_youtube, SIGNAL(videoUrlError(QString)), this, SLOT(videoUrlError(QString)));
}

void DlnaYouTubeVideo::setUrl(const QUrl &url)
{
    m_videoUrlInProgress = false;

    QRegularExpression youtubeUrl("^http.*watch\\?v=(.*)$");
    QRegularExpressionMatch match = youtubeUrl.match(url.toString());
    if (match.hasMatch())
    {
        m_url = url;
        m_videoUrlInProgress = true;
        QString videoId = match.captured(1);
        emit getVideoUrl(videoId);
    }
    else
    {
        qWarning() << "ERROR, invalid URL" << url;
    }
}

void DlnaYouTubeVideo::videoUrlError(const QString &message)
{
    m_videoUrlInProgress = false;
    logError(message);
    replyWaitCondition.wakeAll();
}

void DlnaYouTubeVideo::videoTitle(const QString &title)
{
    m_title = title;
}

void DlnaYouTubeVideo::videoUrl(const QString &url)
{
    if (m_analyzeStream)
    {
        QProcess ffmpeg;

        QStringList argFfmpeg;
        argFfmpeg << "-i" << url;
        argFfmpeg << "-show_format";
        argFfmpeg << "-show_entries" << "stream";
        argFfmpeg << "-of" << "xml";

        ffmpeg.start(programFfmpeg, argFfmpeg);

        bool res = ffmpeg.waitForFinished(60000);

        if (res)
        {
            QDomDocument xml_res;
            xml_res.setContent(ffmpeg.readAll());
            //        qWarning() << xml_res.toString();

            if (xml_res.elementsByTagName("format").size() == 1)
            {
                QDomNode format = xml_res.elementsByTagName("format").at(0);
                if (format.attributes().contains("format_name"))
                    m_format = format.attributes().namedItem("format_name").nodeValue();

                if (format.attributes().contains("duration"))
                    m_durationInMs = QVariant::fromValue(format.attributes().namedItem("duration").nodeValue()).toDouble()*1000;

                if (format.attributes().contains("size"))
                    m_srcSize = QVariant::fromValue(format.attributes().namedItem("size").nodeValue()).toInt();
            }
            else
            {
                logError("ERROR: unable to get video info (format not found).");
                ffmpeg.kill();
                ffmpeg.waitForFinished();
            }

            QDomNodeList streams = xml_res.elementsByTagName("stream");
            for (int i = 0; i<streams.size(); ++i)
            {
                QDomNode stream = streams.at(i);
                if (stream.attributes().namedItem("codec_type").nodeValue() == "video")
                {
                    if (stream.attributes().contains("width") && stream.attributes().contains("height"))
                        m_resolution = QString("%1x%2").arg(stream.attributes().namedItem("width").nodeValue()).arg(stream.attributes().namedItem("height").nodeValue());

                    if (stream.attributes().contains("avg_frame_rate"))
                        m_framerate = stream.attributes().namedItem("avg_frame_rate").nodeValue();
                }
                else if (stream.attributes().namedItem("codec_type").nodeValue() == "audio")
                {
                    if (stream.attributes().contains("channels"))
                        m_channelcount = stream.attributes().namedItem("channels").nodeValue().toInt();
                    if (stream.attributes().contains("sample_rate"))
                        m_samplerate = stream.attributes().namedItem("sample_rate").nodeValue().toInt();
                }
            }
        }
        else
        {
            logError(QString("ERROR: timeout, unable to get video info, ffmpeg returned status %1").arg(res));
            ffmpeg.kill();
            ffmpeg.waitForFinished();
        }
    }

    m_streamUrl = url;

    m_videoUrlInProgress = false;
    replyWaitCondition.wakeAll();
}

bool DlnaYouTubeVideo::waitUrl(const int &timeout)
{    
    if (m_videoUrlInProgress)
    {
        // waiting reply with timeout
        mutex.lock();
        bool ret = replyWaitCondition.wait(&mutex, timeout);
        mutex.unlock();
        return ret;
    }
    else
    {
        return true;
    }
}

TranscodeProcess *DlnaYouTubeVideo::getTranscodeProcess()
{
    FfmpegTranscoding* transcodeProcess = new FfmpegTranscoding(log());

    transcodeProcess->setUrl(m_streamUrl);
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
