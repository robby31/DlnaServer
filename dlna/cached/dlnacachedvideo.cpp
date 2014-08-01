#include "dlnacachedvideo.h"

DlnaCachedVideo::DlnaCachedVideo(Logger* log, MediaLibrary* library, int idMedia, QString host, int port, QObject *parent):
    DlnaVideoItem(log, host, port, parent),
    library(library),
    idMedia(idMedia)
{
}

MencoderTranscoding *DlnaCachedVideo::getTranscodeProcess(HttpRange *range, long timeseek_start, long timeseek_end, QObject *parent) {
    if (!toTranscode()) {

        // use getStream instead of transcoding
        return 0;

    } else {

        MencoderTranscoding* transcodeProcess = new MencoderTranscoding(log, parent != 0 ? parent : this);

        if (transcodeProcess->initialize(range, timeseek_start, timeseek_end, getSystemName(), getLengthInSeconds(), transcodeFormat, bitrate(), audioLanguages(), subtitleLanguages(), framerate())) {

            log->Debug(QString("Video Transcoding process %1 %2").arg(transcodeProcess->program()).arg(transcodeProcess->arguments().join(' ')));
            return transcodeProcess;

        } else {

            return 0;

        }
    }
}
