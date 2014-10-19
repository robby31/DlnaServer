#include "streamingfile.h"

StreamingFile::StreamingFile(QString filename, QObject *parent) :
    QFile(filename, parent),
    m_range(0),
    timeseek_start(-1),
    timeseek_end(-1)
{
}

qint64 StreamingFile::size() const
{
    if (m_range && m_range->getLength()!=-1)
        return m_range->getLength();

    return QFile::size();
}

bool StreamingFile::atEnd() const
{
    if (m_range && m_range->getEndByte()!=-1)
        return pos() > m_range->getEndByte();

    return QFile::atEnd();
}

qint64 StreamingFile::readData(char *data, qint64 maxlen)
{
    if (m_range && pos() < m_range->getStartByte())
        seek(m_range->getStartByte());

    if (m_range && m_range->getEndByte()>0)
    {
        int bytesToRead = m_range->getEndByte() - pos() + 1;
        if (bytesToRead>=0 && bytesToRead<maxlen)
            return QFile::readData(data, bytesToRead);
    }

    return QFile::readData(data, maxlen);
}
