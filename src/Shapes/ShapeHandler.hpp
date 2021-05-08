#ifndef SHAPE_HANDLER_HPP
#define SHAPE_HANDLER_HPP

#include <vector>
#include "../olcPixelGameEngine.h"
#include "Shapes.hpp"

class ShapeHandler
{
private:
    std::vector<Circle> m_Circles;
    std::vector<Rectangle> m_Rectangles;
    olc::vf2d m_WorldTopLeft;
    olc::vf2d m_WorldBottomRight;

    static ShapeHandler* m_s_Instance;

public:
    ShapeHandler();
    ~ShapeHandler();
    static Circle* CreateCircle();
    static Rectangle* CreateRectangle();

    static void SetWorldBounds(olc::vf2d tl, olc::vf2d br);

    static void Update(float fElapsedTime);
    static void ClearCircles();
    static void ClearRectangles();
    
};

#endif