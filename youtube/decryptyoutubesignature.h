#ifndef DECRYPTYOUTUBESIGNATURE_H
#define DECRYPTYOUTUBESIGNATURE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJSEngine>
#include <QRegularExpression>

class DecryptYoutubeSignature : public QObject
{
    Q_OBJECT

public:
    explicit DecryptYoutubeSignature(QNetworkAccessManager *nam, QString youtubeUrl, QString encryptedSignature, QUrl urlForDecoding, QObject *parent = 0);

    void decrypt();

    QString playerType() const { return m_playerType; }
    QString playerId() const { return m_playerId; }

signals:
    void error(const QString &message);
    void decryptedSignature(const QString &url);

private slots:
    void decryptSignature();

private:
    QNetworkAccessManager *nam;
    QString m_youtubeUrl;
    QString m_encryptedSignature;
    QUrl m_urlForDecoding;
    QString m_playerId;
    QString m_playerType;
    static QHash<QString, QJSEngine*> cacheJsEngine;
    static QHash<QString, QString> cacheJsFun;
};

#endif // DECRYPTYOUTUBESIGNATURE_H
