#include "dlnacachedmusictrack.h"

DlnaCachedMusicTrack::DlnaCachedMusicTrack(MediaLibrary *library, int idMedia, QString host, int port, QObject *parent):
    DlnaMusicTrack(host, port, parent),
    library(library),
    idMedia(idMedia)
{
    setTranscodeFormat(MP3);   // default transcode format
}

qint64 DlnaCachedMusicTrack::getResumeTime() const
{
    qint64 res = 0;

    if (library)
        res = library->getmetaData("progress_played", idMedia).toLongLong();

    if (res > 10000)
        return res - 10000;   // returns 10 seconds before resume time
    else
        return 0;
}

QHash<QString, double> DlnaCachedMusicTrack::volumeInfo(const int timeout)
{
    Q_UNUSED(timeout)

    if (library != 0)
    {
        return library->volumeInfo(idMedia);
    }
    else
    {
        QHash<QString, double> result;
        return result;
    }
}
