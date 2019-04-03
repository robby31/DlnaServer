#include "tst_dlnamusictrack.h"

tst_dlnamusictrack::tst_dlnamusictrack(QObject *parent) :
    QObject(parent),
    transcodedBytes(0),
    timeToOpenTranscoding(0),
    m_dlnaProfiles("/Users/doudou/workspaceQT/DLNA_server/app/xml profiles/dlna_profiles.xml")
{
    m_streamingThread = new QThread();
    connect(m_streamingThread, SIGNAL(finished()), this, SLOT(deleteLater()));
    m_streamingThread->start();
}

void tst_dlnamusictrack::cleanup()
{
    QCoreApplication::processEvents();

    QVERIFY2(DlnaResource::objectCounter == 0, QString("memory leak detected, %1 DlnaResource objects.").arg(DlnaResource::objectCounter).toUtf8());

    QCOMPARE(QFfmpegMedia::objectCounter, 0);
    QCOMPARE(QFfmpegStream::objectCounter, 0);
    QCOMPARE(QFfmpegFrame::objectCounter, 0);
    QCOMPARE(QFfmpegCodec::objectCounter, 0);
    QCOMPARE(QFfmpegBuffer::objectCounter, 0);
}

void tst_dlnamusictrack::receivedTranscodedData(const QByteArray &data)
{
    transcodedBytes += data.size();
}

void tst_dlnamusictrack::dataAvailable()
{
    emit requestData(1000000);
}

void tst_dlnamusictrack::transcodingOpened()
{
    if (transcodeTimer.isValid())
        timeToOpenTranscoding = transcodeTimer.elapsed();
}

void tst_dlnamusictrack::LogMessage(const QString &message)
{
    qInfo() << message;
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_MP3() {

    {
        DlnaMusicTrackFile track("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/07 On_Off.mp3");
        track.setHostUrl(QUrl("http://host:600"));

        QStringList sinkProtocol;
        sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
        sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3X;DLNA.ORG_FLAGS=8D100000000000000000000000000000";
        track.setDlnaProfiles(m_dlnaProfiles);
        track.setSinkProtocol(sinkProtocol);

        QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/07 On_Off.mp3");

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

        QDomDocument xml_res;
        xml_res.appendChild(track.getXmlContentDirectory(&xml_res, properties));
        QVERIFY(xml_res.childNodes().size() == 1);
        QVERIFY(xml_res.elementsByTagName("item").size() == 1);
        QDomNode node = xml_res.elementsByTagName("item").at(0);
        QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
        QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
        QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
        QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
        QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "On/Off");
        QVERIFY(xml_res.elementsByTagName("upnp:album").size() == 1);
        QVERIFY(xml_res.elementsByTagName("upnp:album").at(0).firstChild().nodeValue() == "Human After All");
        QVERIFY(xml_res.elementsByTagName("upnp:artist").size() == 1);
        QVERIFY(xml_res.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue() == "Daft Punk");
        QVERIFY(xml_res.elementsByTagName("dc:contributor").size() == 1);
        QVERIFY(xml_res.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue() == "Daft Punk");
        QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
        QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "House");
        QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").size() == 1);
        QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue() == "7");
        QVERIFY(xml_res.elementsByTagName("dc:date").size() == 1);
        QVERIFY(xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue() == "2013-01-02");
        QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
        QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.audioItem.musicTrack");
        QVERIFY(xml_res.elementsByTagName("res").size() == 1);
        QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
        QVERIFY2(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//07%20On_Off.mp3", xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue().toUtf8().constData());
        QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 5);
        QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000", xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue().toUtf8());
        QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
        QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "376593");
        QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:00:19", xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue().toUtf8());
        QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "18751", xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue().toUtf8());
        xml_res.clear();

        QVERIFY(track.mimeType() == "audio/mpeg");
        QVERIFY(track.format() == MP3);
        QVERIFY(track.size() == 376593);
        QVERIFY2(track.bitrate() == 150005, QString("%1").arg(track.bitrate()).toUtf8());
        QVERIFY(track.getLengthInSeconds() == 19);
        QVERIFY2(track.getLengthInMilliSeconds() == 19382, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
        QVERIFY(track.samplerate() == 44100);
        QVERIFY(track.channelCount() == 2);
        QVERIFY(track.toTranscode() == false);

        QVERIFY2(track.getdlnaOrgOpFlags() == "01", track.getdlnaOrgOpFlags().toUtf8());
        QVERIFY(track.getdlnaOrgPN() == "MP3");
        QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000");
        QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000");

        QVERIFY(track.getAlbumArt().isNull() == true);
        QVERIFY(track.getByteAlbumArt().isNull() == true);

        QVERIFY2(track.metaDataPerformerSort() == "Daft Punk", track.metaDataPerformerSort().toUtf8());
        QVERIFY2(track.metaDataAlbumArtist() == "Daft Punk", track.metaDataAlbumArtist().toUtf8());
        QVERIFY2(track.metaDataYear() == 2005, QString("%1").arg(track.metaDataYear()).toUtf8());

        //    QHash<QString, double> result = track.volumeInfo();
        //    QVERIFY2(result.keys().size() == 5, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
        //    QVERIFY2(result["n_samples"] == 1705200, QString("%1").arg(result["n_samples"]).toUtf8());
        //    QVERIFY2(result["mean_volume"] == -21.3, QString("%1").arg(result["mean_volume"]).toUtf8());
        //    QVERIFY2(result["max_volume"] == -8, QString("%1").arg(result["max_volume"]).toUtf8());
        //    QVERIFY2(result["histogram_7db"] == 1, QString("%1").arg(result["histogram_7db"]).toUtf8());
        //    QVERIFY2(result["histogram_8db"] == 3518, QString("%1").arg(result["histogram_8db"]).toUtf8());

        Device* stream = track.getStream();
        QVERIFY(stream != Q_NULLPTR);

        if (stream)
        {
            transcodeTimer.start();
            timeToOpenTranscoding = 0;
            transcodedBytes = 0;
            QVERIFY(stream->open() == true);
            QVERIFY(stream->isOpen() == true);
            QVERIFY(stream->size() == 376593);
            QCOMPARE(track.size(), stream->size());
            QVERIFY(stream->atEnd() == false);

            {
                QScopedPointer<StreamingFile> transcodeProcess(qobject_cast<StreamingFile*>(stream));
                QVERIFY(transcodeProcess != Q_NULLPTR);
                connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
                connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
                connect(transcodeProcess.data(), SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
                QVERIFY(transcodeProcess->atEnd() == false);
                emit startTranscoding();
                QVERIFY(transcodeProcess->waitForFinished(-1) == true);
                qint64 duration = transcodeTimer.elapsed();
                QVERIFY2(timeToOpenTranscoding < 100, QString("%1").arg(timeToOpenTranscoding).toUtf8());
                QVERIFY2(duration < 5000, QString("%1").arg(duration).toUtf8());
                qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
                QVERIFY(transcodeProcess->bytesAvailable() == 0);
                QVERIFY2(transcodedBytes == 376593, QString("%1").arg(transcodedBytes).toUtf8().constData());
                QVERIFY(transcodeProcess->pos() == 376593);
                QVERIFY(transcodeProcess->atEnd() == true);
                QVERIFY(track.size() >= transcodedBytes);
            }

            {
                QScopedPointer<HttpRange>range(new HttpRange("RANGE: BYTES=0-"));
                range->setSize(track.size());
                stream = track.getStream();
                stream->setRange(range->getStartByte(), range->getEndByte());
                QVERIFY(stream != Q_NULLPTR);
                transcodeTimer.start();
                timeToOpenTranscoding = 0;
                transcodedBytes = 0;
                QVERIFY(stream->open() == true);
                QVERIFY(stream->isOpen() == true);
                QVERIFY(stream->size() == 376593);
                QVERIFY(stream->atEnd() == false);

                QScopedPointer<StreamingFile> transcodeProcess(qobject_cast<StreamingFile*>(stream));
                QVERIFY(transcodeProcess != Q_NULLPTR);
                connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
                connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
                connect(transcodeProcess.data(), SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
                QVERIFY(transcodeProcess->atEnd() == false);
                emit startTranscoding();
                QVERIFY(transcodeProcess->waitForFinished(-1) == true);
                qint64 duration = transcodeTimer.elapsed();
                QVERIFY2(timeToOpenTranscoding < 100, QString("%1").arg(timeToOpenTranscoding).toUtf8());
                QVERIFY2(duration < 5000, QString("%1").arg(duration).toUtf8());
                qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
                QVERIFY(transcodeProcess->bytesAvailable() == 0);
                QVERIFY2(transcodedBytes == 376593, QString("%1").arg(transcodedBytes).toUtf8().constData());
                QVERIFY(transcodeProcess->pos() == 376593);
                QVERIFY(transcodeProcess->atEnd() == true);
                QVERIFY(track.size() >= transcodedBytes);
            }

            {
                QScopedPointer<HttpRange>range(new HttpRange("RANGE: BYTES=0-5000"));
                range->setSize(track.size());
                stream = track.getStream();
                stream->setRange(range->getStartByte(), range->getEndByte());
                QVERIFY(stream != Q_NULLPTR);
                transcodeTimer.start();
                timeToOpenTranscoding = 0;
                transcodedBytes = 0;
                QVERIFY(stream->open() == true);
                QVERIFY(stream->isOpen() == true);
                QVERIFY2(stream->size() == 5001, QString("%1").arg(stream->size()).toUtf8().constData());
                QVERIFY(stream->atEnd() == false);

                QScopedPointer<StreamingFile> transcodeProcess(qobject_cast<StreamingFile*>(stream));
                connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
                connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
                connect(transcodeProcess.data(), SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
                QVERIFY(transcodeProcess != Q_NULLPTR);
                QVERIFY(transcodeProcess->atEnd() == false);
                emit startTranscoding();
                QVERIFY(transcodeProcess->waitForFinished(-1) == true);
                qint64 duration = transcodeTimer.elapsed();
                QVERIFY2(timeToOpenTranscoding < 100, QString("%1").arg(timeToOpenTranscoding).toUtf8());
                QVERIFY2(duration < 5000, QString("%1").arg(duration).toUtf8());
                qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
                QVERIFY(transcodeProcess->bytesAvailable() == 0);
                QVERIFY2(transcodedBytes == 5001, QString("%1").arg(transcodedBytes).toUtf8().constData());
                QVERIFY(transcodeProcess->pos() == 5001);
                QVERIFY(transcodeProcess->atEnd() == true);
                QVERIFY(track.size() > transcodedBytes);
            }

            {
                QScopedPointer<HttpRange>range(new HttpRange("RANGE: BYTES=1000-5000"));
                range->setSize(track.size());
                stream = track.getStream();
                stream->setRange(range->getStartByte(), range->getEndByte());
                QVERIFY(stream != Q_NULLPTR);
                transcodeTimer.start();
                timeToOpenTranscoding = 0;
                transcodedBytes = 0;
                QVERIFY(stream->open() == true);
                QVERIFY(stream->isOpen() == true);
                QVERIFY2(stream->size() == 4001, QString("%1").arg(stream->size()).toUtf8().constData());
                QVERIFY(stream->atEnd() == false);

                QScopedPointer<StreamingFile> transcodeProcess(qobject_cast<StreamingFile*>(stream));
                connect(this, SIGNAL(startTranscoding()), transcodeProcess.data(), SLOT(startRequestData()));
                connect(transcodeProcess.data(), SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
                connect(transcodeProcess.data(), SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
                QVERIFY(transcodeProcess != Q_NULLPTR);
                QVERIFY(transcodeProcess->atEnd() == false);
                emit startTranscoding();
                QVERIFY(transcodeProcess->waitForFinished(-1) == true);
                qint64 duration = transcodeTimer.elapsed();
                QVERIFY2(timeToOpenTranscoding < 100, QString("%1").arg(timeToOpenTranscoding).toUtf8());
                QVERIFY2(duration < 5000, QString("%1").arg(duration).toUtf8());
                qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
                QVERIFY(transcodeProcess->bytesAvailable() == 0);
                QVERIFY2(transcodedBytes == 4001, QString("%1").arg(transcodedBytes).toUtf8().constData());
                QCOMPARE(transcodeProcess->pos(), 4001);
                QVERIFY(transcodeProcess->atEnd() == true);
                QVERIFY(track.size() > transcodedBytes);
            }
        }
    }
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_MP3_with_image() {

    {
        DlnaMusicTrackFile track("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/16 Funk Ad.mp3");
        track.setHostUrl(QUrl("http://host:600"));

        QStringList sinkProtocol;
        sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
        track.setDlnaProfiles(m_dlnaProfiles);
        track.setSinkProtocol(sinkProtocol);

        QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/16 Funk Ad.mp3");

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

        QDomDocument xml_res;
        xml_res.appendChild(track.getXmlContentDirectory(&xml_res, properties));
        QVERIFY(xml_res.childNodes().size() == 1);
        QVERIFY(xml_res.elementsByTagName("item").size() == 1);
        QDomNode node = xml_res.elementsByTagName("item").at(0);
        QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
        QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
        QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
        QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
        QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "Funk Ad");
        QVERIFY(xml_res.elementsByTagName("upnp:album").size() == 1);
        QVERIFY(xml_res.elementsByTagName("upnp:album").at(0).firstChild().nodeValue() == "Homework");
        QVERIFY(xml_res.elementsByTagName("upnp:artist").size() == 1);
        QVERIFY(xml_res.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue() == "Daft Punk");
        QVERIFY(xml_res.elementsByTagName("dc:contributor").size() == 1);
        QVERIFY(xml_res.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue() == "Daft Punk");
        QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
        QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "House");
        QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").size() == 1);
        QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue() == "16");
        QVERIFY(xml_res.elementsByTagName("dc:date").size() == 1);
        QVERIFY(xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue() == "2013-01-02");
        QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
        QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.audioItem.musicTrack");
        QVERIFY(xml_res.elementsByTagName("res").size() == 1);
        QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
        QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//16%20Funk%20Ad.mp3");
        QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 5);
        QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000");
        QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
        QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "845029");
        QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:00:51", xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue().toUtf8());
        QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "16000", xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue().toUtf8());
        xml_res.clear();

        QVERIFY(track.mimeType() == "audio/mpeg");
        QVERIFY(track.format() == MP3);
        QVERIFY(track.size() == 845029);
        QVERIFY(track.bitrate() == 128000);
        QVERIFY2(track.getLengthInSeconds() == 51, QString("%1").arg(track.getLengthInSeconds()).toUtf8());
        QVERIFY2(track.getLengthInMilliSeconds() == 51085, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
        QVERIFY(track.samplerate() == 44100);
        QVERIFY(track.channelCount() == 2);
        QVERIFY(track.toTranscode() == false);

        QVERIFY(track.getdlnaOrgOpFlags() == "01");
        QVERIFY(track.getdlnaOrgPN() == "MP3");
        QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000");
        QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000");

        QVERIFY(track.getAlbumArt().isNull() == false);
        QCOMPARE(track.getAlbumArt().size().width(), 300);
        QCOMPARE(track.getAlbumArt().size().height(), 300);
        int size = track.getByteAlbumArt().size();
        QVERIFY2(size == 16554, QString("%1").arg(size).toUtf8());

//        QHash<QString, double> result = track.volumeInfo();
        //    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
        //    QVERIFY2(result["n_samples"] == 4515840, QString("%1").arg(result["n_samples"]).toUtf8());
        //    QVERIFY2(result["mean_volume"] == -15.7, QString("%1").arg(result["mean_volume"]).toUtf8());
        //    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
        //    QVERIFY2(result["histogram_0db"] == 18999, QString("%1").arg(result["histogram_0db"]).toUtf8());

        {
            Device* stream(track.getStream());
            QVERIFY(stream != Q_NULLPTR);
            if (stream)
            {
                QVERIFY(stream->open() == true);
                QVERIFY(stream->isOpen() == true);
                QVERIFY(stream->size() == 845029);
                QCOMPARE(track.size(), stream->size());
                QVERIFY(stream->atEnd() == false);
            }
        }
    }
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_MP3_unicode() {

    {
        DlnaMusicTrackFile track("/Users/doudou/Music/iTunes/iTunes Media/Music/-M-/Mister Mystère/1-02 Phébus.mp3");
        track.setHostUrl(QUrl("http://host:600"));

        QStringList sinkProtocol;
        sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
        track.setDlnaProfiles(m_dlnaProfiles);
        track.setSinkProtocol(sinkProtocol);

        QVERIFY(track.getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music/-M-/Mister Mystère/1-02 Phébus.mp3");

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

        QDomDocument xml_res;
        xml_res.appendChild(track.getXmlContentDirectory(&xml_res, properties));
        QVERIFY(xml_res.childNodes().size() == 1);
        QVERIFY(xml_res.elementsByTagName("item").size() == 1);
        QDomNode node = xml_res.elementsByTagName("item").at(0);
        QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
        QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
        QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
        QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
        QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "Phébus");
        QVERIFY(xml_res.elementsByTagName("upnp:album").size() == 1);
        QVERIFY(xml_res.elementsByTagName("upnp:album").at(0).firstChild().nodeValue() == "Mister Mystère");
        QVERIFY(xml_res.elementsByTagName("upnp:artist").size() == 1);
        QVERIFY(xml_res.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue() == "-M-");
        QVERIFY(xml_res.elementsByTagName("dc:contributor").size() == 1);
        QVERIFY(xml_res.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue() == "-M-");
        QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
        QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "Rock  Français");
        QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").size() == 1);
        QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue() == "2");
        QVERIFY(xml_res.elementsByTagName("dc:date").size() == 1);
        QVERIFY(xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue() == "2013-01-02");
        QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
        QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.audioItem.musicTrack");
        QVERIFY(xml_res.elementsByTagName("res").size() == 1);
        QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
        QVERIFY2(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//1-02%20Phe%CC%81bus.mp3", xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue().toUtf8().constData());
        QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 5);
        QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000");
        QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
        QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "3841064");
        QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:02:39");
        QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "23657", QString("%1").arg(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue()).toUtf8());
        xml_res.clear();

        QVERIFY(track.mimeType() == "audio/mpeg");
        QVERIFY(track.format() == MP3);
        QVERIFY(track.size() == 3841064);
        QVERIFY2(track.bitrate() == 189255, QString("%1").arg(track.bitrate()).toUtf8());
        QVERIFY(track.getLengthInSeconds() == 159);
        QVERIFY2(track.getLengthInMilliSeconds() == 159373, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
        QVERIFY(track.samplerate() == 44100);
        QVERIFY(track.channelCount() == 2);
        QVERIFY(track.toTranscode() == false);

        QVERIFY(track.getdlnaOrgOpFlags() == "01");
        QVERIFY(track.getdlnaOrgPN() == "MP3");
        QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000");
        QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=A1100000000000000000000000000000");

        QVERIFY(track.getAlbumArt().isNull() == false);
        QCOMPARE(track.getAlbumArt().size().width(), 300);
        QCOMPARE(track.getAlbumArt().size().height(), 300);
        QCOMPARE(track.getByteAlbumArt().size(), 5357);

//        QHash<QString, double> result = track.volumeInfo();
        //    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
        //    QVERIFY2(result["n_samples"] == 14052024, QString("%1").arg(result["n_samples"]).toUtf8());
        //    QVERIFY2(result["mean_volume"] == -12.7, QString("%1").arg(result["mean_volume"]).toUtf8());
        //    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
        //    QVERIFY2(result["histogram_0db"] == 25653, QString("%1").arg(result["histogram_0db"]).toUtf8());

        {
            Device* stream(track.getStream());
            QVERIFY(stream != Q_NULLPTR);
            if (stream)
            {
                QVERIFY(stream->open() == true);
                QVERIFY(stream->isOpen() == true);
                QVERIFY(stream->size() == 3841064);
                QCOMPARE(track.size(), stream->size());
                QVERIFY(stream->atEnd() == false);
            }
        }
    }
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_MP3_Trancoding_LPCM()
{
    DlnaMusicTrackFile track("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/07 On_Off.mp3");
    track.setHostUrl(QUrl("http://host:600"));
    track.setTranscodeFormat(LPCM_S16BE);

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:audio/L16:DLNA.ORG_PN=LPCM";
    track.setDlnaProfiles(m_dlnaProfiles);
    track.setSinkProtocol(sinkProtocol);

    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/07 On_Off.mp3");

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

    QDomDocument xml_res;
    xml_res.appendChild(track.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    QDomNode node = xml_res.elementsByTagName("item").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "On/Off");
    QVERIFY(xml_res.elementsByTagName("upnp:album").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:album").at(0).firstChild().nodeValue() == "Human After All");
    QVERIFY(xml_res.elementsByTagName("upnp:artist").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue() == "Daft Punk");
    QVERIFY(xml_res.elementsByTagName("dc:contributor").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue() == "Daft Punk");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "House");
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue() == "7");
    QVERIFY(xml_res.elementsByTagName("dc:date").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue() == "2013-01-02");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.audioItem.musicTrack");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY2(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//07%20On_Off.mp3", xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue().toUtf8().constData());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 5);
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/L16;rate=44100;channels=2:DLNA.ORG_PN=LPCM;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000", xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QCOMPARE(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue(), QString("3419018"));
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:00:19", xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue().toUtf8());
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "176400", xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue().toUtf8());
    xml_res.clear();

    QCOMPARE(track.mimeType(), "audio/L16;rate=44100;channels=2");
    QVERIFY(track.format() == LPCM_S16BE);
    QVERIFY(track.size() == 3419018);
    QVERIFY2(track.bitrate() == 1411200, QString("%1").arg(track.bitrate()).toUtf8());
    QVERIFY(track.getLengthInSeconds() == 19);
    QVERIFY2(track.getLengthInMilliSeconds() == 19382, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 44100);
    QVERIFY(track.channelCount() == 2);
    QVERIFY(track.toTranscode() == true);

    QVERIFY(track.getdlnaOrgOpFlags() == "10");
    QVERIFY(track.getdlnaOrgPN() == "LPCM");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=LPCM;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/L16;rate=44100;channels=2:DLNA.ORG_PN=LPCM;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    QVERIFY(track.getAlbumArt().isNull() == true);
    QVERIFY(track.getByteAlbumArt().isNull() == true);

//    QHash<QString, double> result = track.volumeInfo();
    //    QVERIFY2(result.keys().size() == 5, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    //    QVERIFY2(result["n_samples"] == 1705200, QString("%1").arg(result["n_samples"]).toUtf8());
    //    QVERIFY2(result["mean_volume"] == -21.3, QString("%1").arg(result["mean_volume"]).toUtf8());
    //    QVERIFY2(result["max_volume"] == -8, QString("%1").arg(result["max_volume"]).toUtf8());
    //    QVERIFY2(result["histogram_7db"] == 1, QString("%1").arg(result["histogram_7db"]).toUtf8());
    //    QVERIFY2(result["histogram_8db"] == 3518, QString("%1").arg(result["histogram_8db"]).toUtf8());

    transcodedBytes = 0;
    Device *device = track.getStream();
    QVERIFY(device != Q_NULLPTR);
    QCOMPARE(track.size(), device->size());
    transcodeTimer.start();
    timeToOpenTranscoding = 0;

    auto transcodeProcess = qobject_cast<QFfmpegTranscoding*>(device);
    connect(this, SIGNAL(startTranscoding()), transcodeProcess, SLOT(startRequestData()));
    connect(transcodeProcess, SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
    connect(transcodeProcess, SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
    connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
    connect(this, SIGNAL(requestData(qint64)), transcodeProcess, SLOT(requestData(qint64)));
    connect(transcodeProcess, SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
    QVERIFY(transcodeProcess != Q_NULLPTR);

    if (transcodeProcess)
    {
        QVERIFY(transcodeProcess->atEnd() == true);
        QVERIFY(transcodeProcess->open() == true);

        transcodeProcess->moveToThread(m_streamingThread);
        emit startTranscoding();
        QVERIFY(transcodeProcess->waitForFinished(-1) == true);

        qint64 duration = transcodeTimer.elapsed();
        QVERIFY2(timeToOpenTranscoding < 100, QString("%1").arg(timeToOpenTranscoding).toUtf8());
        QVERIFY2(duration < 6000, QString("%1").arg(duration).toUtf8());
        qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
        QCOMPARE(transcodeProcess->error(), QString());
        QCOMPARE(transcodeProcess->exitCode(), 0);
        QVERIFY2(transcodeProcess->bytesAvailable() == 0, QString("%1").arg(transcodeProcess->bytesAvailable()).toUtf8());
        QVERIFY2(transcodedBytes == 3410400, QString("%1").arg(transcodedBytes).toUtf8().constData());
        QVERIFY(transcodeProcess->atEnd() == true);
        QVERIFY(track.size() > transcodedBytes);
    }

    // delete stream because deleted by deletelater
    device = track.getStream();
    delete device;
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_AAC_Transcoding_MP3()
{
    DlnaMusicTrackFile track("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/01 Monde virtuel.m4a");
    track.setHostUrl(QUrl("http://host:600"));
    track.setTranscodeFormat(MP3);

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
    track.setDlnaProfiles(m_dlnaProfiles);
    track.setSinkProtocol(sinkProtocol);

    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/01 Monde virtuel.m4a");

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

    QDomDocument xml_res;
    xml_res.appendChild(track.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    QDomNode node = xml_res.elementsByTagName("item").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "Monde virtuel");
    QVERIFY(xml_res.elementsByTagName("upnp:album").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:album").at(0).firstChild().nodeValue() == "Je dis aime");
    QVERIFY(xml_res.elementsByTagName("upnp:artist").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue() == "-M-");
    QVERIFY(xml_res.elementsByTagName("dc:contributor").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue() == "-M-");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "Pop");
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue() == "1");
    QVERIFY(xml_res.elementsByTagName("dc:date").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue() == "2013-01-02");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.audioItem.musicTrack");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//01%20Monde%20virtuel.m4a");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 5);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "7561480", xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:03:09");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "40000");
    xml_res.clear();

    QVERIFY(track.mimeType() == "audio/mpeg");
    QVERIFY(track.format() == MP3);
    QVERIFY(track.size() == 7561480);
    QVERIFY(track.bitrate() == 320000);
    QVERIFY(track.getLengthInSeconds() == 189);
    QVERIFY2(track.getLengthInMilliSeconds() == 188987, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 44100);
    QVERIFY(track.channelCount() == 2);
    QVERIFY(track.toTranscode() == true);

    QVERIFY(track.getdlnaOrgOpFlags() == "10");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    QVERIFY(track.getAlbumArt().isNull() == false);
    QVERIFY(track.getByteAlbumArt().isNull() == false);
    QVERIFY(track.getAlbumArt().size().width() == 300);
    QVERIFY(track.getAlbumArt().size().height() == 300);
    int size = track.getByteAlbumArt().size();
    QVERIFY2(size == 22040, QString("%1").arg(size).toUtf8());

    //    QHash<QString, double> result = track.volumeInfo();
    //    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    //    QVERIFY2(result["n_samples"] == 16664448, QString("%1").arg(result["n_samples"]).toUtf8());
    //    QVERIFY2(result["mean_volume"] == -12.5, QString("%1").arg(result["mean_volume"]).toUtf8());
    //    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
    //    QVERIFY2(result["histogram_0db"] == 28983, QString("%1").arg(result["histogram_0db"]).toUtf8());

    transcodedBytes = 0;
    Device *device = track.getStream();
    QVERIFY(device != Q_NULLPTR);
    QCOMPARE(track.size(), device->size());
    transcodeTimer.start();
    timeToOpenTranscoding = 0;

    auto transcodeProcess = qobject_cast<QFfmpegTranscoding*>(device);
    connect(this, SIGNAL(startTranscoding()), transcodeProcess, SLOT(startRequestData()));
    connect(transcodeProcess, SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
    connect(transcodeProcess, SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
    connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
    connect(this, SIGNAL(requestData(qint64)), transcodeProcess, SLOT(requestData(qint64)));
    connect(transcodeProcess, SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
    QVERIFY(transcodeProcess != Q_NULLPTR);

    if (transcodeProcess)
    {
        QVERIFY(transcodeProcess->atEnd() == true);
        QVERIFY(transcodeProcess->open() == true);
        transcodeProcess->moveToThread(m_streamingThread);
        emit startTranscoding();
        QVERIFY(transcodeProcess->waitForFinished(-1) == true);
        qint64 duration = transcodeTimer.elapsed();
        QVERIFY2(timeToOpenTranscoding < 100, QString("%1").arg(timeToOpenTranscoding).toUtf8());
        QVERIFY2(duration < 6100, QString("%1").arg(duration).toUtf8());
        qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
        QCOMPARE(transcodeProcess->error(), QString());
        QVERIFY(transcodeProcess->exitCode() == 0);
        QVERIFY(transcodeProcess->bytesAvailable() == 0);
        QVERIFY2(transcodedBytes == 7559880, QString("%1").arg(transcodedBytes).toUtf8().constData());
        QVERIFY(transcodeProcess->atEnd() == true);
        QVERIFY(track.size() > transcodedBytes);
    }

    // delete stream because deleted by deletelater
    device = track.getStream();
    delete device;
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_WAV_Transcoding_MP3()
{
    DlnaMusicTrackFile track("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");
    track.setHostUrl(QUrl("http://host:600"));
    track.setTranscodeFormat(MP3);

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
    track.setDlnaProfiles(m_dlnaProfiles);
    track.setSinkProtocol(sinkProtocol);

    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");

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

    QDomDocument xml_res;
    xml_res.appendChild(track.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    QDomNode node = xml_res.elementsByTagName("item").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY2(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "test", xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue().toUtf8().constData());
    QVERIFY(xml_res.elementsByTagName("upnp:album").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:album").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:artist").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("dc:contributor").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").size() == 1);
    QVERIFY2(xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue() == "0", xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("dc:date").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue() == "2013-08-27");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.audioItem.musicTrack");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//test.wav");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 7);
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "21787040", xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:09:05");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "40000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "48000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2");
    xml_res.clear();

    QVERIFY(track.mimeType() == "audio/mpeg");
    QVERIFY(track.format() == MP3);
    QVERIFY(track.size() == 21787040);
    QVERIFY(track.bitrate() == 320000);
    QVERIFY(track.getLengthInSeconds() == 545);
    QVERIFY2(track.getLengthInMilliSeconds() == 544626, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 48000);
    QVERIFY(track.channelCount() == 2);

    //    QHash<QString, double> result = track.volumeInfo();
    //    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    //    QVERIFY2(result["n_samples"] == 52284160, QString("%1").arg(result["n_samples"]).toUtf8());
    //    QVERIFY2(result["mean_volume"] == -12.1, QString("%1").arg(result["mean_volume"]).toUtf8());
    //    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
    //    QVERIFY2(result["histogram_0db"] == 300058, QString("%1").arg(result["histogram_0db"]).toUtf8());

    QVERIFY(track.getdlnaOrgOpFlags() == "10");
    QVERIFY(track.getdlnaOrgPN() == "MP3");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    Device *device = track.getStream();

    QVERIFY(device != Q_NULLPTR);
    QCOMPARE(track.size(), device->size());
    transcodeTimer.start();
    timeToOpenTranscoding = 0;

    auto transcodeProcess = qobject_cast<QFfmpegTranscoding*>(device);
    QVERIFY(transcodeProcess != Q_NULLPTR);

    if (transcodeProcess)
    {
        QVERIFY(transcodeProcess->atEnd()==true);
        QVERIFY(transcodeProcess->open() == true);
        transcodedBytes = 0;
        connect(this, SIGNAL(startTranscoding()), transcodeProcess, SLOT(startRequestData()));
        connect(transcodeProcess, SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
        connect(transcodeProcess, SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
        connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
        connect(this, SIGNAL(requestData(qint64)), transcodeProcess, SLOT(requestData(qint64)));
        connect(transcodeProcess, SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
        transcodeProcess->moveToThread(m_streamingThread);
        emit startTranscoding();
        transcodeProcess->waitForFinished(-1);
        qint64 duration = transcodeTimer.elapsed();
        QVERIFY2(timeToOpenTranscoding < 110, QString("%1").arg(timeToOpenTranscoding).toUtf8());
        QVERIFY2(duration < 20111, QString("%1").arg(duration).toUtf8());
        qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
        QVERIFY(transcodeProcess->exitCode() == 0);
        QCOMPARE(transcodeProcess->bytesAvailable(), 0);
        QVERIFY2(transcodedBytes == 21787245, QString("%1").arg(transcodedBytes).toUtf8().constData());
        QVERIFY(transcodeProcess->atEnd()==true);
        QVERIFY2(track.size() > transcodedBytes, QString("%1 %2").arg(track.size()).arg(transcodedBytes).toUtf8());
    }

    // delete stream because deleted by deletelater
    device = track.getStream();
    delete device;
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_WAV_Transcoding_LPCM()
{
    DlnaMusicTrackFile track("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");
    track.setHostUrl(QUrl("http://host:600"));

    track.setTranscodeFormat(MP3);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");

    track.setTranscodeFormat(LPCM_S16BE);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:audio/L16;rate=48000;channels=2:DLNA.ORG_PN=LPCM";
    track.setDlnaProfiles(m_dlnaProfiles);
    track.setSinkProtocol(sinkProtocol);

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

    QDomDocument xml_res;
    xml_res.appendChild(track.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    QDomNode node = xml_res.elementsByTagName("item").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "test");
    QVERIFY(xml_res.elementsByTagName("upnp:album").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:album").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:artist").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("dc:contributor").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue() == "0");
    QVERIFY(xml_res.elementsByTagName("dc:date").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue() == "2013-08-27");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.audioItem.musicTrack");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//test.wav");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 7);
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/L16;rate=48000;channels=2:DLNA.ORG_PN=LPCM;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000", xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "104569237", xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:09:05");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "192000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "48000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2");
    xml_res.clear();

    QVERIFY2(track.mimeType() == "audio/L16;rate=48000;channels=2", track.mimeType().toUtf8());
    QVERIFY(track.format() == LPCM_S16BE);
    QVERIFY(track.size() == 104569237);
    QVERIFY(track.bitrate() == 1536000);
    QVERIFY(track.getLengthInSeconds() == 545);
    QVERIFY2(track.getLengthInMilliSeconds() == 544626, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 48000);
    QVERIFY(track.channelCount() == 2);

    //    QHash<QString, double> result = track.volumeInfo();
    //    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    //    QVERIFY2(result["n_samples"] == 52284160, QString("%1").arg(result["n_samples"]).toUtf8());
    //    QVERIFY2(result["mean_volume"] == -12.1, QString("%1").arg(result["mean_volume"]).toUtf8());
    //    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
    //    QVERIFY2(result["histogram_0db"] == 300058, QString("%1").arg(result["histogram_0db"]).toUtf8());

    QVERIFY(track.getdlnaOrgOpFlags() == "10");
    QVERIFY(track.getdlnaOrgPN() == "LPCM");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=LPCM;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/L16;rate=48000;channels=2:DLNA.ORG_PN=LPCM;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    Device *device = track.getStream();
    QVERIFY(device != Q_NULLPTR);
    QCOMPARE(track.size(), device->size());
    transcodeTimer.start();
    timeToOpenTranscoding = 0;

    auto transcodeProcess = qobject_cast<QFfmpegTranscoding*>(device);
    QVERIFY(transcodeProcess != Q_NULLPTR);

    if (transcodeProcess)
    {
        transcodedBytes = 0;
        QVERIFY(transcodeProcess->atEnd()==true);
        QVERIFY(transcodeProcess->open() == true);
        connect(this, SIGNAL(startTranscoding()), transcodeProcess, SLOT(startRequestData()));
        connect(transcodeProcess, SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
        connect(transcodeProcess, SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
        connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
        connect(this, SIGNAL(requestData(qint64)), transcodeProcess, SLOT(requestData(qint64)));
        connect(transcodeProcess, SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
        transcodeProcess->moveToThread(m_streamingThread);
        emit startTranscoding();
        transcodeProcess->waitForFinished(-1);
        qint64 duration = transcodeTimer.elapsed();
        QVERIFY2(timeToOpenTranscoding < 100, QString("%1").arg(timeToOpenTranscoding).toUtf8());
        if (duration > 700)
            qWarning() << "duration > 700 :" << duration;
        qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
        QVERIFY(transcodeProcess->exitCode() == 0);
        QVERIFY(transcodeProcess->bytesAvailable() == 0);
        QVERIFY2(transcodedBytes == 104568320, QString("%1").arg(transcodedBytes).toUtf8());
        QVERIFY(transcodeProcess->atEnd()==true);
        QVERIFY(track.size() > transcodedBytes);
    }

    // delete stream because deleted by deletelater
    device = track.getStream();
    delete device;
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_WAV_Transcoding_AAC()
{
    DlnaMusicTrackFile track("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");
    track.setHostUrl(QUrl("http://host:600"));
    track.setTranscodeFormat(AAC);

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO";
    track.setDlnaProfiles(m_dlnaProfiles);
    track.setSinkProtocol(sinkProtocol);

    QVERIFY(track.toTranscode()==true);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");

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

    QDomDocument xml_res;
    xml_res.appendChild(track.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    QDomNode node = xml_res.elementsByTagName("item").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "test");
    QVERIFY(xml_res.elementsByTagName("upnp:album").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:album").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:artist").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("dc:contributor").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue() == "0");
    QVERIFY(xml_res.elementsByTagName("dc:date").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue() == "2013-08-27");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.audioItem.musicTrack");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//test.wav");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 7);
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000", xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "22662977", xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:09:05");
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "40000", xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "48000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2");
    xml_res.clear();

    QVERIFY(track.mimeType() == "audio/mp4");
    QVERIFY(track.format() == AAC);
    QVERIFY(track.size() == 22662977);
    QVERIFY(track.bitrate() == 320000);
    QVERIFY(track.getLengthInSeconds() == 545);
    QVERIFY2(track.getLengthInMilliSeconds() == 544626, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 48000);
    QVERIFY(track.channelCount() == 2);

    //    QHash<QString, double> result = track.volumeInfo();
    //    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    //    QVERIFY2(result["n_samples"] == 52284160, QString("%1").arg(result["n_samples"]).toUtf8());
    //    QVERIFY2(result["mean_volume"] == -12.1, QString("%1").arg(result["mean_volume"]).toUtf8());
    //    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
    //    QVERIFY2(result["histogram_0db"] == 300058, QString("%1").arg(result["histogram_0db"]).toUtf8());

    QVERIFY(track.getdlnaOrgOpFlags() == "10");
    QCOMPARE(track.getdlnaOrgPN(), "AAC_ISO");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=AAC_ISO;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    Device *device = track.getStream();
    QVERIFY(device != Q_NULLPTR);
    QCOMPARE(track.size(), device->size());
    transcodeTimer.start();
    timeToOpenTranscoding = 0;

    auto transcodeProcess = qobject_cast<QFfmpegTranscoding*>(device);
    QVERIFY(transcodeProcess != Q_NULLPTR);

    if (transcodeProcess)
    {
        transcodedBytes = 0;
        QVERIFY(transcodeProcess->atEnd()==true);
        QVERIFY(transcodeProcess->open() == true);
        connect(this, SIGNAL(startTranscoding()), transcodeProcess, SLOT(startRequestData()));
        connect(transcodeProcess, SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
        connect(transcodeProcess, SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
        connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
        connect(this, SIGNAL(requestData(qint64)), transcodeProcess, SLOT(requestData(qint64)));
        connect(transcodeProcess, SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
        transcodeProcess->moveToThread(m_streamingThread);
        emit startTranscoding();
        transcodeProcess->waitForFinished(-1);
        qint64 duration = transcodeTimer.elapsed();
        QVERIFY2(timeToOpenTranscoding < 100, QString("%1").arg(timeToOpenTranscoding).toUtf8());
        QVERIFY2(duration < 20000, QString("%1").arg(duration).toUtf8());
        qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
        QCOMPARE(transcodeProcess->exitCode(), 0);
        QVERIFY(transcodeProcess->bytesAvailable() == 0);
        QVERIFY2(transcodedBytes == 21941819, QString("%1").arg(transcodedBytes).toUtf8());
        QVERIFY(transcodeProcess->atEnd()==true);
        QVERIFY(track.size() > transcodedBytes);
    }

    // delete stream because deleted by deletelater
    device = track.getStream();
    delete device;
}

void tst_dlnamusictrack::testCase_DlnaMusicTrack_WAV_Transcoding_ALAC()
{
    DlnaMusicTrackFile track("/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");
    track.setHostUrl(QUrl("http://host:600"));
    track.setTranscodeFormat(ALAC);

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO";
    track.setDlnaProfiles(m_dlnaProfiles);
    track.setSinkProtocol(sinkProtocol);

    QVERIFY(track.toTranscode()==true);
    QVERIFY(track.getSystemName() == "/Users/doudou/workspaceQT/DLNA_server/tests/AUDIO/test.wav");

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

    QDomDocument xml_res;
    xml_res.appendChild(track.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("item").size() == 1);
    QDomNode node = xml_res.elementsByTagName("item").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "test");
    QVERIFY(xml_res.elementsByTagName("upnp:album").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:album").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:artist").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:artist").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("dc:contributor").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:contributor").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:genre").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:genre").at(0).firstChild().nodeValue() == "");
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:originalTrackNumber").at(0).firstChild().nodeValue() == "0");
    QVERIFY(xml_res.elementsByTagName("dc:date").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:date").at(0).firstChild().nodeValue() == "2013-08-27");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.item.audioItem.musicTrack");
    QVERIFY(xml_res.elementsByTagName("res").size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("res").at(0).childNodes().at(0).nodeValue() == "http://host:600/get//test.wav");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().size() == 7);
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue() == "http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000", xml_res.elementsByTagName("res").at(0).attributes().namedItem("protocolInfo").nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("xmlns:dlna").nodeValue() == "urn:schemas-dlna-org:metadata-1-0/");
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue() == "116321656", xml_res.elementsByTagName("res").at(0).attributes().namedItem("size").nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("duration").nodeValue() == "00:09:05");
    QVERIFY2(xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue() == "192000", xml_res.elementsByTagName("res").at(0).attributes().namedItem("bitrate").nodeValue().toUtf8());
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("sampleFrequency").nodeValue() == "48000");
    QVERIFY(xml_res.elementsByTagName("res").at(0).attributes().namedItem("nrAudioChannels").nodeValue() == "2");
    xml_res.clear();

    QVERIFY(track.mimeType() == "audio/mp4");
    QVERIFY(track.format() == ALAC);
    QVERIFY(track.size() == 116321656);
    QVERIFY2(track.bitrate() == 1536000, QString("%1").arg(track.bitrate()).toUtf8());
    QVERIFY(track.getLengthInSeconds() == 545);
    QVERIFY2(track.getLengthInMilliSeconds() == 544626, QString("%1").arg(track.getLengthInMilliSeconds()).toUtf8());
    QVERIFY(track.samplerate() == 48000);
    QVERIFY(track.channelCount() == 2);

    //    QHash<QString, double> result = track.volumeInfo();
    //    QVERIFY2(result.keys().size() == 4, QString("%1").arg(QVariant::fromValue(result.keys()).toString()).toUtf8());
    //    QVERIFY2(result["n_samples"] == 52284160, QString("%1").arg(result["n_samples"]).toUtf8());
    //    QVERIFY2(result["mean_volume"] == -12.1, QString("%1").arg(result["mean_volume"]).toUtf8());
    //    QVERIFY2(result["max_volume"] == 0, QString("%1").arg(result["max_volume"]).toUtf8());
    //    QVERIFY2(result["histogram_0db"] == 300058, QString("%1").arg(result["histogram_0db"]).toUtf8());

    QVERIFY(track.getdlnaOrgOpFlags() == "10");
    QCOMPARE(track.getdlnaOrgPN(), "AAC_ISO");
    QVERIFY(track.getDlnaContentFeatures() == "DLNA.ORG_PN=AAC_ISO;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QVERIFY(track.getProtocolInfo() == "http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    Device *device = track.getStream();
    QVERIFY(device != Q_NULLPTR);
    QCOMPARE(track.size(), device->size());
    transcodeTimer.start();
    timeToOpenTranscoding = 0;

    auto transcodeProcess = qobject_cast<QFfmpegTranscoding*>(device);
    QVERIFY(transcodeProcess != Q_NULLPTR);

    if (transcodeProcess)
    {
        transcodedBytes = 0;
        QVERIFY(transcodeProcess->atEnd()==true);
        QVERIFY(transcodeProcess->open() == true);
        connect(this, SIGNAL(startTranscoding()), transcodeProcess, SLOT(startRequestData()));
        connect(transcodeProcess, SIGNAL(sendDataToClientSignal(QByteArray)), this, SLOT(receivedTranscodedData(QByteArray)));
        connect(transcodeProcess, SIGNAL(openedSignal()), this, SLOT(transcodingOpened()));
        connect(transcodeProcess, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
        connect(this, SIGNAL(requestData(qint64)), transcodeProcess, SLOT(requestData(qint64)));
        connect(transcodeProcess, SIGNAL(LogMessage(QString)), this, SLOT(LogMessage(QString)));
        transcodeProcess->moveToThread(m_streamingThread);
        emit startTranscoding();
        transcodeProcess->waitForFinished(-1);
        qint64 duration = transcodeTimer.elapsed();
        qInfo() << "Transcoding opened in" << timeToOpenTranscoding << "ms and finished in" << duration << "ms.";
        QVERIFY2(timeToOpenTranscoding < 100, QString("%1").arg(timeToOpenTranscoding).toUtf8());
        QVERIFY2(duration < 20000, QString("%1").arg(duration).toUtf8());
        QVERIFY2(transcodeProcess->exitCode() == 0, QString("%1").arg(transcodeProcess->exitCode()).toUtf8());
        QVERIFY(transcodeProcess->bytesAvailable() == 0);
        QVERIFY2(transcodedBytes == 62792391, QString("%1").arg(transcodedBytes).toUtf8());
        QVERIFY(transcodeProcess->atEnd()==true);
        QVERIFY2(track.size() > transcodedBytes, QString("%1 %2").arg(track.size()).arg(transcodedBytes).toUtf8());
    }

    // delete stream because deleted by deletelater
    device = track.getStream();
    delete device;
}
