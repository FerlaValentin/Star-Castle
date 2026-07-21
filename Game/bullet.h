#ifndef BULLET_H
#define BULLET_H

namespace esat{
    struct Vec2;
}

namespace BLT{
    enum TBulletOwner{
        kShipBullet,
        kStarCastleBullet,
        kNone
    };

    void Init();
    void Update(const double* const dt);
    void Draw();
    void Free();
    void Fire(TBulletOwner owner, esat::Vec2 start_position, esat::Vec2 forward);
}

#endif