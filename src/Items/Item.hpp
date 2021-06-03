#pragma once
#ifndef ITEM_HPP
#define ITEM_HPP

#include "../olcPixelGameEngine.h"
#include "../Shapes/HitBox/HitBoxHandler.hpp"

class Item
{
protected:
    olc::vf2d m_Position;
    HitBox* m_DetectionRange = nullptr;
    int m_LayerId = 0;

public:
    Item() 
    {
        m_LayerId = HitBoxHandler::GetLayer("item_layer");
    }
    virtual void Spawn(olc::vf2d pos) = 0;
    virtual void Draw(olc::TileTransformedView *tv) = 0;
};

#endif