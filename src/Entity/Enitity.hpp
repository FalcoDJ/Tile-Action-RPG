#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../olcPixelGameEngine.h"
#include "../olcPGEX_TransformedView.h"
#include "../Shapes/Shapes.hpp"
#include "../Shapes/ShapeHandler.hpp"

class Entity
{
protected:
    Entity()
    {
        m_Bounds = ShapeHandler::CreateCircle();
    }
    Circle* m_Bounds = nullptr;
    olc::vf2d movement_vector;
    float m_Speed;

    olc::Pixel col = olc::GREEN;

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
    }

    virtual void Draw(olc::TileTransformedView* tv, olc::vf2d Camera)
    {
        tv->FillCircle(m_Bounds->pos - Camera, m_Bounds->radius, col);
    }

};

#endif