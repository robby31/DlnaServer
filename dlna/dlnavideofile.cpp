#include "dlnavideofile.h"

DlnaVideoFile::DlnaVideoFile(Logger *log, QString filename, QString host, int port, QObject *parent):
    DlnaVideoItem(log, filename, host, port, parent),
    mediaTag(filename)
{

}

QString DlnaVideoFile::metaDataTitle() {
    return mediaTag.getParameter("Title");
}

QString DlnaVideoFile::metaDataGenre() {
    return mediaTag.getParameter("Genre");
}

QString DlnaVideoFile::metaDataPerformer() {
    return mediaTag.getParameter("Performer");
}

QString DlnaVideoFile::metaDataAlbum() {
    return mediaTag.getParameter("Album");
}

QString DlnaVideoFile::metaDataTrackPosition() {
    return mediaTag.getParameter("Track/Position");
}

QString DlnaVideoFile::metaDataFormat() {
    return mediaTag.getParameter("Format");
}

QByteArray DlnaVideoFile::metaDataPicture() {
    return QByteArray::fromBase64(mediaTag.getCoverData().c_str());
}

int DlnaVideoFile::metaDataDuration() {
    return mediaTag.getParameter("Duration").toInt();
}

int DlnaVideoFile::metaDataBitrate() {
    mediaTag.getParameter("OverallBitRate").toInt();
}

int DlnaVideoFile::channelCount() {
    int audioStreamCount = mediaTag.getAudioStreamCount();
    if (audioStreamCount == 1) {
        return mediaTag.getChannelCount(0);
    }
    return 0;
}

int DlnaVideoFile::samplerate() {
    int audioStreamCount = mediaTag.getAudioStreamCount();
    if (audioStreamCount == 1) {
        return mediaTag.getSamplingRate(0);
    }
    return 0;
}

QString DlnaVideoFile::resolution() {
    int videoStreamCount = mediaTag.getVideoStreamCount();
    if (videoStreamCount == 1) {
        return mediaTag.getResolution(0);
    }
    return QString();
}

QStringList DlnaVideoFile::audioLanguages() {
    return mediaTag.getAudioLanguages();
}

QStringList DlnaVideoFile::subtitleLanguages() {
    return mediaTag.getSubtitleLanguages();
}

QString DlnaVideoFile::framerate() {
    return mediaTag.getVideoFrameRate();
}
