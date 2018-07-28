#ifndef SERVERITEM_H
#define SERVERITEM_H

#include "Models/listitem.h"
#include "upnprootdevice.h"

class ServerItem : public ListItem
{
    Q_OBJECT

public:
    explicit ServerItem(QObject *parent = 0);
    explicit ServerItem(UpnpRootDevice *device, QObject *parent = 0);

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

signals:

private slots:
    void deviceItemChanged(QVector<int> roles);
    void deviceDestroyed(QObject *obj);

private:
    QHash<int, QByteArray> m_roles;

    UpnpRootDevice *m_device = Q_NULLPTR;
    QString status;
};

#endif // SERVERITEM_H
