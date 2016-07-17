#ifndef DEBUGITEM_H
#define DEBUGITEM_H

#include <QHash>
#include <QVariant>
#include <QDebug>
#include "Models/listitem.h"
#include "dlnaresource.h"
#include "httpclient.h"
#include "request.h"
#include "reply.h"
#include "device.h"
#include "dlnafolder.h"
#include "dlnamusictrackfile.h"
#include "dlnarootfolder.h"
#include "dlnavideofile.h"
#include "dlnayoutubevideo.h"

class DebugItem : public ListItem
{
    Q_OBJECT

public:
    enum Roles {
        NameRole = Qt::UserRole+1,
        CounterRole
    };

    explicit DebugItem(QObject *parent = 0);
    explicit DebugItem(const QString &name, QObject *parent = 0);

    virtual QHash<int, QByteArray> roleNames() const { return m_roles; }
    virtual QVariant data(int role) const;
    virtual bool setData(const QVariant &value, const int &role);

private:
    QVariant counterValue() const;

signals:

public slots:

private:
    QHash<int, QByteArray> m_roles;
    QString m_name;
};

#endif // DEBUGITEM_H