#ifndef CHECKNETWORKLINK_H
#define CHECKNETWORKLINK_H

#include <QRunnable>

#include "Models/listmodel.h"
#include "logger.h"
#include "medialibrary.h"
#include "dlnayoutubevideo.h"

class CheckNetworkLink : public QObject, public QRunnable
{
    Q_OBJECT

public:
    CheckNetworkLink(Logger *log, QNetworkAccessManager *nam);

    void run();

signals:
    void addMessage(QString name, QString message);
    void progress(const int &value);
    void refresh(const QUrl &url);

private slots:
    void abort();

private:
    Logger *m_log;
    QNetworkAccessManager *m_nam;
    bool m_abort;
};

#endif // CHECKNETWORKLINK_H
