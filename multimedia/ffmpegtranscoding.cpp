#include "ffmpegtranscoding.h"

const QString FfmpegTranscoding::PROGRAM = QString("/opt/local/bin/ffmpeg");

FfmpegTranscoding::FfmpegTranscoding(Logger *log, QObject *parent) :
    TranscodeProcess(log, parent)
{
    setProgram(PROGRAM);
}

void FfmpegTranscoding::updateArguments()
{
    QString ssOption;
    if (timeSeekStart() > 0) {
        ssOption = QString("%1").arg(timeSeekStart());
        if (range())
            logWarning("timeseek and range are used in the same time, only timeseek is taken into account.");
    }
    else if (range() != 0 && !range()->isNull())
    {
        if (range()->getStartByte() > 0 && lengthInSeconds() > 0) {
            double start_position = double(range()->getStartByte())/double(range()->getSize())*double(lengthInSeconds());
            ssOption = QString("%1").arg(long(start_position));
        }
    }

    QStringList arguments;
//    arguments << "-loglevel" << "warning";
    arguments << "-hide_banner";
    arguments << "-nostats";

    if (!ssOption.isEmpty())
        arguments << "-ss" << ssOption;

    if (!url().isEmpty())
        arguments << "-i" << url();

    if (format() == MP3)
    {
        arguments << "-map" <<  "0:a";

        arguments << "-f" << "mp3" << "-codec:a" << "libmp3lame";

        if (bitrate() > 0)
            arguments << "-b:a" << QString("%1").arg(bitrate());

    }
    else if (format() == AAC)
    {
        arguments << "-map" <<  "0:a";

        arguments << "-strict" << "-2" << "-f" << "mpegts" << "-codec:a" << "aac" << "-movflags" << "+faststart";

        if (bitrate() > 0)
            arguments << "-b:a" << QString("%1").arg(bitrate());
    }
    else if (format() == LPCM)
    {
        arguments << "-map" <<  "0:a";

        arguments << "-f" << "s16be";

        if (bitrate() > 0)
            arguments << "-b:a " << QString("%1").arg(bitrate());
    }
    else if (format() == MPEG2_AC3 or format() == MPEG4_AAC)
    {
        if (!audioLanguages().contains("fre"))
        {
            // select subtitle
            int subtitleStreamIndex = -1;
            if (subtitleLanguages().contains("fre"))
                subtitleStreamIndex = subtitleLanguages().indexOf("fre");
            else if (subtitleLanguages().contains("eng"))
                subtitleStreamIndex = subtitleLanguages().indexOf("eng");

            if (subtitleStreamIndex >= 0)
            {
                if (ssOption.isEmpty())
                    arguments << "-vf" << QString("subtitles=%1:si=%2").arg(url()).arg(subtitleStreamIndex);
                else
                    arguments << "-vf" << QString("setpts=PTS+%3/TB,subtitles=%1:si=%2,setpts=PTS-STARTPTS").arg(url()).arg(subtitleStreamIndex).arg(ssOption);
            }


            if (audioLanguages().contains("eng"))
            {
                // select english language for audio
                arguments << "-map" << QString("0:a:%1").arg(audioLanguages().indexOf("eng"));
                arguments << "-map" << "0:v:0";
            }
        }
        else
        {
            // select french language for audio
            arguments << "-map" << QString("0:a:%1").arg(audioLanguages().indexOf("fre"));
            arguments << "-map" << "0:v:0";
        }

        // set container format to MPEG
        arguments << "-f" << "mpegts";

        // set audio options
        int audio_bitrate = 256000;
        if (format() == MPEG2_AC3)
            arguments << "-c:a" << "ac3";
        else if (format() == MPEG4_AAC)
            arguments << "-c:a" << "libfdk_aac";
        arguments << "-b:a" << QString("%1").arg(audio_bitrate);
        if (audioSampleRate()>0)
            arguments << "-ar" << QString("%1").arg(audioSampleRate());
        if (audioChannelCount()>0)
            arguments << "-ac" << QString("%1").arg(audioChannelCount());

        // set video options
        int video_bitrate = -1;
        if (format() == MPEG2_AC3)
        {
            video_bitrate = (bitrate()-audio_bitrate)/1.09256;
            arguments << "-c:v" << "mpeg2video";
        }
        else if (format() == MPEG4_AAC)
        {
            video_bitrate = bitrate()/1.0851 - audio_bitrate;
            arguments << "-c:v" << "libx264";
            arguments << "-profile:v" << "baseline" << "-level" << "3.0";
            arguments << "-preset" << "veryfast";
//            arguments << "-tune" << "zerolatency";
//            arguments << "-movflags" << "+faststart";
        }

        if (video_bitrate>0)
        {
            arguments << "-b:v" << QString("%1").arg(video_bitrate);
            arguments << "-minrate" << QString("%1").arg(video_bitrate) << "-maxrate" << QString("%1").arg(video_bitrate);
            arguments << "-bufsize" << QString("%1").arg(video_bitrate*3/25);   // bufsize = 3 * bitrate / fps
        }
    }
    else
    {
        logError(QString("Invalid format: %1").arg(format()));
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
        arguments << "-to" << QString("%1").arg(timeSeekEnd());
    }

    arguments << "pipe:";

    setArguments(arguments);
}
