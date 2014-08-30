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

DlnaMusicTrack::DlnaMusicTrack(Logger* log, QString host, int port, QObject *parent):
    DlnaItem(log, host, port, parent)
{
    transcodeFormat = MP3;   // default transcode format
    setdlnaOrgPN("MP3");
}

DlnaMusicTrack::~DlnaMusicTrack() {

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

int DlnaMusicTrack::bitrate() const {
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
            return -1;
        }
    } else {
        return metaDataBitrate();
    }
}

/*
* Returns XML (DIDL) representation of the DLNA node. It gives a
* complete representation of the item, with as many tags as available.
*
* Reference: http://www.upnp.org/specs/av/UPnP-av-ContentDirectory-v1-Service.pdf
*/
QDomElement DlnaMusicTrack::getXmlContentDirectory(QDomDocument *xml, QStringList properties) const {
    if (!xml)
        return QDomElement();

    QDomElement xml_obj = xml->createElement("item");

    updateXmlContentDirectory(xml, &xml_obj, properties);

    // properties optional of audioItem

    if (properties.contains("*") or properties.contains("upnp:genre")) {
        QDomElement upnpGenre = xml->createElement("upnp:genre");
        upnpGenre.appendChild(xml->createTextNode(metaDataGenre()));
        xml_obj.appendChild(upnpGenre);
    }

    if (properties.contains("*") or properties.contains("dc:description")) {

    }

    if (properties.contains("*") or properties.contains("upnp:longDescription")) {

    }

    if (properties.contains("*") or properties.contains("dc:publisher")) {

    }

    if (properties.contains("*") or properties.contains("dc:language")) {

    }

    if (properties.contains("*") or properties.contains("dc:relation")) {

    }

    if (properties.contains("*") or properties.contains("dc:rights")) {

    }

    // properties optional of musicTrack

    if (properties.contains("*") or properties.contains("upnp:artist")) {
        QDomElement upnpArtist = xml->createElement("upnp:artist");
        upnpArtist.appendChild(xml->createTextNode(metaDataPerformer()));
        xml_obj.appendChild(upnpArtist);
    }

    if (properties.contains("*") or properties.contains("upnp:album")) {
        QDomElement upnpAlbum = xml->createElement("upnp:album");
        upnpAlbum.appendChild(xml->createTextNode(metaDataAlbum()));
        xml_obj.appendChild(upnpAlbum);
    }

    if (properties.contains("*") or properties.contains("upnp:originalTrackNumber")) {
        QDomElement upnpTrackNumber = xml->createElement("upnp:originalTrackNumber");
        upnpTrackNumber.appendChild(xml->createTextNode(QString("%1").arg(metaDataTrackPosition())));
        xml_obj.appendChild(upnpTrackNumber);
    }

    if (properties.contains("*") or properties.contains("upnp:playlist")) {

    }

    if (properties.contains("*") or properties.contains("upnp:storageMedium")) {

    }

    if (properties.contains("*") or properties.contains("dc:contributor")) {
        QDomElement upnpCreator = xml->createElement("dc:contributor");
        upnpCreator.appendChild(xml->createTextNode(metaDataPerformer()));
        xml_obj.appendChild(upnpCreator);
    }

    if (properties.contains("*") or properties.contains("dc:date")) {
        QDomElement upnpDate = xml->createElement("dc:date");
        upnpDate.appendChild(xml->createTextNode(metaDataLastModifiedDate()));
        xml_obj.appendChild(upnpDate);
    }

    // properties optional of musicAlbum

    if (properties.contains("*") or properties.contains("upnp:albumArtURI")) {
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
    if (properties.contains("*") or properties.contains("res@bitrate")) {
        // bitrate in bytes/sec
        res.setAttribute("bitrate", QString("%1").arg(qRound(double(bitrate())/8.0)));
    }

    if (properties.contains("*") or properties.contains("res@duration")) {
        res.setAttribute("duration", QString("%1").arg(duration.addSecs(getLengthInSeconds()).toString()));
    }

    if (properties.contains("*") or properties.contains("res@sampleFrequency")) {
        res.setAttribute("sampleFrequency", QString("%1").arg(samplerate()));
    }

    if (properties.contains("*") or properties.contains("res@nrAudioChannels")) {
        res.setAttribute("nrAudioChannels", QString("%1").arg(channelCount()));
    }

    if (properties.contains("*") or properties.contains("res@size")) {
        // size in bytes
        res.setAttribute("size", QString("%1").arg(size()));
    }

    res.appendChild(xml->createTextNode(QString("http://%2:%3/get/%1/%4").arg(getResourceId()).arg(host).arg(port).arg(getName().replace(" ", "+"))));

    xml_obj.appendChild(res);

    return xml_obj;
}

FfmpegTranscoding *DlnaMusicTrack::getTranscodeProcess(HttpRange *range, long timeseek_start, long timeseek_end, QObject *parent) {

    if (!toTranscode()) {

        // use getStream instead of transcoding
        return 0;

    } else {

        FfmpegTranscoding* transcodeProcess = new FfmpegTranscoding(log, parent != 0 ? parent : this);

        if (transcodeProcess->initialize(range, timeseek_start, timeseek_end, getSystemName(), getLengthInSeconds(), transcodeFormat, bitrate())) {

            log->Debug(QString("Audio Transcoding process %1 %2").arg(transcodeProcess->program()).arg(transcodeProcess->arguments().join(' ')));
            return transcodeProcess;

        } else {

            return 0;

        }
    }
}

QString DlnaMusicTrack::mimeType() const {
    if (toTranscode()) {
        // Trancode music track
        if (transcodeFormat == MP3) {
            return AUDIO_MP3_TYPEMIME;
        } else if (transcodeFormat == LPCM) {
            return AUDIO_LPCM_TYPEMIME;
        } else {
            log->Error("Unable to define mimeType of DlnaMusicTrack Transcoding: " + getSystemName());
        }
    } else {
        QString format = metaDataFormat();
        if (format == "mp3" or format == "mp2" or format == "mp1") {
            return AUDIO_MP3_TYPEMIME;
        } else if (format == "aac") {
            return AUDIO_MP4_TYPEMIME;
        } else {
            log->Error(QString("Unable to define mimeType of DlnaMusicTrack: %1 from format <%2>").arg(getSystemName()).arg(format));
        }
    }

    // returns unknown mimeType
    return UNKNOWN_AUDIO_TYPEMIME;
}

QImage DlnaMusicTrack::getAlbumArt() const {
    QImage picture;

    QByteArray bytesPicture = metaDataPicture();
    if (picture.loadFromData(bytesPicture)) {
        return picture;
    }

    return picture;
}
