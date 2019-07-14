#include "tst_dlnafolder.h"

tst_dlnafolder::tst_dlnafolder(QObject *parent) :
    QObject(parent),
    m_dlnaProfiles("/Users/doudou/workspaceQT/DLNA_server/app/xml profiles/dlna_profiles.xml")
{
}

void tst_dlnafolder::cleanup()
{
    QVERIFY2(DlnaResource::objectCounter == 0, QString("memory leak detected, %1 DlnaResource objects.").arg(DlnaResource::objectCounter).toUtf8());

     QCOMPARE(QFfmpegMedia::objectCounter, 0);
     QCOMPARE(QFfmpegStream::objectCounter, 0);
     QCOMPARE(QFfmpegFrame::objectCounter, 0);
     QCOMPARE(QFfmpegCodec::objectCounter, 0);
     QCOMPARE(QFfmpegBuffer::objectCounter, 0);
}

void tst_dlnafolder::testCase_DlnaFolder()
{
    DlnaFolder music("/Users/doudou/Music/iTunes/iTunes Media/Music");
    QVERIFY(music.getId() == "");
    QVERIFY(music.getName() == "Music");
    QVERIFY(music.getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music");
    QVERIFY(music.getDisplayName() == "Music");
    QVERIFY(music.getDlnaParent() == Q_NULLPTR);
    QVERIFY(music.getDlnaParentId() == "-1");
    QVERIFY(music.getResourceId() == "");
    QVERIFY(music.isFolder() == true);
    QVERIFY(music.getUpdateId() == 1);
    QVERIFY(music.getChild(0) != Q_NULLPTR);
    QVERIFY(music.getChildrenSize() == 610);

    music.setId("0$1");
    QVERIFY(music.getId() == "0$1");

    QObject scope;
    QList<DlnaResource*> list_found;
    list_found = music.getDLNAResources("0$1", false, 0, 10, "", &scope);
    QVERIFY(list_found.isEmpty() == false);
    QVERIFY(list_found.size() == 1);

    list_found = music.getDLNAResources("0$1", true, 0, 10, "", &scope);
    QVERIFY(list_found.isEmpty() == false);
    QVERIFY2(list_found.size() == 10, QString("%1").arg(list_found.size()).toUtf8());
    QVERIFY2(list_found.at(0)->getResourceId() == "0$1$1", list_found.at(0)->getResourceId().toUtf8().constData());
    QVERIFY(list_found.at(0)->getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music/-M-");

    list_found = music.getDLNAResources("0$1$1", true, 0, 10, "", &scope);
    QVERIFY(list_found.isEmpty() == false);
    QVERIFY2(list_found.size() == 2, QString("%1").arg(list_found.size()).toUtf8());
    QVERIFY(list_found.at(0)->getResourceId() == "0$1$1$1");
    QVERIFY(list_found.at(0)->getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music/-M-/Je dis aime");

    list_found = music.getDLNAResources(QString(), true, 0, 10, "", &scope);
    QVERIFY(list_found.isEmpty() == true);

    list_found = music.getDLNAResources("", true, 0, 10, "", &scope);
    QVERIFY(list_found.isEmpty() == true);

    list_found = music.getDLNAResources("10000", true, 0, 10, "", &scope);
    QVERIFY(list_found.isEmpty() == true);
}

qint64 tst_dlnafolder::parseFolder(const QString& resourceId, DlnaResource *resource)
{
    QElapsedTimer timer;
    qint64 elapsed = 0;

    QList<DlnaResource*> l_child;
    for (int i = 0; i<3; i++)
    {
        QObject scope;
        timer.restart();
        l_child = resource->getDLNAResources(resourceId, true, 0, 608, "", &scope);
        foreach(DlnaResource* child, l_child)
        {
            auto item = qobject_cast<DlnaItem*>(child);
            if (item)
            {
                item->setHostUrl(QUrl("http://host:600"));

                QStringList sinkProtocol;
                sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
                sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3X";
                sinkProtocol << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA";
                sinkProtocol << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AAC_MULT5";
                sinkProtocol << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AC3";
                item->setDlnaProfiles(&m_dlnaProfiles);
                item->setSinkProtocol(sinkProtocol);

                item->getStringContentDirectory(QStringList("*"));
                Device * stream = item->getStream();
                delete stream;   /* remove stream which is destroyed with deletelater */
            }

        }

        qint64 tmp = timer.elapsed();
        if (tmp > elapsed) {
            elapsed = tmp;
        }
    }

    return elapsed;
}

void tst_dlnafolder::testCase_PerformanceAllArtists()
{
    DlnaFolder music("/Users/doudou/Music/iTunes/iTunes Media/Music");
    QVERIFY(music.getId() == "");
    QVERIFY(music.getName() == "Music");
    QVERIFY(music.getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music");
    QVERIFY(music.getDisplayName() == "Music");
    QVERIFY(music.getDlnaParent() == Q_NULLPTR);
    QVERIFY(music.getDlnaParentId() == "-1");
    QVERIFY(music.getResourceId() == "");
    QVERIFY(music.isFolder() == true);
    QVERIFY(music.getUpdateId() == 1);

    music.setId("0$1");
    QVERIFY(music.getId() == "0$1");

    qint64 duration = parseFolder("0$1", &music);
    if (duration <= 2500)
        qInfo() << "PERFO" << duration << music.getSystemName() << music.getChildrenSize() << "children";
    else
        qCritical() << "PERFO (>2500)" << duration << music.getSystemName() << music.getChildrenSize() << "children";
    QVERIFY(music.getChildrenSize() == 610);
}

void tst_dlnafolder::testCase_PerformanceAllAlbums()
{
    DlnaFolder music("/Users/doudou/Music/iTunes/iTunes Media/Music");
    QVERIFY(music.getId() == "");
    QVERIFY(music.getName() == "Music");
    QVERIFY(music.getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music");
    QVERIFY(music.getDisplayName() == "Music");
    QVERIFY(music.getDlnaParent() == Q_NULLPTR);
    QVERIFY(music.getDlnaParentId() == "-1");
    QVERIFY(music.getResourceId() == "");
    QVERIFY(music.isFolder() == true);
    QVERIFY(music.getUpdateId() == 1);

    music.setId("0$1");
    QVERIFY(music.getId() == "0$1");

    qint64 max = 0;
    for(int index=0;index<music.getChildrenSize();index++)
    {
        QScopedPointer<DlnaResource> artist(music.getChild(index));
        qint64 elapsed;
        elapsed = parseFolder(artist->getResourceId(), &music);
        if (elapsed > max)
        {
            max = elapsed;
            qInfo() << "PERFO" << elapsed << artist->getResourceId() << artist->getSystemName() << artist->getChildrenSize() << "children";
        }
    }

    if (max > 1100)
        qCritical() << "Parse all albums by artist in" << max << "ms.";
}

void tst_dlnafolder::testCase_PerformanceAllTracks()
{
    QElapsedTimer timer;
    timer.start();

    DlnaFolder music("/Users/doudou/Music/iTunes/iTunes Media/Music");
    QVERIFY(music.getId() == "");
    QVERIFY(music.getName() == "Music");
    QVERIFY(music.getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music");
    QVERIFY(music.getDisplayName() == "Music");
    QVERIFY(music.getDlnaParent() == Q_NULLPTR);
    QVERIFY(music.getDlnaParentId() == "-1");
    QVERIFY(music.getResourceId() == "");
    QVERIFY(music.isFolder() == true);
    QVERIFY(music.getUpdateId() == 1);

    music.setId("0$1");
    QVERIFY(music.getId() == "0$1");

    {
        ANALYZER_RESET;
        QScopedPointer<DlnaResource> album(music.search("0$1$27$2", "", true));
        qint64 elapsed = parseFolder(album->getResourceId(), &music);
        qInfo() << "PERFO" << album->getSystemName() << album->getChildrenSize() << QTime(0, 0).addMSecs(static_cast<int>(elapsed)).toString("hh:mm:ss.zzz");
        ANALYZER_DISPLAY_RESULTS;
    }

    ANALYZER_RESET;
    qint64 max = 0;
    for(int idxArtist=0;idxArtist<music.getChildrenSize();idxArtist++)
    {
        QScopedPointer<DlnaResource> artist(music.getChild(idxArtist));
        for(int idxAlbum=0;idxAlbum<artist->getChildrenSize();idxAlbum++)
        {
            QScopedPointer<DlnaResource> album(artist->getChild(idxAlbum));
            qint64 elapsed;
            elapsed = parseFolder(album->getResourceId(), &music);
            if (elapsed > max)
            {
                max = elapsed;
                qInfo() << "PERFO" << QTime(0, 0).addMSecs(static_cast<int>(elapsed)).toString("hh:mm:ss.zzz") << album->getResourceId() << album->getSystemName() << album->getChildrenSize() << "children";
            }
        }
    }
    ANALYZER_DISPLAY_RESULTS;

    qInfo() << "DURATION" << QTime(0, 0).addMSecs(static_cast<int>(timer.elapsed())).toString("hh:mm:ss.zzz");

    QVERIFY2(max < 13000, QString("Parse all tracks by album in %1 ms").arg(max).toUtf8());
}

void tst_dlnafolder::testCase_DlnaFolderPlaylist()
{
    QScopedPointer<DlnaNetworkPlaylist> playlist(new DlnaNetworkPlaylist(QUrl("https://www.ludo.fr/heros/ninjago")));

    QVERIFY(playlist != Q_NULLPTR);
    if (playlist)
    {
        QCOMPARE(playlist->getSystemName(), "ninjago");
        QCOMPARE(playlist->getChildrenSize(), 14);

        {
            QStringList properties;
            properties << "*";

            QDomDocument xml_res;
            xml_res.appendChild(playlist->getXmlContentDirectory(&xml_res, properties));
            check_dlna_storage(xml_res, "", "-1", playlist->getChildrenSize(), "ninjago");
        }

        qint64 elapsed = parseFolder(playlist->getResourceId(), playlist.data());
        qInfo() << "PERFO" << elapsed << playlist->getResourceId() << playlist->getSystemName() << playlist->getChildrenSize() << "children";

        QScopedPointer<DlnaNetworkVideo> video(qobject_cast<DlnaNetworkVideo*>(playlist->getChild(0)));
        QVERIFY(video != Q_NULLPTR);

        {
            QStringList properties;
            properties << "upnp:genre";
            properties << "res@size";
            properties << "res@duration";
            properties << "res@bitrate";
            properties << "res@resolution";
            properties << "res@nrAudioChannels";
            properties << "res@sampleFrequency";

            QDomDocument xml_res;
            xml_res.appendChild(video->getXmlContentDirectory(&xml_res, properties));
            check_dlna_video(xml_res,
                             "$1", "",
                             "S09 E01 Ninjago La m\u00E8re de tous les dragons",
                             "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AC3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000",
                             "00:21:06",
                             "1024x576",
                             2, 48000,
                             312500,
                             437277851,
                             "http://host:600/get/$1/S09%20E01%20Ninjago%20La%20m%C3%A8re%20de%20tous%20les%20dragons");
        }
    }
}
