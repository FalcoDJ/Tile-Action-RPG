#ifndef ENEMY_ENTITY_HPP
#define ENEMY_ENTITY_HPP

#include "../Enitity.hpp"

class Enemy : public Entity
{
private:
    float m_Max_Chase_Radius = 3;

public:
    Enemy()
    {
        m_Layer = "enemy_layer";

        m_DeathPhaseDuration = 0.2f;
        m_InvincibilityDuration = 0.0f;

        m_Health = 10;
        m_MaxHealth = 10;

        col = olc::RED;
        m_Bounds->radius = .2;
        m_Speed = 4;

        Setup();

        m_HurtBox->radius = .4;
    }
    ~Enemy(){}

    void Attack() override
    {
        HitBoxHandler::CreateHitBox(m_Bounds->pos + movement_vector * 0.5, 0.8, 10, HBType::damage, m_HurtBox->layer);
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

    void chase_player(olc::vf2d player_pos)
    {
        movement_vector = olc::vf2d::ZERO();

        if (m_AmIAlive)
        {
            movement_vector = player_pos - m_Bounds->pos;

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
};

#endif