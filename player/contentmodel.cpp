#include "contentmodel.h"

ContentModel::ContentModel(QObject *parent):
    QAbstractListModel(parent)
{

}

ContentModel::ContentModel(UpnpService *service, const QString &objectId, QObject *parent):
    QAbstractListModel(parent),
    m_service(service),
    m_objectId(objectId)
{
    m_roles[titleRole] = "title";
    m_roles[objectIdRole] = "objectId";
    m_roles[classRole] = "upnpClass";
    m_roles[childCountRole] = "childCount";
    m_roles[parentIdRole] = "parentID";
    m_roles[creatorRole] = "creator";
    m_roles[resRole] = "res";
    m_roles[restrictedRole] = "restricted";
    m_roles[writeStatusRole] = "writeStatus";
    m_roles[albumArtRole] = "albumArtURI";
    m_roles[artistRole] = "artist";
    m_roles[albumRole] = "album";
    m_roles[genreRole] = "genre";
    m_roles[dateRole] = "date";
    m_roles[resProtocolInfoRole] = "res@protocolInfo";
    m_roles[resDurationRole] = "res@duration";
    m_roles[itemDurationRoles] = "duration";
    m_roles[resSizeRole] = "res@size";

    if (service && !m_objectId.isEmpty())
    {
        SoapAction action(service->serviceType(), "Browse");
        action.addArgument("ObjectID", m_objectId);
        action.addArgument("BrowseFlag", "BrowseDirectChildren");
        action.addArgument("Filter", "*");
        action.addArgument("StartingIndex", "0");
        action.addArgument("RequestedCount", "0");
        action.addArgument("SortCriteria", "");

        UpnpActionReply *reply = service->runAction(action);
        connect(reply, &UpnpActionReply::finished, this, &ContentModel::actionFinished);
    }
}

int ContentModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.size();
}

QVariant ContentModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        if (index.row()>=0 && index.row()<m_items.size())
        {
            QDomElement item = m_items.at(index.row()).toElement();

            switch (role) {
            case objectIdRole:
            {
                return item.attribute("id");
            }

            case childCountRole:
            {
                return item.attribute("childCount");
            }

            case parentIdRole:
            {
                return item.attribute("parentID");
            }

            case classRole:
            {
                return getParam(item, "class");
            }

            case titleRole:
            {
                return getParam(item, "title");
            }

            case creatorRole:
            {
                return getParam(item, "creator");
            }

            case resRole:
            {
                return getParam(item, "res");
            }

            case restrictedRole:
            {
                return item.attribute("restricted");
            }

            case writeStatusRole:
            {
                return getParam(item, "writeStatus");
            }

            case albumArtRole:
            {
                return getParam(item, "albumArtURI");
            }

            case artistRole:
            {
                return getParam(item, "artist");
            }

            case albumRole:
            {
                return getParam(item, "album");
            }

            case genreRole:
            {
                return getParam(item, "genre");
            }

            case dateRole:
            {
                return getParam(item, "date");
            }

            case resProtocolInfoRole:
            {
                return resParam(item, "protocolInfo");
            }

            case resDurationRole:
            {
                return resParam(item, "duration");
            }

            case itemDurationRoles:
            {
                QTime duration = QTime::fromString(resParam(item, "duration"), "H:mm:ss");
                return QTime(0, 0, 0).msecsTo(duration);
            }

            case resSizeRole:
            {
                return resParam(item, "size");
            }

            default:
            {
                return QVariant::Invalid;
            }
            }
        }
    }

    return QVariant();
}

void ContentModel::actionFinished()
{
    auto reply = qobject_cast<UpnpActionReply *>(sender());

    if (reply)
    {
        SoapActionResponse *answer = reply->response();
        if (answer)
        {
            beginResetModel();

            m_updateId = answer->value("UpdateID");

            QDomDocument xmlDoc;
            xmlDoc.setContent(answer->value("Result"), true);
//            qWarning() << answer->value("Result");

            QDomNode root = xmlDoc.firstChild();
            if (!root.isNull())
            {
                m_items = root.childNodes();
            }

            endResetModel();
        }
        else
        {
            qCritical() << "invalid action response";
        }
    }
}

QHash<int, QByteArray> ContentModel::roleNames() const
{
    return m_roles;
}

ContentModel *ContentModel::getChildrenModel(const QString &objectId)
{
    return new ContentModel(m_service, objectId, this);
}

QString ContentModel::getParam(const QDomElement &item, const QString &name) const
{
    QDomNodeList l_elt = item.elementsByTagName(name);
    if (l_elt.size() == 1)
    {
        QDomElement elt = l_elt.at(0).toElement();
        return elt.firstChild().toText().data();
    }

    return QString();
}

QString ContentModel::resParam(const QDomElement &item, const QString &name) const
{
    QDomNodeList l_res = item.elementsByTagName("res");
    if (l_res.size() == 1)
    {
        QDomElement res = l_res.at(0).toElement();
        return res.attribute(name);
    }

    return QString();
}
