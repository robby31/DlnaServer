#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QTcpSocket>

class HttpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit HttpClient(QObject *parent = 0);

signals:

public slots:

};

#endif // HTTPCLIENT_H
