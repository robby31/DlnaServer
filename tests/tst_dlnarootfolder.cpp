#include "tst_dlnarootfolder.h"

tst_dlnarootfolder::tst_dlnarootfolder(QObject *parent) :
    QObject(parent)
{
}

void tst_dlnarootfolder::init()
{
    m_dlnaProfiles = new Protocol("/Users/doudou/workspaceQT/DLNA_server/app/xml profiles/dlna_profiles.xml");
}

void tst_dlnarootfolder::cleanup()
{
    delete m_dlnaProfiles;

    DebugInfo::display_alive_objects();

    QVERIFY2(DlnaResource::objectCounter == 0, QString("memory leak detected, %1 DlnaResource objects.").arg(DlnaResource::objectCounter).toUtf8());

    QCOMPARE(QFfmpegMedia::objectCounter, 0);
    QCOMPARE(QFfmpegStream::objectCounter, 0);
    QCOMPARE(QFfmpegFrame::objectCounter, 0);
    QCOMPARE(QFfmpegCodec::objectCounter, 0);
    QCOMPARE(QFfmpegBuffer::objectCounter, 0);
}

void tst_dlnarootfolder::testCase_DlnaRootFolder()
{
    DlnaRootFolder rootFolder(this);
    QCOMPARE(rootFolder.getId(), "0");
    QCOMPARE(rootFolder.getName(), "root");
    QCOMPARE(rootFolder.getSystemName(), "root");
    QCOMPARE(rootFolder.getDisplayName(), "root");
    QCOMPARE(rootFolder.getDlnaParent(), Q_NULLPTR);
    QCOMPARE(rootFolder.getDlnaParentId(), "-1");
    QCOMPARE(rootFolder.getResourceId(), "0");
    QCOMPARE(rootFolder.isFolder(), true);
    QCOMPARE(rootFolder.getUpdateId(), 1);
    QCOMPARE(rootFolder.getChildrenSize(), 0);
    QCOMPARE(rootFolder.getChild(0), Q_NULLPTR);

    QStringList properties;
    properties << "dc:title";
    properties << "@childCount";

    QDomDocument xml_res;
    xml_res.appendChild(rootFolder.getXmlContentDirectory(&xml_res, properties));
    check_dlna_storage(xml_res, "0", "-1", 0, "root");
    xml_res.clear();

    DlnaFolder music("/Users/doudou/Music/iTunes/iTunes Media/Music");
    QCOMPARE(music.getName(), "Music");

    xml_res.appendChild(music.getXmlContentDirectory(&xml_res, properties));
    check_dlna_storage(xml_res, "", "-1", 610, "Music");
    xml_res.clear();

    rootFolder.addChild(&music);
    QCOMPARE(music.getId(), "1");
    QCOMPARE(music.getName(), "Music");
    QCOMPARE(music.getSystemName(), "/Users/doudou/Music/iTunes/iTunes Media/Music");
    QVERIFY(music.getDlnaParent() != Q_NULLPTR);
    QCOMPARE(music.getDlnaParent()->getResourceId(), "0");
    QCOMPARE(music.getResourceId(), "0$1");
    QCOMPARE(music.isFolder(), true);
    QVERIFY(rootFolder.getChild(0) != Q_NULLPTR);
    QCOMPARE(rootFolder.getChildrenSize(), 1);

    xml_res.appendChild(music.getXmlContentDirectory(&xml_res, properties));
    check_dlna_storage(xml_res, "0$1", "0", 610, "Music");
    xml_res.clear();

    rootFolder.addChild(&music);
    QCOMPARE(music.getId(), "2");
    QCOMPARE(music.getName(), "Music");
    QCOMPARE(music.getSystemName(), "/Users/doudou/Music/iTunes/iTunes Media/Music");
    QCOMPARE(music.getDlnaParent()->getResourceId(), "0");
    QCOMPARE(music.getResourceId(), "0$2");
    QCOMPARE(music.isFolder(), true);

    DlnaResource* found;
    found = rootFolder.search("0", "", true);
    QCOMPARE(found, &rootFolder);

    found = rootFolder.search("2", "", true);
    QCOMPARE(found, Q_NULLPTR);

    found = rootFolder.search("0$2", "", true);
    QCOMPARE(found, &music);

    found = rootFolder.search("0$0", "", true);
    QCOMPARE(found, Q_NULLPTR);

    found = rootFolder.search("0$2$1$1", "", true);
    QVERIFY(found != Q_NULLPTR);

    QList<DlnaResource*> list_found;
    list_found = rootFolder.getDLNAResources("0", false, 0, 10, "");
    QCOMPARE(list_found.isEmpty(), false);
    QCOMPARE(list_found.size(), 1);
    QCOMPARE(list_found.at(0)->getResourceId(), "0");

    list_found = rootFolder.getDLNAResources("0", true, 0, 10, "");
    QCOMPARE(list_found.isEmpty(), false);
    QCOMPARE(list_found.size(), 2);
    QCOMPARE(list_found.at(0)->getResourceId(), "0$2");
    QCOMPARE(list_found.at(1)->getResourceId(), "0$2");

    list_found = music.getDLNAResources("0$2", true, 0, 10, "");
    QCOMPARE(list_found.isEmpty(), false);
    list_found = music.getDLNAResources("0$2$1", true, 0, 10, "");
    QCOMPARE(list_found.isEmpty(), false);
    list_found = music.getDLNAResources("0$2$1$1", true, 0, 10, "");
    QCOMPARE(list_found.isEmpty(), false);
    auto item_found = qobject_cast<DlnaItem*>(list_found.at(0));

    QStringList sinkProtocol;
    sinkProtocol << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
    m_dlnaProfiles->setProtocols(sinkProtocol);
    item_found->setDlnaProfiles(m_dlnaProfiles);

    QCOMPARE(item_found->getSystemName(), "/Users/doudou/Music/iTunes/iTunes Media/Music/-M-/Je dis aime/01 Monde virtuel.m4a");
    QCOMPARE(item_found->getdlnaOrgOpFlags(), "10");
    QCOMPARE(item_found->getdlnaOrgPN(), "MP3");
    QCOMPARE(item_found->getDlnaContentFeatures(), "DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QCOMPARE(item_found->getProtocolInfo(), "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    item_found->setdlnaOrgPN(QString());
    QCOMPARE(item_found->getdlnaOrgOpFlags(), "10");
    QCOMPARE(item_found->getdlnaOrgPN(), "");
    QCOMPARE(item_found->getDlnaContentFeatures(), "DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");
    QCOMPARE(item_found->getProtocolInfo(), "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1;DLNA.ORG_FLAGS=C1100000000000000000000000000000");

    rootFolder.addFolder("/Users/doudou/Movies");
    QThreadPool::globalInstance()->waitForDone();
    QVERIFY(rootFolder.getChild(0) != Q_NULLPTR);
    QCOMPARE(rootFolder.getChildrenSize(), 3);

    found = rootFolder.search("0$3", "", true);
    QVERIFY(found != Q_NULLPTR);
    if (found)
        QCOMPARE(found->getSystemName(), "/Users/doudou/Movies");
}
