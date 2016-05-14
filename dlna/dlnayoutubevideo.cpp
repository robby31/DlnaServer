#include "dlnayoutubevideo.h"

DlnaYouTubeVideo::DlnaYouTubeVideo(Logger *log, QString host, int port, QObject *parent) :
    DlnaVideoItem(log, host, port, parent),
    m_url(),
    m_analyzeStream(true),
    m_videoUrlInProgress(false),
    m_unavailableMessage(),
    m_title(),
    m_streamUrl(),
    ffmpeg(this),
    m_youtube(0),
    mutex(),
    replyWaitCondition()
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
        ffmpeg.setFilename(url);

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

QHash<QString, double> DlnaYouTubeVideo::volumeInfo(const int timeout)
{
    return ffmpeg.getVolumeInfo(timeout);
}

QString DlnaYouTubeVideo::metaDataFormat() const
{
    return ffmpeg.getFormat();
}

qint64 DlnaYouTubeVideo::sourceSize() const
{
    return ffmpeg.size();
}

int DlnaYouTubeVideo::metaDataDuration() const
{
    return ffmpeg.getDuration();
}

int DlnaYouTubeVideo::samplerate() const
{
    return ffmpeg.getAudioSamplerate();
}

int DlnaYouTubeVideo::channelCount() const
{
    return ffmpeg.getAudioChannelCount();
}

QString DlnaYouTubeVideo::resolution() const
{
    return ffmpeg.getVideoResolution();
}

QString DlnaYouTubeVideo::framerate() const
{
    return QString().sprintf("%2.3f", ffmpeg.getVideoFrameRate());
}
