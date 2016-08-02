#include "checknetworklink.h"

CheckNetworkLink::CheckNetworkLink(Logger *log, QNetworkAccessManager *nam):
    MyRunnable(),
    m_log(log),
    m_nam(nam)
{

}

void CheckNetworkLink::run()
{
    {
        // initialize database in current Thread
        QSqlDatabase database = CREATE_DATABASE("QSQLITE", "MEDIA_DATABASE");
        database.setDatabaseName("/Users/doudou/workspaceQT/DLNA_server/MEDIA.database");
        database.setConnectOptions("Pooling=True;Max Pool Size=100;");

        if (database.isValid())
        {
            emit progress(0);

            MediaLibrary library(m_log);

            int nb = 0;

            QSqlQuery query = library.getAllNetworkLinks();

            query.last();
            int total = query.at();

            query.exec();
            while (query.next())
            {
                ++nb;
                emit progress(nb*100/total);
                if (isAborted())
                    break;  // Abort process

                QString name = QString("%1 %2").arg(query.value("artist").toString()).arg(query.value("title").toString());

                QString url(query.value("filename").toString());
                bool isReachable = query.value("is_reachable").toBool();

                QScopedPointer<DlnaYouTubeVideo, QScopedPointerDeleteLater> movie(new DlnaYouTubeVideo(m_log, "HOST", 80));
                movie->moveToThread(m_nam->thread());
                QObject::connect(m_nam->thread(), SIGNAL(finished()), movie.data(), SLOT(deleteLater()));
                movie->setNetworkAccessManager(m_nam);

                movie->setUrl(url);
                bool res = movie->waitUrl(30000);

                if (!(res && movie->isValid()))
                {
                    if (isReachable)
                    {
                        if (res && !movie->unavailableMessage().isEmpty())
                        {
                            emit addMessage(name, QString("put OFFLINE - %1").arg(movie->unavailableMessage()));

                            QHash<QString, QVariant> data;
                            data["is_reachable"] = QVariant(0);

                            library.updateFromFilename(url, data);
                        }
                    }
                    else
                    {
                        emit addMessage(name, QString("link is still unreachable - %1").arg(movie->unavailableMessage()));
                    }
                }
                else
                {
                    if (!isReachable)
                    {
                        emit addMessage(name, "put ONLINE");

                        QHash<QString, QVariant> data;
                        data["is_reachable"] = QVariant(1);

                        library.updateFromFilename(url, data);
                    }

                    // refresh data
                    emit refresh(QUrl(url));
                }
            }
        }
        else
        {
            qCritical() << "unable to initialize database connection.";
        }
    }

    REMOVE_DATABASE("MEDIA_DATABASE");

    emit progress(-1);
}
