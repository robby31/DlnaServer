#ifndef ELAPSEDTIMER_H
#define ELAPSEDTIMER_H

#include <QElapsedTimer>
#include <QDebug>

class ElapsedTimer
{
public:
    ElapsedTimer();

    void invalidate() { timer.invalidate(); }
    bool isValid() const { return timer.isValid(); }

    void start();
    void pause();
    bool isStatePaused() { return isPaused; }

    qint64 elapsed() const;
    qint64 elapsedFromBeginning() const { return m_elapsedBeforeStarting+elapsed(); }
    void addMSec(const qint64 &time) { m_elapsedBeforeStarting += time; }

private:
    QElapsedTimer timer;
    qint64 m_elapsedBeforeStarting;
    qint64 m_elapsedFromStarting;
    bool isPaused;
};

#endif // ELAPSEDTIMER_H
