#ifndef RING_H
#define RING_H

namespace esat{
    struct Vec2;
}

namespace RNG{
    void Init();
    void Update(double dt);
    void Draw();
    void Free();
    int GetNumOfSegments();
    bool IsSegmentActive(int segment_index);
    esat::Vec2* GetSegmentStartPointer(int segment_index);
}

#endif