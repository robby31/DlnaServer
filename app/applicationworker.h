#ifndef APPLICATIONWORKER_H
#define APPLICATIONWORKER_H

#include <QObject>
#include <QDir>
#include "Worker/worker.h"
#include "dlna/cached/dlnacachedrootfolder.h"
#include "mysqldatabase.h"

class ApplicationWorker : public Worker
{
    Q_OBJECT

public:
    explicit ApplicationWorker(QNetworkAccessManager *nam, QObject *parent = Q_NULLPTR);

signals:
    // signals used by checkNetworkLink
    void addMessage(QString name, QString message);
    void refresh(const QString &url);

private slots:
    void scanFolder(QString path);
    void checkNetworkLink();
    void scanVolumeInfo();

private:
    QNetworkAccessManager *m_nam;
};

#endif // APPLICATIONWORKER_H
