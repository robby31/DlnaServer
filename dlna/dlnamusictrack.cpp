#include "dlnamusictrack.h"

DlnaMusicTrack::DlnaMusicTrack(Logger* log, QString filename, QString host, int port):
    DlnaResource(log),
    fileinfo(QFileInfo(filename)),
    host(host),
    port(port),
    transcodeFormat(MP3)   // default transcode format
{    
    MI.Open(__T(filename.toStdString()));

    setDiscovered(true);

    QMimeDatabase db;
    mime_type = db.mimeTypeForFile(fileinfo);

    updateDLNAOrgPn();
}

DlnaMusicTrack::~DlnaMusicTrack() {
    MI.Close();
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
        return QString(MI.Get(MediaInfoDLL::Stream_General, 0, __T("OverallBitRate")).c_str()).toInt();
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
    int audioStreamCount = MI.Count_Get(MediaInfoDLL::Stream_Audio);
    if (audioStreamCount == 1) {
        return QString::fromStdString(MI.Get(MediaInfoDLL::Stream_Audio, 0, __T("Channel(s)"))).toInt();
    }
    return 0;
}

int DlnaMusicTrack::samplerate() {
    int audioStreamCount = MI.Count_Get(MediaInfoDLL::Stream_Audio);
    if (audioStreamCount == 1) {
        return QString::fromStdString(MI.Get(MediaInfoDLL::Stream_Audio, 0, __T("SamplingRate"))).toInt();
    }
    return 0;
}

int DlnaMusicTrack::getLengthInSeconds() {
    return getLengthInMilliSeconds()/1000;
}

int DlnaMusicTrack::getLengthInMilliSeconds() {
    return QString(MI.Get(MediaInfoDLL::Stream_General, 0, __T("Duration")).c_str()).toInt();
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
    QString title = MI.Get(MediaInfoDLL::Stream_General, 0, __T("Title")).c_str();
    if (title.isEmpty()) {
        title = getDisplayName();
    }
    dcTitle.appendChild(xml->createTextNode(title));
    xml_obj.appendChild(dcTitle);

    QDomElement upnpAlbum = xml->createElement("upnp:album");
    upnpAlbum.appendChild(xml->createTextNode(MI.Get(MediaInfoDLL::Stream_General, 0, __T("Album")).c_str()));
    xml_obj.appendChild(upnpAlbum);

    QDomElement upnpArtist = xml->createElement("upnp:artist");
    upnpArtist.appendChild(xml->createTextNode(MI.Get(MediaInfoDLL::Stream_General, 0, __T("Performer")).c_str()));
    xml_obj.appendChild(upnpArtist);

    QDomElement upnpCreator = xml->createElement("upnp:creator");
    upnpCreator.appendChild(xml->createTextNode(MI.Get(MediaInfoDLL::Stream_General, 0, __T("Performer")).c_str()));
    xml_obj.appendChild(upnpCreator);

    QDomElement upnpGenre = xml->createElement("upnp:genre");
    upnpGenre.appendChild(xml->createTextNode(MI.Get(MediaInfoDLL::Stream_General, 0, __T("Genre")).c_str()));
    xml_obj.appendChild(upnpGenre);

    QDomElement upnpTrackNumber = xml->createElement("upnp:originalTrackNumber");
    upnpTrackNumber.appendChild(xml->createTextNode(MI.Get(MediaInfoDLL::Stream_General, 0, __T("Track/Position")).c_str()));
    xml_obj.appendChild(upnpTrackNumber);

    QDomElement upnpDate = xml->createElement("upnp:date");
    upnpDate.appendChild(xml->createTextNode(fileinfo.lastModified().toString("yyyy-MM-ddThh:mm:ss")));
    xml_obj.appendChild(upnpDate);

    QImage picture = getAlbumArt();
    if (!picture.isNull()) {
        QDomElement upnpAlbumArtURI = xml->createElement("upnp:albumArtURI");
        upnpAlbumArtURI.setAttribute("xmlns:dlna", "urn:schemas-dlna-org:metadata-1-0/");
        upnpAlbumArtURI.setAttribute("dlna:profileID", "JPEG_TN");
        upnpAlbumArtURI.appendChild(xml->createTextNode(QString("http://%1:%2/get/%4/thumbnail0000%3&").arg(host).arg(port).arg(getDisplayName()).arg(getResourceId())));
        xml_obj.appendChild(upnpAlbumArtURI);
    }

    QDomElement upnpClass = xml->createElement("upnp:class");
    upnpClass.appendChild(xml->createTextNode("object.item.audioItem.musicTrack"));
    xml_obj.appendChild(upnpClass);

    QTime duration(0, 0, 0);
    QDomElement res = xml->createElement("res");
    res.setAttribute("xmlns:dlna", "urn:schemas-dlna-org:metadata-1-0/");
    res.setAttribute("protocolInfo", getProtocolInfo());
    res.setAttribute("bitrate", QString("%1").arg(bitrate()/1000));
    res.setAttribute("duration", QString("%1,00").arg(duration.addSecs(getLengthInSeconds()).toString()));
    res.setAttribute("sampleFrequency", QString("%1").arg(samplerate()));
    res.setAttribute("nrAudioChannels", QString("%1").arg(channelCount()));
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
        if (range != 0 && !range->isNull()) {

            if (range->getStartByte(size()) > 0) {
                int start_position = float(range->getStartByte(size()))/size()*getLengthInSeconds();
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

            if (range->getLength(size()) > 0) {
                arguments << "-fs" << QString("%1").arg(range->getLength(size()));
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

QImage DlnaMusicTrack::getAlbumArt() {
    QImage picture;

    QByteArray bytesPicture =  QByteArray::fromBase64(MI.Get(MediaInfoDLL::Stream_General, 0, __T("Cover_Data")).c_str());

    if (picture.loadFromData((const uchar *) bytesPicture.data(), bytesPicture.size())) {
        return picture;
    }

    return picture;
}
