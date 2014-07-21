#include "dlnamusictrackfile.h"

DlnaMusicTrackFile::DlnaMusicTrackFile(Logger* log, QString filename, QString host, int port, QObject *parent):
    DlnaMusicTrack(log, host, port, parent),
    fileinfo(filename),
    mime_type(),
    ffmpeg(filename, this)
{
    QMimeDatabase db;
    mime_type = db.mimeTypeForFile(fileinfo);
}

QString DlnaMusicTrackFile::getDisplayName() const {
    QString title = metaDataTitle();
    if (title.isEmpty())
        title = fileinfo.completeBaseName();
    if (title.isEmpty())
        title = getSystemName();
    return title;
}

long DlnaMusicTrackFile::size() const {
    if (toTranscode()) {
        if (bitrate() != -1) {
            return double(bitrate())*double(getLengthInMilliSeconds())/8000.0;
        } else {
            // variable bitrate, we don't know exactly the size
            return -1;
        }
    } else {
        return fileinfo.size();
    }
}

QString DlnaMusicTrackFile::metaDataTitle() const {
    return ffmpeg.metaData("title");
}

QString DlnaMusicTrackFile::metaDataGenre() const {
    return ffmpeg.metaData("genre");
}

QString DlnaMusicTrackFile::metaDataPerformer() const {
    return ffmpeg.metaData("artist");
}

QString DlnaMusicTrackFile::metaDataAlbum() const {
    return ffmpeg.metaData("album");
}

QString DlnaMusicTrackFile::metaDataTrackPosition() const {
    return ffmpeg.metaData("track").split('/').at(0);
}

QString DlnaMusicTrackFile::metaDataFormat() const {
    return ffmpeg.getAudioFormat();
}

QByteArray DlnaMusicTrackFile::metaDataPicture() const {
    return ffmpeg.getPicture();
}

int DlnaMusicTrackFile::metaDataDuration() const {
    return ffmpeg.getDuration();
}

int DlnaMusicTrackFile::metaDataBitrate() const {
    return ffmpeg.getAudioBitrate();
}

int DlnaMusicTrackFile::channelCount() const {
    return ffmpeg.getAudioChannelCount();
}

int DlnaMusicTrackFile::samplerate() const {
    return ffmpeg.getAudioSamplerate();
}


