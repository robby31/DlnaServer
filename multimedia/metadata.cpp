#include "metadata.h"

MetaData::MetaData(QString filename, QObject *parent) :
    QObject(parent),
    flagOpened(false)
{
    if (MI.Open(filename.toStdString()) == 1) {
        flagOpened = true;
    } else {
        qWarning() << "NOT OPENED" << filename;
    }
}

MetaData::~MetaData() {
    if (flagOpened) {
        MI.Close();
    }
}

QString MetaData::getParameter(QString parameter, MediaInfoDLL::stream_t StreamKing, size_t StreamNumber) {
    MediaInfoDLL::String value = MI.Get(StreamKing, StreamNumber, parameter.toStdString());
    return QString::fromStdString(value);
}

QString MetaData::getAudioFormat(int audioStreamId) {
    return getParameter("Format", MediaInfoDLL::Stream_Audio, audioStreamId);
}

int MetaData::getChannelCount(int audioStreamId) {
    return getParameter("Channel(s)", MediaInfoDLL::Stream_Audio, audioStreamId).toInt();
}

int MetaData::getSamplingRate(int audioStreamId) {
    QString strSamplingRate = getParameter("SamplingRate", MediaInfoDLL::Stream_Audio, audioStreamId);
    return strSamplingRate.split(' ').at(0).toInt();
}

int MetaData::getAudioStreamCount() {
    return MI.Count_Get(MediaInfoDLL::Stream_Audio);
}

int MetaData::getVideoStreamCount() {
    return MI.Count_Get(MediaInfoDLL::Stream_Video);
}

QString MetaData::getVideoCodec(int videoStreamId) {
    return getParameter("Codec/String", MediaInfoDLL::Stream_Video, videoStreamId);
}

QString MetaData::getResolution(int videoStreamId) {
    return QString("%1x%2").arg(getParameter("Width", MediaInfoDLL::Stream_Video, videoStreamId)).arg(getParameter("Height", MediaInfoDLL::Stream_Video, videoStreamId));
}

MediaInfoDLL::String MetaData::getCoverData() {
    return MI.Get(MediaInfoDLL::Stream_General, 0, __T("Cover_Data"));
}
