#ifndef DLNAVIDEOITEM_H
#define DLNAVIDEOITEM_H

#include "dlnaitem.h"
#include "mencodertranscoding.h"

class DlnaVideoItem : public DlnaItem
{

public:
    DlnaVideoItem(Logger* log, QString filename, QString host, int port, QObject *parent = 0);
    virtual ~DlnaVideoItem();

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() { return fileinfo.completeBaseName(); }

    // Return upnp class
    virtual QString getUpnpClass() const { return QString("object.item.videoItem"); }

    // Returns the XML (DIDL) representation of the DLNA node.
    virtual QDomElement getXmlContentDirectory(QDomDocument *xml, QStringList properties);

    // Returns the mimeType for this DLNA node.
    virtual QString mimeType();

    // returns the bitrate of the movie
    virtual int bitrate();

    // return true if the track shall be transcoded
    virtual bool toTranscode() const { return true; }

    // Returns the process for transcoding
    virtual MencoderTranscoding* getTranscodeProcess(HttpRange* range, long timeseek_start=-1, long timeseek_end=-1, QObject *parent=0);

    // Returns album art in jpeg format
    virtual QImage getAlbumArt() { return QImage(); }


    virtual QString resolution() = 0;
    virtual QStringList subtitleLanguages() = 0;
    virtual QStringList audioLanguages() = 0;
    virtual QString framerate() = 0;

    static const QString UNKNOWN_VIDEO_TYPEMIME;
    static const QString MPEG_TYPEMIME;
    static const QString MP4_TYPEMIME;
    static const QString AVI_TYPEMIME;
    static const QString WMV_TYPEMIME;
    static const QString ASF_TYPEMIME;
    static const QString MATROSKA_TYPEMIME;
    static const QString VIDEO_TRANSCODE;

private:
    virtual void updateDLNAOrgPn();
};

#endif // DLNAVIDEOITEM_H
