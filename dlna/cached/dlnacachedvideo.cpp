#include "dlnacachedvideo.h"

DlnaCachedVideo::DlnaCachedVideo(MediaLibrary* library, int idMedia, QString host, int port, QObject *parent):
    DlnaVideoItem(host, port, parent),
    library(library),
    idMedia(idMedia)
{
}

TranscodeProcess *DlnaCachedVideo::getTranscodeProcess()
{
    FfmpegTranscoding* transcodeProcess = new FfmpegTranscoding();
    transcodeProcess->setUrl(getSystemName());
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

qint64 DlnaCachedVideo::getResumeTime() const
{
    qint64 res = 0;

    if (library)
        res = library->getmetaData("progress_played", idMedia).toLongLong();

    if (res > 10000)
        return res - 10000;   // returns 10 seconds before resume time
    else
        return 0;
}

QHash<QString, double> DlnaCachedVideo::volumeInfo(const int timeout)
{
    Q_UNUSED(timeout)

    if (library)
    {
        return library->volumeInfo(idMedia);
    }
    else
    {
        QHash<QString, double> result;
        return result;
    }
}

Device *DlnaCachedVideo::getOriginalStreaming()
{
    return new StreamingFile(getSystemName());
}
