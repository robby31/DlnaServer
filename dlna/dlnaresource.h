#ifndef DLNARESOURCE_H
#define DLNARESOURCE_H

#include <QtXml>
#include <QImage>

#include "logger.h"
#include "httprange.h"
#include "transcodeprocess.h"

/*
 * Represents any item which can be browsed via the UPNP ContentDirectory service.
 * It may be a folder, an audio track or a video.
 */

class DlnaResource: public QObject
{
    Q_OBJECT

public:
    DlnaResource(Logger* log, QObject *parent = 0);
    ~DlnaResource();

    Logger* getLog() const { return log; }

    QString getId() const { return id; }
    void setId(QString id) { this->id = id; }

    /*
     * String representing this resource ID. This string is used by the UPNP
     * ContentDirectory service. There is no hard spec on the actual numbering
     * except for the root container that always has to be "0". In PMS the
     * format used is <i>number($number)+</i>. A common client that expects a
     * different format than the one used here is the XBox360. PMS translates
     * the XBox360 queries on the fly. For more info, check
     * http://www.mperfect.net/whsUpnp360/
     */
    QString getResourceId() const;

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const = 0;

    virtual QString getSystemName() const = 0;

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() = 0;

    virtual bool isFolder() const = 0;

    // Returns the XML (DIDL) representation of the DLNA node.
    virtual QDomElement getXmlContentDirectory(QDomDocument *xml, QStringList properties) = 0;
    QString getStringContentDirectory(QStringList properties);

    // Counter for this resource.
    // When the resource needs to be refreshed, its counter is incremented.
    int getUpdateId() const { return updateId; }
    void setUpdateId(int updateId) { this->updateId = updateId; }

    DlnaResource *getParent() const { return parent; }
    QString getParentId() const;
    void setParent(DlnaResource *parent) { this->parent = parent; }

    /*
     * Adds a new DLNAResource to the child list.
     * Only useful if this object is of the container type.
     */
    void addChild(DlnaResource *child);

    QList<DlnaResource*> getChildren();
    // Parse children and return true if done
    virtual bool discoverChildren() = 0;

    // Returns true when the details of this resource have already been investigated.
    bool isDiscovered() const;
    void setDiscovered(bool discovered) { this->discovered = discovered; }

    // Recursive function that searches for a given ID such as 0$2$13.
    DlnaResource* search(QString searchId, QString searchStr);

    // Search for an item matching the given objectID if returnChildren is false.
    // if returnChildren is true it returns all children of the objectID.
    QList<DlnaResource*> getDLNAResources(QString objectId, bool returnChildren, int start, int count, QString searchStr);

    // return the size of the media
    virtual long size() = 0;

    // return the length in seconds of the media
    virtual int getLengthInSeconds() = 0;
    virtual int getLengthInMilliSeconds() = 0;

    // Returns the stream of this DLNA node.
    virtual QIODevice* getStream() = 0;

    // Returns the process for transcoding
    virtual TranscodeProcess* getTranscodeProcess(HttpRange* range, long timeseek_start=-1, long timeseek_end=-1) = 0;

    // return true if the track shall be transcoded
    virtual bool toTranscode() const = 0;

    // Returns the mimeType for this DLNA node.
    virtual QString mimeType() = 0;

    QString getDlnaContentFeatures() const;

    QString getProtocolInfo();

    QString getdlnaOrgOpFlags() const { return dlnaOrgOpFlags; }
    void setdlnaOrgOpFlags (QString arg) { dlnaOrgOpFlags=arg; }

    QString getdlnaOrgPN() const { return dlnaOrgPN; }
    void setdlnaOrgPN(QString arg) { dlnaOrgPN=arg; }

    // Returns album art in jpeg format
    virtual QImage getAlbumArt() = 0;
    QByteArray getByteAlbumArt();

private:
    Logger* log;

    // id of this resource based on the index in its parent container.
    // Its main purpose is to be unique in the parent container.
    QString id;

    // parent of the DlnaResource
    DlnaResource *parent;

    // children of the DlnaResource
    QList<DlnaResource*> children;

    // flag to knwow if children have been parsed
    bool discovered;

    // update counter for this resource.
    // When the resource needs to be refreshed, its counter should be updated.
    int updateId;

    /*
     * DLNA.ORG_OP flags
     *
     * Two booleans (binary digits) which determine what transport operations the renderer is allowed to
     * perform (in the form of HTTP request headers): the first digit allows the renderer to send
     * TimeSeekRange.DLNA.ORG (seek by time) headers; the second allows it to send RANGE (seek by byte)
     * headers.
     *
     *    00 - no seeking (or even pausing) allowed
     *    01 - seek by byte
     *    10 - seek by time
     *    11 - seek by both
     *
     * See here for an example of how these options can be mapped to keys on the renderer's controller:
     * http://www.ps3mediaserver.org/forum/viewtopic.php?f=2&t=2908&p=12550#p12550
     *
     * Note that seek-by-byte is the preferred option for streamed files [1] and seek-by-time is the
     * preferred option for transcoded files.
     *
     * [1] see http://www.ps3mediaserver.org/forum/viewtopic.php?f=6&t=15841&p=76201#p76201
     *
     * seek-by-time requires a) support by the renderer (via the SeekByTime renderer conf option)
     * and b) support by the transcode engine.
     *
     * The seek-by-byte fallback doesn't work well with transcoded files [2], but it's better than
     * disabling seeking (and pausing) altogether.
     *
     * [2] http://www.ps3mediaserver.org/forum/viewtopic.php?f=6&t=3507&p=16567#p16567 (bottom post)
     */
    QString dlnaOrgOpFlags;

    // DLNA.ORG_PN
    QString dlnaOrgPN;

};

#endif // DLNARESOURCE_H
