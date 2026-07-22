#ifndef UTILS_H
#define UTILS_H

namespace esat{
    struct Vec2;
    struct Mat3;
}

namespace UTL{
    void NormalizePoints(esat::Vec2* const points, const int num_of_points);
    void DebugPoints(const char* const debug_header, esat::Vec2* points, const int num_of_points);
    float AngleToRadians(float angle);
    void TransformWorldPoints(esat::Vec2* const world_points, const esat::Vec2* const local_points, int num_of_points, float scale, float angle, esat::Vec2 translate);
    void InitCircle(esat::Vec2 *circle, int num_of_points);
    float GetMagnitude(const esat::Vec2& vec);
    esat::Vec2 NormalizeVector(const esat::Vec2& vec);
    void ScreenWrapObject(esat::Vec2& obj_position, int displacement_offset);
    esat::Vec2 SumVec2(const esat::Vec2& a, const esat::Vec2& b);
    esat::Vec2 MultVecScalar(const esat::Vec2& a, const int k);
    esat::Vec2 GetVectorDirection(const float& rotation);
}

#endif