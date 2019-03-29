#include "mediamodel.h"

MediaModel::MediaModel(QObject *parent):
    SqlTableModel("MEDIA_DATABASE", parent)
{
    setEditStrategy(OnFieldChange);
    setTable("media");
}

bool MediaModel::remove(const int &index, const int &count)
{
    if (count >= 1)
    {
        // remove foreign dependencies
        for (int row=index;row<(index+count);++row)
        {
            QSqlRecord elt = record(row);
            int mediaId = elt.value("id").toInt();
            if (mediaId >= 0)
            {
                if (!removeForeign("param_value", mediaId))
                    return false;
                if (!removeForeign("media_in_playlists", mediaId))
                    return false;
            }
        }
    }

    return SqlTableModel::remove(index, count);
}

bool MediaModel::removeForeign(const QString &table, const int &id)
{
    QSqlQuery query(database());

    if (!query.prepare(QString("DELETE FROM %1 WHERE media=:id").arg(table)))
    {
        qCritical() << query.lastError().text();
        return  false;
    }

    query.bindValue(":id", id);
    if (!query.exec())
    {
        qCritical() << query.lastError().text();
        return  false;
    }

    return true;
}
