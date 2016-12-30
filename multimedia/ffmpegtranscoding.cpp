#include "ffmpegtranscoding.h"

QString FfmpegTranscoding::EXE_DIRPATH = "";

void FfmpegTranscoding::setDirPath(const QString &folder)
{
    EXE_DIRPATH = folder;
}

FfmpegTranscoding::FfmpegTranscoding(Logger *log, QObject *parent) :
    TranscodeProcess(log, parent),
    variable_bitrate(false)
{
    QDir folder(EXE_DIRPATH);
    setProgram(folder.absoluteFilePath("ffmpeg"));
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
//    arguments << "-loglevel" << "error";
    arguments << "-hide_banner";
    arguments << "-nostats";

//    arguments << "-probesize" << "5000";
    arguments << "-analyzeduration" << "0";

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

        arguments << "-f" << "ipod" << "-codec:a" << "libfdk_aac";

        if (bitrate() > 0)
            arguments << "-b:a" << QString("%1").arg(bitrate());
    }
    else if (format() == ALAC)
    {
        arguments << "-map" <<  "0:a";

        arguments << "-f" << "ipod" << "-codec:a" << "alac" << "-sample_fmt" << "s16p" << "-movflags" << "frag_keyframe+empty_moov+faststart";

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
    else if (format() == WAV)
    {
        arguments << "-map" <<  "0:a";

        arguments << "-f" << "wav";

        if (bitrate() > 0)
            arguments << "-b:a" << QString("%1").arg(bitrate());
    }
    else if (format() == MPEG2_AC3 or format() == H264_AAC or format() == H264_AC3)
    {
        if (!audioLanguages().contains("fre") && !audioLanguages().contains("fra"))
        {
            // select subtitle
            int subtitleStreamIndex = -1;
            if (subtitleLanguages().contains("fre"))
                subtitleStreamIndex = subtitleLanguages().indexOf("fre");
            else if (subtitleLanguages().contains("fra"))
                subtitleStreamIndex = subtitleLanguages().indexOf("fra");
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
            if (audioLanguages().contains("fre"))
                arguments << "-map" << QString("0:a:%1").arg(audioLanguages().indexOf("fre"));
            else if (audioLanguages().contains("fra"))
                arguments << "-map" << QString("0:a:%1").arg(audioLanguages().indexOf("fra"));
            arguments << "-map" << "0:v:0";
        }

        // set container format to MPEGTS
        arguments << "-f" << "mpegts";

//        arguments << "-s" << "1920x1080";
//        arguments << "-s" << "1280x720";
        arguments << "-s" << "736x414";
//        arguments << "-s" << "640x360";

        // set frame rate
        double framerate = 25.0;
        if (frameRate() == "23.976" or frameRate() == "24000/1001")
        {
            arguments << "-r" << "24000/1001";
            framerate = 23.976;
        }
        else if (frameRate() == "29.970" or frameRate() == "30000/1001")
        {
            arguments << "-r" << "30000/1001";
            framerate = 29.97;
        }
        else if (frameRate() == "30.000" or frameRate() == "30/1")
        {
            arguments << "-r" << "30";
            framerate = 30.0;
        }
        else if (frameRate() == "25.000" or frameRate() == "25/1")
        {
            arguments << "-r" << "25";
            framerate = 25.0;
        }
        else
        {
            // default framerate output
            arguments << "-r" << "25";
            framerate = 25.0;
            logInfo(QString("Use default framerate (%1)").arg(frameRate()));
        }

        // set audio options
        int audio_bitrate = 160000;  // default value for audio bitrate
        if (audioChannelCount() > 0)
            audio_bitrate = audioChannelCount()*80000;   // if audio channel count is valid, define bitrate = channel * 80kb/s

        if (format() == MPEG2_AC3 or format() == H264_AC3)
            arguments << "-c:a" << "ac3";
        else if (format() == H264_AAC)
            arguments << "-c:a" << "libfdk_aac";

        arguments << "-b:a" << QString("%1").arg(audio_bitrate);

        if (audioSampleRate() > 0)
            arguments << "-ar" << QString("%1").arg(audioSampleRate());

        if (audioChannelCount() > 0)
            arguments << "-ac" << QString("%1").arg(audioChannelCount());

        if (variable_bitrate)
            arguments << "-af" << "aresample=async=1000";

        // set video options
        if (format() == MPEG2_AC3)
        {
            arguments << "-c:v" << "mpeg2video";
        }
        else if (format() == H264_AAC or format() == H264_AC3)
        {
            arguments << "-c:v" << "libx264";

//            arguments << "-profile:v" << "baseline" << "-level" << "3.0";
//            arguments << "-preset" << "ultrafast";
            arguments << "-tune" << "zerolatency";
        }

        if (!variable_bitrate)
        {
            // constant bitrate
            int video_bitrate = (bitrate()-audio_bitrate);

            arguments << "-b:v" << QString("%1").arg(video_bitrate);
            arguments << "-minrate" << QString("%1").arg(video_bitrate);
            arguments << "-maxrate" << QString("%1").arg(video_bitrate);

            if (format() == H264_AAC or format() == H264_AC3)
            {
                arguments << "-bufsize" << QString("%1").arg(double(video_bitrate)/framerate);
                arguments << "-nal-hrd" << "cbr";
            }
            else
            {
                arguments << "-bufsize" << "1835k";
            }
        }
        else
        {
            // default : variable bitrate
            if (format() == H264_AAC or format() == H264_AC3)
            {
                arguments << "-crf" << "18";   // Very High quality
//                arguments << "-crf" << "23";   // High quality
//                arguments << "-crf" << "28";   // Low quality

            }
            else
            {
                arguments << "-qscale:v" << "3";
            }
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

    // normalize audio
//    arguments << "-af" << "dynaudnorm";

    arguments << "pipe:";

    setArguments(arguments);
}

void FfmpegTranscoding::setVariableBitrate(const bool &flag)
{
    variable_bitrate = flag;
}
