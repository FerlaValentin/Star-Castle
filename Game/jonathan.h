#include <stdio.h>

#include <esat\math.h>

namespace esat{
    struct Vec2;
}

namespace JMN{
    esat::Vec2 CalcInterpolation(esat::Vec2 p1, esat::Vec2 v1, esat::Vec2 p2, esat::Vec2 v2);
}