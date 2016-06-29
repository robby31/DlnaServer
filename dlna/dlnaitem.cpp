#include "dlnaitem.h"

DlnaItem::DlnaItem(Logger *log, QString host, int port, QObject *parent) :
    DlnaResource(log, parent),
    host(host),
    port(port),
    transcodeFormat(UNKNOWN),  // default transcode format
    dlnaOrgOpFlags("01"),      // seek by byte (exclusive)
    dlnaOrgPN(),
    m_userAgent(),
    overheadFactor(1.0)
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
    return metaDataDuration() - getResumeTime();
}

void DlnaItem::setTranscodeFormat(TranscodeFormatAvailable format) {
    if (transcodeFormat != format) {
        transcodeFormat = format;
        updateDLNAOrgPn();

        if (toTranscode()) {
            setdlnaOrgOpFlags("10");         // seek by time (exclusive)

            if (transcodeFormat == MP3)
                overheadFactor = 1.000222;
            else if (transcodeFormat == LPCM)
                overheadFactor = 1.00001;
            else if (transcodeFormat == AAC)
                overheadFactor = 1.01;
            else if (transcodeFormat == ALAC)
                overheadFactor = 1.08;
            else
                overheadFactor = 1.0;
        }
    }
}

Device *DlnaItem::getStream(HttpRange *range, qint64 timeseek_start, qint64 timeseek_end)
{
    if (toTranscode())
    {
        // DLNA node shall be transcoded
        TranscodeProcess* process = getTranscodeProcess();

        if (process)
        {
            if (process->url().isEmpty())
                process->setUrl(getSystemName());
            process->setRange(range);
            process->setTimeSeek(timeseek_start, timeseek_end);
            process->setBitrate(bitrate());
            process->setSize(size());
        }

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

QString DlnaItem::getProtocolInfo() const
{
    return QString("http-get:*:%1:%2").arg(mimeType()).arg(getDlnaContentFeatures());
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

    return result.join(";");
}

qint64 DlnaItem::size() const {
    if (toTranscode()) {
        if (bitrate() != -1 && getLengthInMilliSeconds() != -1)
        {
            return overheadFactor*double(bitrate())*double(getLengthInMilliSeconds())/8000.0;
        } else {
            // variable bitrate, we don't know exactly the size
            return -1;
        }
    } else {
        return sourceSize();
    }
}
