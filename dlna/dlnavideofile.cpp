#include "dlnavideofile.h"

DlnaVideoFile::DlnaVideoFile(Logger *log, QString filename, QString host, int port, QObject *parent):
    DlnaVideoItem(log, filename, host, port, parent),
    ffmpeg(filename, parent)
{

}

QString DlnaVideoFile::metaDataTitle() {
    return ffmpeg.metaData("title");
}

QString DlnaVideoFile::metaDataGenre() {
    return ffmpeg.metaData("genre");
}

QString DlnaVideoFile::metaDataPerformer() {
    return ffmpeg.metaData("artist");
}

QString DlnaVideoFile::metaDataAlbum() {
    return ffmpeg.metaData("album");
}

QString DlnaVideoFile::metaDataTrackPosition() {
    return ffmpeg.metaData("track").split('/').at(0);
}

QString DlnaVideoFile::metaDataFormat() {
    return ffmpeg.getFormat();
}

QByteArray DlnaVideoFile::metaDataPicture() {
    return ffmpeg.getPicture();
}

int DlnaVideoFile::metaDataDuration() {
    return ffmpeg.getDuration();
}

int DlnaVideoFile::metaDataBitrate() {
    return ffmpeg.getBitrate();
}

int DlnaVideoFile::channelCount() {
    return ffmpeg.getAudioChannelCount();
}

int DlnaVideoFile::samplerate() {
    return ffmpeg.getAudioSamplerate();
}

QString DlnaVideoFile::resolution() {
    return ffmpeg.getVideoResolution();
}

QStringList DlnaVideoFile::audioLanguages() {
    return ffmpeg.getAudioLanguages();
}

QStringList DlnaVideoFile::subtitleLanguages() {
    return ffmpeg.getSubtitleLanguages();
}

QString DlnaVideoFile::framerate() {
    return QString().sprintf("%2.3f", ffmpeg.getVideoFrameRate());
}
