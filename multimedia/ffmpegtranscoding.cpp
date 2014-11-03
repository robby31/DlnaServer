#include "ffmpegtranscoding.h"

const QString FfmpegTranscoding::PROGRAM = QString("/opt/local/bin/ffmpeg");

FfmpegTranscoding::FfmpegTranscoding(Logger *log, QObject *parent) :
    TranscodeProcess(log, parent),
    m_lengthInSeconds(-1),
    m_format(MP3)          // Default format
{
    setProgram(PROGRAM);
}

void FfmpegTranscoding::updateArguments()
{
    QStringList arguments;
    if (timeSeekStart() > 0)
    {
        arguments << "-ss" << QString("%1").arg(timeSeekStart());
        if (range())
            m_log->Warning("timeseek and range are used in the same time, only timeseek is taken into account.");
    }
    else if (range() != 0 && !range()->isNull())
    {
        if (range()->getStartByte() > 0 && m_lengthInSeconds > 0)
        {
            double start_position = double(range()->getStartByte())/double(range()->getSize())*double(m_lengthInSeconds);
            arguments << "-ss" << QString("%1").arg(long(start_position));
        }
    }

    if (!url().isEmpty())
        arguments << "-i" << url();
    arguments << "-map" <<  "0:a";

    if (m_format == MP3)
    {
        arguments << "-f" << "mp3";
        arguments << "-map_metadata" << "-1";
        if (bitrate() > 0)
            arguments << "-ab" << QString("%1").arg(bitrate());

    }
    else if (m_format == LPCM)
    {
        arguments << "-f" << "s16be";
    }
    else
    {
        m_log->Error(QString("Invalid format: %1").arg(m_format));
    }

    if (range() != 0 && !range()->isNull())
    {
        if (range()->getLength() > 0)
        {
            if (range()->getHighRange() >= 0)
                arguments << "-fs" << QString("%1").arg(range()->getLength());
        }
        else
        {
            // invalid length
            arguments << "-fs 0";
        }
    }
    else if (timeSeekEnd() > 0)
    {
        arguments << "-fs" << QString("%1").arg(timeSeekEnd());
    }

    arguments << "pipe:";

    setArguments(arguments);
}
