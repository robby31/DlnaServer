#ifndef MEDIAMODEL_H
#define MEDIAMODEL_H

#include "SqlModel/sqltablemodel.h"

class MediaModel : public SqlTableModel
{
    Q_OBJECT

public:
    explicit MediaModel(QObject *parent = Q_NULLPTR);

private:
    bool removeForeign(const QString &table, const int &id);

public slots:
    bool remove(const int &index, const int &count = 1) Q_DECL_OVERRIDE;

};

#endif // MEDIAMODEL_H
