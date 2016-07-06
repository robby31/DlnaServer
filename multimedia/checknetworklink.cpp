#include "checknetworklink.h"

CheckNetworkLink::CheckNetworkLink(Logger *log):
    QRunnable(),
    m_log(log)
{

}

CheckNetworkLink::~CheckNetworkLink()
{
    qWarning() << "Check network links finished" << this;
}

void CheckNetworkLink::run()
{
    m_log->Info("CHECK NETWORK LINK started");

    QThread backend;
    backend.start();

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    manager->moveToThread(&backend);
    QObject::connect(&backend, SIGNAL(finished()), manager, SLOT(deleteLater()));

    MediaLibrary library(m_log);

    int nb = 0;

    QSqlQuery query = library.getAllNetworkLinks();
    while (query.next())
    {
        qWarning() << "check" << query.record().value("title");
        ++nb;

        QString url(query.value("filename").toString());
        bool isReachable = query.value("is_reachable").toBool();

        QScopedPointer<DlnaYouTubeVideo, QScopedPointerDeleteLater> movie(new DlnaYouTubeVideo(m_log, "HOST", 80));
        movie->moveToThread(manager->thread());
        QObject::connect(manager->thread(), SIGNAL(finished()), movie.data(), SLOT(deleteLater()));
        movie->setNetworkAccessManager(manager);

        movie->setUrl(url);
        bool res = movie->waitUrl(30000);

        if (!(res && movie->isValid()))
        {
            if (!res)
                m_log->Warning("TIMEOUT");

            if (isReachable)
            {
                m_log->Error(QString("link %1 is broken, title: %2").arg(query.value("filename").toString()).arg(query.value("title").toString()));

                if (res && !movie->unavailableMessage().isEmpty())
                {
                    m_log->Warning(QString("PUT OFFLINE %1, %2, %3").arg(query.value("id").toString()).arg(query.value("title").toString()).arg(query.value("artist").toString()));

                    QHash<QString, QVariant> data;
                    data["is_reachable"] = QVariant(0);

                    library.updateFromFilename(url, data);
                }
            }
            else
            {
                m_log->Warning(QString("link %1 is still unreachable, title: %2").arg(query.value("filename").toString()).arg(query.value("title").toString()));
            }
        }
        else
        {
            if (!isReachable)
            {
                m_log->Warning(QString("PUT ONLINE %1, %2, %3").arg(query.value("id").toString()).arg(query.value("title").toString()).arg(query.value("artist").toString()));

                QHash<QString, QVariant> data;
                data["is_reachable"] = QVariant(1);

                library.updateFromFilename(url, data);
            }

            // refresh data
//            addResource(QUrl(url));
        }
    }

    m_log->Info(QString("%1 links checked.").arg(nb));

    backend.quit();
    if (!backend.wait(1000))
        m_log->Error("Unable to stop backend");
}
