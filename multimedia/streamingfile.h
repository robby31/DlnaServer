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
    virtual ~StreamingFile() { }

    virtual qint64 size() const;
    virtual bool atEnd() const;
    virtual qint64 bytesAvailable() const;

    virtual bool open(OpenMode mode);
    virtual qint64 readData(char *data, qint64 maxlen);

    void setRange(HttpRange *range)            { m_range = range; }
    void setTimeSeek(qint64 start, qint64 end) { timeseek_start = start; timeseek_end = end;
                                                 if (timeseek_start != -1 or timeseek_end != -1) qWarning() << "TimeSeek Option not taken into account for media" << fileName(); }

signals:
    void LogMessage(const QString &msg);
    void status(const QString &status);
    void errorRaised(const QString &errorString);

public slots:

private:
    HttpRange *m_range;
    qint64 timeseek_start;
    qint64 timeseek_end;
};

#endif // STREAMINGFILE_H
