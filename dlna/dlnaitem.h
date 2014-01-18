#ifndef DLNAITEM_H
#define DLNAITEM_H

#include <QFileInfo>
#include <QFile>

#include "logger.h"
#include "dlnaresource.h"
#include "metadata.h"

class DlnaItem : public DlnaResource
{
    Q_OBJECT
public:
    explicit DlnaItem(Logger* log, QString filename, QString host, int port, QObject *parent = 0);

    virtual bool discoverChildren() { return true; }

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const;

    virtual QString getSystemName() const;

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName();

    virtual bool isFolder() const { return false; }

    QFileInfo getFileInfo() const { return fileinfo; }

    // Returns an InputStream of this DLNA node.
    virtual QIODevice* getStream();

    void setTranscodeFormat(TranscodeFormatAvailable format);

    // return the size of the audio track
    virtual long size();

    // return the length in seconds of the media
    virtual int getLengthInSeconds();
    virtual int getLengthInMilliSeconds();

    // returns the bitrate of the audio track
    virtual int bitrate() = 0;

signals:

public slots:

protected:
    QFileInfo fileinfo;

    MetaData mediaTag;

    QMimeType mime_type;
    QString host;
    int port;

    TranscodeFormatAvailable transcodeFormat;

    virtual void updateDLNAOrgPn() = 0;
};

#endif // DLNAITEM_H
