#ifndef ENEMY_ENTITY_HPP
#define ENEMY_ENTITY_HPP

#include "../Enitity.hpp"

enum class EnemyState {
    IDLE=0,
    CHASE=1,
    WANDER=2
};

class Enemy : public Entity
{
private:
    float m_Max_Chase_Radius = 3;
    EnemyState m_eState = EnemyState::IDLE;
    olc::vf2d m_PlayerPos = olc::vf2d::ZERO();
    
    Timer m_TimeSinceLastWander;

private:
    void SetupAnimations() override
    {

    }

    void wander()
    {
        m_eState = EnemyState::IDLE;
    }

    bool can_chase_player()
    {
        return true;
    }

    void chase_player()
    {
        movement_vector = olc::vf2d::ZERO();

        if (m_AmIAlive)
        {
            movement_vector = m_PlayerPos - m_Bounds->pos;

            if (movement_vector.mag2() > m_Max_Chase_Radius * m_Max_Chase_Radius)
                movement_vector = olc::vf2d::ZERO();
            else
            {
                movement_vector = movement_vector.norm();
                Attack();
            }
        }

        col = olc::RED;
        if (m_State == EntityState::INVINCIBLE)
            col = olc::MAGENTA;
    }

    void BeforeUpdate() override
    {
        switch (m_eState)
        {
            case EnemyState::IDLE:
                movement_vector = olc::vf2d::ZERO();

                if (!m_TimeSinceLastWander.Running())
                m_eState = EnemyState::WANDER;

                m_eState = EnemyState::CHASE;
                break;

            case EnemyState::WANDER:
                wander();
                break;
                
            case EnemyState::CHASE:
                chase_player();
                break;
        }
    }

public:
    Enemy()
    {
        m_Layer = "enemy_layer";

        m_DeathPhaseDuration = 0.2f;
        m_InvincibilityDuration = 0.0f;

        m_Health = 10;
        m_MaxHealth = 10;

        col = olc::RED;
        m_Bounds->radius = 0.2f;
        m_Speed = 2.5f;

        InternalSetup();

        m_HurtBox->radius = 0.25f;
    }
    ~Enemy(){}

    void Attack() override
    {
        HitBoxHandler::CreateHitBox(m_Bounds->pos + movement_vector * 0.2f, 0.35f, 10, HBType::damage, m_HurtBox->layer);
    }

    void GetHurt() override
    {
        if (m_Bounds->vel.mag2() > 0)
        {
            m_Bounds->vel = movement_vector * -1 * m_HurtBox->magnitude;
        }
        m_Health -= m_HurtBox->magnitude;
        m_State = EntityState::INVINCIBLE;
        m_InvincibilityTimer.Start(m_InvincibilityDuration);
    }

    void SetPlayerPos(olc::vf2d pp)
    {
        m_PlayerPos = pp;
    }
};

#endif