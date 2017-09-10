#ifndef FORMATIMAGEPROVIDER_H
#define FORMATIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QDebug>
#include "mysqldatabase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

class FormatImageProvider : public QQuickImageProvider
{
public:
    FormatImageProvider();

    virtual QImage requestImage(const QString &id, QSize *size, const QSize& requestedSize) Q_DECL_OVERRIDE;
};

#endif // FORMATIMAGEPROVIDER_H
