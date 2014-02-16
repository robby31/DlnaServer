#include "dlnafolder.h"

DlnaFolder::DlnaFolder(Logger* log, QString filename, QString host, int port, QObject *parent):
    DlnaStorageFolder(log, host, port, parent),
    fileinfo(filename)
{
}

QList<QFileInfo> DlnaFolder::getChildrenFileInfo() {
    QList<QFileInfo> l_children;

    QDir folder(fileinfo.absoluteFilePath());
    QStringList filter;
    filter << "*";
    foreach (QString fd, folder.entryList(filter,
                                          QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Readable,
                                          QDir::DirsFirst | QDir::Name)) {
        QFileInfo new_file(folder, fd);

        QMimeDatabase mimeDb;

        if (new_file.isDir()) {
            l_children.append(new_file);
        }
        else if (mimeDb.mimeTypeForFile(new_file).name().startsWith("audio/")) {
            l_children.append(new_file);
        }
        else if (mimeDb.mimeTypeForFile(new_file).name().startsWith("video/")) {
            l_children.append(new_file);
        }
        else {
            log->WARNING(QString("Unkwown format %1: %2").arg(mimeDb.mimeTypeForFile(new_file).name()).arg(new_file.absoluteFilePath()));
        }
    }

    return l_children;
}

bool DlnaFolder::discoverChildren() {
    foreach (QFileInfo new_file, getChildrenFileInfo()) {

        DlnaResource* child = 0;

        QMimeDatabase mimeDb;

        if (new_file.isDir()) {
            // TODO: set the parent of the QObject
            child = new DlnaFolder(log, new_file.absoluteFilePath(), host, port);
        }
        else if (mimeDb.mimeTypeForFile(new_file).name().startsWith("audio/")) {
            // TODO: set the parent of the QObject
            child = new DlnaMusicTrackFile(log, new_file.absoluteFilePath(), host, port);
        }
        else if (mimeDb.mimeTypeForFile(new_file).name().startsWith("video/")) {
            // TODO: set the parent of the QObject
            child = new DlnaVideoFile(log, new_file.absoluteFilePath(), host, port);
        }
        else {
            log->WARNING(QString("Unkwown format %1: %2").arg(mimeDb.mimeTypeForFile(new_file).name()).arg(new_file.absoluteFilePath()));
        }

        if (child != 0) {
            addChild(child);
        }
    }

    return true;
}
