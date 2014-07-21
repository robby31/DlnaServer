#include "dlnacachedmusictrack.h"

DlnaCachedMusicTrack::DlnaCachedMusicTrack(Logger* log, MediaLibrary *library, int idMedia, QString host, int port, QObject *parent):
    DlnaMusicTrack(log, host, port, parent),
    library(library),
    idMedia(idMedia)
{
}

long DlnaCachedMusicTrack::size() const {
    if (toTranscode()) {
        if (bitrate() != -1) {
            return double(bitrate())*double(getLengthInMilliSeconds())/8000.0;
        } else {
            // variable bitrate, we don't know exactly the size
            return -1;
        }
    } else {
        return -1;
    }
}
