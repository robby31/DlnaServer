#include "dlna_check_functions.h"

DlnaCheckFunctions::DlnaCheckFunctions(QObject *parent):
    QObject(parent)
{

}

void DlnaCheckFunctions::check_dlna_storage(DlnaResource *dlna, const QString &id, const QString &parentId, const int &childrenSize, const QString &title)
{
    QStringList properties;
    properties << "*";

    QDomDocument xml;
    xml.appendChild(dlna->getXmlContentDirectory(&xml, properties));

    QCOMPARE(xml.childNodes().size(), 1);

    QCOMPARE(xml.elementsByTagName("container").size(), 1);

    QDomNode node = xml.elementsByTagName("container").at(0);

    // check attributes : id, parentID, childCount, restricted
    QCOMPARE(node.attributes().size(), 4);
    QCOMPARE(node.attributes().namedItem("id").nodeValue(), id);
    QCOMPARE(node.attributes().namedItem("parentID").nodeValue(), parentId);
    QCOMPARE(node.attributes().namedItem("childCount").nodeValue().toInt(), childrenSize);
    QCOMPARE(node.attributes().namedItem("restricted").nodeValue(), "true");

    // check children nodes : dc:title, upnp:class
    QCOMPARE(node.childNodes().size(), 2);

    QCOMPARE(xml.elementsByTagName("dc:title").size(), 1);
    QCOMPARE(xml.elementsByTagName("dc:title").at(0).firstChild().nodeValue(), title);

    QCOMPARE(xml.elementsByTagName("upnp:class").size(), 1);
    QCOMPARE(xml.elementsByTagName("upnp:class").at(0).firstChild().nodeValue(), "object.container.storageFolder");
}

void DlnaCheckFunctions::check_dlna_video(DlnaResource *dlna, const QString &id, const QString &parentId, const QString &title, const int &nb_res)
{
    QStringList properties;
    properties << "upnp:genre";
    properties << "res@size";
    properties << "res@duration";
    properties << "res@bitrate";
    properties << "res@resolution";
    properties << "res@nrAudioChannels";
    properties << "res@sampleFrequency";

    QDomDocument xml;
    xml.appendChild(dlna->getXmlContentDirectory(&xml, properties));

    QCOMPARE(xml.childNodes().size(), 1);

    QCOMPARE(xml.elementsByTagName("item").size(), 1);

    QDomNode node = xml.elementsByTagName("item").at(0);

    // check attributes : id, parentID, restricted
    QCOMPARE(node.attributes().size(), 3);
    QCOMPARE(node.attributes().namedItem("id").nodeValue(), id);
    QCOMPARE(node.attributes().namedItem("parentID").nodeValue(), parentId);
    QCOMPARE(node.attributes().namedItem("restricted").nodeValue(), "true");

    // check children nodes : dc:title, upnp:class, upnp:genre, res
    QCOMPARE(node.childNodes().size(), 3+nb_res);

    QCOMPARE(xml.elementsByTagName("dc:title").size(), 1);
    QCOMPARE(xml.elementsByTagName("dc:title").at(0).firstChild().nodeValue(), title);

    QCOMPARE(xml.elementsByTagName("upnp:class").size(), 1);
    QCOMPARE(xml.elementsByTagName("upnp:class").at(0).firstChild().nodeValue(), "object.item.videoItem");

    QCOMPARE(xml.elementsByTagName("upnp:genre").size(), 1);
    QCOMPARE(xml.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue(), "");

    QCOMPARE(xml.elementsByTagName("res").size(), nb_res);
}

void DlnaCheckFunctions::check_dlna_video_res(DlnaResource *dlna, const int &index, const QString &protocolInfo, const QString &duration, const QString &resolution, const int &channels, const int &samplerate, const qint64 &bitrate, const qint64 &size, const QString &link)
{
    QStringList properties;
    properties << "upnp:genre";
    properties << "res@size";
    properties << "res@duration";
    properties << "res@bitrate";
    properties << "res@resolution";
    properties << "res@nrAudioChannels";
    properties << "res@sampleFrequency";

    QDomDocument xml;
    xml.appendChild(dlna->getXmlContentDirectory(&xml, properties));
    qWarning() << "XML CONTENT" << xml.toString();

    QDomNodeList l_res = xml.elementsByTagName("res");
    QVERIFY(index >= 0);
    QVERIFY(l_res.size() > index);

    QDomNode res = l_res.at(index);
    QDomNamedNodeMap attributes = res.attributes();

    QCOMPARE(attributes.size(), 8);
    QCOMPARE(attributes.namedItem("protocolInfo").nodeValue(), protocolInfo);
    QCOMPARE(attributes.namedItem("xmlns:dlna").nodeValue(), "urn:schemas-dlna-org:metadata-1-0/");
    QCOMPARE(attributes.namedItem("duration").nodeValue(), duration);
    QCOMPARE(attributes.namedItem("resolution").nodeValue(), resolution);
    QCOMPARE(attributes.namedItem("nrAudioChannels").nodeValue().toInt(), channels);
    QCOMPARE(attributes.namedItem("sampleFrequency").nodeValue().toInt(), samplerate);
    QCOMPARE(attributes.namedItem("bitrate").nodeValue().toLongLong(), bitrate);
    QCOMPARE(attributes.namedItem("size").nodeValue().toLongLong(), size);

    // check child node : text node
    QCOMPARE(res.childNodes().size(), 1);
    QCOMPARE(res.childNodes().at(0).nodeValue(), link);
}

void DlnaCheckFunctions::check_dlna_audio(DlnaResource *track, const QString &id, const QString &parentId, const QString &title, const QString &album, const QString &artist, const QString &contributor, const QString &genre, const int &trackNumber, const QString &date, const int &nb_res)
{
    QStringList properties;
    properties << "dc:title";
    properties << "upnp:album";
    properties << "upnp:artist";
    properties << "dc:contributor";
    properties << "upnp:genre";
    properties << "upnp:originalTrackNumber";
    properties << "dc:date";
    properties << "res@size";
    properties << "res@duration";
    properties << "res@bitrate";
    properties << "res@sampleFrequency";
    properties << "res@nrAudioChannels";

    QDomDocument xml;
    xml.appendChild(track->getXmlContentDirectory(&xml, properties));

    QCOMPARE(xml.childNodes().size(), 1);

    QCOMPARE(xml.elementsByTagName("item").size(), 1);

    QDomNode node = xml.elementsByTagName("item").at(0);

    // check attributes : id, parentID, restricted
    QCOMPARE(node.attributes().size(), 3);
    QCOMPARE(node.attributes().namedItem("id").nodeValue(), id);
    QCOMPARE(node.attributes().namedItem("parentID").nodeValue(), parentId);
    QCOMPARE(node.attributes().namedItem("restricted").nodeValue(), "true");

    // check children nodes : dc:title, upnp:class, upnp:genre, res ...
    QCOMPARE(node.childNodes().size(), 9);

    QCOMPARE(xml.elementsByTagName("dc:title").size(), 1);
    QCOMPARE(xml.elementsByTagName("dc:title").at(0).firstChild().nodeValue(), title);

    QCOMPARE(xml.elementsByTagName("upnp:album").size(), 1);
    QCOMPARE(xml.elementsByTagName("upnp:album").at(0).firstChild().nodeValue(), album);

    QCOMPARE(xml.elementsByTagName("upnp:artist").size(), 1);
    QCOMPARE(xml.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue(), artist);

    QCOMPARE(xml.elementsByTagName("dc:contributor").size(), 1);
    QCOMPARE(xml.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue(), contributor);

    QCOMPARE(xml.elementsByTagName("upnp:genre").size(), 1);
    QCOMPARE(xml.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue(), genre);

    QCOMPARE(xml.elementsByTagName("upnp:originalTrackNumber").size(), 1);
    QCOMPARE(xml.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue().toInt(), trackNumber);

    QCOMPARE(xml.elementsByTagName("dc:date").size(), 1);
    QCOMPARE(xml.elementsByTagName("dc:date").at(0).firstChild().nodeValue(), date);

    QCOMPARE(xml.elementsByTagName("upnp:class").size(), 1);
    QCOMPARE(xml.elementsByTagName("upnp:class").at(0).firstChild().nodeValue(), "object.item.audioItem.musicTrack");

    QCOMPARE(xml.elementsByTagName("res").size(), nb_res);
}

void DlnaCheckFunctions::check_dlna_audio_res(DlnaResource *track, const int &index, const QString &protocolInfo, const QString &duration, const int &channels, const int &samplerate, const qint64 &bitrate, const qint64 &size, const QString &link)
{
    QStringList properties;
    properties << "dc:title";
    properties << "upnp:album";
    properties << "upnp:artist";
    properties << "dc:contributor";
    properties << "upnp:genre";
    properties << "upnp:originalTrackNumber";
    properties << "dc:date";
    properties << "res@size";
    properties << "res@duration";
    properties << "res@bitrate";
    properties << "res@sampleFrequency";
    properties << "res@nrAudioChannels";

    QDomDocument xml;
    xml.appendChild(track->getXmlContentDirectory(&xml, properties));

    QDomNodeList l_res = xml.elementsByTagName("res");
    QVERIFY(index >= 0);
    QVERIFY(l_res.size() > index);

    QDomNode res = l_res.at(index);
    QDomNamedNodeMap attributes = res.attributes();

    QCOMPARE(attributes.size(), 7);
    QCOMPARE(attributes.namedItem("protocolInfo").nodeValue(), protocolInfo);
    QCOMPARE(attributes.namedItem("xmlns:dlna").nodeValue(), "urn:schemas-dlna-org:metadata-1-0/");
    QCOMPARE(attributes.namedItem("duration").nodeValue(), duration);
    QCOMPARE(attributes.namedItem("nrAudioChannels").nodeValue().toInt(), channels);
    QCOMPARE(attributes.namedItem("sampleFrequency").nodeValue().toInt(), samplerate);
    QCOMPARE(attributes.namedItem("bitrate").nodeValue().toLongLong(), bitrate);
    QCOMPARE(attributes.namedItem("size").nodeValue().toLongLong(), size);

    // check child node : text node
    QCOMPARE(res.childNodes().size(), 1);
    QCOMPARE(res.childNodes().at(0).nodeValue(), link);
}

void DlnaCheckFunctions::receivedTranscodedData(const QByteArray &data)
{
    transcodedBytes += data.size();
}

void DlnaCheckFunctions::dataAvailable()
{
    emit requestData(1000000);
}

void DlnaCheckFunctions::transcodingOpened()
{
    if (transcodeTimer.isValid())
        timeToOpenTranscoding = transcodeTimer.elapsed();
}

void DlnaCheckFunctions::LogMessage(const QString &message)
{
    qInfo() << message;
}

void DlnaCheckFunctions::check_streaming(DlnaItem *media, const qint64 &startTime, const qint64 &endTime, const QString &p_range, const qint64 &p_bytesAvailableBeforeOpen, const qint64 &p_bytesAvailableAfterOpen, const qint64 &p_transcodeSize, const qint64 &p_transcodedSize, const int &maxTimeToTranscode, const bool &variable_bitrate)
{
    QScopedPointer<Device>transcodeProcess(media->getStream());
    QVERIFY(transcodeProcess != Q_NULLPTR);

    connect(this, &DlnaCheckFunctions::startTranscoding, transcodeProcess.data(), &Device::startRequestData);
    connect(transcodeProcess.data(), &Device::sendDataToClientSignal, this, &DlnaCheckFunctions::receivedTranscodedData);
    connect(transcodeProcess.data(), &Device::openedSignal, this, &DlnaCheckFunctions::transcodingOpened);

    connect(transcodeProcess.data(), &Device::readyRead, this, &DlnaCheckFunctions::dataAvailable);
    connect(this, SIGNAL(requestData(qint64)), transcodeProcess.data(), SLOT(requestData(qint64)));
    connect(transcodeProcess.data(), SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));

    auto ffmpeg = qobject_cast<FfmpegTranscoding*>(transcodeProcess.data());
    if (ffmpeg)
        ffmpeg->setVariableBitrate(variable_bitrate);

    if (startTime != -1 or endTime != -1)
        transcodeProcess->setTimeSeek(startTime, endTime);

    QScopedPointer<HttpRange>range;
    if (!p_range.isEmpty())
    {
        range.reset(new HttpRange(p_range));
        range->setSize(media->size());
        transcodeProcess->setRange(range->getStartByte(), range->getEndByte());
    }

    QCOMPARE(transcodeProcess->size(), p_transcodeSize);

    QVERIFY(!transcodeProcess->isOpen());
    QCOMPARE(transcodeProcess->pos(), 0);
    QCOMPARE(transcodeProcess->bytesAvailable(), p_bytesAvailableBeforeOpen);
    QVERIFY(transcodeProcess->atEnd());

    transcodeTimer.start();
    timeToOpenTranscoding = 0;
    transcodedBytes = 0;
    QVERIFY(transcodeProcess->open());

//    QVERIFY(transcodeProcess->isOpen());
    QCOMPARE(transcodeProcess->pos(), 0);
    QCOMPARE(transcodeProcess->bytesAvailable(), p_bytesAvailableAfterOpen);
//    QVERIFY(!transcodeProcess->atEnd());

    emit startTranscoding();
    QVERIFY(transcodeProcess->waitForFinished(-1));

    qint64 duration = transcodeTimer.elapsed();
    if (timeToOpenTranscoding > 100)
        qWarning() << "time to open transcoding" << timeToOpenTranscoding << "(> 100)";
    if (duration >= maxTimeToTranscode)
        qWarning() << "Transcoding done in" << duration << "ms >=" << maxTimeToTranscode << "ms.";
    qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";

    QVERIFY(transcodeProcess->isOpen());
    QCOMPARE(transcodeProcess->pos(), p_transcodedSize);
    QCOMPARE(transcodeProcess->bytesAvailable(), 0);
    QVERIFY(transcodeProcess->atEnd());

    QCOMPARE(transcodedBytes, p_transcodedSize);
    QVERIFY2(p_transcodeSize >= transcodedBytes, QString("%1 %2").arg(p_transcodeSize).arg(transcodedBytes).toUtf8());
    QVERIFY2(media->size() >= transcodedBytes, QString("%1 %2").arg(media->size()).arg(transcodedBytes).toUtf8());
}
