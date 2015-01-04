#include "dlnacachedvideo.h"

DlnaCachedVideo::DlnaCachedVideo(Logger* log, MediaLibrary* library, int idMedia, QString host, int port, QObject *parent):
    DlnaVideoItem(log, host, port, parent),
    library(library),
    idMedia(idMedia)
{
}

TranscodeProcess *DlnaCachedVideo::getTranscodeProcess()
{
    FfmpegTranscoding* transcodeProcess = new FfmpegTranscoding(log());
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

int DlnaCachedVideo::samplerate() const
{
    if (library)
        return library->getmetaData("samplerate", idMedia).toInt();

    return -1;
}

int DlnaCachedVideo::channelCount() const
{
    if (library)
        return library->getmetaData("channelcount", idMedia).toInt();

    return -1;
}

QString DlnaCachedVideo::framerate() const
{
    if (library)
        return library->getmetaData("framerate", idMedia).toString();

    return QString();
}
