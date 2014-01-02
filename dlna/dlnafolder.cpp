#include "dlnafolder.h"

DlnaFolder::DlnaFolder(Logger* log, QString filename, QString host, int port):
    DlnaResource(log),
    fileinfo(QFileInfo(filename)),
    host(host),
    port(port)
{
}

DlnaFolder::~DlnaFolder() {

}

QString DlnaFolder::getName() const {
    return fileinfo.fileName();
}

QString DlnaFolder::getSystemName() const {
    return fileinfo.absoluteFilePath();
}

QString DlnaFolder::getDisplayName() const {
    return fileinfo.completeBaseName();
}

bool DlnaFolder::discoverChildren() {

    QDir folder(fileinfo.absoluteFilePath());
    QStringList filter;
    filter << "*";
    foreach (QString fd, folder.entryList(filter,
                                          QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Readable,
                                          QDir::DirsFirst | QDir::Name)) {
        QFileInfo new_file(folder, fd);
        DlnaResource* child = 0;

        QMimeDatabase mimeDb;

        if (new_file.isDir()) {
            child = new DlnaFolder(getLog(), new_file.absoluteFilePath(), host, port);
        }
        else if (mimeDb.mimeTypeForFile(new_file).name().startsWith("audio/")) {
            child = new DlnaMusicTrack(getLog(), new_file.absoluteFilePath(), host, port);
        }

        if (child != 0) {
            addChild(child);
        }
    }

    return true;
}

/*
* Returns XML (DIDL) representation of the DLNA node. It gives a
* complete representation of the item, with as many tags as available.
*
* Reference: http://www.upnp.org/specs/av/UPnP-av-ContentDirectory-v1-Service.pdf
*/
QDomElement DlnaFolder::getXmlContentDirectory(QDomDocument *xml, QStringList properties) {
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
    }

    return xml_obj;
}

QImage DlnaFolder::getAlbumArt() {
    return QImage();
}
