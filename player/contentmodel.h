#ifndef CONTENTMODEL_H
#define CONTENTMODEL_H

#include <QAbstractListModel>
#include <QTime>
#include "upnpservice.h"
#include "didllite.h"

class ContentModel : public QAbstractListModel
{
    Q_OBJECT

    enum Roles {
        objectIdRole = Qt::UserRole+1,
        childCountRole,
        parentIdRole,
        classRole,
        titleRole,
        creatorRole,
        resRole,
        restrictedRole,
        writeStatusRole,
        albumArtRole,
        artistRole,
        albumRole,
        genreRole,
        dateRole,
        resProtocolInfoRole,
        resDurationRole,
        itemDurationRoles,
        resSizeRole,
        xmlDataRole
    };

public:
    explicit ContentModel(QObject *parent = Q_NULLPTR);
    explicit ContentModel(UpnpService *service, const QString &objectId = QString(), QObject *parent = Q_NULLPTR);

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    Q_INVOKABLE ContentModel *getChildrenModel(const QString &objectId);

private:
    QString getParam(const QDomElement &item, const QString &name) const;
    QString resParam(const QDomElement &item, const QString &name) const;

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
    QString m_xmlData;
};

#endif // CONTENTMODEL_H
