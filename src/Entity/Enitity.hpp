#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <assert.h>
#include "../olcPixelGameEngine.h"
#include "../olcPGEX_TransformedView.h"
#include "../Shapes/ShapeHandler.hpp"
#include "../Shapes/HitBox/HitBoxHandler.hpp"
#include "../Timer/Timer.hpp"


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
    olc::vf2d movement_vector;
    float m_Speed;
    std::string m_Layer;

    olc::Pixel col = olc::GREEN;

protected:
    Timer m_AttackTimer;

    virtual void GetHurt()
    {
        
    }
    virtual void Attack()
    {

    }

public:
    void Spawn(olc::vf2d starting_point)
    {
        m_Bounds->pos = starting_point;
    }

    Circle GetBounds()
    {
        return Circle(m_Bounds->pos,m_Bounds->vel,m_Bounds->radius);
    }
    
    virtual void Update(float fElapsedTime)
    {
        m_Bounds->vel = movement_vector * m_Speed;
        m_HurtBox->pos = m_Bounds->pos;

        if (m_AttackTimer.GetTimeLeft() > 0.0f)
            Attack();

        if (m_HurtBox->magnitude > 0)
        {
            switch (m_HurtBox->type)
            {
                case HBType::damage:
                    m_Bounds->vel = (movement_vector * -1).norm() * m_HurtBox->magnitude;
                    break;
                
                default:
                    break;
            }

            m_HurtBox->magnitude = 0.0f;
            m_HurtBox->type = HBType::null;
        }
    }

    virtual void Draw(olc::TileTransformedView* tv, olc::vf2d Camera)
    {
        tv->FillCircle(m_Bounds->pos - Camera, m_Bounds->radius, col);
    }
};

#endif