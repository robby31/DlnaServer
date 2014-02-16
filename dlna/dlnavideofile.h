#ifndef DLNAVIDEOFILE_H
#define DLNAVIDEOFILE_H

#include "dlnavideoitem.h"

class DlnaVideoFile : public DlnaVideoItem
{
public:
    DlnaVideoFile(Logger* log, QString filename, QString host, int port, QObject *parent = 0);

    virtual int metaDataBitrate();
    virtual int metaDataDuration();
    virtual QString metaDataTitle();
    virtual QString metaDataGenre();
    virtual QString metaDataPerformer();
    virtual QString metaDataAlbum();
    virtual QString metaDataTrackPosition();
    virtual QString metaDataFormat();
    virtual QByteArray metaDataPicture();

    // returns the samplerate of the audio track
    virtual int samplerate();

    //returns the channel number of the audio track
    virtual int channelCount();

    virtual QString resolution();
    virtual QStringList subtitleLanguages();
    virtual QStringList audioLanguages();
    virtual QString framerate();

private:
    MetaData mediaTag;
};

#endif // DLNAVIDEOFILE_H
