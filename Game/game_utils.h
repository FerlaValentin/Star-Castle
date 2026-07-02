#ifndef UTILS_H
#define UTILS_H

namespace esat{
    struct Vec2;
}

namespace UTL{
    void NormalizePoints(esat::Vec2* points, int num_of_points);
    void DebugPoints(const char* const debug_header, esat::Vec2* points, int num_of_points);
}

#endif