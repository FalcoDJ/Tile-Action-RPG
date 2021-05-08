#ifndef MATH_HPP
#define MATH_HPP

class Math
{
public:
    static float lerp(float a, float b, float f)
    {
        return a + f * (b - a);
    }

    template <typename T>
    static T clamp(T minRange, T maxRange, T number)
    {
        return (T)std::max(minRange, std::min(maxRange, number));
    }
};

#endif