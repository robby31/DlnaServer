#ifndef DLNAYOUTUBEVIDEO_H
#define DLNAYOUTUBEVIDEO_H

#include <QTime>
#include <QRegularExpression>

#include "dlnavideoitem.h"
#include "youtube.h"
#include "ffmpegtranscoding.h"
#include "qffmpegprocess.h"

class DlnaYouTubeVideo : public DlnaVideoItem
{
    Q_OBJECT

public:
    explicit DlnaYouTubeVideo(QString host, int port, QObject *parent = 0);
    virtual ~DlnaYouTubeVideo();

    bool isValid() { return m_unavailableMessage.isEmpty() && !m_title.isEmpty() && !resolution().isEmpty() && metaDataDuration()>0; }
    QString unavailableMessage() { return m_unavailableMessage; }
    QString lastError() { return m_error; }

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const { return metaDataTitle(); }

    virtual QString getSystemName() const { return m_url.toString();  }

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() const { return metaDataTitle(); }

    //returns the size of the source
    virtual qint64 sourceSize() const;

    virtual int metaDataBitrate()              const { return -1; }
    virtual int metaDataDuration()             const;
    virtual QString metaDataTitle()            const { return m_title; }
    virtual QString metaDataGenre()            const { return QString(); }
    virtual QString metaDataPerformer()        const { return QString(); }
    virtual QString metaDataPerformerSort()    const { return QString(); }
    virtual QString metaDataAlbum()            const { return QString(); }
    virtual QString metaDataAlbumArtist()      const { return QString(); }
    virtual int metaDataYear()                 const { return -1; }
    virtual int metaDataTrackPosition()        const { return 0; }
    virtual int metaDataDisc()                 const { return 0; }
    virtual QString metaDataFormat()           const;
    virtual QByteArray metaDataPicture()       const { return QByteArray(); }
    virtual QString metaDataLastModifiedDate() const { return QString(); }

    // returns the samplerate of the video track
    virtual int samplerate() const;

    //returns the channel number of the video track
    virtual int channelCount() const;

    virtual QHash<QString, double> volumeInfo(const int timeout = 30000);

    virtual QString resolution() const;
    virtual QStringList subtitleLanguages() const { return QStringList(); }
    virtual QStringList audioLanguages() const { return QStringList(); }
    virtual QString framerate() const;

    QString streamUrl() const { return m_streamUrl; }

    void setAnalyzeStream(const bool &flag) { m_analyzeStream = flag; }

    void setNetworkAccessManager(QNetworkAccessManager *manager);
    QUrl url() const;
    void setUrl(const QUrl &url);
    bool waitUrl(const int &timeout=30000);
    void setPlaybackQuality(const QString &quality);

protected:
    // Returns the process for transcoding
    virtual TranscodeProcess* getTranscodeProcess();

signals:
    void getVideoUrl(const QString &videoId);
    void videoUrlErrorSignal(const QString &message);
    void streamUrlDefined(const QString &url);

private slots:
    void videoUrl(const QString &url);
    void videoTitle(const QString &title);
    void videoUrlError(const QString &message);
    void videoNotAvailable(const QString &message) { m_unavailableMessage  = message; }
    void ffmpegReady();

private:
    QUrl m_url;
    bool m_analyzeStream;
    bool m_videoUrlInProgress;
    QString m_unavailableMessage;
    QString m_title;
    QString m_streamUrl;

    QFfmpegProcess ffmpeg;

    YouTube *m_youtube;
    QMutex mutex;
    QWaitCondition replyWaitCondition;

    QString m_error;

public:
    static qint64 objectCounter;
};

#endif // DLNAYOUTUBEVIDEO_H
