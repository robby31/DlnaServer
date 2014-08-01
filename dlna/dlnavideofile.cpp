#include "dlnavideofile.h"

DlnaVideoFile::DlnaVideoFile(Logger *log, QString filename, QString host, int port, QObject *parent):
    DlnaVideoItem(log, host, port, parent),
    fileinfo(filename),
    mime_type(),
    ffmpeg(filename, this)
{
    QMimeDatabase db;
    mime_type = db.mimeTypeForFile(fileinfo);
}

DlnaVideoFile::~DlnaVideoFile() {

}

MencoderTranscoding *DlnaVideoFile::getTranscodeProcess(HttpRange *range, long timeseek_start, long timeseek_end, QObject *parent) {
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

QString DlnaVideoFile::metaDataTitle() const {
    return ffmpeg.metaData("title");
}

QString DlnaVideoFile::metaDataGenre() const {
    return ffmpeg.metaData("genre");
}

QString DlnaVideoFile::metaDataPerformer() const{
    return ffmpeg.metaData("artist");
}

QString DlnaVideoFile::metaDataAlbum() const {
    return ffmpeg.metaData("album");
}

QString DlnaVideoFile::metaDataTrackPosition() const {
    return ffmpeg.metaData("track").split('/').at(0);
}

QString DlnaVideoFile::metaDataFormat() const {
    return ffmpeg.getFormat();
}

QByteArray DlnaVideoFile::metaDataPicture() const {
    return ffmpeg.getPicture();
}

int DlnaVideoFile::metaDataDuration() const {
    return ffmpeg.getDuration();
}

int DlnaVideoFile::metaDataBitrate() const {
    return ffmpeg.getBitrate();
}

int DlnaVideoFile::channelCount() const {
    return ffmpeg.getAudioChannelCount();
}

int DlnaVideoFile::samplerate() const {
    return ffmpeg.getAudioSamplerate();
}

QString DlnaVideoFile::resolution() const {
    return ffmpeg.getVideoResolution();
}

QStringList DlnaVideoFile::audioLanguages() const {
    return ffmpeg.getAudioLanguages();
}

QStringList DlnaVideoFile::subtitleLanguages() const {
    return ffmpeg.getSubtitleLanguages();
}

QString DlnaVideoFile::framerate() const {
    return QString().sprintf("%2.3f", ffmpeg.getVideoFrameRate());
}
