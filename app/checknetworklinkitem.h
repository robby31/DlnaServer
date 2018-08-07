#ifndef CHECKNETWORKLINKITEM_H
#define CHECKNETWORKLINKITEM_H

#include <QHash>
#include <QVariant>
#include <QDebug>

#include "Models/listitem.h"

class CheckNetworkLinkItem : public ListItem
{
    enum Roles {
        NameRole = Qt::UserRole+1,
        MessageRole
    };

public:
    explicit CheckNetworkLinkItem(QObject *parent = Q_NULLPTR);
    explicit CheckNetworkLinkItem(const QString &name, const QString &message, QObject *parent = Q_NULLPTR);

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE { return m_roles; }
    QVariant data(int role) const Q_DECL_OVERRIDE;
    bool setData(const QVariant &value, const int &role) Q_DECL_OVERRIDE;

private:
    QHash<int, QByteArray> m_roles;
    QString m_name;
    QString m_message;
};

#endif // CHECKNETWORKLINKITEM_H
