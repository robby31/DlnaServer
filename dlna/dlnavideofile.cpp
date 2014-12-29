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

TranscodeProcess *DlnaVideoFile::getTranscodeProcess()
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

int DlnaVideoFile::metaDataTrackPosition() const {
    return ffmpeg.metaData("track").split('/').at(0).toInt();
}

int DlnaVideoFile::metaDataDisc() const
{
    return ffmpeg.metaData("disc").split('/').at(0).toInt();
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
    if (toTranscode())
        return DlnaVideoItem::channelCount();
    else
        return ffmpeg.getAudioChannelCount();
}

int DlnaVideoFile::samplerate() const {
    if (toTranscode())
        return DlnaVideoItem::samplerate();
    else
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
