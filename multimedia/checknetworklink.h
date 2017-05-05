#ifndef CHECKNETWORKLINK_H
#define CHECKNETWORKLINK_H

#include "Worker/myrunnable.h"

#include "Models/listmodel.h"
#include "medialibrary.h"
#include "dlnayoutubevideo.h"

class CheckNetworkLink : public MyRunnable
{
    Q_OBJECT

public:
    explicit CheckNetworkLink(QNetworkAccessManager *nam);

    void run() Q_DECL_OVERRIDE;

signals:
    void addMessage(QString name, QString message);
    void progress(const int &value);
    void refresh(const QUrl &url);

private slots:

private:
    QNetworkAccessManager *m_nam;
};

#endif // CHECKNETWORKLINK_H
