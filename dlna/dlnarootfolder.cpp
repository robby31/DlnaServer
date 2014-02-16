#include "dlnarootfolder.h"

DlnaRootFolder::DlnaRootFolder(Logger* log, QString host, int port, QObject *parent):
    DlnaStorageFolder(log, host, port, parent)
{
    // For root node, id=0
    setId("0");

    // no need to parse children, child is added manually with function addFolder
    setDiscovered(true);
}

bool DlnaRootFolder::addFolder(QString folder) {

    if (QFileInfo(folder).isDir()) {
        DlnaFolder* child = new DlnaFolder(log, folder, host, port, this);
        this->addChild(child);
        return true;
    }

    return false;
}
