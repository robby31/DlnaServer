#ifndef DLNAMUSICTRACKFILE_H
#define DLNAMUSICTRACKFILE_H

#include <QFileInfo>

#include "dlnamusictrack.h"
#include "qffmpeg.h"

class DlnaMusicTrackFile : public DlnaMusicTrack
{
    Q_OBJECT

public:
    explicit DlnaMusicTrackFile(QString filename, QString host, int port, QObject *parent = 0);
    ~DlnaMusicTrackFile();

    QFileInfo getFileInfo() const { return fileinfo; }

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() const;

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const { return fileinfo.fileName(); }

    virtual QString getSystemName() const { return fileinfo.absoluteFilePath(); }

    //returns the size of the source
    virtual qint64 sourceSize() const { return fileinfo.size(); }

    // return true if the track shall be transcoded
    virtual bool toTranscode() const;

    virtual int metaDataBitrate() const ;
    virtual int metaDataDuration() const ;
    virtual QString metaDataTitle() const;
    virtual QString metaDataGenre() const;
    virtual QString metaDataPerformer() const;
    virtual QString metaDataPerformerSort() const;
    virtual QString metaDataAlbum() const;
    virtual QString metaDataAlbumArtist() const;
    virtual int metaDataYear() const;
    virtual int metaDataTrackPosition() const;
    virtual int metaDataDisc() const;
    virtual QString metaDataFormat() const;
    virtual QByteArray metaDataPicture() const;
    virtual QString metaDataLastModifiedDate() const { return fileinfo.lastModified().toString("yyyy-MM-dd"); }

    // returns the samplerate of the audio track
    virtual int samplerate() const;

    //returns the channel number of the audio track
    virtual int channelCount() const;

    virtual QHash<QString, double> volumeInfo(const int timeout = 30000);

private:
    QFileInfo fileinfo;
    QMimeType mime_type;
    QFfmpeg ffmpeg;

public:
    static qint64 objectCounter;
};

#endif // DLNAMUSICTRACKFILE_H
