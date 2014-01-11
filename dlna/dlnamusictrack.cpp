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

DlnaMusicTrack::DlnaMusicTrack(Logger* log, QString filename, QString host, int port):
    DlnaResource(log),
    fileinfo(QFileInfo(filename)),
    mediaTag(filename),
    host(host),
    port(port),
    transcodeFormat(MP3)   // default transcode format
{    
    setDiscovered(true);

    QMimeDatabase db;
    mime_type = db.mimeTypeForFile(fileinfo);

    updateDLNAOrgPn();
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

QString DlnaMusicTrack::getName() const {
    return fileinfo.fileName();
}

QString DlnaMusicTrack::getSystemName() const {
    return fileinfo.filePath();
}

QString DlnaMusicTrack::getDisplayName() {
    QString title = mediaTag.getParameter("Title");
    if (title.isEmpty()) {
        return fileinfo.completeBaseName();
    } else {
        return title;
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
        }

        return 0;
    } else {
        return mediaTag.getParameter("OverallBitRate").toInt();
    }
}

int DlnaMusicTrack::size() {
    if (toTranscode()) {
        return double(bitrate())*double(getLengthInMilliSeconds())/8000.0;
    } else {
        return fileinfo.size();
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

int DlnaMusicTrack::getLengthInSeconds() {
    return qRound(double(getLengthInMilliSeconds())/1000.0);
}

int DlnaMusicTrack::getLengthInMilliSeconds() {
    return mediaTag.getParameter("Duration").toInt();
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

void DlnaMusicTrack::setTranscodeFormat(TranscodeFormatAvailable format) {
    if (transcodeFormat != format) {
        transcodeFormat = format;
        updateDLNAOrgPn();
    }
}

QProcess* DlnaMusicTrack::getTranscodeProcess(HttpRange *range) {

    if (!toTranscode()) {

        // use getStream instead of transcoding
        return 0;

    } else {

        QString program = "/Users/doudou/workspace/DLNA_server/exe/ffmpeg";

        QStringList arguments;
        if (range != 0 && !range->isNull()) {

            if (range->getStartByte() > 0) {
                int start_position = float(range->getStartByte())/size()*getLengthInSeconds();
                arguments << "-ss" << QString("%1").arg(start_position);
            }
        }

        arguments << "-i" << fileinfo.absoluteFilePath();
        arguments << "-map" <<  "0:a";

        if (transcodeFormat == MP3) {
            arguments << "-f" << "mp3";
            arguments << "-map_metadata" << "-1";
            arguments << "-ab" << QString("%1").arg(bitrate());

        } else if (transcodeFormat == LPCM) {
            arguments << "-f" << "s16be";
        }

        if (range != 0 && !range->isNull()) {

            if (range->getLength() > 0) {
                arguments << "-fs" << QString("%1").arg(range->getLength());
            } else {
                // invalid length
                arguments << "-fs 0";
            }

        }

        arguments << "pipe:";

        QProcess* transcodeProcess = new QProcess();
        transcodeProcess->setProgram(program);
        transcodeProcess->setArguments(arguments);

        return transcodeProcess;
    }
}

QIODevice* DlnaMusicTrack::getStream() {

    if (toTranscode()) {

        // DLNA node shall be transcoded
        return 0;

    } else {

        QFile* tmp = new QFile(fileinfo.absoluteFilePath());

        if (!tmp->open(QIODevice::ReadOnly)) {
            return 0;
        }
        else {
            return tmp;
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
