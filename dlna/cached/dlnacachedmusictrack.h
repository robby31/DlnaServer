#ifndef DLNACACHEDMUSICTRACK_H
#define DLNACACHEDMUSICTRACK_H

#include "dlnamusictrack.h"
#include "medialibrary.h"

class DlnaCachedMusicTrack : public DlnaMusicTrack
{
    Q_OBJECT

public:
    explicit DlnaCachedMusicTrack(MediaLibrary* library, int idMedia, QString host, int port, QObject *parent = 0);

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const Q_DECL_OVERRIDE { return QString("Media(%1)").arg(idMedia); }

    virtual QString getSystemName() const Q_DECL_OVERRIDE { if (library != 0) return library->getmetaData("filename", idMedia).toString(); else return QString(); }

    //returns the size of the source
    virtual qint64 sourceSize() const Q_DECL_OVERRIDE { return (double)metaDataDuration()*(double)metaDataBitrate()/8000.0; }

    virtual qint64 getResumeTime() const Q_DECL_OVERRIDE;

    virtual int metaDataBitrate()              const Q_DECL_OVERRIDE { if (library != 0) return library->getmetaData("bitrate", idMedia).toInt(); else return -1; }
    virtual int metaDataDuration()             const Q_DECL_OVERRIDE { if (library != 0) return library->getmetaData("duration", idMedia).toInt(); else return -1; }
    virtual QString metaDataTitle()            const Q_DECL_OVERRIDE { if (library != 0) return library->getmetaData("title", idMedia).toString(); else return QString(); }
    virtual QString metaDataGenre()            const Q_DECL_OVERRIDE { if (library != 0) return library->getmetaData("genre", idMedia).toString(); else return QString(); }
    virtual QString metaDataPerformer()        const Q_DECL_OVERRIDE { if (library != 0) return library->getmetaData("artist", idMedia).toString(); else return QString(); }
    virtual QString metaDataPerformerSort()    const Q_DECL_OVERRIDE { if (library != 0) return library->getmetaDataArtist("artist_sort", idMedia).toString(); else return QString(); }
    virtual QString metaDataAlbum()            const Q_DECL_OVERRIDE { if (library != 0) return library->getmetaData("album", idMedia).toString(); else return QString(); }
    virtual QString metaDataAlbumArtist()      const Q_DECL_OVERRIDE { if (library != 0) return library->getmetaDataAlbum("artist_name", idMedia).toString(); else return QString(); }
    virtual int metaDataYear()                 const Q_DECL_OVERRIDE { if (library != 0) return library->getmetaDataAlbum("year", idMedia).toInt(); else return -1; }
    virtual int metaDataTrackPosition()        const Q_DECL_OVERRIDE { if (library != 0) return library->getmetaData("trackposition", idMedia).toInt(); else return 0; }
    virtual int metaDataDisc()                 const Q_DECL_OVERRIDE { if (library != 0) return library->getmetaData("disc", idMedia).toInt(); else return 0; }
    virtual QString metaDataFormat()           const Q_DECL_OVERRIDE { if (library != 0) return library->getmetaData("format", idMedia).toString(); else return QString(); }
    virtual QByteArray metaDataPicture()       const Q_DECL_OVERRIDE { if (library != 0) return QByteArray::fromHex(library->getmetaData("picture", idMedia).toByteArray()); else return QByteArray(); }
    virtual QString metaDataLastModifiedDate() const Q_DECL_OVERRIDE { if (library != 0) return library->getmetaData("last_modified", idMedia).toDateTime().toString("yyyy-MM-dd"); else return QString(); }

    // returns the samplerate of the audio track
    virtual int samplerate() const Q_DECL_OVERRIDE { if (library != 0) return library->getmetaData("samplerate", idMedia).toInt(); else return -1; }

    //returns the channel number of the audio track
    virtual int channelCount() const Q_DECL_OVERRIDE { if (library != 0) return library->getmetaData("channelcount", idMedia).toInt(); else return -1; }

    virtual QHash<QString, double> volumeInfo(const int timeout = 30000) Q_DECL_OVERRIDE;

private:
    MediaLibrary* library;
    int idMedia;
};

#endif // DLNACACHEDMUSICTRACK_H
