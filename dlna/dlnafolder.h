#ifndef DLNAFOLDER_H
#define DLNAFOLDER_H

#include "dlnaresource.h"
#include "dlnamusictrack.h"
#include <QFileInfo>
#include <QDir>

class DlnaFolder : public DlnaResource
{
public:
    DlnaFolder(Logger* log, QString filename, QString host, int port);
    ~DlnaFolder();

    virtual bool discoverChildren();

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const;

    virtual QString getSystemName() const;

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() const;

    virtual bool isFolder() const { return true; }

    // Returns the XML (DIDL) representation of the DLNA node.
    virtual QDomElement getXmlContentDirectory(QDomDocument *xml);

    // return the size of the DLNA node, no size for a folder.
    virtual int size() { return -1; }

    // Returns an InputStream of this DLNA node, no stream for a folder.
    virtual QByteArray getStream(HttpRange* range) { return QByteArray(); }

    // Returns the process for transcoding, no transcoding for a folder.
    virtual QProcess* getTranscodeProcess(HttpRange* range) { return 0; }

    // return true if the DLNA node shall be transcoded, no transcoding for a folder.
    virtual bool toTranscode() const { return false; }

    // Returns the mimeType for this DLNA node.
    virtual QString mimeType() const { return ""; }

private:
    QFileInfo fileinfo;
    QString host;
    int port;
};

#endif // DLNAFOLDER_H
