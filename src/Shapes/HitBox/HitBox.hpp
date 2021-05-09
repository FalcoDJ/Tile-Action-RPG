#ifndef HIT_BOX_HPP
#define HIT_BOX_HPP

#include "../Shapes.hpp"

enum class HBType
{
    null = 0, // Used to indicate that no hitbox touched a hurtbox
    damage = 1, // Has no effects, but does damage
    stun = 2    // Does no damage, but effects
};

class HitBox : public Circle
{
    public:
        int layer = 0; // HitBoxes wont interact with HurtBoxes of the same layer

        float magnitude = 0.0f; // Magnitude of type, i.e. "damage strength"
        HBType type = HBType::damage;
};

#endif