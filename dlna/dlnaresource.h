#ifndef DLNARESOURCE_H
#define DLNARESOURCE_H

#include <QObject>
#include <QtXml>
#include <QImage>

#include "logobject.h"

/*
 * Represents any item which can be browsed via the UPNP ContentDirectory service.
 * It may be a folder, an audio track or a video.
 */

class DlnaResource: public LogObject
{
    Q_OBJECT

public:
    explicit DlnaResource(Logger* log, QObject *parent = 0);
    virtual ~DlnaResource();

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
    virtual QString getDisplayName() const = 0;

    // Return upnp class
    virtual QString getUpnpClass() const = 0;

    virtual bool isFolder() const = 0;

    // Returns the XML (DIDL) representation of the DLNA node.
    virtual QDomElement getXmlContentDirectory(QDomDocument *xml, QStringList properties) const = 0;
    virtual void updateXmlContentDirectory(QDomDocument *xml, QDomElement* xml_obj, QStringList properties) const;
    QString getStringContentDirectory(QStringList properties) const;

    // ressource shall be refreshed
    void needRefresh() { m_needRefresh = true; }

    // Counter for this resource.
    // When the resource needs to be refreshed, its counter is incremented.
    int getUpdateId() const { return updateId; }

    DlnaResource *getDlnaParent() const { return dlnaParent; }
    QString getDlnaParentId() const;
    void setDlnaParent(DlnaResource *dlna_parent) { dlnaParent = dlna_parent; }

    virtual DlnaResource* getChild(int index, QObject *parent = 0) = 0;
    virtual int getChildrenSize() const = 0;

    // Recursive function that searches for a given ID such as 0$2$13.
    DlnaResource* search(QString searchId, QString searchStr, QObject *parent = 0);

    // Search for an item matching the given objectID if returnChildren is false.
    // if returnChildren is true it returns all children of the objectID.
    QList<DlnaResource*> getDLNAResources(QString objectId, bool returnChildren, int start, int count, QString searchStr, QObject *parent = 0);

    // Returns album art in jpeg format
    virtual QImage getAlbumArt() const = 0;
    QByteArray getByteAlbumArt() const;

signals:
    void dlnaResources(QObject* sender, QList<DlnaResource*>);

private slots:
    void requestDlnaResources(QObject *sender, QString objectId, bool returnChildren, int start, int count, QString searchStr);

private:
    virtual void refreshContent() { qWarning() << "Resource" << getName() << "shall be refreshed, please define function refreshContent() herited from DlnaResource to do it."; }

    void change_parent(QObject *old_parent, QObject *new_parent);

private:
    // id of this resource based on the index in its parent container.
    // Its main purpose is to be unique in the parent container.
    QString id;

    // parent of the DlnaResource
    DlnaResource *dlnaParent;

    // flag to know if content of the ressource shall be refreshed
    bool m_needRefresh;

    // update counter for this resource.
    // When the resource needs to be refreshed, its counter should be updated.
    int updateId;
};

#endif // DLNARESOURCE_H
