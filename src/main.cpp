#include "Engine.hpp"

int main(int argc, char const *argv[])
{
    Engine demo;
    if (demo.Construct(demo.Canvas_Size.x, demo.Canvas_Size.y, demo.Canvas_Scale.x, demo.Canvas_Scale.y))
    demo.Start();
    return 0;
}
