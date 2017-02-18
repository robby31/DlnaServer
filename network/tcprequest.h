#ifndef TCPREQUEST_H
#define TCPREQUEST_H

#include <QObject>

class TcpRequest : public QTcpSocket
{
public:
    TcpRequest();
};

#endif // TCPREQUEST_H