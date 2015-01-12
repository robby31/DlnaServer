#ifndef MEDIARENDERER_H
#define MEDIARENDERER_H

#include "Models/listmodel.h"


class MediaRenderer : public ListItem
{
    Q_OBJECT

public:
    explicit MediaRenderer(QObject *parent = 0);
    explicit MediaRenderer(const QString &ip, const int &port, const QString &userAgent, QObject *parent = 0);

    enum Roles {
        statusRole = Qt::UserRole+1,
        nameRole,
        networkAddressRole,
        userAgentRole,
    };

    virtual QHash<int, QByteArray> roleNames() const { return m_roles; }
    virtual QVariant data(int role) const;
    virtual bool setData(const QVariant &value, const int &role);

signals:

private slots:

private:
    QHash<int, QByteArray> m_roles;

    QString ip;
    int port;
    QString userAgent;
    QString status;
    QString name;
};

#endif // MEDIARENDERER_H
