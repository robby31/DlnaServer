#ifndef HTTPRANGE_H
#define HTTPRANGE_H

/*
 * Http range specified here: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.35
 */

#include <QObject>
#include <QRegExp>

class HttpRange : public QObject
{
    Q_OBJECT

public:
    explicit HttpRange(QObject *parent = 0);
    explicit HttpRange(QString range, QObject *parent = 0);

    bool isNull() const { return null; }

    QString getUnit() const { return unit; }

    long getLowRange() const { return lowRange; }

    long getHighRange() const { return highRange; }

    // returns the position of the first byte of the range
    long getStartByte() const;

    // returns the position of the last byte of the range
    long getEndByte() const;

    // returns the length of the range
    long getLength() const;

    // Set the size which is the total size in bytes of the data stream where is applied the range
    void setSize(long size) { this->size = size; }

    // returns the size of the data stream
    long getSize() { return this->size; }

signals:

public slots:

private:
    bool null;
    QString unit;
    long lowRange;
    long highRange;
    long size;

    QRegExp rxRange;

};

#endif // HTTPRANGE_H
