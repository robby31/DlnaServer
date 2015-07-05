#ifndef DLNACACHEDNETWORKVIDEO_H
#define DLNACACHEDNETWORKVIDEO_H

#include "dlnacachedvideo.h"
#include "ffmpegtranscoding.h"
#include "dlnayoutubevideo.h"

class DlnaCachedNetworkVideo : public DlnaCachedVideo
{
    Q_OBJECT

public:
    explicit DlnaCachedNetworkVideo(Logger* log, QNetworkAccessManager *manager, MediaLibrary* library, int idMedia, QString host, int port, QObject *parent = 0);

    //returns the size of the source
    virtual qint64 sourceSize() const { return -1; }

    virtual QString metaDataTitle() const { if (library != 0) return library->getmetaData("title", idMedia).toString(); else return QString(); }

protected:
    // Returns the process for transcoding
    virtual TranscodeProcess* getTranscodeProcess();

private:
    QNetworkAccessManager *m_nam;
    QString m_streamUrl;
};

#endif // DLNACACHEDNETWORKVIDEO_H
