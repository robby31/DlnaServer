#include "applicationworker.h"

ApplicationWorker::ApplicationWorker(QNetworkAccessManager *nam, QObject *parent):
    Worker(parent),
    m_nam(nam)
{

}
