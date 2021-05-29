#ifndef MATH_HPP
#define MATH_HPP

namespace name
{
    float lerp(float a, float b, float f)
    {
        return a + f * (b - a);
    }

    template <typename T>
    T clamp(T minRange, T maxRange, T number)
    {
        return (T)std::max(minRange, std::min(maxRange, number));
    }
};

#endif