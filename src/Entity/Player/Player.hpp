#ifndef PLAYER_ENTITY_HPP
#define PLAYER_ENTITY_HPP

#include "../Enitity.hpp"

class Player : public Entity
{
private:
    olc::Key Left = olc::Key::A, Right = olc::Key::D, Up = olc::Key::W, Down = olc::Key::S, Run = olc::Key::SHIFT;
public:
    Player()
    {
        m_Bounds->radius = .5;
        m_Speed = 5;
    }
    ~Player(){}

    void handleInput(olc::PixelGameEngine* pge)
    {
        movement_vector = olc::vf2d::ZERO();
        int x_value = (int)pge->GetKey(Right).bHeld - (int)pge->GetKey(Left).bHeld;
        int y_value = (int)pge->GetKey(Down).bHeld - (int)pge->GetKey(Up).bHeld;
        movement_vector = olc::vf2d(x_value,y_value);

        if (movement_vector.mag2() > 0)
        movement_vector = movement_vector.norm() * (pge->GetKey(Run).bHeld ? 1.5 : 1);
    }
};

#endif