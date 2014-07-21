#include "dlnacachednetworkvideo.h"

DlnaCachedNetworkVideo::DlnaCachedNetworkVideo(Logger* log, MediaLibrary* library, int idMedia, QString host, int port, QObject *parent):
    DlnaCachedVideo(log, library, idMedia, host, port, parent)
{
}

YouTubeTranscoding *DlnaCachedNetworkVideo::getTranscodeProcess(HttpRange *range, long timeseek_start, long timeseek_end, QObject *parent) {
    if (!toTranscode()) {

        // use getStream instead of transcoding
        return 0;

    } else {

        YouTubeTranscoding* transcodeProcess = new YouTubeTranscoding(log, parent != 0 ? parent : this);

        if (transcodeProcess->initialize(range, timeseek_start, timeseek_end, getSystemName(), getLengthInSeconds(), transcodeFormat, bitrate(), audioLanguages(), subtitleLanguages(), framerate())) {

            log->Debug(QString("YouTube Video Transcoding process %1 %2").arg(transcodeProcess->program()).arg(transcodeProcess->arguments().join(' ')));
            return transcodeProcess;

        } else {

            return 0;

        }
    }
}
