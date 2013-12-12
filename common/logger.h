#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>

class Logger
{
public:
    Logger();

    void setLevel(int level);

    void TRACE(QString message) const;
    void DEBUG(QString message) const;
    void INFO(QString message) const;
    void ERROR(QString message) const;
    void WARNING(QString message) const;

private:
    // LOG level:
    //    0: TRACE
    //    1: DEBUG
    //    2: INFO
    //    3: WARNING
    //    4: ERROR
    int level;
};

#endif // LOGGER_H
