#include "dlna_check_functions.h"

void check_dlna_storage(const QDomDocument &dlna, const QString &id, const QString &parentId, const int &childrenSize, const QString &title)
{
    QCOMPARE(dlna.childNodes().size(), 1);

    QCOMPARE(dlna.elementsByTagName("container").size(), 1);

    QDomNode node = dlna.elementsByTagName("container").at(0);

    // check attributes : id, parentID, childCount, restricted
    QCOMPARE(node.attributes().size(), 4);
    QCOMPARE(node.attributes().namedItem("id").nodeValue(), id);
    QCOMPARE(node.attributes().namedItem("parentID").nodeValue(), parentId);
    QCOMPARE(node.attributes().namedItem("childCount").nodeValue().toInt(), childrenSize);
    QCOMPARE(node.attributes().namedItem("restricted").nodeValue(), "true");

    // check children nodes : dc:title, upnp:class
    QCOMPARE(node.childNodes().size(), 2);

    QCOMPARE(dlna.elementsByTagName("dc:title").size(), 1);
    QCOMPARE(dlna.elementsByTagName("dc:title").at(0).firstChild().nodeValue(), title);

    QCOMPARE(dlna.elementsByTagName("upnp:class").size(), 1);
    QCOMPARE(dlna.elementsByTagName("upnp:class").at(0).firstChild().nodeValue(), "object.container.storageFolder");
}

void check_dlna_video(const QDomDocument &dlna, const QString &id, const QString &parentId, const QString &title, const QString &protocolInfo, const QString &duration, const QString &resolution, const int &channels, const int &samplerate, const qint64 &bitrate, const qint64 &size, const QString link)
{
    QCOMPARE(dlna.childNodes().size(), 1);

    QCOMPARE(dlna.elementsByTagName("item").size(), 1);

    QDomNode node = dlna.elementsByTagName("item").at(0);

    // check attributes : id, parentID, restricted
    QCOMPARE(node.attributes().size(), 3);
    QCOMPARE(node.attributes().namedItem("id").nodeValue(), id);
    QCOMPARE(node.attributes().namedItem("parentID").nodeValue(), parentId);
    QCOMPARE(node.attributes().namedItem("restricted").nodeValue(), "true");

    // check children nodes : dc:title, upnp:class, upnp:genre, res
    QCOMPARE(node.childNodes().size(), 4);

    QCOMPARE(dlna.elementsByTagName("dc:title").size(), 1);
    QCOMPARE(dlna.elementsByTagName("dc:title").at(0).firstChild().nodeValue(), title);

    QCOMPARE(dlna.elementsByTagName("upnp:class").size(), 1);
    QCOMPARE(dlna.elementsByTagName("upnp:class").at(0).firstChild().nodeValue(), "object.item.videoItem");

    QCOMPARE(dlna.elementsByTagName("upnp:genre").size(), 1);
    QCOMPARE(dlna.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue(), "");

    QCOMPARE(dlna.elementsByTagName("res").size(), 1);
    QDomNode res = dlna.elementsByTagName("res").at(0);

    // check attributes : protocolInfo, xmlns:dlna, duration, resolution, nrAudioChannels, sampleFrequency, bitrate, size
    check_dlna_video_res(res, protocolInfo, duration, resolution, channels, samplerate, bitrate, size, link);
}

void check_dlna_video_res(const QDomNode &res, const QString &protocolInfo, const QString &duration, const QString &resolution, const int &channels, const int &samplerate, const qint64 &bitrate, const qint64 &size, const QString &link)
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

void check_dlna_audio(const QDomDocument &dlna, const QString &id, const QString &parentId, const QString &title, const QString &album, const QString &artist, const QString &contributor, const QString &genre, const int &track, const QString &date, const QString &protocolInfo, const QString &duration, const int &channels, const int &samplerate, const qint64 &bitrate, const qint64 &size, const QString &link)
{
    QCOMPARE(dlna.childNodes().size(), 1);

    QCOMPARE(dlna.elementsByTagName("item").size(), 1);

    QDomNode node = dlna.elementsByTagName("item").at(0);

    // check attributes : id, parentID, restricted
    QCOMPARE(node.attributes().size(), 3);
    QCOMPARE(node.attributes().namedItem("id").nodeValue(), id);
    QCOMPARE(node.attributes().namedItem("parentID").nodeValue(), parentId);
    QCOMPARE(node.attributes().namedItem("restricted").nodeValue(), "true");

    // check children nodes : dc:title, upnp:class, upnp:genre, res ...
    QCOMPARE(node.childNodes().size(), 9);

    QCOMPARE(dlna.elementsByTagName("dc:title").size(), 1);
    QCOMPARE(dlna.elementsByTagName("dc:title").at(0).firstChild().nodeValue(), title);

    QCOMPARE(dlna.elementsByTagName("upnp:album").size(), 1);
    QCOMPARE(dlna.elementsByTagName("upnp:album").at(0).firstChild().nodeValue(), album);

    QCOMPARE(dlna.elementsByTagName("upnp:artist").size(), 1);
    QCOMPARE(dlna.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue(), artist);

    QCOMPARE(dlna.elementsByTagName("dc:contributor").size(), 1);
    QCOMPARE(dlna.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue(), contributor);

    QCOMPARE(dlna.elementsByTagName("upnp:genre").size(), 1);
    QCOMPARE(dlna.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue(), genre);

    QCOMPARE(dlna.elementsByTagName("upnp:originalTrackNumber").size(), 1);
    QCOMPARE(dlna.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue().toInt(), track);

    QCOMPARE(dlna.elementsByTagName("dc:date").size(), 1);
    QCOMPARE(dlna.elementsByTagName("dc:date").at(0).firstChild().nodeValue(), date);

    QCOMPARE(dlna.elementsByTagName("upnp:class").size(), 1);
    QCOMPARE(dlna.elementsByTagName("upnp:class").at(0).firstChild().nodeValue(), "object.item.audioItem.musicTrack");

    QCOMPARE(dlna.elementsByTagName("res").size(), 1);
    QDomNode res = dlna.elementsByTagName("res").at(0);

    // check attributes : protocolInfo, xmlns:dlna, duration, nrAudioChannels, sampleFrequency, bitrate, size
    check_dlna_audio_res(res, protocolInfo, duration, channels, samplerate, bitrate, size, link);

    // check child node : text node
    QCOMPARE(res.childNodes().size(), 1);
    QVERIFY(!res.childNodes().at(0).nodeValue().isEmpty());
}

void check_dlna_audio_res(const QDomNode &res, const QString &protocolInfo, const QString &duration, const int &channels, const int &samplerate, const qint64 &bitrate, const qint64 &size, const QString &link)
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