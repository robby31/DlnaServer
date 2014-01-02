#ifndef DLNAMUSICTRACK_H
#define DLNAMUSICTRACK_H

#include <QFileInfo>
#include <QFile>

#include "logger.h"
#include "dlnaresource.h"

#include "MediaInfoDLL/MediaInfoDLL.h"

// Format available for transcoding
enum TranscodeFormatAvailable {MP3, LPCM};

class DlnaMusicTrack : public DlnaResource
{
    Q_OBJECT

public:
    DlnaMusicTrack(Logger* log, QString filename, QString host, int port);
    ~DlnaMusicTrack();

    virtual bool discoverChildren() { return true; }

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const;

    virtual QString getSystemName() const;

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() const;

    virtual bool isFolder() const { return false; }

    // Returns the XML (DIDL) representation of the DLNA node.
    virtual QDomElement getXmlContentDirectory(QDomDocument *xml, QStringList properties);

    // Returns an InputStream of this DLNA node.
    virtual QByteArray getStream(HttpRange* range);

    // Returns the process for transcoding
    virtual QProcess* getTranscodeProcess(HttpRange* range);

    // Returns the mimeType for this DLNA node.
    virtual QString mimeType() const;

    QFileInfo getFileInfo() const { return fileinfo; }

    void setTranscodeFormat(TranscodeFormatAvailable format);

    // return true if the track shall be transcoded
    virtual bool toTranscode() const { return mime_type.name() != "audio/mpeg"; }

    // return the size of the audio track
    virtual int size();

    // return the length in seconds of the media
    virtual int getLengthInSeconds();
    virtual int getLengthInMilliSeconds();

    // returns the bitrate of the audio track
    int bitrate();

    // returns the samplerate of the audio track
    int samplerate();

    //returns the channel number
    int channelCount();

    virtual QImage getAlbumArt();


private:
    QFileInfo fileinfo;

    MediaInfoDLL::MediaInfo MI;

    QMimeType mime_type;
    QString host;
    int port;

    TranscodeFormatAvailable transcodeFormat;

    void updateDLNAOrgPn();
};

#endif // DLNAMUSICTRACK_H
