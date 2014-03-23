#include "dlnaitem.h"

DlnaItem::DlnaItem(Logger *log, QString filename, QString host, int port, QObject *parent) :
    DlnaResource(log, parent),
    fileinfo(filename),
    host(host),
    port(port),
    transcodeFormat(UNKNOWN),  // default transcode format
    dlnaOrgOpFlags("01"),      // seek by byte (exclusive)
    dlnaOrgPN()
{
    QMimeDatabase db;
    mime_type = db.mimeTypeForFile(fileinfo);
}

DlnaItem::~DlnaItem() {

}

QString DlnaItem::getDisplayName() {
    QString title = metaDataTitle();
    if (title.isEmpty())
        return fileinfo.completeBaseName();
    else
        return title;
}

long DlnaItem::size() {
    if (toTranscode()) {
        if (bitrate() != -1) {
            return double(bitrate())*double(getLengthInMilliSeconds())/8000.0;
        } else {
            // variable bitrate, we don't know exactly the size
            return -1;
        }
    } else {
        return fileinfo.size();
    }
}

int DlnaItem::getLengthInSeconds() {
    return qRound(double(getLengthInMilliSeconds())/1000.0);
}

int DlnaItem::getLengthInMilliSeconds() {
    return metaDataDuration();
}

void DlnaItem::setTranscodeFormat(TranscodeFormatAvailable format) {
    if (transcodeFormat != format) {
        transcodeFormat = format;
        updateDLNAOrgPn();
    }
}

QIODevice* DlnaItem::getStream(QObject *parent) {

    if (toTranscode()) {

        // DLNA node shall be transcoded
        return 0;

    } else {

        QFile* tmp = new QFile(fileinfo.absoluteFilePath(),
                               parent != 0 ? parent : this);

        if (!tmp->open(QIODevice::ReadOnly)) {
            return 0;
        }
        else {
            return tmp;
        }
    }
}

QString DlnaItem::getProtocolInfo() {
    QStringList result;

    if (!getdlnaOrgPN().isNull()) {
        result << QString("DLNA.ORG_PN=%1").arg(getdlnaOrgPN());
    }

    result << QString("DLNA.ORG_OP=%1").arg(getdlnaOrgOpFlags());

    return QString("http-get:*:%1:").arg(mimeType()) + result.join(";");
}

QString DlnaItem::getDlnaContentFeatures() const {
    QStringList result;

    if (!getdlnaOrgPN().isNull()) {
        result << QString("DLNA.ORG_PN=%1").arg(getdlnaOrgPN());
    }

    result << QString("DLNA.ORG_OP=%1").arg(getdlnaOrgOpFlags());
    result << "DLNA.ORG_CI=0";
    result << "DLNA.ORG_FLAGS=01700000000000000000000000000000";

    return result.join(";");
}
