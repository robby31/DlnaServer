#ifndef SSDPMESSAGE_H
#define SSDPMESSAGE_H

#include <QStringList>
#include <QVariant>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

enum SsdpFormat { NOTIFY, SEARCH, HTTP, INVALID };

class SsdpMessage
{
public:
    SsdpMessage();
    SsdpMessage(SsdpFormat format);

    bool addHeader(const QString &param, const QString &value);
    QString getHeader(const QString &param) const;

    SsdpFormat format();
    QString startLine();

    QByteArray toUtf8() const;
    QStringList toStringList() const;

    static SsdpMessage fromByteArray(const QByteArray &input);

    // Carriage return and line feed.
    static const QString CRLF;

private:
    bool addRawHeader(const QString &data);

private:
    QStringList m_header;
};

#endif // SSDPMESSAGE_H
