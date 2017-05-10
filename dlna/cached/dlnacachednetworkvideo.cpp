#include "dlnacachednetworkvideo.h"

DlnaCachedNetworkVideo::DlnaCachedNetworkVideo(QNetworkAccessManager *manager, MediaLibrary* library, int idMedia, QString host, int port, QObject *parent):
    DlnaCachedVideo(library, idMedia, host, port, parent),
    m_nam(manager),
    m_streamUrl()
{
}

TranscodeProcess *DlnaCachedNetworkVideo::getTranscodeProcess()
{
    FfmpegTranscoding* transcodeProcess = Q_NULLPTR;

    QString sysName = getSystemName();

    if (sysName.startsWith("http") && sysName.contains("youtube"))
    {
        // YOUTUBE stream
        if (!m_nam)
        {
            qCritical() << "network not initialised, cannot manage Youtube stream.";
            return 0;
        }
        else
        {
            transcodeProcess = new FfmpegTranscoding();

            // request from Youtube url for streaming
            DlnaYouTubeVideo *movie = new DlnaYouTubeVideo(host, port, transcodeProcess);
            connect(movie, SIGNAL(streamUrlDefined(QString)), transcodeProcess, SLOT(setUrl(QString)));
            connect(movie, SIGNAL(videoUrlErrorSignal(QString)), transcodeProcess, SLOT(urlError(QString)));
            movie->setAnalyzeStream(false);
            movie->setNetworkAccessManager(m_nam);
            movie->setUrl(sysName);
        }
    }
    else if (!m_streamUrl.isEmpty())
    {
        // local video
        transcodeProcess = new FfmpegTranscoding();
        transcodeProcess->setUrl(m_streamUrl);
    }

    if (transcodeProcess)
    {
        transcodeProcess->setLengthInSeconds(getLengthInSeconds());
        transcodeProcess->setFormat(transcodeFormat);
        transcodeProcess->setBitrate(bitrate());
        transcodeProcess->setAudioLanguages(audioLanguages());
        transcodeProcess->setSubtitleLanguages(subtitleLanguages());
        transcodeProcess->setFrameRate(framerate());
        transcodeProcess->setAudioChannelCount(channelCount());
        transcodeProcess->setAudioSampleRate(samplerate());
        transcodeProcess->setVolumeInfo(volumeInfo());

        return transcodeProcess;
    }
    else
    {
        qCritical() << "No stream to return.";
        return 0;
    }
}

Device *DlnaCachedNetworkVideo::getOriginalStreaming()
{
    QString sysName = getSystemName();
    if (sysName.startsWith("http") && sysName.contains("youtube"))
    {
        TranscodeProcess *process = getTranscodeProcess();
        process->setFormat(COPY);
        return process;
    }
    else
    {
        return new StreamingFile(getSystemName());
    }
}
