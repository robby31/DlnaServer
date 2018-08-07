#ifndef MEDIAIMAGEPROVIDER_H
#define MEDIAIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QDebug>
#include "mysqldatabase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

class MediaImageProvider : public QQuickImageProvider
{
public:
    MediaImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize& requestedSize) Q_DECL_OVERRIDE;
};

#endif // MEDIAIMAGEPROVIDER_H
