#ifndef MEDIARENDERERMODEL_H
#define MEDIARENDERERMODEL_H

#include <QDebug>
#include "mediarenderer.h"
#include "Models/listmodel.h"

class MediaRendererModel : public ListModel
{
    Q_OBJECT

public:
    explicit MediaRendererModel(QObject *parent = 0);

    MediaRenderer* getFromIp(const QString &ip) const;


signals:


private slots:
    void addRenderer(const QString &ip, const int &port, const QString &userAgent);

    // the renderer is serving a new media
    void serving(const QString &ip, const QString &mediaName);
    void stopServing(const QString &ip);
};

#endif // MEDIARENDERERMODEL_H
