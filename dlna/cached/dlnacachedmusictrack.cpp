#include "dlnacachedmusictrack.h"

DlnaCachedMusicTrack::DlnaCachedMusicTrack(Logger* log, QString filename, MediaLibrary *library, int idMedia, QString host, int port, QObject *parent):
    DlnaMusicTrack(log, filename, host, port, parent),
    library(library),
    idMedia(idMedia)
{
}
