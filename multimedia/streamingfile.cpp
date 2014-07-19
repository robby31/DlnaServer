#include "streamingfile.h"

StreamingFile::StreamingFile(QString filename, QObject *parent) :
    QObject(parent),
    file(filename, this),
    m_range(0)
{
}

void StreamingFile::setRange(HttpRange *range)
{
    m_range = range;

    if (m_range && m_range->getStartByte()>0)
        file.seek(m_range->getStartByte());
}

qint64 StreamingFile::size() const
{
    if (m_range && m_range->getLength()!=-1)
        return m_range->getLength();

    return file.size();
}

bool StreamingFile::atEnd() const
{
    if (m_range && m_range->getEndByte()!=-1)
        return file.pos() > m_range->getEndByte();

    return file.atEnd();
}

qint64 StreamingFile::read(char *data, const qint64 &maxSize)
{
    if (atEnd())
        return 0;

    if (m_range && m_range->getEndByte()>0) {
        int bytesToRead = m_range->getEndByte() - file.pos() + 1;
        if (bytesToRead>=0 && bytesToRead<maxSize)
            return file.read(data, bytesToRead);
    }

    return file.read(data, maxSize);
}
