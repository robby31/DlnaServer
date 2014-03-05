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

QString MetaData::getParameter(QString parameter, stream_t StreamKing, size_t StreamNumber) {
    String value = MI.Get(StreamKing, StreamNumber, parameter.toStdString());
    if (value.empty()) {
        return QString();
    } else {
        // detects encoding charset
        QTextCodec *codec = QTextCodec::codecForUtfText(value.c_str());

        return codec->toUnicode(value.c_str());
    }
}

QString MetaData::getAudioFormat(int audioStreamId) {
    return getParameter("Format", Stream_Audio, audioStreamId);
}

int MetaData::getChannelCount(int audioStreamId) {
    return getParameter("Channel(s)", Stream_Audio, audioStreamId).toInt();
}

int MetaData::getSamplingRate(int audioStreamId) {
    QString strSamplingRate = getParameter("SamplingRate", Stream_Audio, audioStreamId);
    return strSamplingRate.split(' ').at(0).toInt();
}

int MetaData::getAudioStreamCount() {
    return MI.Count_Get(Stream_Audio);
}

QStringList MetaData::getAudioLanguages() {
    QStringList languages;
    for (int audioStreamId = 0; audioStreamId < getAudioStreamCount(); audioStreamId++) {
        languages.append(getParameter("Language/String", Stream_Audio, audioStreamId));
    }
    return languages;
}

int MetaData::getVideoStreamCount() {
    return MI.Count_Get(Stream_Video);
}

int MetaData::getSubtitleStreamCount() {
    return MI.Count_Get(Stream_Text);
}

QString MetaData::getVideoCodec(int videoStreamId) {
    return getParameter("Codec/String", Stream_Video, videoStreamId);
}

QString MetaData::getVideoFrameRate() {
    QString framerate = getParameter("FrameRate", Stream_Video, 0);
    if (!framerate.isEmpty()) {
        return framerate;
    }

    return getParameter("FrameRate_Original", Stream_Video, 0);
}

QString MetaData::getResolution(int videoStreamId) {
    return QString("%1x%2").arg(getParameter("Width", Stream_Video, videoStreamId)).arg(getParameter("Height", Stream_Video, videoStreamId));
}

String MetaData::getCoverData() {
    return MI.Get(Stream_General, 0, __T("Cover_Data"));
}

QStringList MetaData::getSubtitleLanguages() {
    QStringList languages;
    for (int subtitleStreamId = 0; subtitleStreamId < getSubtitleStreamCount(); subtitleStreamId++) {
        languages.append(getParameter("Language/String", Stream_Text, subtitleStreamId));
    }
    return languages;
}
