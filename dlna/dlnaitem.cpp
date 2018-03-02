#include "dlnaitem.h"

DlnaItem::DlnaItem(QString host, int port, QObject *parent) :
    DlnaResource(parent),
    host(host),
    port(port),
    transcodeFormat(UNKNOWN),  // default transcode format
    dlnaOrgOpFlags("01"),      // seek by byte (exclusive)
    dlnaOrgPN(),
    m_userAgent(),
    m_stream(Q_NULLPTR)
{
}

DlnaItem::~DlnaItem()
{
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
    if (metaDataDuration() > getResumeTime())
        return metaDataDuration() - getResumeTime();
    else
        return 0;
}

void DlnaItem::setTranscodeFormat(TranscodeFormatAvailable format) {
    if (transcodeFormat != format) {
        transcodeFormat = format;
        updateDLNAOrgPn();

        if (toTranscode()) {
            setdlnaOrgOpFlags("10");         // seek by time (exclusive)
        }
    }
}

Device *DlnaItem::getStream()
{
    if (m_stream)
    {
        qDebug() << "stream in cache" << m_stream << m_stream->isOpen();
        if (!m_stream->isOpen())
        {
            return m_stream;
        }
    }

    if (toTranscode())
    {
        // DLNA node shall be transcoded
        TranscodeDevice* process = getTranscodeProcess();

        if (process)
        {
            process->setBitrate(bitrate());
        }

        setStream(process);

        return process;
    }
    else
    {
        Device* tmp = getOriginalStreaming();

        setStream(tmp);

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

qint64 DlnaItem::size() {
    if (toTranscode())
    {
        Device *stream = getStream();
        if (stream)
            return stream->size();
        else
            return -1;
    }
    else
    {
        return sourceSize();
    }
}

void DlnaItem::setStream(Device *stream)
{
    if (stream)
    {
        if (m_stream)
            m_stream->deleteLater();

        connect(stream, SIGNAL(destroyed(QObject*)), this, SLOT(streamDestroyed(QObject*)));
        connect(this, SIGNAL(destroyed(QObject*)), stream, SLOT(deleteLater()));
        m_stream = stream;
    }
    else
    {
        qCritical() << "invalid stream" << stream;
    }
}

void DlnaItem::streamDestroyed(QObject *obj)
{
    qDebug() << "stream destroyed" << obj;
    m_stream = Q_NULLPTR;
}
