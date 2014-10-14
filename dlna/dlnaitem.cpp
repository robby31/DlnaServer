#include "dlnaitem.h"

DlnaItem::DlnaItem(Logger *log, QString host, int port, QObject *parent) :
    DlnaResource(log, parent),
    host(host),
    port(port),
    transcodeFormat(UNKNOWN),  // default transcode format
    dlnaOrgOpFlags("01"),      // seek by byte (exclusive)
    dlnaOrgPN()
{
}

DlnaItem::~DlnaItem() {

}

QString DlnaItem::getDisplayName() const {
    QString title = metaDataTitle();
    if (title.isEmpty())
        title = getName();
    if (title.isEmpty())
        title = getSystemName();
    return title;
}

int DlnaItem::getLengthInSeconds() const {
    return qRound(double(getLengthInMilliSeconds())/1000.0);
}

int DlnaItem::getLengthInMilliSeconds() const {
    return metaDataDuration();
}

void DlnaItem::setTranscodeFormat(TranscodeFormatAvailable format) {
    if (transcodeFormat != format) {
        transcodeFormat = format;
        updateDLNAOrgPn();
    }
}

QIODevice *DlnaItem::getStream(HttpRange *range, long timeseek_start, long timeseek_end, QObject *parent)
{
    if (toTranscode())
    {
        // DLNA node shall be transcoded
        TranscodeProcess* process = getTranscodeProcess(range, timeseek_start, timeseek_end, parent != 0 ? parent : this);

        // set buffer size to 10 seconds
        process->setBufferSize(bitrate()/8*10);

        process->launch();

        return process;
    }
    else
    {
        StreamingFile* tmp = new StreamingFile(getSystemName(),
                                               parent != 0 ? parent : this);

        if (!tmp->open(QIODevice::ReadOnly))
        {
            return 0;
        }
        else
        {
            tmp->setRange(range);
            return tmp;
        }
    }
}

QString DlnaItem::getProtocolInfo() const {
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

qint64 DlnaItem::size() const {
    if (toTranscode()) {
        if (bitrate() != -1) {
            return double(bitrate())*double(getLengthInMilliSeconds())/8000.0;
        } else {
            // variable bitrate, we don't know exactly the size
            return -1;
        }
    } else {
        return sourceSize();
    }
}
