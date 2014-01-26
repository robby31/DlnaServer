#ifndef MEDIARENDERERMODEL_H
#define MEDIARENDERERMODEL_H

#include <QAbstractTableModel>
#include "mediarenderer.h"

class MediaRendererModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit MediaRendererModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int,QByteArray> roleNames() const;

    // Create a new mediarenderer and add it to the model
    MediaRenderer* addRenderer(Logger* log, QString ip, int port, QString userAgent);

    Q_INVOKABLE QVariant get(int index, int roleIndex);
    MediaRenderer* getFromIp(QString ip);

    // remove all renderers from the model
    void clearAll();

    // the renderer is serving a new media
    void serving(QString ip, QString mediaName);
    void stopServing(QString ip);

public slots:
    void rendererChanged();

private:
    QList<MediaRenderer *> mRecords;

    QHash<int, QByteArray> mRoles;
};

#endif // MEDIARENDERERMODEL_H
