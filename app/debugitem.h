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
#include "dlna/dlnanetworkvideo.h"
#include "dlna/dlnanetworkplaylist.h"
#include "dlna/dlnastoragefolder.h"
#include "dlna/cached/dlnacachedfolder.h"
#include "dlna/cached/dlnacachedfoldermetadata.h"
#include "dlna/cached/dlnacachedgroupedfoldermetadata.h"
#include "dlna/cached/dlnacachedplaylists.h"

class DebugItem : public ListItem
{
    Q_OBJECT

public:
    enum Roles {
        NameRole = Qt::UserRole+1,
        CounterRole
    };

    explicit DebugItem(QObject *parent = Q_NULLPTR);
    explicit DebugItem(const QString &name, QObject *parent = Q_NULLPTR);

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE { return m_roles; }
    QVariant data(int role) const Q_DECL_OVERRIDE;
    bool setData(const QVariant &value, const int &role) Q_DECL_OVERRIDE;

private:
    QVariant counterValue() const;

signals:

public slots:

private:
    QHash<int, QByteArray> m_roles;
    QString m_name;
};

#endif // DEBUGITEM_H
