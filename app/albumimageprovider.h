#ifndef ALBUMIMAGEPROVIDER_H
#define ALBUMIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QDebug>
#include "mysqldatabase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

class AlbumImageProvider : public QQuickImageProvider
{

public:
    AlbumImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) Q_DECL_OVERRIDE;

};

#endif // ALBUMIMAGEPROVIDER_H
