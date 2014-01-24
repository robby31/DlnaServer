#include "dlnaresource.h"

DlnaResource::DlnaResource(Logger *log, QObject *parent):
    QObject(parent),
    log(log),
    parent(0),
    discovered(false),
    updateId(1)
{
}

DlnaResource::~DlnaResource() {
}

QString DlnaResource::getResourceId() const {
    if (getId().isNull())
    {
        return QString();
    }

    if (getParent() != 0)
    {
        return getParent()->getResourceId() + '$' + getId();
    }
    else
    {
        return getId();
    }
}

void DlnaResource::addChild(DlnaResource *child) {

    if (child == 0) {
        log->ERROR(QString("Child is null, unable to append child to node %1").arg(getName()));
    } else {
        if (!child->getId().isNull()) {
            log->ERROR(QString("Node %1 already has an ID %2, which is overridden now. The previous parent node was: %3").arg(child->getName()).arg(child->getResourceId()).arg(child->getParent()->getName()));
        }

        child->setId(QString("%1").arg(children.length()+1));
        children.append(child);
        child->setParent(this);
    }

}

QList<DlnaResource*> DlnaResource::getChildren() {
    if (!isDiscovered()) {
        setDiscovered(discoverChildren());
    }

    return children;
}

DlnaResource* DlnaResource::search(QString searchId, QString searchStr) {
    if (getResourceId() == searchId) {
        return this;
    }
    else if (getResourceId().length() < searchId.length() and searchId.startsWith(getResourceId())) {

        int child_index = searchId.split("$").at(getResourceId().split("$").length()).toInt()-1;

        if ((child_index >= 0) && (child_index < getChildren().size())) {
            return children.at(child_index)->search(searchId, searchStr);
        }
    }

    return 0;
}

QList<DlnaResource*> DlnaResource::getDLNAResources(QString objectId, bool returnChildren, int start, int count, QString searchStr) {
    QList<DlnaResource*> resources;
    DlnaResource* dlna = search(objectId, searchStr);

    if (dlna != 0) {
        if (!returnChildren) {
            resources.append(dlna);
        } else {
            if (count > 0) {
                for (int i = start; i < start + count; i++) {
                    if (i < dlna->getChildren().size()) {
                        DlnaResource* child = dlna->children.at(i);
                        resources.append(child);
                    }
                }
            }
        }
    }

    return resources;
}

bool DlnaResource::isDiscovered() const {
    return discovered;
}

QString DlnaResource::getParentId() const {
    if (getParent() != 0) {
        return getParent()->getResourceId();
    } else {
        return "-1";
    }

}

QString DlnaResource::getStringContentDirectory(QStringList properties) {
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

QByteArray DlnaResource::getByteAlbumArt() {
    QImage picture = getAlbumArt();
    if (!picture.isNull()) {
        QByteArray result;
        QBuffer buffer(&result);
        if (buffer.open(QIODevice::WriteOnly)) {
            if (picture.save(&buffer, "JPEG")) {
                return result;
            }
        }
    }
    return QByteArray();
}
