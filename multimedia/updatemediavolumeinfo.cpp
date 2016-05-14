#include "updatemediavolumeinfo.h"

UpdateMediaVolumeInfo::UpdateMediaVolumeInfo(Logger *log):
    QRunnable(),
    m_log(log)
{

}

UpdateMediaVolumeInfo::~UpdateMediaVolumeInfo()
{
    qWarning() << "Volume Information update finished" << this;
}

void UpdateMediaVolumeInfo::run()
{
    QThread backend;
    backend.start();

    QNetworkAccessManager manager;
    manager.moveToThread(&backend);

    MediaLibrary library(m_log);

    QSqlQuery query;
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
                    qWarning() << "Analyze audio" << filename;

                    DlnaMusicTrackFile track(m_log, filename, "HOST", 80);
                    if (!library.setVolumeInfo(idMedia, track.volumeInfo()))
                        qWarning() << "Unable to set volume information for" << filename;
                }
                else if (mime_type.startsWith("video/")  && !filename.startsWith("http"))
                {
                    qWarning() << "Analyze local video" << filename;

                    DlnaVideoFile movie(m_log, filename, "HOST", 80);
                    if (!library.setVolumeInfo(idMedia, movie.volumeInfo(-1)))
                        qWarning() << "Unable to set volume information for" << filename;
                }
                else if (mime_type.startsWith("video/") && filename.startsWith("http"))
                {
                    qWarning() << "Analyze internet video" << filename;

                    DlnaYouTubeVideo video(m_log, "HOST", 80);
                    video.moveToThread(&backend);
                    video.setNetworkAccessManager(&manager);
                    video.setUrl(filename);
                    bool res = video.waitUrl(30000);

                    if (res && video.isValid())
                    {
                        if (!library.setVolumeInfo(idMedia, video.volumeInfo(-1)))
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
        qWarning() << "ERROR in request" << query.lastError().text();
    }

    backend.quit();
}
