#include "tst_dlnarootfolder.h"

tst_dlnarootfolder::tst_dlnarootfolder(QObject *parent) :
    QObject(parent)
{
}

void tst_dlnarootfolder::testCase_DlnaRootFolder()
{
    DlnaRootFolder rootFolder(this);
    QVERIFY(rootFolder.getId() == "0");
    QVERIFY(rootFolder.getName() == "root");
    QVERIFY(rootFolder.getSystemName() == "root");
    QVERIFY(rootFolder.getDisplayName() == "root");
    QVERIFY(rootFolder.getDlnaParent() == Q_NULLPTR);
    QVERIFY(rootFolder.getDlnaParentId() == "-1");
    QVERIFY(rootFolder.getResourceId() == "0");
    QVERIFY(rootFolder.isFolder() == true);
    QVERIFY(rootFolder.getUpdateId() == 1);
    QVERIFY(rootFolder.getChildrenSize() == 0);
    QVERIFY(rootFolder.getChild(0) == Q_NULLPTR);

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

    DlnaFolder music("/Users/doudou/Music/iTunes/iTunes Media/Music");
    QVERIFY(music.getName() == "Music");

    xml_res.appendChild(music.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("container").size() == 1);
    node = xml_res.elementsByTagName("container").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "-1");
    QVERIFY2(node.attributes().namedItem("childCount").nodeValue() == "609", QString("%1").arg(node.attributes().namedItem("childCount").nodeValue()).toUtf8().constData());
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
    QVERIFY(music.getDlnaParent() != Q_NULLPTR);
    QVERIFY(music.getDlnaParent()->getResourceId() == "0");
    QVERIFY(music.getResourceId() == "0$1");
    QVERIFY(music.isFolder() == true);
    QVERIFY(rootFolder.getChild(0) != Q_NULLPTR);
    QVERIFY(rootFolder.getChildrenSize() == 1);

    xml_res.appendChild(music.getXmlContentDirectory(&xml_res, properties));
    QVERIFY(xml_res.childNodes().size() == 1);
    QVERIFY(xml_res.elementsByTagName("container").size() == 1);
    node = xml_res.elementsByTagName("container").at(0);
    QVERIFY(node.attributes().namedItem("id").nodeValue() == "0$1");
    QVERIFY(node.attributes().namedItem("parentID").nodeValue() == "0");
    QVERIFY(node.attributes().namedItem("childCount").nodeValue() == "609");
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
    QVERIFY(music.getDlnaParent()->getResourceId() == "0");
    QVERIFY(music.getResourceId() == "0$2");
    QVERIFY(music.isFolder() == true);

    DlnaResource* found;
    found = rootFolder.search("0", "");
    QVERIFY(found == &rootFolder);

    found = rootFolder.search("2", "");
    QVERIFY(found == Q_NULLPTR);

    found = rootFolder.search("0$2", "");
    QVERIFY(found == &music);

    found = rootFolder.search("0$0", "");
    QVERIFY(found == Q_NULLPTR);

    found = rootFolder.search("0$2$1$1", "");
    QVERIFY(found != Q_NULLPTR);

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
    auto item_found = qobject_cast<DlnaItem*>(list_found.at(0));
    QVERIFY(item_found->getSystemName() == "/Users/doudou/Music/iTunes/iTunes Media/Music/-M-/Je dis aime/01 Monde virtuel.m4a");
    QVERIFY(item_found->getdlnaOrgOpFlags() == "10");
    QVERIFY(item_found->getdlnaOrgPN() == "MP3");
    QVERIFY(item_found->getDlnaContentFeatures() == "DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1");
    QVERIFY(item_found->getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=10;DLNA.ORG_CI=1");

    item_found->setdlnaOrgPN(QString());
    QVERIFY(item_found->getdlnaOrgOpFlags() == "10");
    QVERIFY(item_found->getdlnaOrgPN() == "");
    QVERIFY(item_found->getDlnaContentFeatures() == "DLNA.ORG_OP=10;DLNA.ORG_CI=1");
    QVERIFY(item_found->getProtocolInfo() == "http-get:*:audio/mpeg:DLNA.ORG_OP=10;DLNA.ORG_CI=1");

    rootFolder.addFolder("/Users/doudou/Movies");
    QThreadPool::globalInstance()->waitForDone();
    QVERIFY(rootFolder.getChild(0) != Q_NULLPTR);
    QVERIFY(rootFolder.getChildrenSize() == 3);

    found = rootFolder.search("0$3", "");
    QVERIFY(found != Q_NULLPTR);
    if (found)
        QVERIFY2(found->getSystemName() == "/Users/doudou/Movies", found->getSystemName().toUtf8());
}
