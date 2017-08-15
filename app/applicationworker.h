#ifndef APPLICATIONWORKER_H
#define APPLICATIONWORKER_H

#include <QObject>
#include <QDir>
#include "Worker/worker.h"
#include "cached/dlnacachedrootfolder.h"
#include "mysqldatabase.h"

class ApplicationWorker : public Worker
{
    Q_OBJECT

public:
    explicit ApplicationWorker(QNetworkAccessManager *nam, QObject *parent = 0);

signals:
    // signals used by checkNetworkLink
    void addMessage(QString name, QString message);
    void refresh(const QString &url);

private slots:
    void scanFolder(QDir path);
    void checkNetworkLink();
    void scanVolumeInfo();

private:
    QNetworkAccessManager *m_nam;
};

#endif // APPLICATIONWORKER_H
