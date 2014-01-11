#include "tst_dlnarootfolder.h"

tst_dlnarootfolder::tst_dlnarootfolder(QObject *parent) :
    QObject(parent)
{
}

void tst_dlnarootfolder::testCase_DlnaRootFolder()
{
    Logger log;
    DlnaRootFolder rootFolder(&log, "host", 100);
    QVERIFY(rootFolder.getId() == "0");
    QVERIFY(rootFolder.getName() == "root");
    QVERIFY(rootFolder.getSystemName() == "root");
    QVERIFY(rootFolder.getDisplayName() == "root");
    QVERIFY(rootFolder.getParent() == 0);
    QVERIFY(rootFolder.getParentId() == "-1");
    QVERIFY(rootFolder.getResourceId() == "0");
    QVERIFY(rootFolder.isFolder() == true);
    QVERIFY(rootFolder.isDiscovered() == true);
    QVERIFY(rootFolder.getUpdateId() == 1);
    QVERIFY(rootFolder.getChildren().isEmpty() == true);
    QVERIFY(rootFolder.mimeType() == 0);
    QVERIFY(rootFolder.getdlnaOrgOpFlags() == "01");
    QVERIFY(rootFolder.getdlnaOrgPN() == "");

    QStringList properties;
    properties << "dc:title";
    properties << "@childCount";

    QDomDocument xml_res;
    xml_res.appendChild(rootFolder.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("container").size() == 1);
    QDomNode node = xml_res.elementsByTagName("container").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "0");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY(node.attributes().namedItem("childCount").nodeValue() == "0");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "root");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.container.storageFolder");
    xml_res.clear();

    DlnaFolder music(&log, "/Users/doudou/Music/iTunes/iTunes Media/Music", "host", 500);
    QVERIFY(music.getName() == "Music");

    xml_res.appendChild(music.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("container").size() == 1);
    node = xml_res.elementsByTagName("container").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY(node.attributes().namedItem("childCount").nodeValue() == "608");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "Music");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.container.storageFolder");
    xml_res.clear();

    rootFolder.addChild(&music);
    QVERIFY(music.getId() == "1");
    QVERIFY(music.getName() == "Music");
    QVERIFY(music.getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music");
    QVERIFY(music.getParent()->getResourceId() == "0");
    QVERIFY(music.getResourceId() == "0$1");
    QVERIFY(music.isFolder() == true);
    QVERIFY(rootFolder.getChildren().isEmpty() == false);
    QVERIFY(rootFolder.getChildren().size() == 1);
    QVERIFY(music.mimeType() == 0);
    QVERIFY(music.getdlnaOrgOpFlags() == "01");
    QVERIFY(music.getdlnaOrgPN() == "");

    xml_res.appendChild(music.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("container").size() == 1);
    node = xml_res.elementsByTagName("container").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "0$1");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "0");
    QVERIFY(node.attributes().namedItem("childCount").nodeValue() == "608");
    QVERIFY(node.attributes().namedItem("restricted").nodeValue() == "true");
    QVERIFY(xml_res.elementsByTagName("dc:title").size() == 1);
    QVERIFY(xml_res.elementsByTagName("dc:title").at(0).firstChild().nodeValue() == "Music");
    QVERIFY(xml_res.elementsByTagName("upnp:class").size() == 1);
    QVERIFY(xml_res.elementsByTagName("upnp:class").at(0).firstChild().nodeValue() == "object.container.storageFolder");
    xml_res.clear();

    rootFolder.addChild(&music);
    QVERIFY(music.getId() == "2");
    QVERIFY(music.getName() == "Music");
    QVERIFY(music.getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music");;
    QVERIFY(music.getParent()->getResourceId() == "0");
    QVERIFY(music.getResourceId() == "0$2");
    QVERIFY(music.isFolder() == true);
    QVERIFY(music.mimeType() == 0);

    DlnaResource* found;
    found = rootFolder.search("0", "");
    QVERIFY(found == &rootFolder);

    found = rootFolder.search("2", "");
    QVERIFY(found == 0);

    found = rootFolder.search("0$2", "");
    QVERIFY(found == &music);

    found = rootFolder.search("0$0", "");
    QVERIFY(found == 0);

    found = rootFolder.search("0$2$1$1", "");
    QVERIFY(found != 0);

    QList<DlnaResource*> list_found;
    list_found = rootFolder.getDLNAResources("0", false, 0, 10, "");
    QVERIFY(list_found.isEmpty() == false);
    QVERIFY(list_found.size() == 1);
    QVERIFY(list_found.at(0)->getResourceId() == "0");

    list_found = rootFolder.getDLNAResources("0", true, 0, 10, "");
    QVERIFY(list_found.isEmpty() == false);
    QVERIFY(list_found.size() == 2);
    QVERIFY(list_found.at(0)->getResourceId() == "0$2");
    QVERIFY(list_found.at(1)->getResourceId() == "0$2");

    list_found = music.getDLNAResources("0$2", true, 0, 10, "");
    QVERIFY(list_found.isEmpty() == false);
    list_found = music.getDLNAResources("0$2$1", true, 0, 10, "");
    QVERIFY(list_found.isEmpty() == false);
    list_found = music.getDLNAResources("0$2$1$1", true, 0, 10, "");
    QVERIFY(list_found.isEmpty() == false);
    QVERIFY(list_found.at(0)->getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music/-M-/Je dis aime/01 Monde virtuel.m4a");
    QVERIFY(list_found.at(0)->getdlnaOrgOpFlags() == "01");
    QVERIFY(list_found.at(0)->getdlnaOrgPN() == "MP3");
    QVERIFY(list_found.at(0)->getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(list_found.at(0)->getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01");

    list_found.at(0)->setdlnaOrgPN(QString());
    QVERIFY(list_found.at(0)->getdlnaOrgOpFlags() == "01");
    QVERIFY(list_found.at(0)->getdlnaOrgPN() == "");
    QVERIFY(list_found.at(0)->getDlnaContentFeatures() == "DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    QVERIFY(list_found.at(0)->getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_OP=01");

    int res;
    res = rootFolder.addFolder("/Users/doudou/Music/iTunes/iTunes Media/Video");
    QVERIFY(res == false);

    res = rootFolder.addFolder("/Users/doudou/Movies");
    QVERIFY(res == true);
    QVERIFY(rootFolder.getChildren().isEmpty() == false);
    QVERIFY(rootFolder.getChildren().size() == 3);

    found = rootFolder.search("0$3", "");
    QVERIFY(found != 0);
    QVERIFY2(found->getSystemName() == "/Users/doudou/Movies", found->getSystemName().toUtf8());

}
