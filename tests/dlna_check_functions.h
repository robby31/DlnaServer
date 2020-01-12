#ifndef DLNA_CHECK_FUNCTIONS_H
#define DLNA_CHECK_FUNCTIONS_H

#include <QtTest>
#include <QDebug>
#include <QDomDocument>

#include "dlna/dlnaresource.h"
#include "dlna/dlnamusictrackfile.h"
#include "dlna/dlnanetworkvideo.h"

#include <QElapsedTimer>
#include "Http/httprange.h"

class DlnaCheckFunctions : public QObject
{
    Q_OBJECT

public:
    explicit DlnaCheckFunctions(QObject *parent = Q_NULLPTR);

    void check_dlna_storage(DlnaResource *dlna,
                            const QString &id, const QString &parentId, const int &childrenSize, const QString &title);

    void check_dlna_video(DlnaResource *dlna,
                          const QString &id, const QString &parentId,
                          const QString &title,
                          const int &nb_res);

    void check_dlna_video_res(DlnaResource *dlna,
                              const int &index,
                              const QString &protocolInfo,
                              const QString &duration,
                              const QString &resolution,
                              const int &channels, const int &samplerate,
                              const qint64 &bitrate,
                              const qint64 &size,
                              const QString &link);

    void check_dlna_audio(DlnaResource *track,
                          const QString &id, const QString &parentId,
                          const QString &title, const QString &album, const QString &artist, const QString &contributor, const QString &genre, const int &trackNumber, const QString &date,
                          const int &nb_res);

    void check_dlna_audio_res(DlnaResource *track,
                              const int &index,
                              const QString &protocolInfo,
                              const QString &duration,
                              const int &channels, const int &samplerate,
                              const qint64 &bitrate,
                              const qint64 &size,
                              const QString &link);

    void check_streaming(DlnaItem *media, const qint64 &startTime, const qint64 &endTime,
                         const QString &p_range,
                         const qint64 &p_bytesAvailableBeforeOpen, const qint64 &p_bytesAvailableAfterOpen,
                         const qint64 &p_transcodeSize, const qint64 &p_transcodedSize,
                         const int &maxTimeToTranscode,
                         const bool &variable_bitrate = true);

signals:
    void startTranscoding();
    void requestData(const qint64 &maxlen);

public slots:
    void receivedTranscodedData(const QByteArray &data);
    void dataAvailable();
    void transcodingOpened();
    void LogMessage(const QString &message);

private:
    qint64 transcodedBytes = 0;
    QElapsedTimer transcodeTimer;
    qint64 timeToOpenTranscoding = 0;
};

#endif // DLNA_CHECK_FUNCTIONS_H
