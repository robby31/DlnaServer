#include "dlnacachedvideo.h"

DlnaCachedVideo::DlnaCachedVideo(Logger* log, QString filename, MediaLibrary* library, int idMedia, QString host, int port, QObject *parent):
    DlnaVideoItem(log, filename, host, port, parent),
    library(library),
    idMedia(idMedia)
{
}
