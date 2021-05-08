#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "../olcPixelGameEngine.h"

class Circle
{
    public:
        Circle(){}
        Circle(olc::vf2d p, olc::vf2d v, float r) : pos(p), vel(v), radius(r) {}
        ~Circle(){}
        olc::vf2d pos = {0,0}, vel = {0,0};
        float radius = 0.0f;

        bool operator != (const Circle& ctc) const { return (this->pos != ctc.pos || this->radius != ctc.radius || this->vel != ctc.vel); }

};

class Rectangle
{
    public:
        Rectangle(){}
        Rectangle(olc::vf2d p, olc::vf2d v, olc::vf2d s) : pos(p), vel(v), size(s) {}
        ~Rectangle(){}
        olc::vf2d pos = {0,0},vel = {0,0},size = {0,0};
};

#endif