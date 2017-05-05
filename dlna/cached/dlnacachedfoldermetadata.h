#ifndef DLNACACHEDFOLDERMETADATA_H
#define DLNACACHEDFOLDERMETADATA_H

#include "dlnastoragefolder.h"
#include "dlnacachedfolder.h"

#include "medialibrary.h"

class DlnaCachedFolderMetaData : public DlnaStorageFolder
{
    Q_OBJECT

public:
    explicit DlnaCachedFolderMetaData(MediaLibrary* library,
                                      QString stringQuery,
                                      QString stringQueryForChild,
                                      QString name, QString host, int port,
                                      QObject *parent = 0);

    virtual DlnaResource* getChild(int index, QObject *parent = 0);
    virtual int getChildrenSize() const { return nbChildren; }

    // Any resource needs to represent the container or item with a String.
    // String to be showed in the UPNP client.
    virtual QString getName() const { return m_name; }

    virtual QString getSystemName() const { return m_name; }

    // Returns the DisplayName that is shown to the Renderer.
    virtual QString getDisplayName() const { return m_name; }

    void setNetworkAccessManager(QNetworkAccessManager *manager) { m_nam = manager; }


private:
    MediaLibrary* library;
    QString m_name;
    QSqlQuery query;
    QString stringQueryForChild;
    int nbChildren;
    QNetworkAccessManager *m_nam;
};

#endif // DLNACACHEDFOLDERMETADATA_H
