#ifndef DLNAMUSICTRACK_H
#define DLNAMUSICTRACK_H

#include "dlnaitem.h"
#include "ffmpegtranscoding.h"

class DlnaMusicTrack : public DlnaItem
{
    Q_OBJECT

public:
    DlnaMusicTrack(Logger* log, QString filename, QString host, int port, QObject *parent = 0);

    // Return upnp class
    virtual QString getUpnpClass() const { return QString("object.item.audioItem.musicTrack"); }

    // returns the bitrate of the audio track
    virtual int bitrate();

    // Returns the mimeType for this DLNA node.
    virtual QString mimeType();

    // Returns the XML (DIDL) representation of the DLNA node.
    virtual QDomElement getXmlContentDirectory(QDomDocument *xml, QStringList properties);

    // return true if the track shall be transcoded
    virtual bool toTranscode() const { return mime_type.name() != "audio/mpeg"; }

    // Returns the process for transcoding
    virtual FfmpegTranscoding* getTranscodeProcess(HttpRange* range, long timeseek_start=-1, long timeseek_end=-1);

    // Returns album art in jpeg format
    virtual QImage getAlbumArt();

    static const QString UNKNOWN_AUDIO_TYPEMIME;
    static const QString AUDIO_MP3_TYPEMIME;
    static const QString AUDIO_MP4_TYPEMIME;
    static const QString AUDIO_WAV_TYPEMIME;
    static const QString AUDIO_WMA_TYPEMIME;
    static const QString AUDIO_FLAC_TYPEMIME;
    static const QString AUDIO_OGG_TYPEMIME;
    static const QString AUDIO_LPCM_TYPEMIME;
    static const QString AUDIO_TRANSCODE;

protected:

    virtual void updateDLNAOrgPn();
};

#endif // DLNAMUSICTRACK_H
