#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <assert.h>
#include "../olcPixelGameEngine.h"
#include "../olcPGEX_TransformedView.h"
#include "../Shapes/ShapeHandler.hpp"
#include "../Shapes/HitBox/HitBoxHandler.hpp"
#include "../Timer/Timer.hpp"

enum class EntityState {
    NORMAL=0,
    INVINCIBLE=1
};

class Entity
{
protected:
    Entity()
    {
        assert(m_Bounds == nullptr);
        m_Bounds = ShapeHandler::CreateCircle();
    }

    void Setup() // Each inherited class will call this in its' constructor
    {
        assert(m_HurtBox == nullptr);
        m_HurtBox = HitBoxHandler::CreateHurtBox(m_Layer);
    }

    Circle* m_Bounds = nullptr;
    HurtBox* m_HurtBox = nullptr;
    EntityState m_State = EntityState::NORMAL;
    float m_Health = 100;
    float m_MaxHealth = 100;

    std::string m_Layer;
    olc::Pixel col = olc::GREEN;

    float m_Speed;
    olc::vf2d movement_vector;

protected:
    Timer m_AttackTimer;
    Timer m_DeathPhaseTimer;
    Timer m_InvincibilityTimer;

    bool m_AmIAlive = true;
    bool m_DiedYet = false;
    float m_DeathPhaseDuration = 0.0f;
    float m_InvincibilityDuration = 0.0f;

    virtual void GetHurt()
    {
        m_State = EntityState::INVINCIBLE;
        m_InvincibilityTimer.Start(m_InvincibilityDuration);
        m_Health -= m_HurtBox->magnitude;
    }
    virtual void Attack()
    {

    }
    virtual void Die()
    {
        m_Health = 0.0f;
        m_Bounds->vel = {0,0};
        m_HurtBox->layer = HitBoxHandler::GetNullLayer();
        m_State = EntityState::NORMAL;
        m_AmIAlive = false;
    }

public:
    bool AmIAlive() { return m_AmIAlive; }

    void Spawn(olc::vf2d starting_point)
    {
        m_Bounds->pos = starting_point;
    }

    Circle GetBounds()
    {
        return Circle(m_Bounds->pos,m_Bounds->vel,m_Bounds->radius);
    }

    float GetHealth()
    {
        return m_Health;
    }

    float GetHealthPercentage()
    {
        return  m_Health/m_MaxHealth;
    }
    
    virtual void BeforeUpdate() {}

    virtual void Update(float fElapsedTime)
    {
        BeforeUpdate();

        m_Bounds->vel = movement_vector * m_Speed;
        m_HurtBox->pos = m_Bounds->pos;

        if (m_AttackTimer.GetTimeLeft() > 0.0f)
            Attack();

        if (m_State != EntityState::INVINCIBLE)
        {

            if (m_HurtBox->type != HBType::null)
            {
                GetHurt();
                m_HurtBox->type = HBType::null;
                m_HurtBox->magnitude = 0.0f;
            }   
        }
        else
        {
            if (m_InvincibilityTimer.JustFinished()) { m_State = EntityState::NORMAL; }
        }

        if (m_Health <= 0.0f && !m_DiedYet)
        {
            m_DiedYet = true;
            m_DeathPhaseTimer.Start(m_DeathPhaseDuration);
        }

        if (m_DeathPhaseTimer.JustFinished())
        {
            Die();
        }

        AfterUpdate();
    }

    virtual void AfterUpdate() {}

    virtual void Draw(olc::TileTransformedView* tv, olc::vf2d Camera)
    {
        tv->FillCircle(m_Bounds->pos - Camera, m_Bounds->radius, col);
    }
};

#endif