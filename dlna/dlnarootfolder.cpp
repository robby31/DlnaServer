#include "dlnarootfolder.h"

qint64 DlnaRootFolder::objectCounter = 0;

DlnaRootFolder::DlnaRootFolder(QString host, int port, QObject *parent):
    DlnaStorageFolder(host, port, parent),
    children()
{
    ++objectCounter;

    connect(this, SIGNAL(addFolderSignal(QString)), this, SLOT(addFolderSlot(QString)));
    connect(this, SIGNAL(addChildSignal(DlnaResource*)), this, SLOT(addChildSlot(DlnaResource*)));
    connect(this, SIGNAL(clearChildrenSignal()), this, SLOT(clearChildrenSlot()));

    // For root node, id=0
    setId("0");
}

DlnaRootFolder::~DlnaRootFolder() {
    --objectCounter;

}

void DlnaRootFolder::addChildSlot(DlnaResource *child) {

    if (child == 0) {
        qCritical() << QString("Child is null, unable to append child to node %1").arg(getName());
    } else {
        if (!child->getId().isNull()) {
            if (child->getDlnaParent() != 0) {
                qCritical() << QString("Node %1 already has an ID %2, which is overridden now. The previous parent node was: %3").arg(child->getName()).arg(child->getResourceId()).arg(child->getDlnaParent()->getName());
            } else {
                qCritical() << QString("Node %1 already has an ID %2, which is overridden now.").arg(child->getName()).arg(child->getResourceId());
            }
        }

        child->setId(QString("%1").arg(children.length()+1));
        children.append(child);
        child->setDlnaParent(this);
    }

}

bool DlnaRootFolder::addFolderSlot(QString folder) {

    if (QFileInfo(folder).isDir()) {
        DlnaFolder* child = new DlnaFolder(folder, host, port, this);
        addChild(child);
        return true;
    }

    return false;
}

DlnaResource *DlnaRootFolder::getChild(int index, QObject *parent)
{
    Q_UNUSED(parent)

    if (index>=0 && index<children.size())
        return children.at(index);
    else
        return 0;
}
