#ifndef DLNAMUSICTRACKFILE_H
#define DLNAMUSICTRACKFILE_H

#include <QFileInfo>

#include "dlnamusictrack.h"
#include "qffmpeginputmedia.h"

class DlnaMusicTrackFile : public DlnaMusicTrack
{
    Q_OBJECT

public:
    explicit DlnaMusicTrackFile(QString filename, QString host, int port, QObject *parent = 0);
    ~DlnaMusicTrackFile();

    QFileInfo getFileInfo() const { return fileinfo; }

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() const Q_DECL_OVERRIDE;

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const Q_DECL_OVERRIDE { return fileinfo.fileName(); }

    virtual QString getSystemName() const Q_DECL_OVERRIDE { return fileinfo.absoluteFilePath(); }

    //returns the size of the source
    virtual qint64 sourceSize() const Q_DECL_OVERRIDE { return fileinfo.size(); }

    // return true if the track shall be transcoded
    virtual bool toTranscode() const Q_DECL_OVERRIDE;

    virtual int metaDataBitrate() const Q_DECL_OVERRIDE;
    virtual int metaDataDuration() const Q_DECL_OVERRIDE;
    virtual QString metaDataTitle() const Q_DECL_OVERRIDE;
    virtual QString metaDataGenre() const Q_DECL_OVERRIDE;
    virtual QString metaDataPerformer() const Q_DECL_OVERRIDE;
    virtual QString metaDataPerformerSort() const Q_DECL_OVERRIDE;
    virtual QString metaDataAlbum() const Q_DECL_OVERRIDE;
    virtual QString metaDataAlbumArtist() const Q_DECL_OVERRIDE;
    virtual int metaDataYear() const Q_DECL_OVERRIDE;
    virtual int metaDataTrackPosition() const Q_DECL_OVERRIDE;
    virtual int metaDataDisc() const Q_DECL_OVERRIDE;
    virtual QString metaDataFormat() const Q_DECL_OVERRIDE;
    virtual QByteArray metaDataPicture() const Q_DECL_OVERRIDE;
    virtual QString metaDataLastModifiedDate() const Q_DECL_OVERRIDE { return fileinfo.lastModified().toString("yyyy-MM-dd"); }

    // returns the samplerate of the audio track
    virtual int samplerate() const Q_DECL_OVERRIDE;

    //returns the channel number of the audio track
    virtual int channelCount() const Q_DECL_OVERRIDE;

    virtual QHash<QString, double> volumeInfo(const int timeout = 30000) Q_DECL_OVERRIDE;

protected:
    // Returns the process for transcoding
    virtual QFfmpegTranscoding* getTranscodeProcess() Q_DECL_OVERRIDE;

private:
    QFileInfo fileinfo;
    QMimeType mime_type;
    QFfmpegInputMedia ffmpeg;

public:
    static qint64 objectCounter;
};

#endif // DLNAMUSICTRACKFILE_H
