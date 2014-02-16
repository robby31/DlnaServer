#include "dlnamusictrackfile.h"

DlnaMusicTrackFile::DlnaMusicTrackFile(Logger* log, QString filename, QString host, int port, QObject *parent):
    DlnaMusicTrack(log, filename, host, port, parent),
    mediaTag(filename)
{

}

QString DlnaMusicTrackFile::metaDataTitle() {
    return mediaTag.getParameter("Title");
}

QString DlnaMusicTrackFile::metaDataGenre() {
    return mediaTag.getParameter("Genre");
}

QString DlnaMusicTrackFile::metaDataPerformer() {
    return mediaTag.getParameter("Performer");
}

QString DlnaMusicTrackFile::metaDataAlbum() {
    return mediaTag.getParameter("Album");
}

QString DlnaMusicTrackFile::metaDataTrackPosition() {
    return mediaTag.getParameter("Track/Position");
}

QString DlnaMusicTrackFile::metaDataFormat() {
    return mediaTag.getParameter("Format");
}

QByteArray DlnaMusicTrackFile::metaDataPicture() {
    return QByteArray::fromBase64(mediaTag.getCoverData().c_str());
}

int DlnaMusicTrackFile::metaDataDuration() {
    return mediaTag.getParameter("Duration").toInt();
}

int DlnaMusicTrackFile::metaDataBitrate() {
    return mediaTag.getParameter("OverallBitRate").toInt();
}

int DlnaMusicTrackFile::channelCount() {
    int audioStreamCount = mediaTag.getAudioStreamCount();
    if (audioStreamCount == 1) {
        return mediaTag.getChannelCount(0);
    }
    return 0;
}

int DlnaMusicTrackFile::samplerate() {
    int audioStreamCount = mediaTag.getAudioStreamCount();
    if (audioStreamCount == 1) {
        return mediaTag.getSamplingRate(0);
    }
    return 0;
}


