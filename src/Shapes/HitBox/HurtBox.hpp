#ifndef HURT_BOX_HPP
#define HURT_BOX_HPP

#include <assert.h>
#include "HitBox.hpp"

class HurtBox : public HitBox
{
    public:
        HurtBox(){}
        HurtBox(int l)
        {
            layer = l;
            type = HBType::null;
        }
        ~HurtBox(){}
};

#endif