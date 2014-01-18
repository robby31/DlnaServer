#ifndef DLNAROOTFOLDER_H
#define DLNAROOTFOLDER_H

#include <QFileInfo>
#include "dlnaresource.h"
#include "dlnafolder.h"

class DlnaRootFolder : public DlnaResource
{
public:
    DlnaRootFolder(Logger* log, QString host, int port);
    ~DlnaRootFolder();

    virtual bool discoverChildren() { return true; }

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const;

    virtual QString getSystemName() const;

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName();

    virtual bool isFolder() const { return true; }

    // returns true if the folder is added to Root.
    bool addFolder(QString folder);

    // Returns the XML (DIDL) representation of the DLNA node.
    virtual QDomElement getXmlContentDirectory(QDomDocument *xml, QStringList properties);

    // return the size of the DLNA node, no size for a folder.
    virtual long size() { return -1; }

    // return the length in seconds of the media
    virtual int getLengthInSeconds() { return -1; }
    virtual int getLengthInMilliSeconds() { return -1; }

    // Returns an InputStream of this DLNA node, no stream for a folder.
    virtual QIODevice* getStream() { return 0; }

    // Returns the process for transcoding, no transcoding for a folder.
    virtual TranscodeProcess* getTranscodeProcess(HttpRange* range) { Q_UNUSED(range); return 0; }

    // return true if the DLNA node shall be transcoded, no transcoding for a folder.
    virtual bool toTranscode() const { return false; }

    // Returns the mimeType for this DLNA node.
    virtual QString mimeType() { return ""; }

    virtual QImage getAlbumArt();

private:
    QString host;
    int port;
};

#endif // DLNAROOTFOLDER_H
