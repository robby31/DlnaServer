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
    HttpClient client(&log);
    Request request(&log, &client, "uuid", "server name", "host IP", 56);

    QVERIFY2(request.data(Request::hostRole) == "host IP (-1)", request.data(Request::hostRole).toString().toUtf8());
    QVERIFY(request.data(Request::argumentRole) == "");
    QVERIFY(request.data(Request::methodRole) == "");
    QVERIFY(request.data(Request::peerAddressRole) == "");
    QVERIFY(request.data(Request::statusRole) == "init");
    QVERIFY(request.data(Request::headerRole) == "");
    QVERIFY(request.data(Request::contentRole) == "");
    QVERIFY(request.isHttp10() == true);
    QVERIFY(request.getParamHeader("USER-AGENT") == "");
    QVERIFY(request.getTransferMode() == "");
    QVERIFY(request.getContentFeatures() == "");

    QVERIFY(request.getParamHeader("SOAPACTION") == "");
//    QVERIFY(request.appendHeader("CALLBACK: field_callback") == true);
//    QVERIFY(request.getParamHeader("SOAPACTION") == "field_callback");

//    QVERIFY(request.appendHeader("SOAPACTION: field_soapaction") == true);
//    QVERIFY2(request.getParamHeader("SOAPACTION") == "field_soapaction", request.getParamHeader("SOAPACTION").toUtf8().constData());

//    QVERIFY(request.appendHeader("") == false);
//    QVERIFY(request.appendHeader("METHOD") == false);

//    QVERIFY(request.isHttp10() == true);
//    QVERIFY(request.appendHeader("GET /get/0$4$1$9$2$11/thumbnail00001-11+Tout+sauf+toi.mp3 HTTP/1.1") == true);
//    QVERIFY(request.getMethod() == "GET");
//    QVERIFY(request.getArgument() == "get/0$4$1$9$2$11/thumbnail00001-11+Tout+sauf+toi.mp3");
//    QVERIFY(request.isHttp10() == false);

//    QVERIFY(request.appendHeader("User-Agent: DLNADOC/1.50") == true);
//    QVERIFY(request.getParamHeader("USER-AGENT") == "DLNADOC/1.50");

//    QVERIFY(request.appendHeader("CONTENT-LENGTH: 632") == true);
//    QVERIFY(request.getReceivedContentLength() == 632);

//    QVERIFY(request.appendHeader("Content-Length: 2122") == true);
//    QVERIFY(request.getReceivedContentLength() == 2122);

//    QVERIFY(request.appendHeader("transferMode.dlna.org: Interactive ") == true);
//    QVERIFY(request.getTransferMode() == "Interactive");

//    QVERIFY(request.appendHeader("getcontentFeatures.dlna.org:1") == true);
//    QVERIFY(request.getContentFeatures() == "1");

//    QVERIFY(request.appendHeader("Host: 192.168.1.103:5002") == true);

//    QVERIFY(request.appendHeader("ACCEPT-LANGUAGE: en-us") == true);

//    QVERIFY(request.appendHeader("Accept-Encoding: gzip, deflate") == true);

//    QVERIFY(request.appendHeader("Accept: */*") == true);

//    QVERIFY(request.appendHeader("Connection: keep-alive") == true);

//    QVERIFY(request.appendHeader("Content-Type: text/xml; charset=\"utf-8\"") == true);

//    QVERIFY(request.appendHeader("Timeout: Second-1800") == true);

//    QVERIFY(request.appendHeader("Nt: upnp:event") == true);

//    QVERIFY(request.appendHeader("Icy-MetaData: 1") == true);

//    QVERIFY(request.isHttp10() == false);
//    QVERIFY(request.appendHeader("GET /get/0$4$1$9$2$11/thumbnail00001-11+Tout+sauf+toi.mp3 HTTP/1.0") == true);
//    QVERIFY(request.getMethod() == "GET");
//    QVERIFY(request.getArgument() == "get/0$4$1$9$2$11/thumbnail00001-11+Tout+sauf+toi.mp3");
//    QVERIFY(request.isHttp10() == true);
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

