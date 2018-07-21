#ifndef DEBUGITEM_H
#define DEBUGITEM_H

#include <QHash>
#include <QVariant>
#include <QDebug>
#include "Models/listitem.h"
#include "dlna/dlnaresource.h"
#include "device.h"
#include "dlna/dlnafolder.h"
#include "dlna/dlnamusictrackfile.h"
#include "dlna/dlnarootfolder.h"
#include "dlna/dlnavideofile.h"
#include "dlna/dlnayoutubevideo.h"

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

    virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE { return m_roles; }
    virtual QVariant data(int role) const Q_DECL_OVERRIDE;
    virtual bool setData(const QVariant &value, const int &role) Q_DECL_OVERRIDE;

private:
    QVariant counterValue() const;

signals:

public slots:

private:
    QHash<int, QByteArray> m_roles;
    QString m_name;
};

#endif // DEBUGITEM_H
