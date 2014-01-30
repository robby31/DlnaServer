#ifndef METADATA_H
#define METADATA_H

#include <QObject>
#include <QDebug>
#include <QStringList>
#include <QTextCodec>

#include "MediaInfo/MediaInfo.h"
#define MediaInfoNameSpace MediaInfoLib;
using namespace MediaInfoNameSpace

class MetaData : public QObject
{
    Q_OBJECT

public:
    explicit MetaData(QString filename, QObject *parent = 0);
    ~MetaData();

    bool isOpened() { return flagOpened; }

    QString getParameter(QString parameter, stream_t StreamKing = Stream_General, size_t StreamNumber = 0);

    int getAudioStreamCount();
    QString getAudioFormat(int audioStreamId);
    int getChannelCount(int audioStreamId);
    int getSamplingRate(int audioStreamId);
    QStringList getAudioLanguages();

    int getVideoStreamCount();
    QString getVideoCodec(int videoStreamId);
    QString getResolution(int videoStreamId);
    QString getVideoFrameRate();

    int getSubtitleStreamCount();
    QStringList getSubtitleLanguages();

    String getCoverData();

private:
    MediaInfo MI;
    bool flagOpened;   // True if the MetaData is correctly opened
};

#endif // METADATA_H
