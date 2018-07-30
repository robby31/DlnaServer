#ifndef CONTENTMODEL_H
#define CONTENTMODEL_H

#include <QAbstractListModel>
#include "upnpservice.h"
#include "didllite.h"

class ContentModel : public QAbstractListModel
{
    Q_OBJECT

    enum Roles {
        titleRole = Qt::UserRole+1,
        objectIdRole,
        childCountRole,
        parentIdRole,
        classRole
    };

public:
    explicit ContentModel(QObject *parent = Q_NULLPTR);
    explicit ContentModel(UpnpService *service, const QString &objectId = QString(), QObject *parent = Q_NULLPTR);

    virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    Q_INVOKABLE ContentModel *getChildrenModel(const QString &objectId);

signals:
    void childrenModelChanged();

private slots:
    void actionFinished();

private:
    QHash<int, QByteArray> m_roles;
    UpnpService *m_service = Q_NULLPTR;
    QString m_updateId;
    QString m_objectId;
    QDomNodeList m_items;
};

#endif // CONTENTMODEL_H
