#ifndef UTILS_H
#define UTILS_H

namespace esat{
    struct Vec2;
    struct Mat3;
}

namespace UTL{
    void NormalizePoints(esat::Vec2* const points, const int num_of_points);
    void DebugPoints(const char* const debug_header, esat::Vec2* points, const int num_of_points);
    void TransformWorldPoints(esat::Vec2* const world_points, const esat::Vec2* const local_points, int num_of_points, float scale, float translate_x, float translate_y);
    void InitCircle(esat::Vec2 *circle, int num_of_points);
}

#endif