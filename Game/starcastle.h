#ifndef STAR_CASTLE_H
#define STAR_CASTLE_H

namespace esat{
    struct Vec2;
}

namespace STCT{
    void Init();
    void Update(const double& dt, esat::Vec2& ship_position);
    void Draw();
    void Free();
}

#endif