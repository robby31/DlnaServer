#ifndef DLNACACHEDVIDEO_H
#define DLNACACHEDVIDEO_H

#include "dlnavideoitem.h"
#include "medialibrary.h"

class DlnaCachedVideo : public DlnaVideoItem
{

public:
    explicit DlnaCachedVideo(Logger* log, QString filename, MediaLibrary* library, int idMedia, QString host, int port, QObject *parent = 0);

    virtual int metaDataBitrate() { if (library != 0) return library->getmetaData("bitrate", idMedia).toInt(); else return -1; }
    virtual int metaDataDuration() { if (library != 0) return library->getmetaData("duration", idMedia).toInt(); else return -1; }
    virtual QString metaDataTitle() { return ""; }
    virtual QString metaDataGenre() { return ""; }
    virtual QString metaDataPerformer() { return ""; }
    virtual QString metaDataAlbum() { return ""; }
    virtual QString metaDataTrackPosition() { return ""; }
    virtual QString metaDataFormat() { if (library != 0) return library->getmetaData("format", idMedia).toString(); else return QString(); }
    virtual QByteArray metaDataPicture() { return QByteArray(); }

    // returns the samplerate of the audio track
    virtual int samplerate() { if (library != 0) return library->getmetaData("samplerate", idMedia).toInt(); else return -1; }

    //returns the channel number of the audio track
    virtual int channelCount() { if (library != 0) return library->getmetaData("channelcount", idMedia).toInt(); else return -1; }

    virtual QString resolution() { if (library != 0) return library->getmetaData("resolution", idMedia).toString(); else return QString(); }
    virtual QStringList subtitleLanguages() { if (library != 0) return library->getmetaData("subtitlelanguages", idMedia).toString().split(","); else return QStringList(); }
    virtual QStringList audioLanguages() { if (library != 0) return library->getmetaData("audiolanguages", idMedia).toString().split(","); else return QStringList(); }
    virtual QString framerate() { if (library != 0) return library->getmetaData("framerate", idMedia).toString(); else return QString(); }

private:
    MediaLibrary* library;
    int idMedia;
};

#endif // DLNACACHEDVIDEO_H
