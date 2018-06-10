#ifndef SERVICECONNECTIONMANAGER_H
#define SERVICECONNECTIONMANAGER_H

#include <QObject>
#include "Http/httprequest.h"
#include "soapaction.h"
#include "soapactionresponse.h"
#include "upnpcontrolpoint.h"

class ServiceConnectionManager : public QObject
{
    Q_OBJECT

public:
    explicit ServiceConnectionManager(UpnpControlPoint *upnp,  QNetworkAccessManager *nam, QObject *parent = 0);

    QStringList getSourceProtocolInfo();
    QStringList getSinkProtocolInfo();
    QString getConnectionIDs();

    void reply(HttpRequest *request);

    void sendEvent(const QString &uuid);

signals:

public slots:

private slots:
    void sendEventReply();

private:
    QNetworkAccessManager *netManager;

    UpnpControlPoint *m_upnp = Q_NULLPTR;
    QHash<QString, QStringList> m_subscription;
};

#endif // SERVICECONNECTIONMANAGER_H
