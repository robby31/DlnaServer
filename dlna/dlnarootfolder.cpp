#include "dlnarootfolder.h"

DlnaRootFolder::DlnaRootFolder(Logger* log, QString host, int port):
    DlnaResource(log),
    host(host),
    port(port)
{
    // For root node, id=0
    setId("0");

    // no need to parse children, child is added manually with function addFolder
    setDiscovered(true);
}

DlnaRootFolder::~DlnaRootFolder() {
    /*foreach(DlnaResource* child, getChildren()) {
        delete child;
    }*/
}

QString DlnaRootFolder::getName() const {
    return QString("root");
}

QString DlnaRootFolder::getSystemName() const {
    return getName();
}

QString DlnaRootFolder::getDisplayName() {
    return getName();
}

bool DlnaRootFolder::addFolder(QString folder) {

    if (QFileInfo(folder).isDir()) {
        DlnaFolder* child = new DlnaFolder(getLog(), folder, host, port);
        this->addChild(child);
        return true;
    }

    return false;
}

/*
* Returns XML (DIDL) representation of the DLNA node. It gives a
* complete representation of the item, with as many tags as available.
*
* Reference: http://www.upnp.org/specs/av/UPnP-av-ContentDirectory-v1-Service.pdf
*/
QDomElement DlnaRootFolder::getXmlContentDirectory(QDomDocument *xml, QStringList properties) {
    QDomElement xml_obj;

    xml_obj = xml->createElement("container");

    // mandatory properties are: id, parentID, title, class, restricted

    xml_obj.setAttribute("id", getResourceId());

    xml_obj.setAttribute("parentID", getParentId());

    QDomElement dcTitle = xml->createElement("dc:title");
    dcTitle.appendChild(xml->createTextNode(getDisplayName()));
    xml_obj.appendChild(dcTitle);

    QDomElement upnpClass = xml->createElement("upnp:class");
    upnpClass.appendChild(xml->createTextNode("object.container.storageFolder"));
    xml_obj.appendChild(upnpClass);

    xml_obj.setAttribute("restricted", "true");

    if (properties.contains("@childCount")) {
        xml_obj.setAttribute("childCount", QString("%1").arg(getChildren().size()));
    }

    return xml_obj;
}

QImage DlnaRootFolder::getAlbumArt() {
    return QImage();
}
