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
        col = olc::RED;
        m_Bounds->radius = .2;
        m_Speed = 3;
    }
    ~Enemy(){}

    void chase_player(olc::vf2d player_pos)
    {
        movement_vector = player_pos - m_Bounds->pos;

        if (movement_vector.mag2() > m_Max_Chase_Radius * m_Max_Chase_Radius)
            movement_vector = olc::vf2d::ZERO();
        else
            movement_vector = movement_vector.norm();
    }
};

#endif