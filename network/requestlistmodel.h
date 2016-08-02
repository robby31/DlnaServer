#ifndef REQUESTLISTMODEL_H
#define REQUESTLISTMODEL_H

#include "Models/listmodel.h"
#include "request.h"
#include "logger.h"

class RequestListModel : public ListModel
{
    Q_OBJECT

public:
    explicit RequestListModel(Logger *log, QObject *parent = 0);
    explicit RequestListModel(QObject *parent = 0);
    virtual ~RequestListModel();


signals:
    void newRequest(Request *request);


private slots:
    void createRequest(HttpClient *client, QString uuid, QString servername, QString host, int port);
    void requestDestroyed(Request *request);


private:
    Logger *m_log;
};

#endif // REQUESTLISTMODEL_H
