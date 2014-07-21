#include "dlnafolder.h"

DlnaFolder::DlnaFolder(Logger* log, QString filename, QString host, int port, QObject *parent):
    DlnaStorageFolder(log, host, port, parent),
    fileinfo(filename),
    children()
{
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
            log->Warning(QString("Unkwown format %1: %2").arg(mimeDb.mimeTypeForFile(new_file).name()).arg(new_file.absoluteFilePath()));
        }
    }
}

DlnaFolder::~DlnaFolder() {
}

DlnaResource *DlnaFolder::getChild(int index, QObject *parent)  {
    DlnaResource* child = 0;

    if (index >= 0 and index < children.size()) {
        QFileInfo fileinfo = children.at(index);

        QMimeDatabase mimeDb;

        if (fileinfo.isDir()) {
            child = new DlnaFolder(log, fileinfo.absoluteFilePath(), host, port,
                                   parent != 0 ? parent : this);
        }
        else if (mimeDb.mimeTypeForFile(fileinfo).name().startsWith("audio/")) {
            child = new DlnaMusicTrackFile(log, fileinfo.absoluteFilePath(), host, port,
                                           parent != 0 ? parent : this);
        }
        else if (mimeDb.mimeTypeForFile(fileinfo).name().startsWith("video/")) {
            child = new DlnaVideoFile(log, fileinfo.absoluteFilePath(), host, port,
                                      parent != 0 ? parent : this);
        }
        else {
            log->Warning(QString("Unkwown format %1: %2").arg(mimeDb.mimeTypeForFile(fileinfo).name()).arg(fileinfo.absoluteFilePath()));
        }
    }

    if (child != 0) {
        child->setId(QString("%1").arg(index+1));
        child->setDlnaParent(this);
    }

    return child;
}

