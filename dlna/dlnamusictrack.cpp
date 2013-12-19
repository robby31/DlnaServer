#include "dlnamusictrack.h"

DlnaMusicTrack::DlnaMusicTrack(Logger* log, QString filename, QString host, int port):
    DlnaResource(log),
    fileinfo(QFileInfo(filename)),
    taglibFile(fileinfo.absoluteFilePath().toUtf8()),
    host(host),
    port(port),
    transcodeFormat(LPCM)   // default transcode format
{    
    setDiscovered(true);

    QMimeDatabase db;
    mime_type = db.mimeTypeForFile(fileinfo);

    updateDLNAOrgPn();
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

QString DlnaMusicTrack::getDisplayName() const {
    return fileinfo.completeBaseName();
}

int DlnaMusicTrack::bitrate() {
    if (toTranscode()) {
        if (transcodeFormat == MP3) {
            return 320;

        } else if (transcodeFormat == LPCM) {

            if (taglibFile.audioProperties()->sampleRate() == 44100) {
                return 1411;

            } else {
                return 1536;
            }
        }

        return 0;
    } else {
        return taglibFile.audioProperties()->bitrate();
    }
}

int DlnaMusicTrack::size() {
    if (toTranscode()) {
        if (transcodeFormat == MP3) {
            return 320000*(taglibFile.audioProperties()->length()+1)/8;

        } else if (transcodeFormat == LPCM) {

            if (taglibFile.audioProperties()->sampleRate() == 44100) {
                return 1411*1000*(taglibFile.audioProperties()->length()+1)/8;

            } else {
                return 1536*1000*(taglibFile.audioProperties()->length()+1)/8;
            }
        }

        return 0;
    } else {
        return fileinfo.size();
    }
}

/*
* Returns XML (DIDL) representation of the DLNA node. It gives a
* complete representation of the item, with as many tags as available.
*
* Reference: http://www.upnp.org/specs/av/UPnP-av-ContentDirectory-v1-Service.pdf
*/
QDomElement DlnaMusicTrack::getXmlContentDirectory(QDomDocument *xml) {
    QDomElement xml_obj;

    xml_obj = xml->createElement("item");
    xml_obj.setAttribute("id", getResourceId());
    xml_obj.setAttribute("parentID", getParentId());
    xml_obj.setAttribute("restricted", "true");

    QDomElement dcTitle = xml->createElement("dc:title");
    QString title = taglibFile.tag()->title().toCString(true);
    if (title.isEmpty()) {
        title = getDisplayName();
    }
    dcTitle.appendChild(xml->createTextNode(title));
    xml_obj.appendChild(dcTitle);

    QDomElement upnpAlbum = xml->createElement("upnp:album");
    upnpAlbum.appendChild(xml->createTextNode(taglibFile.tag()->album().toCString(true)));
    xml_obj.appendChild(upnpAlbum);

    QDomElement upnpArtist = xml->createElement("upnp:artist");
    upnpArtist.appendChild(xml->createTextNode(taglibFile.tag()->artist().toCString(true)));
    xml_obj.appendChild(upnpArtist);

    QDomElement upnpCreator = xml->createElement("upnp:creator");
    upnpCreator.appendChild(xml->createTextNode(taglibFile.tag()->artist().toCString(true)));
    xml_obj.appendChild(upnpCreator);

    QDomElement upnpGenre = xml->createElement("upnp:genre");
    upnpGenre.appendChild(xml->createTextNode(taglibFile.tag()->genre().toCString(true)));
    xml_obj.appendChild(upnpGenre);

    QDomElement upnpTrackNumber = xml->createElement("upnp:originalTrackNumber");
    upnpTrackNumber.appendChild(xml->createTextNode(QString("%1").arg(taglibFile.tag()->track())));
    xml_obj.appendChild(upnpTrackNumber);

    QDomElement upnpDate = xml->createElement("upnp:date");
    upnpDate.appendChild(xml->createTextNode(fileinfo.lastModified().toString("yyyy-MM-ddThh:mm:ss")));
    xml_obj.appendChild(upnpDate);

    QDomElement upnpClass = xml->createElement("upnp:class");
    upnpClass.appendChild(xml->createTextNode("object.item.audioItem.musicTrack"));
    xml_obj.appendChild(upnpClass);

    QTime duration(0, 0, 0);
    QDomElement res = xml->createElement("res");
    res.setAttribute("xmlns:dlna", "urn:schemas-dlna-org:metadata-1-0/");
    res.setAttribute("protocolInfo", getProtocolInfo());
    res.setAttribute("bitrate", QString("%1").arg(bitrate()));
    res.setAttribute("duration", QString("%1,00").arg(duration.addSecs(taglibFile.audioProperties()->length()).toString()));
    res.setAttribute("sampleFrequency", QString("%1").arg(taglibFile.audioProperties()->sampleRate()));
    res.setAttribute("nrAudioChannels", QString("%1").arg(taglibFile.audioProperties()->channels()));
    res.setAttribute("size", QString("%1").arg(size()));
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
        arguments << "-i" << fileinfo.absoluteFilePath();
        if (transcodeFormat == MP3) {
            arguments << "-f" << "mp3";
            arguments << "-ab" << "320000";

            if (range == 0) {
                // no range requested
            } else if (range->isNull()) {
                // invalid range
            } else {
                // range is valid
                if (range->getLength(size()) > 0) {
                    arguments << "-fs" << QString("%1").arg(range->getLength(size()));
                }
            }

        } else if (transcodeFormat == LPCM) {
            arguments << "-f" << "s16be";
        }
        arguments << "pipe:";

        QProcess* transcodeProcess = new QProcess();
        transcodeProcess->setProgram(program);
        transcodeProcess->setArguments(arguments);

        return transcodeProcess;
    }
}

QByteArray DlnaMusicTrack::getStream(HttpRange* range) {

    if (toTranscode()) {

        // DLNA node shall be transcoded
        return QByteArray();

    } else {

        QFile tmp(fileinfo.absoluteFilePath());

        if (!tmp.open(QIODevice::ReadOnly)) {
            return QByteArray();
        }
        else {
            if (range == 0) {
                // no range requested
                return tmp.readAll();
            } else if (range->isNull()) {
                // invalid range
                return tmp.readAll();
            } else {
                // range is valid
                if (range->getStartByte(size()) > 0) {
                    if (!tmp.seek(range->getStartByte(size()))) {
                        getLog()->ERROR(QString("Unable to set position %1 on stream").arg(range->getStartByte(size())));
                    }
                }
                return tmp.read(range->getLength(size()));
            }
        }
    }
}

QString DlnaMusicTrack::mimeType() const {
    if (toTranscode()) {
        // Trancode music track
        if (transcodeFormat == MP3) {
            return "audio/mpeg";
        } else if (transcodeFormat == LPCM) {
            return "audio/L16";
        }
    }
    return mime_type.name();
}