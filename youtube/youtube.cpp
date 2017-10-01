#include "youtube.h"

YouTube::YouTube(QObject *parent) :
    QObject(parent),
    nam(0),
    fileToBeUploaded(0),
    uploadUrl(),
    uploadTime(),
    uploadRetries(0),
    uploadReply(0),
    developerKey("AI39si6x9O1gQ1Z_BJqo9j2n_SdVsHu1pk2uqvoI3tVq8d6alyc1og785IPCkbVY3Q5MFuyt-IFYerMYun0MnLdQX5mo2BueSw"),
    accessToken(),
    currentUser(),
    playbackFormat(22),
    pbMap(),
    playbackQuality()
{
    pbMap["720p"] = 22;
    pbMap["480p"] = 35;
    pbMap["360p"] = 34;
    pbMap["hq"] = 18;
    pbMap["mobile"] = 5;

    playbackQuality << 5 << 18 << 22;
}

void YouTube::setPlaybackQuality(const QString &quality)
{
    playbackFormat = pbMap.value(quality, 18);
}

void YouTube::setUserCredentials(const QString &user, const QString &token)
{
    setCurrentUser(user);
    setAccessToken(token);
}

void YouTube::setCurrentUser(const QString &user)
{
    currentUser = user;
    emit currentUserChanged();
}

void YouTube::setAccessToken(const QString &token)
{
    accessToken = token;
    emit accessTokenChanged();
}

void YouTube::uploadVideo(const QString &filename, const QString &title, const QString &description, const QString &tags, const QString &category, const bool &isPrivate)
{
//    qDebug() << filename << "/n" << title << "/n" << description << "/n" << tags << "/n" << category << "/n" << isPrivate;

    emit uploadStatusChanged("preparing");
    fileToBeUploaded = new QFile(filename);
    if (!fileToBeUploaded->exists()) {
        emit uploadStatusChanged("failed");
        return;
    }

    QUrl url("http://uploads.gdata.youtube.com/resumable/feeds/api/users/default/uploads");
    QByteArray xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" \
                   "<entry xmlns=\"http://www.w3.org/2005/Atom\"\n" \
                   "xmlns:media=\"http://search.yahoo.com/mrss/\"\n" \
                   "xmlns:yt=\"http://gdata.youtube.com/schemas/2007\">\n" \
                   "<media:group>\n" \
                   "<media:title>" + title.toUtf8().toPercentEncoding(" \n\t#[]{}=+$&*()<>@|',/!\":;?") + "</media:title>\n" \
                   "<media:description>\n" + description.toUtf8().toPercentEncoding(" \n\t#[]{}=+$&*()<>@|',/!\":;?") + "\n\n" +
#ifdef Q_WS_MAEMO_5
                   "Uploaded via cuteTube\n</media:description>\n"
#elif (defined(Q_WS_X11))	// NPM: aka, MeeGo 
                   "Uploaded via cuteTube for MeeGo\n</media:description>\n"
#else
                   "Uploaded via cuteTube on Symbian\n</media:description>\n"
#endif
                   "<media:category scheme=\"http://gdata.youtube.com/schemas/2007/categories.cat\">\n" + category.toUtf8() + "\n</media:category>\n" \
                   "<media:keywords>" + tags.toUtf8().toPercentEncoding(" \n\t#[]{}=+$&*()<>@|',/!\":;?") + "</media:keywords>\n" \
                   "</media:group>\n" \
                   "</entry>");

    if (isPrivate) {
        int index = xml.lastIndexOf("<");
        xml.insert(index, "<yt:private/>\n");
    }

    QNetworkRequest request(url);
    request.setRawHeader("Host", "uploads.gdata.youtube.com");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/atom+xml; charset=UTF-8");
    request.setHeader(QNetworkRequest::ContentLengthHeader, xml.length());
    request.setRawHeader("Authorization", "AuthSub token=" + accessToken.toUtf8());
    request.setRawHeader("GData-Version", "2");
    request.setRawHeader("X-Gdata-Key", "key=" + developerKey);
    request.setRawHeader("Slug", filename.split("/").last().toUtf8());
    uploadReply = nam->post(request, xml);
    connect(uploadReply, SIGNAL(finished()), this, SLOT(setUploadUrl()));
}

void YouTube::setUploadUrl()
{
    if (uploadReply->error())
        return;

    int statusCode = uploadReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray statusText = uploadReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();
    //qDebug() << "Status is:" << statusCode << ":" << statusText;
    if (statusCode == 200) {
        uploadUrl = QUrl(uploadReply->rawHeader("Location"));
        //        qDebug() << uploadUrl;
        performVideoUpload();
    }
    else {
        emit alert(tr("Error - Server repsonse is: ") + statusText);
        emit uploadStatusChanged("failed");
    }
}

void YouTube::performVideoUpload()
{
    uploadRetries = 3;

    fileToBeUploaded->open(QIODevice::ReadOnly);
    QNetworkRequest request(uploadUrl);
    request.setRawHeader("Host", "uploads.gdata.youtube.com");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    request.setHeader(QNetworkRequest::ContentLengthHeader, fileToBeUploaded->size());
    uploadReply = nam->put(request, fileToBeUploaded);
    connect(uploadReply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(updateUploadProgress(qint64,qint64)));
    connect(uploadReply, SIGNAL(finished()), this, SLOT(uploadFinished()));
    emit uploadStatusChanged("started");
    uploadTime.start();
}

void YouTube::updateUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    double speed = bytesSent * 1000.0 / uploadTime.elapsed();
    QString unit;
    if (speed < 1024) {
        unit = "bytes/sec";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "kB/s";
    } else {
        speed /= 1024*1024;
        unit = "MB/s";
    }

    emit uploadProgressChanged(bytesSent, bytesTotal, QString::fromLatin1("%1 %2")
                               .arg(speed, 3, 'f', 1).arg(unit));
}

void YouTube::resumeVideoUpload()
{
    uploadRetries--;

    QByteArray rangeHeader = uploadReply->rawHeader("Range");
    QByteArray startByte = rangeHeader.split('-').last();
    QByteArray locationHeader = uploadReply->rawHeader("Location");

    //qDebug() << rangeHeader << startByte << locationHeader;

    if (locationHeader.length() > 0) {
        uploadUrl = QUrl(locationHeader);
    }

    fileToBeUploaded->open(QIODevice::ReadOnly);
    int fs = fileToBeUploaded->size();
    QByteArray fileSize = QByteArray::number(fs);
    QByteArray endByte = QByteArray::number(fs - 1);
    QByteArray range(startByte + '-' + endByte + '/' + fileSize);
    QNetworkRequest request(uploadUrl);
    request.setRawHeader("Host", "uploads.gdata.youtube.com");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    request.setHeader(QNetworkRequest::ContentLengthHeader, fs - startByte.toInt());
    request.setRawHeader("Content-Range", range);
    uploadReply = nam->put(request, fileToBeUploaded);
    connect(uploadReply, SIGNAL(uploadProgress(qint64,qint64)), this, SIGNAL(updateUploadProgress(qint64,qint64)));
    connect(uploadReply, SIGNAL(finished()), this, SLOT(uploadFinished()));
    emit uploadStatusChanged("started");
}

void YouTube::abortVideoUpload()
{
    uploadReply->abort();
    fileToBeUploaded->close();
    emit uploadStatusChanged("aborted");
}

void YouTube::uploadFinished()
{
    fileToBeUploaded->close();

    if (uploadReply->error()) {
        if (uploadReply->error() == QNetworkReply::OperationCanceledError) {
            emit uploadStatusChanged("aborted");
            return;
        }
        else {
            int statusCode = uploadReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode == 308) {
                if (uploadRetries > 0) {
                    emit uploadStatusChanged("interrupted");
                    QTimer::singleShot(3000, this, SLOT(resumeVideoUpload()));
                }
                else {
                    emit uploadStatusChanged("failed");
                }
            }
        }
    }
    else {
        emit uploadStatusChanged("completed");
    }
}

void YouTube::postRequest(const QUrl &url, const QByteArray &xml)
{
    /* Helper method that posts HTTP POST requests */

    QNetworkRequest request(url);
    request.setRawHeader("Host", "gdata.youtube.com");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/atom+xml");
    request.setHeader(QNetworkRequest::ContentLengthHeader, xml.length());
    request.setRawHeader("Authorization", "AuthSub token=" + accessToken.toUtf8());
    request.setRawHeader("GData-Version", "2");
    request.setRawHeader("X-Gdata-Key", "key=" + developerKey);
    QNetworkReply* reply = nam->post(request, xml);
    connect(reply, SIGNAL(finished()), this, SLOT(postFinished()));
}

void YouTube::postFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        emit alert(tr("Error - YouTube server unreachable"));
        return;
    }

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray statusText = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();
    //    qDebug() << "Status is:" << statusCode << ":" << statusText;
    if ((statusCode == 200) || (statusCode == 201)) {
        emit postSuccessful();
    }
    else if ((statusText == "Bad Request") || ((statusText == "Forbidden") && !(currentUser == ""))) {
        emit postFailed();
    }
    else {
        emit alert(tr("Error - Server repsonse is: ") + statusText);
    }
    disconnect(this, SIGNAL(postSuccessful()), 0, 0);
    disconnect(this, SIGNAL(postFailed()), 0, 0);
    reply->deleteLater();
}

void YouTube::deleteRequest(const QUrl &url)
{
    /* Helper method that posts HTTP DELETE requests */

    QNetworkRequest request(url);
    request.setRawHeader("Host", "gdata.youtube.com");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/atom+xml");
    request.setRawHeader("Authorization", "AuthSub token=" + accessToken.toUtf8());
    request.setRawHeader("GData-Version", "2");
    request.setRawHeader("X-Gdata-Key", "key=" + developerKey);
    QNetworkReply* reply = nam->deleteResource(request);
    connect(reply, SIGNAL(finished()), this, SLOT(postFinished()));
}

void YouTube::addToFavourites(const QString &videoId)
{
    QUrl url("http://gdata.youtube.com/feeds/api/users/default/favorites");
    QByteArray xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" \
                   "<entry xmlns=\"http://www.w3.org/2005/Atom\">\n" \
                   "<id>" + videoId.toUtf8() + "</id>\n" \
                   "</entry>");
    postRequest(url, xml);
    connect(this, SIGNAL(postSuccessful()), this, SIGNAL(addedToFavourites()));
    connect(this, SIGNAL(postFailed()), this, SIGNAL(videoInFavourites()));
}

void YouTube::deleteFromFavourites(const QString &favouriteId)
{
    QUrl url("http://gdata.youtube.com/feeds/api/users/" + currentUser + "/favorites/" + favouriteId);
    deleteRequest(url);
    connect(this, SIGNAL(postSuccessful()), this, SIGNAL(deletedFromFavourites()));
}

void YouTube::addToPlaylist(const QString &videoId, const QString &playlistId)
{
    QUrl url("http://gdata.youtube.com/feeds/api/playlists/" + playlistId);
    QByteArray xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" \
                   "<entry xmlns=\"http://www.w3.org/2005/Atom\"\n" \
                   "xmlns:yt=\"http://gdata.youtube.com/schemas/2007\">\n" \
                   "<id>" + videoId.toUtf8() + "</id>\n" \
                   "</entry>");
    postRequest(url, xml);
    connect(this, SIGNAL(postSuccessful()), this, SIGNAL(addedToPlaylist()));
}

void YouTube::deleteFromPlaylist(const QString &playlistId, const QString &playlistVideoId)
{
    QUrl url("http://gdata.youtube.com/feeds/api/playlists/" + playlistId + "/" + playlistVideoId);
    deleteRequest(url);
    connect(this, SIGNAL(postSuccessful()), this, SIGNAL(deletedFromPlaylist()));
}

void YouTube::createNewPlaylist(const QString &title, const QString &description, const bool &isPrivate)
{
    QUrl url("http://gdata.youtube.com/feeds/api/users/default/playlists");
    QByteArray xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" \
                   "<entry xmlns=\"http://www.w3.org/2005/Atom\"\n" \
                   "xmlns:yt=\"http://gdata.youtube.com/schemas/2007\">\n" \
                   "<title>" + title.toUtf8().toPercentEncoding(" \n\t#[]{}=+$&*()<>@|',/!\":;?") + "</title>\n" \
                   "<summary>" + description.toUtf8().toPercentEncoding(" \n\t#[]{}=+$&*()<>@|',/!\":;?") + "</summary>\n" \
                   "</entry>");
    if (isPrivate) {
        int index = xml.lastIndexOf("<");
        xml.insert(index, "<yt:private/>\n");
    }
    postRequest(url, xml);
    connect(this, SIGNAL(postSuccessful()), this, SIGNAL(playlistCreated()));
}

void YouTube::deletePlaylist(const QString &playlistId)
{
    QUrl url("http://gdata.youtube.com/feeds/api/users/" + currentUser + "/playlists/" + playlistId);
    deleteRequest(url);
    connect(this, SIGNAL(postSuccessful()), this, SIGNAL(playlistDeleted()));
}

void YouTube::subscribeToChannel(const QString &username)
{
    QUrl url("http://gdata.youtube.com/feeds/api/users/default/subscriptions");
    QByteArray xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" \
                   "<entry xmlns=\"http://www.w3.org/2005/Atom\"\n" \
                   "xmlns:yt=\"http://gdata.youtube.com/schemas/2007\">\n" \
                   "<category scheme=\"http://gdata.youtube.com/schemas/2007/subscriptiontypes.cat\"\n" \
                   "term=\"channel\"/>\n" \
                   "<yt:username>" + username.toUtf8() + "</yt:username>\n" \
                   "</entry>");
    postRequest(url, xml);
    connect(this, SIGNAL(postSuccessful()), this, SIGNAL(subscribed()));
}

void YouTube::unsubscribeToChannel(const QString &subscriptionId)
{
    QUrl url("http://gdata.youtube.com/feeds/api/users/" + currentUser + "/subscriptions/" + subscriptionId);
    deleteRequest(url);
    connect(this, SIGNAL(postSuccessful()), this, SIGNAL(unsubscribed()));
}

void YouTube::rateVideo(const QString &videoId, const QString &likeOrDislike)
{
    QUrl url("http://gdata.youtube.com/feeds/api/videos/" + videoId + "/ratings");
    QByteArray xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" \
                   "<entry xmlns=\"http://www.w3.org/2005/Atom\"\n" \
                   "xmlns:yt=\"http://gdata.youtube.com/schemas/2007\">\n" \
                   "<yt:rating value=\"" + likeOrDislike.toUtf8() + "\"/>\n" \
                   "</entry>");
    postRequest(url, xml);
    connect(this, SIGNAL(postSuccessful()), this, SIGNAL(videoRated()));
    connect(this, SIGNAL(postFailed()), this, SIGNAL(cannotRate()));
}

void YouTube::addComment(const QString &videoId, const QString &comment)
{
    QUrl url("http://gdata.youtube.com/feeds/api/videos/" + videoId + "/comments");
    QByteArray xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" \
                   "<entry xmlns=\"http://www.w3.org/2005/Atom\"\n" \
                   "xmlns:yt=\"http://gdata.youtube.com/schemas/2007\">\n" \
                   "<content>" + comment.toUtf8().toPercentEncoding(" \n\t#[]{}=+$&*()<>@|',/!\":;?") + "</content>\n" \
                   "</entry>");
    postRequest(url, xml);
    connect(this, SIGNAL(postSuccessful()), this, SIGNAL(commentAdded()));
}

void YouTube::replyToComment(const QString &videoId, const QString &commentId, const QString &comment)
{
    QUrl url("http://gdata.youtube.com/feeds/api/videos/" + videoId + "/comments");
    QByteArray xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" \
                   "<entry xmlns=\"http://www.w3.org/2005/Atom\"\n" \
                   "xmlns:yt=\"http://gdata.youtube.com/schemas/2007\">\n" \
                   "<link rel=\"http://gdata.youtube.com/schemas/2007#in-reply-to\"\n" \
                   "type=\"application/atom+xml\"\n" \
                   "href=\"http://gdata.youtube.com/feeds/api/videos/" + videoId.toUtf8() + "/comments/" + commentId.toUtf8() + "\"/>\n" \
                   "<content>" + comment.toUtf8().toPercentEncoding(" \n\t#[]{}=+$&*()<>@|',/!\":;?") + "</content>\n" \
                   "</entry>");
    postRequest(url, xml);
}

void YouTube::sslErrorsRaised(QList<QSslError> errors)
{
    Q_UNUSED(errors)

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    reply->ignoreSslErrors();
}

void YouTube::getVideoUrl(const QString &videoId)
{
    QString playerUrl = QString("https://www.youtube.com/watch?v=%1&gl=US&hl=en&has_verified=1&bpctr=9999999999").arg(videoId);

    if (nam == 0)
    {
        emit videoUrlError("ERROR, no network manager initialized.");
    }
    else
    {
        QNetworkRequest request;
        request.setUrl(QUrl(playerUrl));

        QNetworkReply *reply = nam->get(request);
        connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrorsRaised(QList<QSslError>)));
        connect(reply, SIGNAL(finished()), this, SLOT(parseVideoPage()));
    }
}

void YouTube::parseVideoPage()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError)
    {
        emit videoTitleError();
        emit videoUrlError(reply->errorString());
    }
    else
    {
        QString video_webpage(reply->readAll());

        QRegularExpression ytplayer(";ytplayer\\.config\\s*=\\s*(\\{.*?\\});");
        QRegularExpressionMatch match_ytplayer = ytplayer.match(video_webpage);
        if (match_ytplayer.hasMatch())
        {
            QJsonDocument json = QJsonDocument::fromJson(match_ytplayer.captured(1).toUtf8());
            if (json.isObject())
            {
                QJsonObject jsonObject = json.object();
                if (jsonObject.contains("args") && jsonObject["args"].isObject())
                {
                    QJsonObject videoInfo = jsonObject["args"].toObject();

                    if (videoInfo.contains("title"))
                        emit gotVideoTitle(videoInfo["title"].toString());
                    else
                        emit videoTitleError();

                    QString encoded_url_map = videoInfo["url_encoded_fmt_stream_map"].toString() + "," + videoInfo["adaptive_fmts"].toString();

                    int indexMaxQuality = playbackQuality.indexOf(playbackFormat);
                    int indexQualityFound = -1;
                    QHash<QString, QString> url_dataFound;

                    // parse url for streaming
                    foreach (const QString &url_data_str, encoded_url_map.split(","))
                    {
                        if (!url_data_str.isEmpty())
                        {
                            // convert url_data_str in QHash object url_data
                            QHash<QString, QString> url_data;
                            foreach (const QString &data, url_data_str.split("&"))
                            {
                                if (data.contains("="))
                                {
                                    int index = data.indexOf("=");
                                    QString param = data.left(index);
                                    QString value = QByteArray::fromPercentEncoding(data.right(data.size()-index-1).toUtf8());
                                    url_data[param] = value;
                                }
                                else
                                {
                                    qWarning() << "ERROR, invalid data" << url_data_str << data << "from encoded url" << url_data_str;
                                }
                            }

                            // read url_data
                            if (!url_data.contains("itag") || !url_data.contains("url"))
                            {
                                qWarning() << "ERROR, invalid data" << url_data;
                            }
                            else
                            {
                                int indexQuality = playbackQuality.indexOf(url_data["itag"].toInt());
                                if (indexQuality != -1 && indexQuality <= indexMaxQuality && indexQuality > indexQualityFound)
                                {
                                    indexQualityFound = indexQuality;
                                    url_dataFound = url_data;
                                }
                            }
                        }
                    }

                    QString url;
                    if (indexQualityFound != -1)
                    {
                        url = url_dataFound["url"];

                        if (url_dataFound.contains("sig"))
                        {
                            // signature not encrypted

                            url += "&signature=" + url_dataFound["sig"];

                            emit gotVideoUrl(url);
                        }
                        else if (url_dataFound.contains("s"))
                        {
                            // signature encrypted

                            QString encrypted_sig = url_dataFound["s"];
                            QString jsplayer_url_json = jsonObject["assets"].toObject()["js"].toString();
                            if (jsplayer_url_json.startsWith("//"))
                                jsplayer_url_json = QString("https:")+jsplayer_url_json;
                            if (jsplayer_url_json.startsWith("/"))
                                jsplayer_url_json = QString("https://s.ytimg.com")+jsplayer_url_json;

                            DecryptYoutubeSignature *request = new DecryptYoutubeSignature(nam, url, encrypted_sig, QUrl(jsplayer_url_json), this);
                            connect(request, SIGNAL(error(QString)), this, SIGNAL(videoUrlError(QString)));
                            connect(request, SIGNAL(decryptedSignature(QString)), this, SIGNAL(gotVideoUrl(QString)));

                            request->decrypt();
                        }
                        else
                        {
                            // no signature

                            emit gotVideoUrl(url);
                        }
                    }
                }
                else
                {
                    emit videoUrlError("ERROR, no args in json object to parse video page.");
                }
            }
            else
            {
                emit videoUrlError("ERROR, invalid json object to parse video page.");
            }
        }
        else
        {
            QRegularExpression errorMessage("\"unavailable-message\"(.+?)>(.+?)</h1>", QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);
            QRegularExpressionMatch match_errorMessage = errorMessage.match(video_webpage);
            if (match_errorMessage.hasMatch())
            {
                QString message = match_errorMessage.captured(2).trimmed();
                emit videoUrlError(message);
                emit videoNotAvailable(message);
            }
            else
            {
                emit videoUrlError("ERROR, video not available.");
            }
        }
    }
    reply->deleteLater();
}

void YouTube::getLiveVideoUrl(const QString &videoId)
{
    QString playerUrl = "http://www.youtube.com/get_video_info?&video_id=" + videoId + "&el=detailpage&ps=default&eurl=&gl=US&hl=en";

    if (nam == 0)
    {
        emit videoUrlError("ERROR, no network manager initialized.");
    }
    else
    {
        QNetworkRequest request;
        request.setUrl(QUrl(playerUrl));

        QNetworkReply *reply = nam->get(request);
        connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrorsRaised(QList<QSslError>)));
        connect(reply, SIGNAL(finished()), this, SLOT(parseLiveVideoPage()));
    }
}

void YouTube::parseLiveVideoPage()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError)
    {
        emit videoUrlError(reply->errorString());
    }
    else
    {
        QByteArray response = reply->readAll();
        response = QByteArray::fromPercentEncoding(response.simplified().replace(QByteArray(" "), QByteArray("")));
        //    qDebug() << response;
        int pos = response.indexOf("fmt_stream_map=") + 18;
        int pos2 = response.indexOf('|', pos);
        response = response.mid(pos, pos2 - pos);
        QByteArray videoUrl = response.replace(QByteArray("\\/"), QByteArray("/")).replace(QByteArray("\\u0026"), QByteArray("&")).replace(QByteArray("%2C"), QByteArray(","));
        if (!(videoUrl.startsWith("http")))
        {
            emit alert(tr("Error: Unable to retrieve video"));
            emit videoUrlError(tr("Error: Unable to retrieve video"));
        }
        else
        {
            emit gotVideoUrl(QString(videoUrl));
        }
    }

    reply->deleteLater();
}


