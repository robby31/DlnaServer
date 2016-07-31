#ifndef CREATEDATABASETHREAD_H
#define CREATEDATABASETHREAD_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include "mysqldatabase.h"

class CreateDatabaseThread : public QObject
{
    Q_OBJECT

public:
    explicit CreateDatabaseThread(QObject *parent = 0);
    virtual ~CreateDatabaseThread() Q_DECL_OVERRIDE;

    void run() { emit runSignal(); }

signals:
    void runSignal();

private slots:
    void runSlot();
};

#endif // CREATEDATABASETHREAD_H
