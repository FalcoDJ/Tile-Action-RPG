#pragma once

#include "olcPixelGameEngine.h"
#include "Math.hpp"

class Camera2d
{
public:
    olc::vf2d pos = {0.0f,0.0f};
    olc::vf2d size = {0.0f,0.0f};
    olc::vf2d bounds_top_Left;
    olc::vf2d bounds_bottom_right;
    
    void LerpCamera(olc::vf2d destination, float speed)
    {
        pos.x = Math::lerp(pos.x,destination.x - size.x/2.0f,speed);
        pos.y = Math::lerp(pos.y,destination.y - size.y/2.0f,speed);
        
        if (pos.x < bounds_top_Left.x) pos.x = bounds_top_Left.x;
        if (pos.y < bounds_top_Left.y) pos.y = bounds_top_Left.y;
        if (pos.x + size.x > bounds_bottom_right.x) pos.x = bounds_bottom_right.x - size.x;
        if (pos.y + size.y > bounds_bottom_right.y) pos.y = bounds_bottom_right.y - size.y;
    }

    void SetBounds(olc::vf2d topLeft, olc::vf2d bottomRight)
    {
        bounds_top_Left = topLeft;
        bounds_bottom_right = bottomRight;
    }
};