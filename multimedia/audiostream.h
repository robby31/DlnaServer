#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

#include <QObject>

class AudioStream : public QObject
{
    Q_OBJECT
public:
    explicit AudioStream(QObject *parent = 0);

signals:

public slots:
};

#endif // AUDIOSTREAM_H