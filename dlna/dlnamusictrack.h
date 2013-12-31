#ifndef DLNAMUSICTRACK_H
#define DLNAMUSICTRACK_H

#include <QFileInfo>
#include <QFile>

#include "logger.h"
#include "dlnaresource.h"

// TagLib includes
#include "fileref.h"

#include "mp4file.h"
#include "mp4tag.h"
#include "mp4coverart.h"

#include "mpegfile.h"
#include "id3v2tag.h"
#include "attachedpictureframe.h"


// Format available for transcoding
enum TranscodeFormatAvailable {MP3, LPCM};

class DlnaMusicTrack : public DlnaResource
{
    Q_OBJECT

public:
    DlnaMusicTrack(Logger* log, QString filename, QString host, int port);

    virtual bool discoverChildren() { return true; }

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const;

    virtual QString getSystemName() const;

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() const;

    virtual bool isFolder() const { return false; }

    // Returns the XML (DIDL) representation of the DLNA node.
    virtual QDomElement getXmlContentDirectory(QDomDocument *xml);

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

    // return the bitrate of the audio track
    int bitrate();

    virtual QImage getAlbumArt() const;


private:
    QFileInfo fileinfo;
    TagLib::FileRef taglibFile;
    QMimeType mime_type;
    QString host;
    int port;

    TranscodeFormatAvailable transcodeFormat;

    void updateDLNAOrgPn();
};

#endif // DLNAMUSICTRACK_H
