#ifndef DLNACACHEDMUSICTRACK_H
#define DLNACACHEDMUSICTRACK_H

#include "dlnamusictrack.h"
#include "medialibrary.h"

class DlnaCachedMusicTrack : public DlnaMusicTrack
{

public:
    explicit DlnaCachedMusicTrack(Logger* log, QString filename, MediaLibrary* library, int idMedia, QString host, int port, QObject *parent = 0);

    virtual int metaDataBitrate() { if (library != 0) return library->getmetaData("bitrate", idMedia).toInt(); else return -1; }
    virtual int metaDataDuration() { if (library != 0) return library->getmetaData("duration", idMedia).toInt(); else return -1; }
    virtual QString metaDataTitle() { if (library != 0) return library->getmetaData("title", idMedia).toString(); else return QString(); }
    virtual QString metaDataGenre() { if (library != 0) return library->getmetaData("genre", idMedia).toString(); else return QString(); }
    virtual QString metaDataPerformer() { if (library != 0) return library->getmetaData("artist", idMedia).toString(); else return QString(); }
    virtual QString metaDataAlbum() { if (library != 0) return library->getmetaData("album", idMedia).toString(); else return QString(); }
    virtual QString metaDataTrackPosition() { if (library != 0) return library->getmetaData("trackposition", idMedia).toString(); else return QString(); }
    virtual QString metaDataFormat() { if (library != 0) return library->getmetaData("format", idMedia).toString(); else return QString(); }
    virtual QByteArray metaDataPicture() { if (library != 0) return QByteArray::fromHex(library->getmetaData("picture", idMedia).toByteArray()); else return QByteArray(); }

    // returns the samplerate of the audio track
    virtual int samplerate() { if (library != 0) return library->getmetaData("samplerate", idMedia).toInt(); else return -1; }

    //returns the channel number of the audio track
    virtual int channelCount() { if (library != 0) return library->getmetaData("channelcount", idMedia).toInt(); else return -1; }

private:
    MediaLibrary* library;
    int idMedia;
};

#endif // DLNACACHEDMUSICTRACK_H
