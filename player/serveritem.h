#ifndef SERVERITEM_H
#define SERVERITEM_H

#include "Models/listitem.h"
#include "upnprootdevice.h"
#include "contentmodel.h"

class ServerItem : public ListItem
{
    Q_OBJECT

    Q_PROPERTY(ContentModel *contentModel READ contentModel WRITE setContentModel NOTIFY contentModelChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QUrl iconurl READ iconurl NOTIFY iconurlChanged)

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

    UpnpRootDevice *device() const;

    ContentModel *contentModel() const;
    void setContentModel(ContentModel *model);

    QString name() const;
    QUrl iconurl() const;

private:
    void initializeContentDirectory();

signals:
    void removeItem();
    void contentModelChanged();
    void nameChanged();
    void iconurlChanged();

private slots:
    void deviceItemChanged(QVector<int> roles);
    void deviceDestroyed(QObject *obj);

private:
    QHash<int, QByteArray> m_roles;

    UpnpRootDevice *m_device = Q_NULLPTR;
    QString status;
    UpnpService *m_contentDirectory = Q_NULLPTR;
    ContentModel *m_contentModel = Q_NULLPTR;
};

#endif // SERVERITEM_H
