#include "dlnamusictrack.h"

const QString DlnaMusicTrack::UNKNOWN_AUDIO_TYPEMIME = "audio/mpeg";
const QString DlnaMusicTrack::AUDIO_MP3_TYPEMIME = "audio/mpeg";
const QString DlnaMusicTrack::AUDIO_MP4_TYPEMIME = "audio/mp4";
const QString DlnaMusicTrack::AUDIO_WAV_TYPEMIME = "audio/wav";
const QString DlnaMusicTrack::AUDIO_WMA_TYPEMIME = "audio/x-ms-wma";
const QString DlnaMusicTrack::AUDIO_FLAC_TYPEMIME = "audio/x-flac";
const QString DlnaMusicTrack::AUDIO_OGG_TYPEMIME = "audio/x-ogg";
const QString DlnaMusicTrack::AUDIO_LPCM_TYPEMIME = "audio/L16";
const QString DlnaMusicTrack::AUDIO_TRANSCODE = "audio/transcode";

DlnaMusicTrack::DlnaMusicTrack(QString host, int port, QObject *parent):
    DlnaItem(host, port, parent)
{
}

void DlnaMusicTrack::updateDLNAOrgPn() {
    if (mimeType() == AUDIO_MP3_TYPEMIME) {
        setdlnaOrgPN("MP3");
    } else if (mimeType() == AUDIO_MP4_TYPEMIME) {
        setdlnaOrgPN("AAC_ISO_320");
    } else if (mimeType() == AUDIO_LPCM_TYPEMIME) {
        setdlnaOrgPN("LPCM");
    } else if (mimeType() == AUDIO_WAV_TYPEMIME) {
        setdlnaOrgPN("WAV");
    }
}

int DlnaMusicTrack::bitrate() const {
    // returns bitrate in bits/sec
    if (toTranscode()) {
        if (transcodeFormat == MP3 or transcodeFormat == AAC)
        {
            return 320000;
        }
        else if (transcodeFormat == LPCM or transcodeFormat == WAV or transcodeFormat == ALAC) {

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
QDomElement DlnaMusicTrack::getXmlContentDirectory(QDomDocument *xml, QStringList properties)  {
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
            upnpAlbumArtURI.appendChild(xml->createTextNode(QString("http://%1:%2/get/%3/thumbnail0000%4&").arg(host).arg(port).arg(getResourceId()).arg(getDisplayName().toUtf8().toPercentEncoding().constData())));
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

    res.appendChild(xml->createTextNode(QString("http://%2:%3/get/%1/%4").arg(getResourceId()).arg(host).arg(port).arg(getName().toUtf8().toPercentEncoding().constData())));

    xml_obj.appendChild(res);

    return xml_obj;
}

FfmpegTranscoding *DlnaMusicTrack::getTranscodeProcess()
{
    FfmpegTranscoding* transcodeProcess = new FfmpegTranscoding();
    transcodeProcess->setUrl(getSystemName());
    transcodeProcess->setLengthInMSeconds(metaDataDuration());
    transcodeProcess->setFormat(transcodeFormat);
    transcodeProcess->setBitrate(bitrate());
//    transcodeProcess->setVolumeInfo(volumeInfo());

    return transcodeProcess;
}

Device *DlnaMusicTrack::getOriginalStreaming()
{
    return new StreamingFile(getSystemName());
}

QString DlnaMusicTrack::mimeType() const {
    if (toTranscode()) {
        // Trancode music track
        if (transcodeFormat == MP3) {
            return AUDIO_MP3_TYPEMIME;
        } else if (transcodeFormat == AAC or transcodeFormat == ALAC) {
            return AUDIO_MP4_TYPEMIME;
        } else if (transcodeFormat == LPCM) {
            return AUDIO_LPCM_TYPEMIME;
        } else if (transcodeFormat == WAV) {
            return AUDIO_WAV_TYPEMIME;
        } else {
            qCritical() << "Unable to define mimeType of DlnaMusicTrack Transcoding: " << getSystemName();
        }
    } else {
        QString format = metaDataFormat();
        if (format == "mp3" or format == "mp2" or format == "mp1") {
            return AUDIO_MP3_TYPEMIME;
        } else if (format == "aac" or format == "alac") {
            return AUDIO_MP4_TYPEMIME;
        } else if (format == "pcm_s16le") {
            return AUDIO_WAV_TYPEMIME;
        } else {
            qCritical() << QString("Unable to define mimeType of DlnaMusicTrack: %1 from format <%2>").arg(getSystemName()).arg(format);
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
