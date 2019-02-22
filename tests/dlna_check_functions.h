#ifndef DLNA_CHECK_FUNCTIONS_H
#define DLNA_CHECK_FUNCTIONS_H

#include <QtTest>
#include <QDebug>
#include <QDomDocument>

void check_dlna_storage(const QDomDocument &dlna,
                        const QString &id, const QString &parentId, const int &childrenSize, const QString &title);

void check_dlna_video(const QDomDocument &dlna,
                      const QString &id, const QString &parentId,
                      const QString &title,
                      const QString &protocolInfo, const QString &duration, const QString &resolution, const int &channels, const int &samplerate, const qint64 &bitrate, const qint64 &size);

void check_dlna_video_res(const QDomNode &res,
                          const QString &protocolInfo,
                          const QString &duration,
                          const QString &resolution,
                          const int &channels, const int &samplerate,
                          const qint64 &bitrate,
                          const qint64 &size);

void check_dlna_audio(const QDomDocument &dlna,
                      const QString &id, const QString &parentId,
                      const QString &title, const QString &album, const QString &artist, const QString &contributor, const QString &genre, const int &track, const QString &date,
                      const QString &protocolInfo, const QString &duration, const int &channels, const int &samplerate, const qint64 &bitrate, const qint64 &size);

void check_dlna_audio_res(const QDomNode &res,
                          const QString &protocolInfo,
                          const QString &duration,
                          const int &channels, const int &samplerate,
                          const qint64 &bitrate,
                          const qint64 &size);

#endif // DLNA_CHECK_FUNCTIONS_H
