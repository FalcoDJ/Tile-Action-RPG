#ifndef PLAYER_ENTITY_HPP
#define PLAYER_ENTITY_HPP

#include "../Enitity.hpp"

class Player : public Entity
{
private:
    olc::Key Left = olc::Key::A, Right = olc::Key::D, Up = olc::Key::W, Down = olc::Key::S, Run = olc::Key::SHIFT, Attack_Key = olc::Key::SPACE;
    
    float m_DurationOfAttack = 1.0f;

public:
    Player()
    {
        m_Layer = "player_layer";

        m_Bounds->radius = .5;
        m_Speed = 5;

        Setup();

        m_HurtBox->radius = .4;
    }
    ~Player(){}

    void Attack() override
    {
        std::cout << "Space Key Pressed!\n";
        HitBoxHandler::CreateHitBox(m_Bounds->pos + movement_vector * 1.5, 2, 10, HBType::damage, m_HurtBox->layer);
    }

    void handleInput(olc::PixelGameEngine* pge)
    {
        if (pge->GetKey(Attack_Key).bPressed)
            m_AttackTimer.Start(m_DurationOfAttack);

        movement_vector = olc::vf2d::ZERO();
        int x_value = (int)pge->GetKey(Right).bHeld - (int)pge->GetKey(Left).bHeld;
        int y_value = (int)pge->GetKey(Down).bHeld - (int)pge->GetKey(Up).bHeld;
        movement_vector = olc::vf2d(x_value,y_value);

        if (movement_vector.mag2() > 0)
        movement_vector = movement_vector.norm() * (pge->GetKey(Run).bHeld ? 1.5 : 1);
    }
};

#endif