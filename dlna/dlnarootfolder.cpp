#include "dlnarootfolder.h"

DlnaRootFolder::DlnaRootFolder(Logger* log, QString host, int port, QObject *parent):
    DlnaStorageFolder(log, host, port, parent),
    children()
{
    // For root node, id=0
    setId("0");
}

void DlnaRootFolder::addChild(DlnaResource *child) {

    if (child == 0) {
        log->Error(QString("Child is null, unable to append child to node %1").arg(getName()));
    } else {
        if (!child->getId().isNull()) {
            if (child->getDlnaParent() != 0) {
                log->Error(QString("Node %1 already has an ID %2, which is overridden now. The previous parent node was: %3").arg(child->getName()).arg(child->getResourceId()).arg(child->getDlnaParent()->getName()));
            } else {
                log->Error(QString("Node %1 already has an ID %2, which is overridden now.").arg(child->getName()).arg(child->getResourceId()));
            }
        }

        child->setId(QString("%1").arg(children.length()+1));
        children.append(child);
        child->setDlnaParent(this);
    }

}

bool DlnaRootFolder::addFolder(QString folder) {

    if (QFileInfo(folder).isDir()) {
        DlnaFolder* child = new DlnaFolder(log, folder, host, port, this);
        this->addChild(child);
        return true;
    }

    return false;
}
