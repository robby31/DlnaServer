#include "elapsedtimer.h"

ElapsedTimer::ElapsedTimer():
    timer(),
    m_elapsedBeforeStarting(0),
    m_elapsedFromStarting(0),
    isPaused(true)
{
    timer.invalidate();
}

void ElapsedTimer::start()
{
    if (isPaused) {
        if (timer.isValid()) {
            if (timer.elapsed() < 2000) {
                // cancel pause
                m_elapsedFromStarting += timer.restart();
                isPaused = false;
            } else {
                timer.start();
                isPaused = false;
            }
        } else {
            timer.start();
            isPaused = false;
        }
    }
}

void ElapsedTimer::pause()
{
    if (!isPaused && timer.isValid()) {
        m_elapsedFromStarting += timer.restart();
        isPaused = true;
    }
}

qint64 ElapsedTimer::elapsed() const
{
    if (!timer.isValid())
        return 0;

    if (isPaused)
        return m_elapsedFromStarting;
    else
        return timer.elapsed()+m_elapsedFromStarting;
}
