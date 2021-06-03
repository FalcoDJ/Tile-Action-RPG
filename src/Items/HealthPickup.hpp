#pragma once
#ifndef HEALTH_PICKUP_HPP
#define HEALTH_PICKUP_HPP

#include "Item.hpp"

class HealthPickup : public Item
{
private:
    float m_HealthValue = 0.0f;
public:
    HealthPickup() {}
    ~HealthPickup() {}

    void SetHealthAmount(float health = 10.00f)
    {
        m_HealthValue = health;
    }

    void Spawn(olc::vf2d pos) override
    {
        m_Position = pos;
        m_DetectionRange = HitBoxHandler::CreateLongTermHitBox(pos, 0.5f, m_HealthValue, HBType::null, m_LayerId);
    }
    void Draw(olc::TileTransformedView *tv) override
    {
        // tv->
    }
};

#endif