#ifndef DLNAYOUTUBEVIDEO_H
#define DLNAYOUTUBEVIDEO_H

#include "dlnavideoitem.h"
#include "ffmpegtranscoding.h"
#include "youtube.h"
#include <QProcess>
#include <QTime>
#include <QRegularExpression>

class DlnaYouTubeVideo : public DlnaVideoItem
{
    Q_OBJECT

public:
    explicit DlnaYouTubeVideo(Logger* log, QString host, int port, QObject *parent = 0);
    virtual ~DlnaYouTubeVideo();

    bool isValid() { return m_unavailableMessage.isEmpty() && !m_title.isEmpty() && !m_resolution.isEmpty() && m_durationInMs>0; }
    QString unavailableMessage() { return m_unavailableMessage; }

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const { return metaDataTitle(); }

    virtual QString getSystemName() const { return m_url.toString();  }

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() const { return metaDataTitle(); }

    //returns the size of the source
    virtual qint64 sourceSize() const { return m_srcSize; }

    virtual int metaDataBitrate()              const { return m_bitrate; }
    virtual int metaDataDuration()             const { return m_durationInMs; }
    virtual QString metaDataTitle()            const { return m_title; }
    virtual QString metaDataGenre()            const { return QString(); }
    virtual QString metaDataPerformer()        const { return QString(); }
    virtual QString metaDataPerformerSort()        const { return QString(); }
    virtual QString metaDataAlbum()            const { return QString(); }
    virtual QString metaDataAlbumArtist()      const { return QString(); }
    virtual int metaDataYear()                 const { return -1; }
    virtual int metaDataTrackPosition()        const { return 0; }
    virtual int metaDataDisc()                 const { return 0; }
    virtual QString metaDataFormat()           const { return m_format; }
    virtual QByteArray metaDataPicture()       const { return QByteArray(); }
    virtual QString metaDataLastModifiedDate() const { return QString(); }

    // returns the samplerate of the video track
    virtual int samplerate() const { return m_samplerate; }

    //returns the channel number of the video track
    virtual int channelCount() const { return m_channelcount; }

    virtual QString resolution() const { return m_resolution; }
    virtual QStringList subtitleLanguages() const { return QStringList(); }
    virtual QStringList audioLanguages() const { return QStringList(); }
    virtual QString framerate() const { return m_framerate; }

    QString streamUrl() const { return m_streamUrl; }

    void setAnalyzeStream(const bool &flag) { m_analyzeStream = flag; }

    void setNetworkAccessManager(QNetworkAccessManager *manager) {
        if (manager)
        {
            m_youtube.setNetworkAccessManager(manager);
            m_youtube.moveToThread(manager->thread());
        }
    }
    void setUrl(const QUrl &url);
    bool waitUrl(const int &timeout=30000);
    void setPlaybackQuality(const QString &quality) { m_youtube.setPlaybackQuality(quality); }

protected:
    // Returns the process for transcoding
    virtual TranscodeProcess* getTranscodeProcess();

signals:
    void getVideoUrl(const QString &videoId);

private slots:
    void videoUrl(const QString &url);
    void videoTitle(const QString &title);
    void videoUrlError(const QString &message);
    void videoNotAvailable(const QString &message) { m_unavailableMessage  = message; }

private:
    QUrl m_url;
    bool m_analyzeStream;
    bool m_videoUrlInProgress;
    QString m_unavailableMessage;
    QString m_title;
    QString m_streamUrl;
    QString m_format;
    int m_srcSize;
    int m_durationInMs;
    QString m_resolution;
    QString m_framerate;
    int m_bitrate;
    int m_samplerate;
    int m_channelcount;

    QString programFfmpeg;

    YouTube m_youtube;
    QMutex mutex;
    QWaitCondition replyWaitCondition;
};

#endif // DLNAYOUTUBEVIDEO_H
