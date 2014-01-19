#include "tst_request.h"
#include <QtXml>
#include <QTcpSocket>
#include <QApplication>

TestRequest::TestRequest()
{
}

void TestRequest::testCase_request()
{
    Logger log;
    DlnaRootFolder rootFolder(&log, "host", 400);
    QTcpSocket client;
    Request request(&log, &client, "uuid", "server name", "host IP", 56, &rootFolder);

    QVERIFY(request.getHighRange() == -1);
    QVERIFY(request.getLowRange() == -1);
    QVERIFY2(request.getHost() == "host IP", "Failure with function getHost()");
    QVERIFY(request.getArgument() == "");
    QVERIFY(request.getMethod() == "");
    QVERIFY(request.getpeerAddress() == "");
    QVERIFY(request.getStatus() == "init");
    QVERIFY(request.getTextHeader() == "");
    QVERIFY(request.getTextContent() == "");
    QVERIFY(request.isHttp10() == false);
    QVERIFY(request.getUserAgent() == "");
    QVERIFY(request.getReceivedContentLength() == -1);
    QVERIFY(request.getTransferMode() == "");
    QVERIFY(request.getContentFeatures() == "");

    QVERIFY(request.getSoapaction() == "");
    QVERIFY(request.appendHeader("CALLBACK: field_callback") == true);
    QVERIFY(request.getSoapaction() == "field_callback");

    QVERIFY(request.appendHeader("SOAPACTION: field_soapaction") == true);
    QVERIFY(request.getSoapaction() == "field_soapaction");

    QVERIFY(request.appendHeader("") == false);
    QVERIFY(request.appendHeader("METHOD") == false);

    QVERIFY(request.appendHeader("GET /get/0$4$1$9$2$11/thumbnail00001-11+Tout+sauf+toi.mp3 HTTP/1.0") == true);
    QVERIFY(request.getMethod() == "GET");
    QVERIFY(request.getArgument() == "get/0$4$1$9$2$11/thumbnail00001-11+Tout+sauf+toi.mp3");
    QVERIFY(request.isHttp10() == true);

    QVERIFY(request.appendHeader("RANGE: BYTES=10-500") == true);
    QVERIFY(request.getLowRange() == 10);
    QVERIFY(request.getHighRange() == 500);

    QVERIFY(request.appendHeader("User-Agent: DLNADOC/1.50") == true);
    QVERIFY(request.getUserAgent() == "DLNADOC/1.50");

    QVERIFY(request.appendHeader("CONTENT-LENGTH: 632") == true);
    QVERIFY(request.getReceivedContentLength() == 632);

    QVERIFY(request.appendHeader("Content-Length: 2122") == true);
    QVERIFY(request.getReceivedContentLength() == 2122);

    QVERIFY(request.appendHeader("transferMode.dlna.org: Interactive ") == true);
    QVERIFY(request.getTransferMode() == "Interactive");

    QVERIFY(request.appendHeader("getcontentFeatures.dlna.org:1") == true);
    QVERIFY(request.getContentFeatures() == "1");

    QVERIFY(request.appendHeader("Host: 192.168.1.103:5002") == true);

    QVERIFY(request.appendHeader("ACCEPT-LANGUAGE: en-us") == true);

    QVERIFY(request.appendHeader("Accept-Encoding: gzip, deflate") == true);

    QVERIFY(request.appendHeader("Accept: */*") == true);

    QVERIFY(request.appendHeader("Connection: keep-alive") == true);

    QVERIFY(request.appendHeader("Content-Type: text/xml; charset=\"utf-8\"") == true);

    QVERIFY(request.appendHeader("Timeout: Second-1800") == true);

    QVERIFY(request.appendHeader("Nt: upnp:event") == true);

    QVERIFY(request.appendHeader("Icy-MetaData: 1") == true);

}

//void TestRequest::testCase_DlnaMusicTrack_MP3_streaming() {
//    server = new Server();

//    Logger* log = new Logger(this);
//    Request* request;

//    DlnaRootFolder* rootFolder = new DlnaRootFolder(log, "host", 400, this);
//    QVERIFY(rootFolder->addFolder("/Users/doudou/workspace/DLNA_server/tests/AUDIO") == true);
//    rootFolder->getDLNAResources("0$1$3", false, 0, 0, "");

//    QTcpSocket* client = new QTcpSocket();
//    client->connectToHost(QHostAddress("127.0.0.1"), 10000);
//    QVERIFY(client->waitForConnected(-1) == true);

//    // read all media
//    request = new Request(log, client, "uuid", "server name", "host", 400, rootFolder, this);
//    request->appendHeader("GET /get/0$1$3/07+On_Off.mp3 HTTP/1.0");
//    request->appendHeader("RANGE: BYTES=0-");
//    //request->start();

//    int argc;
//    char* argv[10];
//    QApplication app(argc, argv);
//    app.exec();

//    qWarning() << server->hasPendingConnections();

//    QVERIFY(track.getStream(range)->readAll().size() == 376593);
//    QVERIFY(track.getStream(range)->readAll().size() == 376593);
//    delete range;

//    // invalid range
//    range = new HttpRange("RANGE: BYTES=3190950-");
//    range->setSize(track.size());
//    QVERIFY(track.getStream(range)->readAll().size() == 0);
//    delete range;

//    // read partial media
//    range = new HttpRange("RANGE: BYTES=93-");
//    range->setSize(track.size());
//    QVERIFY(track.getStream(range)->readAll().size() == 376500);
//    delete range;

//    // read partial media
//    range = new HttpRange("RANGE: BYTES=14-1000");
//    range->setSize(track.size());
//    QVERIFY(track.getStream(range)->readAll().size() == 987);
//    delete range;
//}

