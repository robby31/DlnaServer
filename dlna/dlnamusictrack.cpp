#include "dlnamusictrack.h"

const QString DlnaMusicTrack::UNKNOWN_AUDIO_TYPEMIME = "audio/mpeg";
const QString DlnaMusicTrack::AUDIO_MP3_TYPEMIME = "audio/mpeg";
const QString DlnaMusicTrack::AUDIO_MP4_TYPEMIME = "audio/x-m4a";
const QString DlnaMusicTrack::AUDIO_WAV_TYPEMIME = "audio/wav";
const QString DlnaMusicTrack::AUDIO_WMA_TYPEMIME = "audio/x-ms-wma";
const QString DlnaMusicTrack::AUDIO_FLAC_TYPEMIME = "audio/x-flac";
const QString DlnaMusicTrack::AUDIO_OGG_TYPEMIME = "audio/x-ogg";
const QString DlnaMusicTrack::AUDIO_LPCM_TYPEMIME = "audio/L16";
const QString DlnaMusicTrack::AUDIO_TRANSCODE = "audio/transcode";

DlnaMusicTrack::DlnaMusicTrack(Logger* log, QString filename, QString host, int port, QObject *parent):
    DlnaItem(log, filename, host, port, parent)
{
    setTranscodeFormat(MP3);  // default transcode format
}

void DlnaMusicTrack::updateDLNAOrgPn() {
    if (mimeType() == "audio/mpeg") {
        setdlnaOrgPN("MP3");
    } else if (mimeType() == "audio/x-m4a") {
        setdlnaOrgPN("AAC_ISO");
    } else if (mimeType().contains("audio/L16")) {
        setdlnaOrgPN("LPCM");
    }
}

int DlnaMusicTrack::bitrate() {
    // returns bitrate in bits/sec
    if (toTranscode()) {
        if (transcodeFormat == MP3) {
            return 320000;

        } else if (transcodeFormat == LPCM) {

            if (samplerate() == 44100) {
                return 1411000;

            } else {
                return 1536000;
            }
        } else {
            // invalid transcode format
            return 0;
        }
    } else {
        return mediaTag.getParameter("OverallBitRate").toInt();
    }
}

int DlnaMusicTrack::channelCount() {
    int audioStreamCount = mediaTag.getAudioStreamCount();
    if (audioStreamCount == 1) {
        return mediaTag.getChannelCount(0);
    }
    return 0;
}

int DlnaMusicTrack::samplerate() {
    int audioStreamCount = mediaTag.getAudioStreamCount();
    if (audioStreamCount == 1) {
        return mediaTag.getSamplingRate(0);
    }
    return 0;
}

/*
* Returns XML (DIDL) representation of the DLNA node. It gives a
* complete representation of the item, with as many tags as available.
*
* Reference: http://www.upnp.org/specs/av/UPnP-av-ContentDirectory-v1-Service.pdf
*/
QDomElement DlnaMusicTrack::getXmlContentDirectory(QDomDocument *xml, QStringList properties) {
    QDomElement xml_obj;

    xml_obj = xml->createElement("item");

    // mandatory properties are: id, parentID, title, class, restricted

    xml_obj.setAttribute("id", getResourceId());

    xml_obj.setAttribute("parentID", getParentId());

    QDomElement dcTitle = xml->createElement("dc:title");
    dcTitle.appendChild(xml->createTextNode(getDisplayName()));
    xml_obj.appendChild(dcTitle);

    QDomElement upnpClass = xml->createElement("upnp:class");
    upnpClass.appendChild(xml->createTextNode("object.item.audioItem.musicTrack"));
    xml_obj.appendChild(upnpClass);

    xml_obj.setAttribute("restricted", "true");

    // properties optional of audioItem

    if (properties.contains("upnp:genre")) {
        QDomElement upnpGenre = xml->createElement("upnp:genre");
        upnpGenre.appendChild(xml->createTextNode(mediaTag.getParameter("Genre")));
        xml_obj.appendChild(upnpGenre);
    }

    if (properties.contains("dc:description")) {

    }

    if (properties.contains("upnp:longDescription")) {

    }

    if (properties.contains("dc:publisher")) {

    }

    if (properties.contains("dc:language")) {

    }

    if (properties.contains("dc:relation")) {

    }

    if (properties.contains("dc:rights")) {

    }

    // properties optional of musicTrack

    if (properties.contains("upnp:artist")) {
        QDomElement upnpArtist = xml->createElement("upnp:artist");
        upnpArtist.appendChild(xml->createTextNode(mediaTag.getParameter("Performer")));
        xml_obj.appendChild(upnpArtist);
    }

    if (properties.contains("upnp:album")) {
        QDomElement upnpAlbum = xml->createElement("upnp:album");
        upnpAlbum.appendChild(xml->createTextNode(mediaTag.getParameter("Album")));
        xml_obj.appendChild(upnpAlbum);
    }

    if (properties.contains("upnp:originalTrackNumber")) {
        QDomElement upnpTrackNumber = xml->createElement("upnp:originalTrackNumber");
        upnpTrackNumber.appendChild(xml->createTextNode(mediaTag.getParameter("Track/Position")));
        xml_obj.appendChild(upnpTrackNumber);
    }

    if (properties.contains("upnp:playlist")) {

    }

    if (properties.contains("upnp:storageMedium")) {

    }

    if (properties.contains("dc:contributor")) {
        QDomElement upnpCreator = xml->createElement("dc:contributor");
        upnpCreator.appendChild(xml->createTextNode(mediaTag.getParameter("Performer")));
        xml_obj.appendChild(upnpCreator);
    }

    if (properties.contains("dc:date")) {
        QDomElement upnpDate = xml->createElement("dc:date");
        upnpDate.appendChild(xml->createTextNode(fileinfo.lastModified().toString("yyyy-MM-dd")));
        xml_obj.appendChild(upnpDate);
    }

    // properties optional of musicAlbum

    if (properties.contains("upnp:albumArtURI")) {
        QImage picture = getAlbumArt();
        if (!picture.isNull()) {
            QDomElement upnpAlbumArtURI = xml->createElement("upnp:albumArtURI");
            upnpAlbumArtURI.setAttribute("xmlns:dlna", "urn:schemas-dlna-org:metadata-1-0/");
            upnpAlbumArtURI.setAttribute("dlna:profileID", "JPEG_TN");
            upnpAlbumArtURI.appendChild(xml->createTextNode(QString("http://%1:%2/get/%4/thumbnail0000%3&").arg(host).arg(port).arg(getDisplayName()).arg(getResourceId())));
            xml_obj.appendChild(upnpAlbumArtURI);
        }
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

FfmpegTranscoding *DlnaMusicTrack::getTranscodeProcess(HttpRange *range, long timeseek_start, long timeseek_end) {

    if (!toTranscode()) {

        // use getStream instead of transcoding
        return 0;

    } else {

        // TODO: set the parent of the QObject
        FfmpegTranscoding* transcodeProcess = new FfmpegTranscoding();

        if (transcodeProcess->initialize(range, timeseek_start, timeseek_end, fileinfo.filePath(), getLengthInSeconds(), transcodeFormat, bitrate())) {

            getLog()->DEBUG(QString("Audio Transcoding process %1 %2").arg(transcodeProcess->program()).arg(transcodeProcess->arguments().join(' ')));
            return transcodeProcess;

        } else {

            return 0;

        }
    }
}

QString DlnaMusicTrack::mimeType() {
    if (toTranscode()) {
        // Trancode music track
        if (transcodeFormat == MP3) {
            return AUDIO_MP3_TYPEMIME;
        } else if (transcodeFormat == LPCM) {
            return AUDIO_LPCM_TYPEMIME;
        } else {
            getLog()->ERROR("Unable to define mimeType of DlnaVideoItem Transcoding: " + getSystemName());
        }
    } else {
        QString format = mediaTag.getParameter("Format");
        if (format == "MPEG Audio") {
            return AUDIO_MP3_TYPEMIME;
        } else {
            getLog()->ERROR("Unable to define mimeType of DlnaVideoItem: " + getSystemName());
        }
    }

    // returns unknown mimeType
    return UNKNOWN_AUDIO_TYPEMIME;
}

QImage DlnaMusicTrack::getAlbumArt() {
    QImage picture;

    QByteArray bytesPicture =  QByteArray::fromBase64(mediaTag.getCoverData().c_str());

    if (picture.loadFromData((const uchar *) bytesPicture.data(), bytesPicture.size())) {
        return picture;
    }

    return picture;
}
