#ifndef UPNPHELPER_H
#define UPNPHELPER_H

#include <QHostAddress>
#include <QTimer>
#include <QUdpSocket>
#include <QDateTime>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "logobject.h"

class UPNPHelper : public LogObject
{
    Q_OBJECT

public:
    explicit UPNPHelper(Logger* log, QObject *parent = 0);
    virtual ~UPNPHelper();

    void start() { emit startSignal(); }
    void close();

    void setUuid(const QString &uuid)       { m_uuid = uuid;        }
    void setServerName(const QString &name) { m_servername = name;  }
    void setServerUrl(const QString &url)   { m_serverurl = url;    }

private:
    // Build a UPnP message string based on a message.
    QByteArray _buildMsg(const QString &nt, const QString &nts);

    // Send the provided message to the socket.
    void _sendMessage(const QString &nt, const QString &nts);

    // Send UPnP discovery search message to discover devices of interest on the network.
    void _sendDiscover(const QHostAddress &host, const int &port, const QString &st);

    // Send reply.
    void _sendReply(const QHostAddress &host, const int &port, const QByteArray &msg);

    // Send the UPnP BYEBYE message.
    void _sendByeBye();

signals:
    void startSignal();

private slots:
    void _start();

    // Send Alive for broadcasting
    void _sendAlive();

    // Function called when a request is received
    void _processPendingDatagrams();


private:
    QString m_uuid;
    QString m_servername;
    QString m_serverurl;

    QUdpSocket udpSocketBroadcast;
    QUdpSocket udpSocketReceiver;

    // Timer to broadcast UPnP ALIVE messages
    QTimer timerAlive;
    int counterAlive;

    int m_bootid;
    int m_configid;

    // Carriage return and line feed.
    static const QString CRLF;

    // The Constant ALIVE.
    static const QString ALIVE;

    /*
     * IPv4 Multicast channel reserved for SSDP by Internet Assigned Numbers Authority (IANA).
     * MUST be 239.255.255.250.
     */
    static const QHostAddress IPV4_UPNP_HOST;

    /*
     * Multicast channel reserved for SSDP by Internet Assigned Numbers Authority (IANA).
     * MUST be 1900.
     */
    static const int UPNP_PORT;

    // The Constant BYEBYE.
    static const QString BYEBYE;
};

#endif // UPNPHELPER_H
