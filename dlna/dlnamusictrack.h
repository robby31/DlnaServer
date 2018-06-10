#ifndef DLNAMUSICTRACK_H
#define DLNAMUSICTRACK_H

#include "dlnaitem.h"
#include "qffmpegtranscoding.h"

class DlnaMusicTrack : public DlnaItem
{
    Q_OBJECT

public:
    DlnaMusicTrack(QString host, int port, QObject *parent = 0);

    // Return upnp class
    virtual QString getUpnpClass() const Q_DECL_OVERRIDE { return QString("object.item.audioItem.musicTrack"); }

    // returns the bitrate of the audio track
    virtual int bitrate() const Q_DECL_OVERRIDE;

    // Returns the mimeType for this DLNA node.
    virtual QString mimeType() const Q_DECL_OVERRIDE;

    // Returns the mimeType of the source.
    virtual QString sourceMimeType() const Q_DECL_OVERRIDE;

    // Returns the XML (DIDL) representation of the DLNA node.
    virtual QDomElement getXmlContentDirectory(QDomDocument *xml, QStringList properties) Q_DECL_OVERRIDE;

    // Returns album art in jpeg format
    virtual QImage getAlbumArt() const Q_DECL_OVERRIDE;

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
    virtual QFfmpegTranscoding* getTranscodeProcess() Q_DECL_OVERRIDE;

    // Returns the process for original streaming
    virtual Device* getOriginalStreaming() Q_DECL_OVERRIDE;

private:
    virtual void updateDLNAOrgPn() Q_DECL_OVERRIDE;
};

#endif // DLNAMUSICTRACK_H
