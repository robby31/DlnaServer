#include "dlnacachedmusictrack.h"

DlnaCachedMusicTrack::DlnaCachedMusicTrack(Logger* log, MediaLibrary *library, int idMedia, QString host, int port, QObject *parent):
    DlnaMusicTrack(log, host, port, parent),
    library(library),
    idMedia(idMedia)
{
    setTranscodeFormat(MP3);   // default transcode format
}
