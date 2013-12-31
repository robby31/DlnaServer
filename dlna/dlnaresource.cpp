#include "dlnaresource.h"

DlnaResource::DlnaResource(Logger *log):
    log(log),
    parent(0),
    discovered(false),
    updateId(1),
    dlnaOrgOpFlags("01") // seek by byte (exclusive)
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
        foreach (DlnaResource* child, children) {
            DlnaResource* found = child->search(searchId, searchStr);
            if (found != 0) {
                return found;
            }
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
                        DlnaResource* child = dlna->getChildren().at(i);
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

QString DlnaResource::getStringContentDirectory() {
    QDomDocument xml;
    xml.appendChild(getXmlContentDirectory(&xml));

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

QString DlnaResource::getDlnaContentFeatures() const {
    QStringList result;

    if (!getdlnaOrgPN().isNull()) {
        result << QString("DLNA.ORG_PN=%1").arg(getdlnaOrgPN());
    }

    result << QString("DLNA.ORG_OP=%1").arg(getdlnaOrgOpFlags());
    result << "DLNA.ORG_CI=0";
    result << "DLNA.ORG_FLAGS=01700000000000000000000000000000";

    return result.join(";");
}

QString DlnaResource::getProtocolInfo() const {
    QStringList result;

    if (!getdlnaOrgPN().isNull()) {
        result << QString("DLNA.ORG_PN=%1").arg(getdlnaOrgPN());
    }

    result << QString("DLNA.ORG_OP=%1").arg(getdlnaOrgOpFlags());

    return QString("http-get:*:%1:").arg(mimeType()) + result.join(";");
}

QByteArray DlnaResource::getByteAlbumArt() const {
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
