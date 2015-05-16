#include "dlnacachednetworkvideo.h"

DlnaCachedNetworkVideo::DlnaCachedNetworkVideo(Logger* log, QNetworkAccessManager *manager, MediaLibrary* library, int idMedia, QString host, int port, QObject *parent):
    DlnaCachedVideo(log, library, idMedia, host, port, parent),
    m_nam(manager),
    m_streamUrl()
{
}

TranscodeProcess *DlnaCachedNetworkVideo::getTranscodeProcess()
{
    if (m_streamUrl.isNull())
    {
        DlnaYouTubeVideo movie(log(), host, port);
        movie.setAnalyzeStream(false);
        if (m_nam)
        {
            movie.moveToThread(m_nam->thread());
            movie.setNetworkAccessManager(m_nam);
        }
        movie.setUrl(getSystemName());

        bool res = movie.waitUrl(30000);
        if (res)
            m_streamUrl = movie.streamUrl();
    }

    if (!m_streamUrl.isEmpty())
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
    else
    {
        logError("Invalid streaming url.");
        return 0;
    }
}
