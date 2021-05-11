#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "../olcPixelGameEngine.h"

enum Directions {
    NORTH=0,
    WEST=1,
    EAST=2,
    SOUTH=3
};

class Circle
{
    public:
        Circle(){}
        Circle(int id) : ID(id) {}
        Circle(olc::vf2d p, olc::vf2d v, float r) : pos(p), vel(v), radius(r) {}
        ~Circle(){}
        olc::vf2d pos = {0,0}, vel = {0,0};
        float radius = 0.0f;
        int ID = 0;


        bool operator != (const Circle& ctc) const { return (this->pos != ctc.pos || this->radius != ctc.radius || this->vel != ctc.vel); }
        bool operator == (const Circle& ctc) const { return (this->pos == ctc.pos && this->radius == ctc.radius && this->vel == ctc.vel); }

};

class Rectangle
{
    public:
        Rectangle(){}
        Rectangle(int id) : ID(id) {}
        Rectangle(olc::vf2d p, olc::vf2d v, olc::vf2d s) : pos(p), vel(v), size(s) {}
        ~Rectangle(){}
        olc::vf2d pos = {0,0},vel = {0,0},size = {0,0};
        int ID = 0;

        bool operator != (const Rectangle& rtc) const { return (this->pos != rtc.pos || this->size != rtc.size || this->vel != rtc.vel); }
        bool operator == (const Rectangle& rtc) const { return (this->pos == rtc.pos && this->size == rtc.size && this->vel == rtc.vel); }
};

#endif