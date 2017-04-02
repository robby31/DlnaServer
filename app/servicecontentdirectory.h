#ifndef SERVICECONTENTDIRECTORY_H
#define SERVICECONTENTDIRECTORY_H

#include <QObject>
#include "cached/dlnacachedrootfolder.h"
#include "Http/httprequest.h"
#include "soapaction.h"
#include "soapactionresponse.h"
#include "didllite.h"

class ServiceContentDirectory : public QObject
{
    Q_OBJECT

public:
    explicit ServiceContentDirectory(Logger* log, QString host, int port, QObject *parent = 0);

    void setNetworkAccessManager(QNetworkAccessManager *nam);

    void reply(HttpRequest *request);

signals:
    void addFolderSignal(QString folder);
    void folderAdded(QString folder);
    void error_addFolder(QString folder);

    void addNetworkLinkSignal(const QString url);
    void linkAdded(QString url);
    void error_addNetworkLink(QString url);
    void brokenLink(QString url, QString title);

    void reloadLibrarySignal(const QStringList localFolder);

    void updateMediaFromId(const int &id, const QHash<QString, QVariant> &data);

    void getDLNAResourcesSignal(QObject *sender, QString objectId, bool returnChildren, int start, int count, QString searchStr);
    void dlnaResources(QObject* sender, QList<DlnaResource*>);

    void servingRendererSignal(QString ip, const QString &mediaName);

    // emit signal to provide progress on serving media
    void servingSignal(QString host, QString filename, int playedDurationInMs);

    // emit signal when serving is finished
    //   status = 0 if serving finished successfully
    //   status = 1 if error occurs
    void servingFinishedSignal(QString host, QString filename, int status);


private slots:
    void _addFolder(const QString &folder);

    void folderAddedSlot(QString folder);

    void reloadLibrary();

    void requestDLNAResourcesSignal(QString objectId, bool returnChildren, int start, int count, QString searchStr);

    void readTimeSeekRange(const QString &data, qint64 *start, qint64*end);


private:
    DlnaCachedRootFolder rootFolder;

    // root folder containing DLNA nodes
    QStringList listFolderAdded;
};

#endif // SERVICECONTENTDIRECTORY_H