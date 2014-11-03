#include "dlnacachedvideo.h"

DlnaCachedVideo::DlnaCachedVideo(Logger* log, MediaLibrary* library, int idMedia, QString host, int port, QObject *parent):
    DlnaVideoItem(log, host, port, parent),
    library(library),
    idMedia(idMedia)
{
}

TranscodeProcess *DlnaCachedVideo::getTranscodeProcess()
{
    MencoderTranscoding* transcodeProcess = new MencoderTranscoding(log());
    transcodeProcess->setLengthInSeconds(getLengthInSeconds());
    transcodeProcess->setFormat(transcodeFormat);
    transcodeProcess->setBitrate(bitrate());
    transcodeProcess->setAudioLanguages(audioLanguages());
    transcodeProcess->setSubtitleLanguages(subtitleLanguages());
    transcodeProcess->setFrameRate(framerate());
    return transcodeProcess;
}
