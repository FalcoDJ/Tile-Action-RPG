#ifndef TIMER_HPP
#define TIMER_HPP

#include "../olcPixelGameEngine.h"

class Timer: public olc::PGEX
{
private:
    float m_Duration = 0.0f;
    float m_TimeLeft = 0.0f;
    bool m_Running = false;
public:
    Timer() : olc::PGEX(true) {}

    void Restart()
    {
        m_TimeLeft = m_Duration;
    }
    void Start(float Duration)
    {
        m_Running = true;
        m_TimeLeft = Duration;
        m_Duration = Duration;
    }

    float GetTimeLeft()
    {
        return m_TimeLeft;
    }

    void OnBeforeUserUpdate(float &fElapsedTime) override
    {
        if (m_Running)
        {
            m_TimeLeft -= fElapsedTime;
            if (m_TimeLeft <= 0.0f)
            {
                m_Running = false;
            }
        }
    }
};

#endif