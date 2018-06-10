#ifndef DLNAVIDEOITEM_H
#define DLNAVIDEOITEM_H

#include "dlnaitem.h"

class DlnaVideoItem : public DlnaItem
{
    Q_OBJECT

public:
    DlnaVideoItem(QString host, int port, QObject *parent = 0);

    // Return upnp class
    virtual QString getUpnpClass() const Q_DECL_OVERRIDE { return QString("object.item.videoItem"); }

    // Returns the XML (DIDL) representation of the DLNA node.
    virtual QDomElement getXmlContentDirectory(QDomDocument *xml, QStringList properties) Q_DECL_OVERRIDE;

    // Returns the mimeType for this DLNA node.
    virtual QString mimeType() const Q_DECL_OVERRIDE;

    // Returns the mimeType of the source.
    virtual QString sourceMimeType() const Q_DECL_OVERRIDE;

    // returns the bitrate of the movie
    virtual int bitrate() const Q_DECL_OVERRIDE;

    // return true if the track shall be transcoded
    virtual bool toTranscode() const Q_DECL_OVERRIDE { return true; }

    // Returns album art in jpeg format
    virtual QImage getAlbumArt() const Q_DECL_OVERRIDE { return QImage(); }

    virtual QString resolution() const = 0;
    virtual QStringList subtitleLanguages() const = 0;
    virtual QStringList audioLanguages() const = 0;
    virtual QString framerate() const = 0;

    static const QString UNKNOWN_VIDEO_TYPEMIME;
    static const QString MPEG_TYPEMIME;
    static const QString MP4_TYPEMIME;
    static const QString AVI_TYPEMIME;
    static const QString WMV_TYPEMIME;
    static const QString ASF_TYPEMIME;
    static const QString MATROSKA_TYPEMIME;
    static const QString VIDEO_TRANSCODE;

private:
    virtual void updateDLNAOrgPn() Q_DECL_OVERRIDE;
};

#endif // DLNAVIDEOITEM_H
