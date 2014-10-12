#ifndef STREAMINGFILE_H
#define STREAMINGFILE_H

#include <QFile>
#include <QDebug>
#include <httprange.h>

class StreamingFile : public QFile
{
    Q_OBJECT

public:
    explicit StreamingFile(QString filename, QObject *parent = 0);

    virtual qint64 size() const;
    virtual bool atEnd() const;

    virtual qint64 readData(char *data, qint64 maxlen);

    void setRange(HttpRange *range);

signals:
    void LogMessage(const QString &msg);
    void status(const QString &status);
    void errorRaised(const QString &errorString);

public slots:

private:
    HttpRange *m_range;
};

#endif // STREAMINGFILE_H
