#ifndef PLAYER_ENTITY_HPP
#define PLAYER_ENTITY_HPP

#include "../Enitity.hpp"

class Player : public Entity
{
private:
    olc::Key Left = olc::Key::A, Right = olc::Key::D, Up = olc::Key::W, Down = olc::Key::S, Run = olc::Key::SHIFT, Attack_Key = olc::Key::SPACE;
    
    float m_DurationOfAttack = 0.3f;
    olc::vf2d m_AttackNormal = {0,0};
    float m_HitboxRadius = 1.5f;
    
    float m_ChargeDuration = 0.75f;
    StopWatch m_ButtonClock;

    float m_DefaultSpeed = 1.0;
    float m_RunSpeed = 1.5;
    float m_ChargeSpeed = 0.5;

public:
    Player()
    {
        m_Layer = "player_layer";

        m_DeathPhaseDuration = 0.2f;
        m_InvincibilityDuration = 0.75f;

        m_Bounds->radius = .5;
        m_Speed = 5;

        Setup();

        m_HurtBox->radius = .4;
    }
    ~Player(){}

    void GetHurt() override
    {
        if (m_Bounds->vel.mag2() > 0)
        {
            m_Bounds->vel = movement_vector * -1 * m_HurtBox->magnitude;
        }
        m_Health -= m_HurtBox->magnitude;
        std::cout << m_Health << "\n";
        m_State = EntityState::INVINCIBLE;
        m_InvincibilityTimer.Start(m_InvincibilityDuration);
    }

    void Attack() override
    {
        HitBoxHandler::CreateHitBox(m_Bounds->pos + m_AttackNormal * 1.4, m_HitboxRadius, 10, HBType::damage, m_HurtBox->layer);
    }

    void handleInput(olc::PixelGameEngine* pge)
    {
        movement_vector = olc::vf2d::ZERO();
        
        if (m_AmIAlive)
        {
            // Reset Attack Normal
            if (!m_AttackTimer.Running())
                m_AttackNormal = olc::vf2d::ZERO();

            // Get Direction of movement
            int x_value = (int)pge->GetKey(Right).bHeld - (int)pge->GetKey(Left).bHeld;
            int y_value = (int)pge->GetKey(Down).bHeld - (int)pge->GetKey(Up).bHeld;
            
            // Set Direction of movement
            movement_vector = olc::vf2d(x_value,y_value);

            // Are we moving?
            if (movement_vector.mag2() > 0)
            {
                // We are so...

                movement_vector = movement_vector.norm() * (pge->GetKey(Run).bHeld ? m_RunSpeed : m_DefaultSpeed);

                // Set attack normal if not attacking
                if (!m_AttackTimer.Running())
                    m_AttackNormal = movement_vector.norm();
            }

            // Handle Attacking
            if (!m_AttackTimer.Running())
            {
                // Start m_ButtonClock when button pressed
                if (!m_ButtonClock.Running() && (pge->GetMouse(0).bHeld || pge->GetKey(Attack_Key).bHeld))
                {
                    m_ButtonClock.Start();
                }

                // Stop m_ButtonClock when button released
                if (pge->GetMouse(0).bReleased || pge->GetKey(Attack_Key).bReleased)
                {
                    m_ButtonClock.Stop();
                }

                // Decide how to attack based on duration of button press

                // Light Attack
                if (m_ButtonClock.GetElapsedTime() <= m_ChargeDuration && m_ButtonClock.JustFinished())
                {
                    m_HitboxRadius = 1.75f;
                    m_AttackTimer.Start(m_DurationOfAttack);
                }

                // Charge Attack
                if (m_ButtonClock.GetElapsedTime() >= m_ChargeDuration && m_ButtonClock.JustFinished())
                {
                    m_HitboxRadius = 2.5f;
                    m_AttackNormal = olc::vf2d::ZERO();
                    m_AttackTimer.Start(m_DurationOfAttack);
                }

            }
        }

        // Turn color based on state(s)
        col = olc::GREEN;
        if (m_State == EntityState::INVINCIBLE)
            col = olc::YELLOW;

        // Turn blue if charged
        if (m_ButtonClock.Running() && m_ButtonClock.GetElapsedTime() >= m_ChargeDuration)
        {
            col = olc::CYAN;
        }
    }
};

#endif