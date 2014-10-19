#ifndef DLNAVIDEOFILE_H
#define DLNAVIDEOFILE_H

#include "dlnavideoitem.h"
#include "mencodertranscoding.h"
#include "qffmpeg.h"

class DlnaVideoFile : public DlnaVideoItem
{

public:
    explicit DlnaVideoFile(Logger* log, QString filename, QString host, int port, QObject *parent = 0);
    virtual ~DlnaVideoFile();

    QFileInfo getFileInfo() const { return fileinfo; }

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const { return fileinfo.fileName(); }

    virtual QString getSystemName() const { return fileinfo.absoluteFilePath(); }

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() const { return fileinfo.completeBaseName(); }

    //returns the size of the source
    virtual qint64 sourceSize() const { return fileinfo.size(); }

    virtual int metaDataBitrate() const;
    virtual int metaDataDuration() const;
    virtual QString metaDataTitle() const;
    virtual QString metaDataGenre() const;
    virtual QString metaDataPerformer() const;
    virtual QString metaDataAlbum() const;
    virtual int metaDataTrackPosition() const;
    virtual int metaDataDisc() const;
    virtual QString metaDataFormat() const;
    virtual QByteArray metaDataPicture() const;
    virtual QString metaDataLastModifiedDate() const { return fileinfo.lastModified().toString("yyyy-MM-dd"); }

    // returns the samplerate of the video track
    virtual int samplerate() const;

    //returns the channel number of the video track
    virtual int channelCount() const;

    virtual QString resolution() const;
    virtual QStringList subtitleLanguages() const;
    virtual QStringList audioLanguages() const;
    virtual QString framerate() const;

protected:
    // Returns the process for transcoding
    virtual TranscodeProcess* getTranscodeProcess(QObject *parent=0);

private:
    QFileInfo fileinfo;
    QMimeType mime_type;
    QFfmpeg ffmpeg;
};

#endif // DLNAVIDEOFILE_H
