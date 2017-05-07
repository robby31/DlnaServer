#ifndef DLNACACHEDNETWORKVIDEO_H
#define DLNACACHEDNETWORKVIDEO_H

#include "dlnacachedvideo.h"
#include "ffmpegtranscoding.h"
#include "dlnayoutubevideo.h"

class DlnaCachedNetworkVideo : public DlnaCachedVideo
{
    Q_OBJECT

public:
    explicit DlnaCachedNetworkVideo(QNetworkAccessManager *manager, MediaLibrary* library, int idMedia, QString host, int port, QObject *parent = 0);

    //returns the size of the source
    virtual qint64 sourceSize() const { return (double)metaDataDuration()*(double)metaDataBitrate()/8000.0; }

    virtual QString metaDataTitle() const { if (library != 0) return library->getmetaData("title", idMedia).toString(); else return QString(); }

    // return true if the track shall be transcoded
    virtual bool toTranscode() const { return true; }

protected:
    // Returns the process for transcoding
    virtual TranscodeProcess* getTranscodeProcess();

    // Returns the process for original streaming
    virtual Device* getOriginalStreaming();

private:
    QNetworkAccessManager *m_nam;
    QString m_streamUrl;
};

#endif // DLNACACHEDNETWORKVIDEO_H
