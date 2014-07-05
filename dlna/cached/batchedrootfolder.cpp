#include "batchedrootfolder.h"

BatchedRootFolder::BatchedRootFolder(DlnaCachedRootFolder* root, QObject *parent) :
    QObject(parent),
    m_root(root),
    stop(false)
{
}

void BatchedRootFolder::addFolder(QString folder) {
    readDirectory(QDir(folder));
}

void BatchedRootFolder::quit() {
    stop = true;
}

int BatchedRootFolder::countDirectory(QDir folder) {
    QFileInfoList files = folder.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries);
    int size = 0;
    foreach(const QFileInfo &fileinfo, files) {
        size ++;

        if (fileinfo.isDir())
            size += countDirectory(fileinfo.absoluteFilePath());

        if (stop)
            break;
    }
    return size;
}

void BatchedRootFolder::readDirectory(QDir folder, bool flag_root) {
    static int index;
    static int size;

    QFileInfoList files = folder.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries);

    if (flag_root) {
        index = 0;
        size = countDirectory(folder);
    }

    foreach(const QFileInfo &fileinfo, files) {
        index++;
        emit progress(int(100.0*double(index)/double(size)));

        QString currentPath = fileinfo.absoluteFilePath();
        if(fileinfo.isDir())
            readDirectory(currentPath, false);
        else if (fileinfo.isFile())
            m_root->addResource(fileinfo);

        if (stop)
            break;
    }

    m_root->refreshLastAddedMedia();
}
