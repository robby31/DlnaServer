#ifndef DLNAMUSICTRACKFILE_H
#define DLNAMUSICTRACKFILE_H

#include "dlnamusictrack.h"

class DlnaMusicTrackFile : public DlnaMusicTrack
{
public:
    DlnaMusicTrackFile(Logger* log, QString filename, QString host, int port, QObject *parent = 0);

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

private:
    MetaData mediaTag;
};

#endif // DLNAMUSICTRACKFILE_H
