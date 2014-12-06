#include "httpclient.h"

HttpClient::HttpClient(QObject *parent) :
    QTcpSocket(parent)
{
}
