#ifndef MEDIARENDERERMODEL_H
#define MEDIARENDERERMODEL_H

#include <QDebug>
#include <QAbstractTableModel>
#include "mediarenderer.h"

class MediaRendererModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit MediaRendererModel(QObject *parent = 0);
    virtual ~MediaRendererModel();

    enum Roles {
        statusRole = Qt::UserRole+1,
        nameRole,
        networkAddressRole,
        userAgentRole,
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int,QByteArray> roleNames() const { return mRoles; }

    Q_INVOKABLE QVariant get(const int &index, const int &roleIndex) const;
    MediaRenderer* getFromIp(const QString &ip) const;

    // remove all renderers from the model
    void clearAll();


signals:


public slots:
    void addRendererInModel(MediaRenderer *renderer);
    void rendererChanged(const QString &roleChanged);

    // the renderer is serving a new media
    void serving(const QString &ip, const QString &mediaName);
    void stopServing(const QString &ip);

private:
    QList<MediaRenderer *> mRecords;
    QHash<int, QByteArray> mRoles;
};

#endif // MEDIARENDERERMODEL_H
