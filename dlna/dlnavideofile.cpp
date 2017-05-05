#include "dlnavideofile.h"

qint64 DlnaVideoFile::objectCounter = 0;

DlnaVideoFile::DlnaVideoFile(QString filename, QString host, int port, QObject *parent):
    DlnaVideoItem(host, port, parent),
    fileinfo(filename),
    mime_type(),
    ffmpeg(filename, this)
{
    ++objectCounter;

    QMimeDatabase db;
    mime_type = db.mimeTypeForFile(fileinfo);
}

DlnaVideoFile::~DlnaVideoFile() {
    --objectCounter;

}

TranscodeProcess *DlnaVideoFile::getTranscodeProcess()
{
    FfmpegTranscoding* transcodeProcess = new FfmpegTranscoding();
    transcodeProcess->setUrl(getSystemName());
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

QString DlnaVideoFile::metaDataPerformerSort() const{
    return ffmpeg.metaData("artist-sort");
}

QString DlnaVideoFile::metaDataAlbum() const {
    return ffmpeg.metaData("album");
}

QString DlnaVideoFile::metaDataAlbumArtist() const {
    return ffmpeg.metaData("album_artist");
}

int DlnaVideoFile::metaDataYear() const
{
    QDateTime date = QDateTime::fromString(ffmpeg.metaData("date"), "yyyy-MM-dd");
    if (date.isValid())
        return date.toString("yyyy").toInt();
    return -1;
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

int DlnaVideoFile::channelCount() const
{
    return ffmpeg.getAudioChannelCount();
}

int DlnaVideoFile::samplerate() const
{
    return ffmpeg.getAudioSamplerate();
}

QHash<QString, double> DlnaVideoFile::volumeInfo(const int timeout)
{
    return ffmpeg.getVolumeInfo(timeout);
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
