#ifndef DLNAITEM_H
#define DLNAITEM_H

#include "dlnaresource.h"

#include "transcodeprocess.h"
#include "streamingfile.h"


class DlnaItem : public DlnaResource
{
    Q_OBJECT

public:
    explicit DlnaItem(QString host, int port, QObject *parent = 0);
    virtual ~DlnaItem();

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() const;

    virtual bool isFolder() const { return false; }
    virtual DlnaResource* getChild(int index, QObject *parent = 0) { Q_UNUSED(index) Q_UNUSED(parent) return 0; }
    virtual int getChildrenSize() const { return 0; }

    // Returns the time where the media has been stopped during last play
    virtual qint64 getResumeTime() const { return 0; }

    // Returns an InputStream of this DLNA node.
    Device *getStream();

    // return true if the track shall be transcoded
    virtual bool toTranscode() const = 0;

    TranscodeFormatAvailable format() const { return transcodeFormat; }
    void setTranscodeFormat(TranscodeFormatAvailable format);

    // Returns the mimeType for this DLNA node.
    virtual QString mimeType() const = 0;

    // returns the size of the dlna item
    virtual qint64 size();

    //returns the size of the source
    virtual qint64 sourceSize() const = 0;

    // return the length in seconds of the media
    int getLengthInSeconds() const;
    int getLengthInMilliSeconds() const;

    // returns the samplerate of the audio track
    virtual int samplerate() const = 0;

    //returns the channel number of the audio track
    virtual int channelCount() const = 0;

    // returns the bitrate in bits/sec
    virtual int bitrate() const = 0;

    // returns volume information
    virtual QHash<QString, double> volumeInfo(const int timeout = 30000) = 0;

    virtual int metaDataBitrate() const = 0;
    virtual int metaDataDuration() const = 0;
    virtual QString metaDataTitle() const = 0;
    virtual QString metaDataGenre() const = 0;
    virtual QString metaDataPerformer() const = 0;
    virtual QString metaDataPerformerSort() const = 0;
    virtual QString metaDataAlbum() const = 0;
    virtual QString metaDataAlbumArtist() const = 0;
    virtual int metaDataYear() const = 0;
    virtual int metaDataTrackPosition() const = 0;
    virtual int metaDataDisc() const = 0;
    virtual QString metaDataFormat() const = 0;
    virtual QByteArray metaDataPicture() const = 0;
    virtual QString metaDataLastModifiedDate() const = 0;

    QString getProtocolInfo() const;

    QString getDlnaContentFeatures() const;

    QString getdlnaOrgOpFlags() const { return dlnaOrgOpFlags; }
    void setdlnaOrgOpFlags (QString arg) { dlnaOrgOpFlags=arg; }

    QString getdlnaOrgPN() const { return dlnaOrgPN; }
    void setdlnaOrgPN(QString arg) { dlnaOrgPN=arg; }

    void setUserAgent(QString userAgent) { m_userAgent = userAgent; emit userAgentChanged(); }

    void setStream(Device *stream);

protected:
    // Returns the process for transcoding
    virtual TranscodeDevice* getTranscodeProcess() = 0;

    // Returns the process for original streaming
    virtual Device* getOriginalStreaming() = 0;

private:
    virtual void updateDLNAOrgPn() = 0;

signals:
    void userAgentChanged();

private slots:
    void streamDestroyed(QObject *obj);

protected:
    QString host;
    int port;

    TranscodeFormatAvailable transcodeFormat;

    /*
     * DLNA.ORG_OP flags
     *
     * Two booleans (binary digits) which determine what transport operations the renderer is allowed to
     * perform (in the form of HTTP request headers): the first digit allows the renderer to send
     * TimeSeekRange.DLNA.ORG (seek by time) headers; the second allows it to send RANGE (seek by byte)
     * headers.
     *
     *    00 - no seeking (or even pausing) allowed
     *    01 - seek by byte
     *    10 - seek by time
     *    11 - seek by both
     *
     * See here for an example of how these options can be mapped to keys on the renderer's controller:
     * http://www.ps3mediaserver.org/forum/viewtopic.php?f=2&t=2908&p=12550#p12550
     *
     * Note that seek-by-byte is the preferred option for streamed files [1] and seek-by-time is the
     * preferred option for transcoded files.
     *
     * [1] see http://www.ps3mediaserver.org/forum/viewtopic.php?f=6&t=15841&p=76201#p76201
     *
     * seek-by-time requires a) support by the renderer (via the SeekByTime renderer conf option)
     * and b) support by the transcode engine.
     *
     * The seek-by-byte fallback doesn't work well with transcoded files [2], but it's better than
     * disabling seeking (and pausing) altogether.
     *
     * [2] http://www.ps3mediaserver.org/forum/viewtopic.php?f=6&t=3507&p=16567#p16567 (bottom post)
     */
    QString dlnaOrgOpFlags;

    // DLNA.ORG_PN
    QString dlnaOrgPN;

    QString m_userAgent;

    Device *m_stream;
};

#endif // DLNAITEM_H
