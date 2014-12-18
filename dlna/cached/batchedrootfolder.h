#ifndef BATCHEDROOTFOLDER_H
#define BATCHEDROOTFOLDER_H

#include "dlnacachedrootfolder.h"

class BatchedRootFolder : public QObject
{
    Q_OBJECT

public:
    explicit BatchedRootFolder(DlnaCachedRootFolder* root, QObject *parent = 0);

    void setRootFolder(DlnaCachedRootFolder *root) { m_root = root; }
    bool resetLibrary() { if (m_root) return m_root->resetLibrary(); else return false; }

    void quit() { stop = true; }

private:
    int countDirectory(const QDir &folder);

    void readDirectory(const QDir &folder, const bool &flag_root=true);

signals:
    void progress(const int &value);

private slots:
    void addFolder(const QString &folder) { readDirectory(QDir(folder)); }

private:
    DlnaCachedRootFolder *m_root;
    bool stop;   //boolean to stop the batch process
};

#endif // BATCHEDROOTFOLDER_H
