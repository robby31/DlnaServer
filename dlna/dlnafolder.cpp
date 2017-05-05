#include "dlnafolder.h"

qint64 DlnaFolder::objectCounter = 0;

DlnaFolder::DlnaFolder(QString filename, QString host, int port, QObject *parent):
    DlnaStorageFolder(host, port, parent),
    fileinfo(filename),
    children()
{
    ++objectCounter;

    QDir folder(fileinfo.absoluteFilePath());
    QStringList filter;
    filter << "*";
    foreach (QString fd, folder.entryList(filter,
                                          QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Readable,
                                          QDir::DirsFirst | QDir::Name)) {
        QFileInfo new_file(folder, fd);

        QMimeDatabase mimeDb;

        if (new_file.isDir()) {
            children.append(new_file);
        }
        else if (mimeDb.mimeTypeForFile(new_file).name().startsWith("audio/")) {
            children.append(new_file);
        }
        else if (mimeDb.mimeTypeForFile(new_file).name().startsWith("video/")) {
            children.append(new_file);
        }
        else {
            qWarning() << QString("Unkwown format %1: %2").arg(mimeDb.mimeTypeForFile(new_file).name()).arg(new_file.absoluteFilePath());
        }
    }
}

DlnaFolder::~DlnaFolder() {
    --objectCounter;
}

DlnaResource *DlnaFolder::getChild(int index, QObject *parent)  {
    DlnaResource* child = 0;

    if (index >= 0 and index < children.size()) {
        QFileInfo fileinfo = children.at(index);

        QMimeDatabase mimeDb;

        if (fileinfo.isDir()) {
            child = new DlnaFolder(fileinfo.absoluteFilePath(), host, port,
                                   parent != 0 ? parent : this);
        }
        else if (mimeDb.mimeTypeForFile(fileinfo).name().startsWith("audio/")) {
            child = new DlnaMusicTrackFile(fileinfo.absoluteFilePath(), host, port,
                                           parent != 0 ? parent : this);
        }
        else if (mimeDb.mimeTypeForFile(fileinfo).name().startsWith("video/")) {
            child = new DlnaVideoFile(fileinfo.absoluteFilePath(), host, port,
                                      parent != 0 ? parent : this);
        }
        else {
            qWarning() << QString("Unkwown format %1: %2").arg(mimeDb.mimeTypeForFile(fileinfo).name()).arg(fileinfo.absoluteFilePath());
        }
    }

    if (child != 0) {
        child->setId(QString("%1").arg(index+1));
        child->setDlnaParent(this);
    }

    return child;
}

