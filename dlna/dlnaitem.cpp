#include "dlnaitem.h"

DlnaItem::DlnaItem(Logger *log, QString host, int port, QObject *parent) :
    DlnaResource(log, parent),
    host(host),
    port(port),
    transcodeFormat(UNKNOWN),  // default transcode format
    dlnaOrgOpFlags("01"),      // seek by byte (exclusive)
    dlnaOrgPN(),
    m_userAgent()
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

Device *DlnaItem::getStream(HttpRange *range, qint64 timeseek_start, qint64 timeseek_end)
{
    if (toTranscode())
    {
        // DLNA node shall be transcoded
        TranscodeProcess* process = getTranscodeProcess();
        if (process->url().isEmpty())
            process->setUrl(getSystemName());
        process->setRange(range);
        process->setTimeSeek(timeseek_start, timeseek_end);
        process->setBitrate(bitrate());
        process->setSize(size());
        return process;
    }
    else
    {
        StreamingFile* tmp = new StreamingFile(getSystemName(), log());
        tmp->setRange(range);
        tmp->setTimeSeek(timeseek_start, timeseek_end);
        return tmp;
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
    if (toTranscode())
        result << "DLNA.ORG_CI=1";
    else
        result << "DLNA.ORG_CI=0";
    result << "DLNA.ORG_FLAGS=01700000000000000000000000000000";

    return result.join(";");
}

qint64 DlnaItem::size() const {
    if (toTranscode()) {
        if (bitrate() != -1 && getLengthInMilliSeconds() != -1)
        {
            return double(bitrate())*double(getLengthInMilliSeconds())/8000.0;
        } else {
            // variable bitrate, we don't know exactly the size
            return -1;
        }
    } else {
        return sourceSize();
    }
}
