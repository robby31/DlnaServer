#include "dlnamusictrackfile.h"

qint64 DlnaMusicTrackFile::objectCounter = 0;

DlnaMusicTrackFile::DlnaMusicTrackFile(QString filename, QString host, int port, QObject *parent):
    DlnaMusicTrack(host, port, parent),
    fileinfo(filename)
{
    ++objectCounter;

    ffmpeg.open(filename, true);

    QMimeDatabase db;
    mime_type = db.mimeTypeForFile(fileinfo);

    setTranscodeFormat(MP3);   // default transcode format
}

DlnaMusicTrackFile::~DlnaMusicTrackFile()
{
    --objectCounter;
}

bool DlnaMusicTrackFile::toTranscode() const
{
    if (format() == WAV)
        return mime_type.name() != AUDIO_WAV_TYPEMIME;
    else if (format() == AAC || format() == ALAC)
        return mime_type.name() != AUDIO_MP4_TYPEMIME;
    else
        return mime_type.name() != AUDIO_MP3_TYPEMIME;
}

QString DlnaMusicTrackFile::getDisplayName() const {
    QString title = metaDataTitle();
    if (title.isEmpty())
        title = fileinfo.completeBaseName();
    if (title.isEmpty())
        title = getSystemName();
    return title;
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

QString DlnaMusicTrackFile::metaDataPerformerSort() const {
    return ffmpeg.metaData("artist-sort");
}

QString DlnaMusicTrackFile::metaDataAlbum() const {
    return ffmpeg.metaData("album");
}

QString DlnaMusicTrackFile::metaDataAlbumArtist() const {
    return ffmpeg.metaData("album_artist");
}

int DlnaMusicTrackFile::metaDataYear() const
{
    QString rawData = ffmpeg.metaData("date");

    if (rawData.isEmpty())
        return -1;

    bool valid = false;
    int year = rawData.toInt(&valid);
    if (valid)
        return year;

    QDateTime date = QDateTime::fromString(rawData, "yyyy-MM-dd");
    if (date.isValid())
        return date.toString("yyyy").toInt();

    date = QDateTime::fromString(rawData, "yyyy-MM");
    if (date.isValid())
        return date.toString("yyyy").toInt();

    qCritical() << "invalid date format" << rawData << fileinfo.absoluteFilePath();

    return -1;
}

int DlnaMusicTrackFile::metaDataTrackPosition() const {
    return ffmpeg.metaData("track").split('/').at(0).toInt();
}

int DlnaMusicTrackFile::metaDataDisc() const
{
    return ffmpeg.metaData("disc").split('/').at(0).toInt();
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

QHash<QString, double> DlnaMusicTrackFile::volumeInfo(const int timeout)
{
    return ffmpeg.getVolumeInfo(timeout);
}

QFfmpegTranscoding *DlnaMusicTrackFile::getTranscodeProcess()
{
    QFfmpegTranscoding* transcodeProcess = new QFfmpegTranscoding();
    transcodeProcess->setFormat(transcodeFormat);
    transcodeProcess->setBitrate(bitrate());
//    transcodeProcess->setVolumeInfo(volumeInfo());
    transcodeProcess->setInput(&ffmpeg);

    connect(transcodeProcess, SIGNAL(openedSignal()), transcodeProcess, SLOT(startDemux()));

    return transcodeProcess;
}
