#ifndef MENCODERTRANSCODING_H
#define MENCODERTRANSCODING_H

#include "transcodeprocess.h"

class MencoderTranscoding : public TranscodeProcess
{
    Q_OBJECT

public:
    explicit MencoderTranscoding(QObject *parent = 0);

private:
    virtual void updateArguments();

signals:

public slots:

private:
    static const QString PROGRAM;
};

#endif // MENCODERTRANSCODING_H
