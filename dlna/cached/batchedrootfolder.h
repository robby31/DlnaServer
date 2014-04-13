#ifndef BATCHEDROOTFOLDER_H
#define BATCHEDROOTFOLDER_H

#include <QThread>
#include "dlnacachedrootfolder.h"

class BatchedRootFolder : public QObject
{
    Q_OBJECT

public:
    explicit BatchedRootFolder(DlnaCachedRootFolder* root, QObject *parent = 0);

private:
    int countDirectory(QDir folder);

    void readDirectory(QDir folder, bool flag_root=true);

public slots:
    void addFolder(QString folder);

    void quit();

signals:
    void progress(int value);

private:
    DlnaCachedRootFolder root;
    bool stop;   //boolean to stop the batch process
};

#endif // BATCHEDROOTFOLDER_H
