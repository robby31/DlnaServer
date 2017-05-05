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
    virtual QString getName() const { return QString("Media(%1)").arg(idMedia); }

    virtual QString getSystemName() const { if (library != 0) return library->getmetaData("filename", idMedia).toString(); else return QString(); }

    //returns the size of the source
    virtual qint64 sourceSize() const { return -1; }

    // return true if the track shall be transcoded
    virtual bool toTranscode() const { return true; }

    virtual qint64 getResumeTime() const;

    virtual int metaDataBitrate()              const { if (library != 0) return library->getmetaData("bitrate", idMedia).toInt(); else return -1; }
    virtual int metaDataDuration()             const { if (library != 0) return library->getmetaData("duration", idMedia).toInt(); else return -1; }
    virtual QString metaDataTitle()            const { if (library != 0) return library->getmetaData("title", idMedia).toString(); else return QString(); }
    virtual QString metaDataGenre()            const { if (library != 0) return library->getmetaData("genre", idMedia).toString(); else return QString(); }
    virtual QString metaDataPerformer()        const { if (library != 0) return library->getmetaData("artist", idMedia).toString(); else return QString(); }
    virtual QString metaDataPerformerSort()    const { if (library != 0) return library->getmetaDataArtist("artist_sort", idMedia).toString(); else return QString(); }
    virtual QString metaDataAlbum()            const { if (library != 0) return library->getmetaData("album", idMedia).toString(); else return QString(); }
    virtual QString metaDataAlbumArtist()      const { if (library != 0) return library->getmetaDataAlbum("artist_name", idMedia).toString(); else return QString(); }
    virtual int metaDataYear()                 const { if (library != 0) return library->getmetaDataAlbum("year", idMedia).toInt(); else return -1; }
    virtual int metaDataTrackPosition()        const { if (library != 0) return library->getmetaData("trackposition", idMedia).toInt(); else return 0; }
    virtual int metaDataDisc()                 const { if (library != 0) return library->getmetaData("disc", idMedia).toInt(); else return 0; }
    virtual QString metaDataFormat()           const { if (library != 0) return library->getmetaData("format", idMedia).toString(); else return QString(); }
    virtual QByteArray metaDataPicture()       const { if (library != 0) return QByteArray::fromHex(library->getmetaData("picture", idMedia).toByteArray()); else return QByteArray(); }
    virtual QString metaDataLastModifiedDate() const { if (library != 0) return library->getmetaData("last_modified", idMedia).toDateTime().toString("yyyy-MM-dd"); else return QString(); }

    // returns the samplerate of the audio track
    virtual int samplerate() const { if (library != 0) return library->getmetaData("samplerate", idMedia).toInt(); else return -1; }

    //returns the channel number of the audio track
    virtual int channelCount() const { if (library != 0) return library->getmetaData("channelcount", idMedia).toInt(); else return -1; }

    virtual QHash<QString, double> volumeInfo(const int timeout = 30000);

private:
    MediaLibrary* library;
    int idMedia;
};

#endif // DLNACACHEDMUSICTRACK_H
