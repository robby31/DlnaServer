#include "dlnamusictrackfile.h"

DlnaMusicTrackFile::DlnaMusicTrackFile(Logger* log, QString filename, QString host, int port, QObject *parent):
    DlnaMusicTrack(log, filename, host, port, parent),
    ffmpeg(filename, this)
{
}

QString DlnaMusicTrackFile::metaDataTitle() {
    return ffmpeg.metaData("title");
}

QString DlnaMusicTrackFile::metaDataGenre() {
    return ffmpeg.metaData("genre");
}

QString DlnaMusicTrackFile::metaDataPerformer() {
    return ffmpeg.metaData("artist");
}

QString DlnaMusicTrackFile::metaDataAlbum() {
    return ffmpeg.metaData("album");
}

QString DlnaMusicTrackFile::metaDataTrackPosition() {
    return ffmpeg.metaData("track").split('/').at(0);
}

QString DlnaMusicTrackFile::metaDataFormat() {
    return ffmpeg.getAudioFormat();
}

QByteArray DlnaMusicTrackFile::metaDataPicture() {
    return ffmpeg.getPicture();
}

int DlnaMusicTrackFile::metaDataDuration() {
    return ffmpeg.getDuration();
}

int DlnaMusicTrackFile::metaDataBitrate() {
    return ffmpeg.getAudioBitrate();
}

int DlnaMusicTrackFile::channelCount() {
    return ffmpeg.getAudioChannelCount();
}

int DlnaMusicTrackFile::samplerate() {
    return ffmpeg.getAudioSamplerate();
}


