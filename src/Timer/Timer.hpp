#ifndef TIMER_HPP
#define TIMER_HPP

#include "../olcPixelGameEngine.h"

class Timer: public olc::PGEX
{
private:
    float m_Duration = 0.0f;
    float m_TimeLeft = 0.0f;
    bool m_Running = false;
    bool m_WasIRunningLastFrame = false;

public:
    Timer() : olc::PGEX(true) {}

    void Restart()
    {
        m_TimeLeft = m_Duration;
        m_Running = true;
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

    bool Running()
    {
        return m_Running;
    }

    void OnBeforeUserUpdate(float &fElapsedTime) override
    {
        m_WasIRunningLastFrame = m_Running;
        
        if (m_Running)
        {
            m_TimeLeft -= fElapsedTime;
            if (m_TimeLeft <= 0.0f)
            {
                m_Running = false;
                m_TimeLeft = 0.0f;
            }
        }
    }

    bool JustFinished()
    {
        return (m_WasIRunningLastFrame && !m_Running); // If I just finished return true;
    }
};

class StopWatch : public olc::PGEX
{
private:
    float m_ElapsedTime = 0.0f;
    bool m_Running = false;
    bool m_WasIRunningLastFrame = false;

public:
    StopWatch() : olc::PGEX(true) {}

    void Start()
    {
        m_ElapsedTime = 0.0f;
        m_Running = true;
    }

    void Stop()
    {
        m_Running = false;
    }

    float GetElapsedTime()
    {
        return m_ElapsedTime;
    }

    bool Running()
    {
        return m_Running;
    }

    void OnBeforeUserUpdate(float &fElapsedTime) override
    {
        m_WasIRunningLastFrame = m_Running;

        if (m_Running)
        {
            m_ElapsedTime += fElapsedTime;
        }
    }
    
    bool JustFinished()
    {
        return (m_WasIRunningLastFrame && !m_Running); // If I just finished return true;
    }
};

#endif