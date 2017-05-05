#ifndef DLNACACHEDFOLDER_H
#define DLNACACHEDFOLDER_H

#include "dlnastoragefolder.h"
#include "dlnacachedvideo.h"
#include "dlnacachednetworkvideo.h"
#include "dlnacachedmusictrack.h"

#include "medialibrary.h"

#include <QDir>

class DlnaCachedFolder : public DlnaStorageFolder
{
    Q_OBJECT

public:
    explicit DlnaCachedFolder(MediaLibrary* library,
                              QSqlQuery query,
                              QString name, QString host, int port, bool cacheEnabled = false, int maxSize = -1, QObject *parent = 0);

    virtual DlnaResource* getChild(int index, QObject *parent = 0);
    virtual int getChildrenSize() const { return nbChildren; }

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const { return name; }

    virtual QString getSystemName() const { return name; }

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() const { return name; }

    void setLimitSizeMax(const int &size) { limitSizeMax = size; refreshContent(); }

    void setNetworkAccessManager(QNetworkAccessManager *manager) { m_nam = manager; }

private:
    virtual void refreshContent();

private:
    MediaLibrary* library;
    QString name;
    QSqlQuery query;
    int nbChildren;
    bool cacheEnabled;
    QList<int> cache;
    int limitSizeMax;
    QNetworkAccessManager *m_nam;
};

#endif // DLNACACHEDFOLDER_H
