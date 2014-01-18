#include "dlnavideoitem.h"

const QString DlnaVideoItem::UNKNOWN_VIDEO_TYPEMIME = "video/mpeg";
const QString DlnaVideoItem::MPEG_TYPEMIME = "video/mpeg";
const QString DlnaVideoItem::MP4_TYPEMIME = "video/mp4";
const QString DlnaVideoItem::AVI_TYPEMIME = "video/avi";
const QString DlnaVideoItem::WMV_TYPEMIME = "video/x-ms-wmv";
const QString DlnaVideoItem::ASF_TYPEMIME = "video/x-ms-asf";
const QString DlnaVideoItem::MATROSKA_TYPEMIME = "video/x-matroska";
const QString DlnaVideoItem::VIDEO_TRANSCODE = "video/transcode";

DlnaVideoItem::DlnaVideoItem(Logger *log, QString filename, QString host, int port):
    DlnaItem(log, filename, host, port)
{
    setTranscodeFormat(MPEG2_AC3);   // default transcode format
}

QString DlnaVideoItem::getDisplayName() {
    return fileinfo.completeBaseName();
}

/*
* Returns XML (DIDL) representation of the DLNA node. It gives a
* complete representation of the item, with as many tags as available.
*
* Reference: http://www.upnp.org/specs/av/UPnP-av-ContentDirectory-v1-Service.pdf
*/
QDomElement DlnaVideoItem::getXmlContentDirectory(QDomDocument *xml, QStringList properties) {
    QDomElement xml_obj;

    xml_obj = xml->createElement("item");

    // mandatory properties are: id, parentID, title, class, restricted

    xml_obj.setAttribute("id", getResourceId());

    xml_obj.setAttribute("parentID", getParentId());

    QDomElement dcTitle = xml->createElement("dc:title");
    dcTitle.appendChild(xml->createTextNode(getDisplayName()));
    xml_obj.appendChild(dcTitle);

    QDomElement upnpClass = xml->createElement("upnp:class");
    upnpClass.appendChild(xml->createTextNode("object.item.videoItem"));
    xml_obj.appendChild(upnpClass);

    xml_obj.setAttribute("restricted", "true");

    // properties optional of videoItem

    if (properties.contains("upnp:genre")) {
        QDomElement upnpGenre = xml->createElement("upnp:genre");
        upnpGenre.appendChild(xml->createTextNode(mediaTag.getParameter("Genre")));
        xml_obj.appendChild(upnpGenre);
    }

    if (properties.contains("upnp:longDescription")) {

    }

    if (properties.contains("upnp:producer")) {

    }

    if (properties.contains("upnp:rating")) {

    }

    if (properties.contains("upnp:actor")) {

    }

    if (properties.contains("upnp:director")) {

    }

    if (properties.contains("dc:description")) {

    }

    if (properties.contains("dc:publisher")) {

    }

    if (properties.contains("dc:language")) {

    }

    if (properties.contains("dc:relation")) {

    }

    // add <res> element

    QTime duration(0, 0, 0);
    QDomElement res = xml->createElement("res");
    res.setAttribute("xmlns:dlna", "urn:schemas-dlna-org:metadata-1-0/");

    // mandatory properties: protocolInfo
    res.setAttribute("protocolInfo", getProtocolInfo());

    // optional properties
    if (properties.contains("res@bitrate")) {
        // bitrate in bytes/sec
        res.setAttribute("bitrate", QString("%1").arg(qRound(double(bitrate())/8.0)));
    }

    if (properties.contains("res@resolution")) {
        res.setAttribute("resolution", resolution());
    }

    if (properties.contains("res@duration")) {
        res.setAttribute("duration", QString("%1").arg(duration.addSecs(getLengthInSeconds()).toString()));
    }

    if (properties.contains("res@sampleFrequency")) {
        res.setAttribute("sampleFrequency", QString("%1").arg(samplerate()));
    }

    if (properties.contains("res@nrAudioChannels")) {
        res.setAttribute("nrAudioChannels", QString("%1").arg(channelCount()));
    }

    if (properties.contains("res@size")) {
        // size in bytes
        res.setAttribute("size", QString("%1").arg(size()));
    }

    res.appendChild(xml->createTextNode(QString("http://%2:%3/get/%1/%4").arg(getResourceId()).arg(host).arg(port).arg(fileinfo.fileName().replace(" ", "+"))));

    xml_obj.appendChild(res);

    return xml_obj;
}

int DlnaVideoItem::bitrate() {
    // returns bitrate in bits/sec
    if (toTranscode()) {
        if (transcodeFormat == MPEG2_AC3) {
            return 5448000;

        } else {
            // invalid transcode format
            return 0;
        }
    } else {
        return mediaTag.getParameter("OverallBitRate").toInt();
    }
}

FfmpegTranscoding *DlnaVideoItem::getTranscodeProcess(HttpRange *range) {
    qWarning() << "MOVIE" << mediaTag.getVideoCodec(0) << mediaTag.getAudioFormat(0);
    if (!toTranscode()) {

        // use getStream instead of transcoding
        return 0;

    } else {

        FfmpegTranscoding* transcodeProcess = new FfmpegTranscoding();

        if (transcodeProcess->initialize(range, fileinfo.filePath(), getLengthInSeconds(), transcodeFormat, bitrate(), audioLanguages(), subtitleLanguages(), framerate())) {

            getLog()->DEBUG(QString("Video Transcoding process %1 %2").arg(transcodeProcess->program()).arg(transcodeProcess->arguments().join(' ')));
            return transcodeProcess;

        } else {

            return 0;

        }
    }
}

QString DlnaVideoItem::mimeType() {
    QString format = mediaTag.getParameter("Format");
    if (format == "AVI") {
        return AVI_TYPEMIME;

    } else if (format == "Matroska") {
        return MATROSKA_TYPEMIME;

    } else {
        getLog()->ERROR("Unable to define mimeType of DlnaVideoItem: " + getSystemName());

        // returns unknown mimeType
        return UNKNOWN_VIDEO_TYPEMIME;
    }
}

void DlnaVideoItem::updateDLNAOrgPn() {
    setdlnaOrgPN("MPEG_PS_PAL");
}

int DlnaVideoItem::channelCount() {
    int audioStreamCount = mediaTag.getAudioStreamCount();
    if (audioStreamCount == 1) {
        return mediaTag.getChannelCount(0);
    }
    return 0;
}

int DlnaVideoItem::samplerate() {
    int audioStreamCount = mediaTag.getAudioStreamCount();
    if (audioStreamCount == 1) {
        return mediaTag.getSamplingRate(0);
    }
    return 0;
}

QString DlnaVideoItem::resolution() {
    int videoStreamCount = mediaTag.getVideoStreamCount();
    if (videoStreamCount == 1) {
        return mediaTag.getResolution(0);
    }
    return QString();
}

QStringList DlnaVideoItem::audioLanguages() {
    return mediaTag.getAudioLanguages();
}

QStringList DlnaVideoItem::subtitleLanguages() {
    return mediaTag.getSubtitleLanguages();
}

QString DlnaVideoItem::framerate() {
    return mediaTag.getVideoFrameRate();
}
