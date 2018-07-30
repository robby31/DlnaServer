#ifndef UPNPCLASSIMAGEPROVIDER_H
#define UPNPCLASSIMAGEPROVIDER_H

#include <QDebug>
#include <QQuickImageProvider>

class UpnpClassImageProvider : public QQuickImageProvider
{
public:
    UpnpClassImageProvider();

    virtual QImage requestImage(const QString &id, QSize *size, const QSize& requestedSize) Q_DECL_OVERRIDE;
};

#endif // UPNPCLASSIMAGEPROVIDER_H
