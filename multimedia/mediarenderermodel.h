#ifndef MEDIARENDERERMODEL_H
#define MEDIARENDERERMODEL_H

#include <QDebug>
#include "mediarenderer.h"
#include "Models/listmodel.h"
#include "ssdpmessage.h"
#include <QHostAddress>

class MediaRendererModel : public ListModel
{
    Q_OBJECT

public:
    explicit MediaRendererModel(QObject *parent = 0);

    MediaRenderer* getFromIp(const QString &ip) const;


signals:


private slots:
    void addMediaRenderer(const QHostAddress &ip, const int &port, const SsdpMessage &info);

    // the renderer is serving a new media
    void serving(const QString &ip, const QString &mediaName);
    void stopServing(const QString &ip);
};

#endif // MEDIARENDERERMODEL_H
