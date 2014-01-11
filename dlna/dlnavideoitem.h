#ifndef DLNAVIDEOITEM_H
#define DLNAVIDEOITEM_H

#include "dlnamusictrack.h"

class DlnaVideoItem : public DlnaMusicTrack
{
    Q_OBJECT

public:
    DlnaVideoItem(Logger* log, QString filename, QString host, int port);

    // Returns the XML (DIDL) representation of the DLNA node.
    virtual QDomElement getXmlContentDirectory(QDomDocument *xml, QStringList properties);

    // Returns the mimeType for this DLNA node.
    virtual QString mimeType();

    // return true if the track shall be transcoded
    virtual bool toTranscode() const { return true; }

    // Returns the process for transcoding
    virtual QProcess* getTranscodeProcess(HttpRange* range);

    QString resolution();

    static const QString UNKNOWN_VIDEO_TYPEMIME;
    static const QString MPEG_TYPEMIME;
    static const QString MP4_TYPEMIME;
    static const QString AVI_TYPEMIME;
    static const QString WMV_TYPEMIME;
    static const QString ASF_TYPEMIME;
    static const QString MATROSKA_TYPEMIME;
    static const QString VIDEO_TRANSCODE;

protected:
    virtual void updateDLNAOrgPn();
};

#endif // DLNAVIDEOITEM_H
