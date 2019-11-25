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

void DlnaCheckFunctions::check_dlna_video(DlnaResource *dlna, const QString &id, const QString &parentId, const QString &title, const QString &protocolInfo, const QString &duration, const QString &resolution, const int &channels, const int &samplerate, const qint64 &bitrate, const qint64 &size, const QString &link)
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
    QCOMPARE(node.childNodes().size(), 4);

    QCOMPARE(xml.elementsByTagName("dc:title").size(), 1);
    QCOMPARE(xml.elementsByTagName("dc:title").at(0).firstChild().nodeValue(), title);

    QCOMPARE(xml.elementsByTagName("upnp:class").size(), 1);
    QCOMPARE(xml.elementsByTagName("upnp:class").at(0).firstChild().nodeValue(), "object.item.videoItem");

    QCOMPARE(xml.elementsByTagName("upnp:genre").size(), 1);
    QCOMPARE(xml.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue(), "");

    QCOMPARE(xml.elementsByTagName("res").size(), 1);
    QDomNode res = xml.elementsByTagName("res").at(0);

    // check attributes : protocolInfo, xmlns:dlna, duration, resolution, nrAudioChannels, sampleFrequency, bitrate, size
    check_dlna_video_res(res, protocolInfo, duration, resolution, channels, samplerate, bitrate, size, link);
}

void DlnaCheckFunctions::check_dlna_video_res(const QDomNode &res, const QString &protocolInfo, const QString &duration, const QString &resolution, const int &channels, const int &samplerate, const qint64 &bitrate, const qint64 &size, const QString &link)
{
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

void DlnaCheckFunctions::check_dlna_audio(DlnaResource *track, const QString &id, const QString &parentId, const QString &title, const QString &album, const QString &artist, const QString &contributor, const QString &genre, const int &trackNumber, const QString &date, const QString &protocolInfo, const QString &duration, const int &channels, const int &samplerate, const qint64 &bitrate, const qint64 &size, const QString &link)
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

    QCOMPARE(xml.elementsByTagName("res").size(), 1);
    QDomNode res = xml.elementsByTagName("res").at(0);

    // check attributes : protocolInfo, xmlns:dlna, duration, nrAudioChannels, sampleFrequency, bitrate, size
    check_dlna_audio_res(res, protocolInfo, duration, channels, samplerate, bitrate, size, link);

    // check child node : text node
    QCOMPARE(res.childNodes().size(), 1);
    QVERIFY(!res.childNodes().at(0).nodeValue().isEmpty());
}

void DlnaCheckFunctions::check_dlna_audio_res(const QDomNode &res, const QString &protocolInfo, const QString &duration, const int &channels, const int &samplerate, const qint64 &bitrate, const qint64 &size, const QString &link)
{
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

void DlnaCheckFunctions::check_streaming(DlnaMusicTrackFile *track, const QString &p_range, const qint64 &p_bytesAvailableBeforeOpen, const qint64 &p_bytesAvailableAfterOpen, const qint64 &p_transcodeSize, const qint64 &p_transcodedSize, const int &maxTimeToTranscode)
{
    QScopedPointer<Device>transcodeProcess(track->getStream());
    QVERIFY(transcodeProcess != Q_NULLPTR);

    connect(this, &DlnaCheckFunctions::startTranscoding, transcodeProcess.data(), &Device::startRequestData);
    connect(transcodeProcess.data(), &Device::sendDataToClientSignal, this, &DlnaCheckFunctions::receivedTranscodedData);
    connect(transcodeProcess.data(), &Device::openedSignal, this, &DlnaCheckFunctions::transcodingOpened);

    connect(transcodeProcess.data(), &Device::readyRead, this, &DlnaCheckFunctions::dataAvailable);
    connect(this, SIGNAL(requestData(qint64)), transcodeProcess.data(), SLOT(requestData(qint64)));
    connect(transcodeProcess.data(), SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));

    QScopedPointer<HttpRange>range;
    if (!p_range.isEmpty())
    {
        range.reset(new HttpRange(p_range));
        range->setSize(track->size());
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

    QVERIFY(transcodeProcess->isOpen());
    QCOMPARE(transcodeProcess->pos(), 0);
    QCOMPARE(transcodeProcess->bytesAvailable(), p_bytesAvailableAfterOpen);
    QVERIFY(!transcodeProcess->atEnd());

    emit startTranscoding();
    QVERIFY(transcodeProcess->waitForFinished(-1));

    qint64 duration = transcodeTimer.elapsed();
    QVERIFY2(timeToOpenTranscoding < 100, QString("time to open transcoding %1 (>= 100)").arg(timeToOpenTranscoding).toUtf8());
    if (duration >= maxTimeToTranscode)
        qWarning() << "Transcoding done in" << duration << "ms >=" << maxTimeToTranscode << "ms.";
    qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";

    QVERIFY(transcodeProcess->isOpen());
    QCOMPARE(transcodeProcess->pos(), p_transcodedSize);
    QCOMPARE(transcodeProcess->bytesAvailable(), 0);
    QVERIFY(transcodeProcess->atEnd());

    QCOMPARE(transcodedBytes, p_transcodedSize);
    QVERIFY2(p_transcodeSize >= transcodedBytes, QString("%1 %2").arg(p_transcodeSize).arg(transcodedBytes).toUtf8());
    QVERIFY2(track->size() >= transcodedBytes, QString("%1 %2").arg(track->size()).arg(transcodedBytes).toUtf8());
}
