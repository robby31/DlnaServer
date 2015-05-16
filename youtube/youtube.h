#ifndef YouTube_H
#define YouTube_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QTime>
#include <QTimer>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include "decryptyoutubesignature.h"

class QNetworkAccessManager;
class QNetworkReply;

class YouTube : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString currentUser
               READ getCurrentUser
               NOTIFY currentUserChanged)
    Q_PROPERTY(QString accessToken
               READ getAccessToken
               NOTIFY accessTokenChanged)
public:
    explicit YouTube(QObject *parent = 0);
    virtual ~YouTube() { }

    void setNetworkAccessManager(QNetworkAccessManager *manager) { nam = manager; }
    QString getCurrentUser() const { return currentUser; }
    QString getAccessToken() const { return accessToken; }

public slots:
    void setUserCredentials(const QString &user, const QString &token);
    void addToFavourites(const QString &videoId);
    void deleteFromFavourites(const QString &favouriteId);
    void addToPlaylist(const QString &videoId, const QString &playlistId);
    void deleteFromPlaylist(const QString &playlistId, const QString &playlistVideoId);
    void createNewPlaylist(const QString &title, const QString &description, const bool &isPrivate);
    void deletePlaylist(const QString &playlistId);
    void subscribeToChannel(const QString &username);
    void unsubscribeToChannel(const QString &subscriptionId);
    void rateVideo(const QString &videoId, const QString &likeOrDislike);
    void addComment(const QString &videoId, const QString &comment);
    void replyToComment(const QString &videoId, const QString &commentId, const QString &comment);
    void setPlaybackQuality(const QString &quality);
    void getVideoUrl(const QString &videoId);
    void getLiveVideoUrl(const QString &playerUrl);
    void uploadVideo(const QString &filename, const QString &title, const QString &description, const QString &tags, const QString &category, const bool &isPrivate);
    void abortVideoUpload();

private slots:
    void setCurrentUser(const QString &user);
    void setAccessToken(const QString &token);
    void setUploadUrl();
    void performVideoUpload();
    void updateUploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void resumeVideoUpload();
    void uploadFinished();
    void postRequest(const QUrl &url, const QByteArray &xml);
    void deleteRequest(const QUrl &url);
    void postFinished();
    void parseVideoPage();
    void parseLiveVideoPage();

private:
    QNetworkAccessManager *nam;
    QFile *fileToBeUploaded;
    QUrl uploadUrl;
    QTime uploadTime;
    int uploadRetries;
    QNetworkReply *uploadReply;
    QByteArray developerKey;
    QString accessToken;
    QString currentUser;
    int playbackFormat;
    QHash<QString, int> pbMap;

signals:
    void gotVideoUrl(const QString &videoUrl);
    void videoNotAvailable(const QString &message);
    void videoUrlError(const QString &message);
    void gotVideoTitle(const QString &title);
    void videoTitleError();

    void alert(const QString &message);
    void currentUserChanged();
    void accessTokenChanged();
    void postSuccessful();
    void postFailed();
    void addedToFavourites();
    void deletedFromFavourites();
    void videoInFavourites();
    void addedToPlaylist();
    void deletedFromPlaylist();
    void playlistCreated();
    void playlistDeleted();
    void subscribed();
    void unsubscribed();
    void uploadStarted();
    void commentAdded();
    void videoRated();
    void cannotRate();
    void uploadProgressChanged(qint64 bytesSent, qint64 bytesTotal, const QString &speed);
    void uploadStatusChanged(const QString &status);
};

#endif // YouTube_H
