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

            case classRole: {
                QDomNodeList l_class = item.elementsByTagName("class");
                if (l_class.size() == 1)
                {
                    QDomElement upnpClass = l_class.at(0).toElement();
                    return upnpClass.firstChild().toText().data();
                }
            }

            case titleRole:
            {
                QDomNodeList l_title = item.elementsByTagName("title");
                if (l_title.size() == 1)
                {
                    QDomElement title = l_title.at(0).toElement();
                    return title.firstChild().toText().data();
                }
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
    UpnpActionReply *reply = (UpnpActionReply *)(sender());

    if (reply)
    {
        SoapActionResponse *answer = reply->response();
        if (answer)
        {
            beginResetModel();

            m_updateId = answer->value("UpdateID");

            QDomDocument xmlDoc;
            xmlDoc.setContent(answer->value("Result"), true);

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
