#include "dlnaresource.h"

DlnaResource::DlnaResource(Logger *log, QObject *parent):
    LogObject(log, parent),
    id(),
    dlnaParent(0),
    m_needRefresh(false),
    updateId(1)
{
    qRegisterMetaType<QList<DlnaResource*> >("QList<DlnaResource*>");
}

DlnaResource::~DlnaResource() {

}

QString DlnaResource::getResourceId() const {
    if (getId().isNull())
        return QString();

    if (getDlnaParent() != 0)
        return getDlnaParent()->getResourceId() + '$' + getId();
    else
        return getId();
}

DlnaResource* DlnaResource::search(QString searchId, QString searchStr, QObject *parent) {
    if (getResourceId() == searchId) {
        return this;
    }
    else if (getResourceId().length() < searchId.length() and searchId.startsWith(getResourceId())) {

        int child_index = searchId.split("$").at(getResourceId().split("$").length()).toInt()-1;

        if ((child_index >= 0) && (child_index < getChildrenSize())) {
            DlnaResource *child = getChild(child_index, parent);
            if (child != 0)
                return child->search(searchId, searchStr, parent);
        }
    }

    return 0;
}

QList<DlnaResource*> DlnaResource::getDLNAResources(QString objectId, bool returnChildren, int start, int count, QString searchStr, QObject *parent) {
    QList<DlnaResource*> resources;
    DlnaResource* dlna = search(objectId, searchStr, parent);
    if (dlna != 0) {
        if (!returnChildren) {
            resources.append(dlna);
        } else {
            int nbChildren = dlna->getChildrenSize();
            if (count <= 0)
                count = nbChildren - start;  // return all children
            for (int i = start; i < start + count; i++) {
                if (i < nbChildren) {
                    DlnaResource* child = dlna->getChild(i, parent);
                    if (child != 0) {
                        if (child->m_needRefresh) {
                            child->refreshContent();
                            ++child->updateId;
                            child->m_needRefresh = false;
                        }
                        resources.append(child);
                    }
                }
            }
        }
    }

    return resources;
}

QString DlnaResource::getDlnaParentId() const {
    if (getDlnaParent() != 0)
        return getDlnaParent()->getResourceId();
    else
        return "-1";
}

QString DlnaResource::getStringContentDirectory(QStringList properties) const {
    QDomDocument xml;
    xml.appendChild(getXmlContentDirectory(&xml, properties));

    QDomDocument res;
    QDomElement result = res.createElement("Result");
    result.appendChild(res.createTextNode(xml.toString().replace("\n", "")));
    res.appendChild(result);

    QString strRes = res.toString();

    // remove <Result> from beginning
    strRes.chop(10);

    // remove </Result> from ending
    strRes.remove(0, 8);

    return strRes;
}

QByteArray DlnaResource::getByteAlbumArt() const {
    QImage picture = getAlbumArt();

    if (!picture.isNull()) {
        QByteArray result;
        QBuffer buffer(&result);
        if (buffer.open(QIODevice::WriteOnly)) {
            if (picture.save(&buffer, "JPEG")) {
                buffer.close();
                return result;
            }
        }
    }
    return QByteArray();
}

void DlnaResource::updateXmlContentDirectory(QDomDocument *xml, QDomElement *xml_obj, QStringList properties) const {
    Q_UNUSED(properties);

    if (xml and xml_obj) {
        xml_obj->setAttribute("id", getResourceId());

        xml_obj->setAttribute("parentID", getDlnaParentId());

        QDomElement dcTitle = xml->createElement("dc:title");
        dcTitle.appendChild(xml->createTextNode(getDisplayName()));
        xml_obj->appendChild(dcTitle);

        QDomElement upnpClass = xml->createElement("upnp:class");
        upnpClass.appendChild(xml->createTextNode(getUpnpClass()));
        xml_obj->appendChild(upnpClass);

        xml_obj->setAttribute("restricted", "true");
    }
}

void DlnaResource::change_parent(QObject *old_parent, QObject *new_parent)
{
    if (new_parent && parent() == old_parent)
    {
        // set parent to null
        setParent(0);

        // change the thread
        moveToThread(new_parent->thread());

        if (getDlnaParent())
            getDlnaParent()->change_parent(old_parent, new_parent);
    }
}

void DlnaResource::requestDlnaResources(QObject *sender, QString objectId, bool returnChildren, int start, int count, QString searchStr)
{
    QObject context;
    QList<DlnaResource*> res = getDLNAResources(objectId, returnChildren, start, count, searchStr, &context);

    for (int index=0;index<res.count();++index)
    {
        DlnaResource *item = res.at(index);
        if (item)
            item->change_parent(&context, sender);
    }

    emit dlnaResources(sender, res);
}
