#include "dlnayoutubevideo.h"

qint64 DlnaYouTubeVideo::objectCounter = 0;

DlnaYouTubeVideo::DlnaYouTubeVideo(QString host, int port, QObject *parent) :
    DlnaVideoItem(host, port, parent),
    m_url(),
    m_analyzeStream(true),
    m_videoUrlInProgress(false),
    m_unavailableMessage(),
    m_title(),
    m_streamUrl(),
    m_youtube(0),
    mutex(),
    replyWaitCondition(),
    m_error()
{
    ++objectCounter;
}

DlnaYouTubeVideo::~DlnaYouTubeVideo()
{
    // if mutex is locked, wait until it is unlocked
    QMutexLocker locker(&mutex);

    --objectCounter;
}

void DlnaYouTubeVideo::setNetworkAccessManager(QNetworkAccessManager *manager)
{
    if (manager)
    {
        QMutexLocker locker(&mutex);

        m_youtube = new YouTube();
        connect(this, SIGNAL(destroyed()), m_youtube, SLOT(deleteLater()));

        m_youtube->setNetworkAccessManager(manager);
        m_youtube->moveToThread(manager->thread());
        connect(manager->thread(), SIGNAL(finished()), m_youtube, SLOT(deleteLater()));

        connect(this, SIGNAL(getVideoUrl(QString)), m_youtube, SLOT(getVideoUrl(QString)));
        connect(m_youtube, SIGNAL(videoNotAvailable(QString)), this, SLOT(videoNotAvailable(QString)));
        connect(m_youtube, SIGNAL(gotVideoTitle(QString)), this, SLOT(videoTitle(QString)));
        connect(m_youtube, SIGNAL(gotVideoUrl(QString)), this, SLOT(videoUrl(QString)));
        connect(m_youtube, SIGNAL(videoUrlError(QString)), this, SLOT(videoUrlError(QString)));
    }
}

void DlnaYouTubeVideo::setPlaybackQuality(const QString &quality)
{
    QMutexLocker locker(&mutex);
    if (m_youtube)
        m_youtube->setPlaybackQuality(quality);
    else
        qCritical() << "Unable to set playback quality because Youtube is not initialized (call setNetworkAccessManager before).";
}

QUrl DlnaYouTubeVideo::url() const
{
    return m_url;
}

void DlnaYouTubeVideo::setUrl(const QUrl &url)
{
    QMutexLocker locker(&mutex);

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
    QMutexLocker locker(&mutex);

    m_error = message;

    if (m_videoUrlInProgress)
    {
        m_videoUrlInProgress = false;
        replyWaitCondition.wakeAll();
    }

    emit videoUrlErrorSignal(message);
}

void DlnaYouTubeVideo::videoTitle(const QString &title)
{
    QMutexLocker locker(&mutex);
    m_title = title;
}

void DlnaYouTubeVideo::videoUrl(const QString &url)
{
    QMutexLocker locker(&mutex);

    m_streamUrl = url;

    if (m_analyzeStream)
        ffmpeg.open(url);

    if (m_videoUrlInProgress)
    {
        m_videoUrlInProgress = false;
        replyWaitCondition.wakeAll();
    }

    emit streamUrlDefined(url);
}

bool DlnaYouTubeVideo::waitUrl(const int &timeout)
{    
    QMutexLocker locker(&mutex);

    if (m_videoUrlInProgress)
    {
        // waiting reply with timeout
        return replyWaitCondition.wait(locker.mutex(), timeout);
    }
    else
    {
        return true;
    }
}

TranscodeProcess *DlnaYouTubeVideo::getTranscodeProcess()
{
    FfmpegTranscoding* transcodeProcess = new FfmpegTranscoding();

    transcodeProcess->setOriginalLengthInMSeconds(metaDataDuration());
    transcodeProcess->setFormat(transcodeFormat);
    transcodeProcess->setBitrate(bitrate());
    transcodeProcess->setAudioLanguages(audioLanguages());
    transcodeProcess->setSubtitleLanguages(subtitleLanguages());
    transcodeProcess->setFrameRate(framerate());
    transcodeProcess->setAudioChannelCount(channelCount());
    transcodeProcess->setAudioSampleRate(samplerate());
    transcodeProcess->setUrl(streamUrl());
    return transcodeProcess;
}

Device *DlnaYouTubeVideo::getOriginalStreaming()
{
    TranscodeProcess *process = getTranscodeProcess();
    process->setFormat(COPY);
    return process;
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
