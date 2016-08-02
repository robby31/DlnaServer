#ifndef CHECKNETWORKLINK_H
#define CHECKNETWORKLINK_H

#include "Worker/myrunnable.h"

#include "Models/listmodel.h"
#include "logger.h"
#include "medialibrary.h"
#include "dlnayoutubevideo.h"

class CheckNetworkLink : public MyRunnable
{
    Q_OBJECT

public:
    explicit CheckNetworkLink(Logger *log, QNetworkAccessManager *nam);

    void run() Q_DECL_OVERRIDE;

signals:
    void addMessage(QString name, QString message);
    void progress(const int &value);
    void refresh(const QUrl &url);

private slots:

private:
    Logger *m_log;
    QNetworkAccessManager *m_nam;
};

#endif // CHECKNETWORKLINK_H
