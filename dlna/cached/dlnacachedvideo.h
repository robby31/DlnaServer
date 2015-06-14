#ifndef DLNACACHEDVIDEO_H
#define DLNACACHEDVIDEO_H

#include "dlnavideoitem.h"
#include "ffmpegtranscoding.h"
#include "medialibrary.h"

class DlnaCachedVideo : public DlnaVideoItem
{
    Q_OBJECT

public:
    explicit DlnaCachedVideo(Logger* log, MediaLibrary* library, int idMedia, QString host, int port, QObject *parent = 0);

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const { return QString("Media(%1)").arg(idMedia); }

    virtual QString getSystemName() const { if (library != 0) return library->getmetaData("filename", idMedia).toString(); else return QString(); }

    //returns the size of the source
    virtual qint64 sourceSize() const { return -1; }

    // return true if the track shall be transcoded
    virtual bool toTranscode() const { return true; }

    virtual qint64 getResumeTime() const { if (library != 0) return library->getmetaData("progress_played", idMedia).toLongLong(); else return 0; }

    virtual int metaDataBitrate()              const { if (library != 0) return library->getmetaData("bitrate", idMedia).toInt(); else return -1; }
    virtual int metaDataDuration()             const { if (library != 0) return library->getmetaData("duration", idMedia).toInt(); else return -1; }
    virtual QString metaDataTitle()            const { return QFileInfo(getSystemName()).completeBaseName(); }
    virtual QString metaDataGenre()            const { return ""; }
    virtual QString metaDataPerformer()        const { return ""; }
    virtual QString metaDataAlbum()            const { return ""; }
    virtual QString metaDataAlbumArtist()      const { return ""; }
    virtual int metaDataTrackPosition()        const { return 0; }
    virtual int metaDataDisc()                 const { return 0; }
    virtual QString metaDataFormat()           const { if (library != 0) return library->getmetaData("format", idMedia).toString(); else return QString(); }
    virtual QByteArray metaDataPicture()       const { return QByteArray(); }
    virtual QString metaDataLastModifiedDate() const { if (library != 0) return library->getmetaData("last_modified", idMedia).toString(); else return QString(); }

    // returns the samplerate of the video track
    virtual int samplerate() const;

    //returns the channel number of the video track
    virtual int channelCount() const;

    virtual QString resolution()            const { if (library != 0) return library->getmetaData("resolution", idMedia).toString(); else return QString(); }
    virtual QStringList subtitleLanguages() const { if (library != 0) return library->getmetaData("subtitlelanguages", idMedia).toString().split(","); else return QStringList(); }
    virtual QStringList audioLanguages()    const { if (library != 0) return library->getmetaData("audiolanguages", idMedia).toString().split(","); else return QStringList(); }
    virtual QString framerate()             const;

protected:
    // Returns the process for transcoding
    virtual TranscodeProcess* getTranscodeProcess();

    MediaLibrary* library;
    int idMedia;
};

#endif // DLNACACHEDVIDEO_H
