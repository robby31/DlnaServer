#include "ffmpegtranscoding.h"

FfmpegTranscoding::FfmpegTranscoding(Logger *log, QObject *parent) :
    TranscodeProcess(log, parent)
{
}

bool FfmpegTranscoding::initialize(HttpRange *range, const long &timeseek_start, const long &timeseek_end, const QString &filePath, const int &lengthInSeconds, const TranscodeFormatAvailable &transcodeFormat, const int &bitrate)
{
    QString program = "/Users/doudou/workspaceQT/DLNA_server/exe/ffmpeg";

    QStringList arguments;
    if (timeseek_start > 0)
    {
        arguments << "-ss" << QString("%1").arg(timeseek_start);
        if (range)
            m_log->Warning("timeseek and range are used in the same time, only timeseek is taken into account.");
    }
    else if (range != 0 && !range->isNull())
    {
        if (range->getStartByte() > 0)
        {
            double start_position = double(range->getStartByte())/double(range->getSize())*double(lengthInSeconds);
            arguments << "-ss" << QString("%1").arg(long(start_position));
        }
    }

    arguments << "-i" << filePath;
    arguments << "-map" <<  "0:a";

    if (transcodeFormat == MP3)
    {
        arguments << "-f" << "mp3";
        arguments << "-map_metadata" << "-1";
        arguments << "-ab" << QString("%1").arg(bitrate);

    }
    else if (transcodeFormat == LPCM)
    {
        arguments << "-f" << "s16be";
    }
    else
    {
        // invalid transcode format
        return false;
    }

    if (range != 0 && !range->isNull())
    {
        if (range->getLength() > 0)
        {
            if (range->getHighRange() >= 0)
                arguments << "-fs" << QString("%1").arg(range->getLength());
        }
        else
        {
            // invalid length
            arguments << "-fs 0";
        }
    }
    else if (timeseek_end > 0)
    {
        arguments << "-fs" << QString("%1").arg(timeseek_end);
    }

    arguments << "pipe:";

    setProgram(program);
    setArguments(arguments);

    return true;
}
