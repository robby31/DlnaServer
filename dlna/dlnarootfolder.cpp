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

QString DlnaRootFolder::getDisplayName() const {
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
QDomElement DlnaRootFolder::getXmlContentDirectory(QDomDocument *xml) {
    QDomElement xml_obj;

    xml_obj = xml->createElement("container");

    xml_obj.setAttribute("id", getResourceId());

    if (!isDiscovered() && getChildren().size() == 0) {
        //  When a folder has not been scanned for resources, it will automatically have zero children.
        //  Some renderers like XBMC will assume a folder is empty when encountering childCount="0" and
        //  will not display the folder. By returning childCount="1" these renderers will still display
        //  the folder. When it is opened, its children will be discovered and childrenNumber() will be
        //  set to the right value.
        xml_obj.setAttribute("childCount", "1");
    } else {
        xml_obj.setAttribute("childCount", QString("%1").arg(getChildren().size()));
    }


    xml_obj.setAttribute("parentID", getParentId());
    xml_obj.setAttribute("restricted", "true");

    QDomElement dcTitle = xml->createElement("dc:title");
    dcTitle.appendChild(xml->createTextNode(getDisplayName()));
    xml_obj.appendChild(dcTitle);

    QDomElement upnpClass = xml->createElement("upnp:class");
    upnpClass.appendChild(xml->createTextNode("object.container.storageFolder"));
    xml_obj.appendChild(upnpClass);

    return xml_obj;
}
