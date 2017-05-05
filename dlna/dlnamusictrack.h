#ifndef DLNAMUSICTRACK_H
#define DLNAMUSICTRACK_H

#include "dlnaitem.h"
#include "ffmpegtranscoding.h"

class DlnaMusicTrack : public DlnaItem
{
    Q_OBJECT

public:
    DlnaMusicTrack(QString host, int port, QObject *parent = 0);

    // Return upnp class
    virtual QString getUpnpClass() const { return QString("object.item.audioItem.musicTrack"); }

    // returns the bitrate of the audio track
    virtual int bitrate() const;

    // Returns the mimeType for this DLNA node.
    virtual QString mimeType() const;

    // Returns the XML (DIDL) representation of the DLNA node.
    virtual QDomElement getXmlContentDirectory(QDomDocument *xml, QStringList properties) const;

    // Returns album art in jpeg format
    virtual QImage getAlbumArt() const;

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
    // Returns the process for transcoding
    virtual FfmpegTranscoding* getTranscodeProcess();

private:
    virtual void updateDLNAOrgPn();
};

#endif // DLNAMUSICTRACK_H
