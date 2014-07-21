#include "dlnacachedvideo.h"

DlnaCachedVideo::DlnaCachedVideo(Logger* log, MediaLibrary* library, int idMedia, QString host, int port, QObject *parent):
    DlnaVideoItem(log, host, port, parent),
    library(library),
    idMedia(idMedia)
{
}

long DlnaCachedVideo::size() const {
    if (toTranscode()) {
        if (bitrate() != -1) {
            return double(bitrate())*double(getLengthInMilliSeconds())/8000.0;
        } else {
            // variable bitrate, we don't know exactly the size
            return -1;
        }
    } else {
        return -1;
    }
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
