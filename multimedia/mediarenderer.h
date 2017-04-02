#ifndef MEDIARENDERER_H
#define MEDIARENDERER_H

#include "Models/listmodel.h"
#include "upnprootdevice.h"


class MediaRenderer : public ListItem
{
    Q_OBJECT

public:
    explicit MediaRenderer(QObject *parent = 0);
    explicit MediaRenderer(UpnpRootDevice *device, QObject *parent = 0);

    enum Roles {
        statusRole = Qt::UserRole+1,
        nameRole,
        networkAddressRole,
        iconUrlRole,
        availableRole
    };

    virtual QString id() const Q_DECL_OVERRIDE;

    virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    virtual QVariant data(int role) const Q_DECL_OVERRIDE;
    virtual bool setData(const QVariant &value, const int &role) Q_DECL_OVERRIDE;

    QString netWorkAddress() const;

signals:
    void removeRenderer();

private slots:
    void deviceItemChanged(QVector<int> roles);
    void deviceDestroyed(QObject *obj);

private:
    QHash<int, QByteArray> m_roles;

    UpnpRootDevice *m_device;
    QString status;
};

#endif // MEDIARENDERER_H
