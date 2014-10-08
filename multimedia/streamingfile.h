#ifndef STREAMINGFILE_H
#define STREAMINGFILE_H

#include <QFile>
#include <QDebug>
#include <httprange.h>

class StreamingFile : public QObject
{
    Q_OBJECT

public:
    explicit StreamingFile(QString filename, QObject *parent = 0);

    bool open(const QIODevice::OpenMode &mode) { return file.open(mode); }
    bool isOpen() const { return file.isOpen(); }

    qint64 pos() const { return file.pos(); }
    qint64 size() const;
    bool atEnd() const;

    QByteArray read(const qint64 &maxSize);

    void setRange(HttpRange *range);

signals:

public slots:

private:
    QFile file;
    HttpRange *m_range;
};

#endif // STREAMINGFILE_H
