#include "dlnacachednetworkvideo.h"

DlnaCachedNetworkVideo::DlnaCachedNetworkVideo(Logger* log, MediaLibrary* library, int idMedia, QString host, int port, QObject *parent):
    DlnaCachedVideo(log, library, idMedia, host, port, parent)
{
}

TranscodeProcess *DlnaCachedNetworkVideo::getTranscodeProcess()
{
    FfmpegTranscoding* transcodeProcess = new FfmpegTranscoding(log());

    // set the url for transcoding
    QProcess process;
    process.setProgram("/usr/local/bin/youtube-dl");
    QStringList argList;
    argList << "--max-quality" << "18" << "-g" << getSystemName();
    process.setArguments(argList);

    if (process.open())
    {
        if (process.waitForFinished())
        {
            QString url = process.readAllStandardOutput().trimmed();
            if (url.startsWith("http"))
                transcodeProcess->setUrl(url);
        }
        else
        {
            logError("ERROR: unable to get url.");
            process.kill();
            process.waitForFinished();
        }
    }

    transcodeProcess->setLengthInSeconds(getLengthInSeconds());
    transcodeProcess->setFormat(transcodeFormat);
    transcodeProcess->setBitrate(bitrate());
    transcodeProcess->setAudioLanguages(audioLanguages());
    transcodeProcess->setSubtitleLanguages(subtitleLanguages());
    transcodeProcess->setFrameRate(framerate());
    transcodeProcess->setAudioChannelCount(channelCount());
    transcodeProcess->setAudioSampleRate(samplerate());
    return transcodeProcess;
}
