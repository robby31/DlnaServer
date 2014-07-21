#ifndef DLNAYOUTUBEVIDEO_H
#define DLNAYOUTUBEVIDEO_H

#include "dlnavideoitem.h"
#include "youtubetranscoding.h"
#include <QProcess>

class DlnaYouTubeVideo : public DlnaVideoItem
{
    Q_OBJECT

public:
    explicit DlnaYouTubeVideo(Logger* log, QUrl url, QString host, int port, QObject *parent = 0);

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const { return metaDataTitle(); }

    virtual QString getSystemName() const { return m_url.toString();  }

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() const { return metaDataTitle(); }

    // return the size
    virtual long size() const;

    virtual int metaDataBitrate()              const { return -1; }
    virtual int metaDataDuration()             const { return -1; }
    virtual QString metaDataTitle()            const { return m_title; }
    virtual QString metaDataGenre()            const { return QString(); }
    virtual QString metaDataPerformer()        const { return QString(); }
    virtual QString metaDataAlbum()            const { return QString(); }
    virtual QString metaDataTrackPosition()    const { return QString(); }
    virtual QString metaDataFormat()           const { return QString(); }
    virtual QByteArray metaDataPicture()       const { return QByteArray(); }
    virtual QString metaDataLastModifiedDate() const { return QString(); }

    // returns the samplerate of the video track
    virtual int samplerate() const { return -1; }

    //returns the channel number of the video track
    virtual int channelCount() const { return -1; }

    virtual QString resolution() const { return QString(); }
    virtual QStringList subtitleLanguages() const { return QStringList(); }
    virtual QStringList audioLanguages() const { return QStringList(); }
    virtual QString framerate() const { return QString(); }

    // Returns the process for transcoding
    virtual YouTubeTranscoding* getTranscodeProcess(HttpRange* range, long timeseek_start=-1, long timeseek_end=-1, QObject *parent=0);

signals:

public slots:

private:
    void requestTitle();
    void requestVideoInfo();

private:
    QUrl m_url;
    QString m_title;

    QString programYouTube;
    QString programFfmpeg;
};

#endif // DLNAYOUTUBEVIDEO_H
