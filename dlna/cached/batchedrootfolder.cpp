#include "batchedrootfolder.h"

BatchedRootFolder::BatchedRootFolder(DlnaCachedRootFolder* root, QObject *parent) :
    QObject(parent),
    m_root(root),
    stop(false)
{
}

int BatchedRootFolder::countDirectory(const QDir &folder) {
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

void BatchedRootFolder::readDirectory(const QDir &folder, const bool &flag_root) {
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
        else if (m_root && fileinfo.isFile())
            m_root->addResource(fileinfo);

        if (stop)
            break;
    }

    if (m_root)
        m_root->refreshLastAddedMedia();
}
