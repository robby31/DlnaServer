#ifndef MEDIARENDERER_H
#define MEDIARENDERER_H

#include <QObject>

#include "logger.h"

class MediaRenderer : public QObject
{
    Q_OBJECT

public:
    explicit MediaRenderer(Logger* log, QString ip, int port, QString userAgent, QObject *parent = 0);

    QString getStatus() const { return status; }
    void setStatus(QString status) { this->status = status; emit dataChanged(); }

    QString getName() const { return name; }

    QString getNetworkAddress() { return ip; }

    QString getUserAgent() { return userAgent; }

signals:
    // emit signal when data changed
    void dataChanged();

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
