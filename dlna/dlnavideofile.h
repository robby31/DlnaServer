#ifndef DLNAVIDEOFILE_H
#define DLNAVIDEOFILE_H

#include "dlnavideoitem.h"
#include "qffmpeg.h"

class DlnaVideoFile : public DlnaVideoItem
{

public:
    explicit DlnaVideoFile(Logger* log, QString filename, QString host, int port, QObject *parent = 0);
    virtual ~DlnaVideoFile();

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
    QFfmpeg ffmpeg;
};

#endif // DLNAVIDEOFILE_H
