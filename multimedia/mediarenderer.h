#ifndef MEDIARENDERER_H
#define MEDIARENDERER_H

#include <QObject>

#include "logger.h"

class MediaRenderer : public QObject
{
    Q_OBJECT

public:
    explicit MediaRenderer(Logger* log, const QString &ip, const int &port, const QString &userAgent, QObject *parent = 0);

    QString getStatus() const { return status; }
    void setStatus(const QString &status) { this->status = status; emit dataChanged("status"); }

    QString getName() const { return name; }

    QString getNetworkAddress() const { return ip; }

    QString getUserAgent() const { return userAgent; }

signals:
    // emit signal when data changed
    void dataChanged(const QString &roleChanged);

public slots:

private:
    Logger* log;
    QString ip;
    int port;
    QString userAgent;

    QString status;
    QString name;
};

#endif // MEDIARENDERER_H
