#include "dlnavideoitem.h"

const QString DlnaVideoItem::UNKNOWN_VIDEO_TYPEMIME = "video/mpeg";
const QString DlnaVideoItem::MPEG_TYPEMIME = "video/mpeg";
const QString DlnaVideoItem::MP4_TYPEMIME = "video/mp4";
const QString DlnaVideoItem::AVI_TYPEMIME = "video/avi";
const QString DlnaVideoItem::WMV_TYPEMIME = "video/x-ms-wmv";
const QString DlnaVideoItem::ASF_TYPEMIME = "video/x-ms-asf";
const QString DlnaVideoItem::MATROSKA_TYPEMIME = "video/x-matroska";
const QString DlnaVideoItem::VIDEO_TRANSCODE = "video/transcode";

DlnaVideoItem::DlnaVideoItem(Logger *log, QString host, int port, QObject *parent):
    DlnaItem(log, host, port, parent)
{
    setTranscodeFormat(MPEG2_AC3);   // default transcode format

    if (toTranscode()) {
        setdlnaOrgOpFlags("10");         // seek by time (exclusive)
    }
}

DlnaVideoItem::~DlnaVideoItem() {

}

/*
* Returns XML (DIDL) representation of the DLNA node. It gives a
* complete representation of the item, with as many tags as available.
*
* Reference: http://www.upnp.org/specs/av/UPnP-av-ContentDirectory-v1-Service.pdf
*/
QDomElement DlnaVideoItem::getXmlContentDirectory(QDomDocument *xml, QStringList properties) const {
    if (!xml)
        return QDomElement();

    QDomElement xml_obj = xml->createElement("item");

    updateXmlContentDirectory(xml, &xml_obj, properties);

    // properties optional of videoItem

    if (properties.contains("*") or properties.contains("upnp:genre")) {
        QDomElement upnpGenre = xml->createElement("upnp:genre");
        upnpGenre.appendChild(xml->createTextNode(metaDataGenre()));
        xml_obj.appendChild(upnpGenre);
    }

    if (properties.contains("*") or properties.contains("upnp:longDescription")) {

    }

    if (properties.contains("*") or properties.contains("upnp:producer")) {

    }

    if (properties.contains("*") or properties.contains("upnp:rating")) {

    }

    if (properties.contains("*") or properties.contains("upnp:actor")) {

    }

    if (properties.contains("*") or properties.contains("upnp:director")) {

    }

    if (properties.contains("*") or properties.contains("dc:description")) {

    }

    if (properties.contains("*") or properties.contains("dc:publisher")) {

    }

    if (properties.contains("*") or properties.contains("dc:language")) {

    }

    if (properties.contains("*") or properties.contains("dc:relation")) {

    }

    // add <res> element

    QTime duration(0, 0, 0);
    QDomElement res = xml->createElement("res");
    res.setAttribute("xmlns:dlna", "urn:schemas-dlna-org:metadata-1-0/");

    // mandatory properties: protocolInfo
    res.setAttribute("protocolInfo", getProtocolInfo());

    // optional properties
    if ((properties.contains("*") or properties.contains("res@bitrate")) and bitrate() != -1) {
        // bitrate in bytes/sec
        res.setAttribute("bitrate", QString("%1").arg(qRound(double(bitrate())/8.0)));
    }

    if (properties.contains("*") or properties.contains("res@resolution")) {
        res.setAttribute("resolution", resolution());
    }

    if (properties.contains("*") or properties.contains("res@duration")) {
        res.setAttribute("duration", QString("%1").arg(duration.addSecs(getLengthInSeconds()).toString("hh:mm:ss")));
    }

    if (properties.contains("*") or properties.contains("res@sampleFrequency")) {
        res.setAttribute("sampleFrequency", QString("%1").arg(samplerate()));
    }

    if (properties.contains("*") or properties.contains("res@nrAudioChannels")) {
        res.setAttribute("nrAudioChannels", QString("%1").arg(channelCount()));
    }

    if ((properties.contains("*") or properties.contains("res@size")) and size() != -1) {
        // size in bytes
        res.setAttribute("size", QString("%1").arg(size()));
    }

    res.appendChild(xml->createTextNode(QString("http://%2:%3/get/%1/%4").arg(getResourceId()).arg(host).arg(port).arg(getName().toUtf8().toPercentEncoding().constData())));

    xml_obj.appendChild(res);

    return xml_obj;
}

int DlnaVideoItem::bitrate() const {
    // returns bitrate in bits/sec
    if (toTranscode()) {
        // variable bitrate
        return 5718800;
    } else {
        return metaDataBitrate();
    }
}

QString DlnaVideoItem::mimeType() const {
    if (toTranscode()) {
        if (transcodeFormat == MPEG2_AC3) {
            return MPEG_TYPEMIME;

        } else {
            logError("Unable to define mimeType of DlnaVideoItem: " + getSystemName());

            // returns unknown mimeType
            return UNKNOWN_VIDEO_TYPEMIME;
        }
    } else {
        QString format = metaDataFormat();
        if (format == "AVI") {
            return AVI_TYPEMIME;

        } else if (format == "Matroska") {
            return MATROSKA_TYPEMIME;

        } else {
            logError("Unable to define mimeType of DlnaVideoItem: " + format + " " + getSystemName());

            // returns unknown mimeType
            return UNKNOWN_VIDEO_TYPEMIME;
        }
    }
}

void DlnaVideoItem::updateDLNAOrgPn() {
    setdlnaOrgPN("MPEG_PS_PAL");
}

int DlnaVideoItem::channelCount() const {
    if (toTranscode())
        return 6;
    else
        return -1;
}

int DlnaVideoItem::samplerate() const {
    if (toTranscode())
        return 48000;
    else
        return -1;
}
