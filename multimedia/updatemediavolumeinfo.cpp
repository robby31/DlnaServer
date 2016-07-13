#include "updatemediavolumeinfo.h"

UpdateMediaVolumeInfo::UpdateMediaVolumeInfo(Logger *log, QNetworkAccessManager *nam):
    QRunnable(),
    m_log(log),
    m_nam(nam)
{

}

UpdateMediaVolumeInfo::~UpdateMediaVolumeInfo()
{
    qDebug() << "Volume Information update finished" << this;
}

void UpdateMediaVolumeInfo::run()
{
    MediaLibrary library(m_log);

    QSqlDatabase db = QSqlDatabase::database("MEDIA_DATABASE");
    QSqlQuery query(db);
    query.prepare("SELECT media.id, media.filename, mime_type.name from media LEFT OUTER JOIN mime_type ON media.mime_type=mime_type.id WHERE media.is_reachable=1");

    if (query.exec())
    {
        while (query.next())
        {
            int idMedia = query.record().value("id").toInt();
            QString filename = query.record().value("filename").toString();
            QString mime_type = query.record().value("name").toString();
            QHash<QString, double> volumeInfo = library.volumeInfo(idMedia);

            if (volumeInfo.isEmpty())
            {
                if (mime_type.startsWith("audio/"))
                {
                    qDebug() << "Analyze audio" << filename;

                    DlnaMusicTrackFile track(m_log, filename, "HOST", 80);
                    if (!library.setVolumeInfo(idMedia, track.volumeInfo()))
                        qWarning() << "Unable to set volume information for" << filename;
                }
                else if (mime_type.startsWith("video/")  && !filename.startsWith("http"))
                {
                    qDebug() << "Analyze local video" << filename;

                    DlnaVideoFile movie(m_log, filename, "HOST", 80);
                    if (!library.setVolumeInfo(idMedia, movie.volumeInfo(-1)))
                        qWarning() << "Unable to set volume information for" << filename;
                }
                else if (mime_type.startsWith("video/") && filename.startsWith("http"))
                {
                    qDebug() << "Analyze internet video" << filename;

                    QScopedPointer<DlnaYouTubeVideo, QScopedPointerDeleteLater> video(new DlnaYouTubeVideo(m_log, "HOST", 80));
                    video->moveToThread(m_nam->thread());
                    QObject::connect(m_nam->thread(), SIGNAL(finished()), video.data(), SLOT(deleteLater()));
                    video->setNetworkAccessManager(m_nam);
                    video->setUrl(filename);
                    bool res = video->waitUrl(30000);

                    if (res && video->isValid())
                    {
                        if (!library.setVolumeInfo(idMedia, video->volumeInfo(-1)))
                            qWarning() << "Unable to set volume information for" << filename;
                    }
                    else
                    {
                        qWarning() << "NO VOLUME INFO (TIMEOUT)" << filename;
                    }
                }
                else
                {
                    qWarning() << "NO VOLUME INFO" << filename;
                }
            }
        }
    }
    else
    {
        qCritical() << "ERROR in request" << query.lastError().text();
    }
}
