#ifndef METADATA_H
#define METADATA_H

#include <QObject>
#include <QDebug>
#include <QStringList>

#include "MediaInfoDLL/MediaInfoDLL.h"

class MetaData : public QObject
{
    Q_OBJECT
public:
    explicit MetaData(QString filename, QObject *parent = 0);
    ~MetaData();

    bool isOpened() { return flagOpened; }

    QString getParameter(QString parameter, MediaInfoDLL::stream_t StreamKing = MediaInfoDLL::Stream_General, size_t StreamNumber = 0);

    int getAudioStreamCount();
    QString getAudioFormat(int audioStreamId);
    int getChannelCount(int audioStreamId);
    int getSamplingRate(int audioStreamId);
    QStringList getAudioLanguages();

    int getVideoStreamCount();
    QString getVideoCodec(int videoStreamId);
    QString getResolution(int videoStreamId);

    int getSubtitleStreamCount();
    QStringList getSubtitleLanguages();

    MediaInfoDLL::String getCoverData();

private:
    MediaInfoDLL::MediaInfo MI;
    bool flagOpened;   // True if the MetaData is correctly opened

};

#endif // METADATA_H
