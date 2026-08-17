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
    struct TBullet;

    void Init();
    void Update(const double& dt);
    void Draw();
    void Free();
    void Fire(TBulletOwner owner, esat::Vec2 start_position, esat::Vec2 forward);
    int GetNumOfBullets();
    bool IsBulletActive(int bullet_index);
    esat::Vec2 GetBulletPos(int bullet_index);
    esat::Vec2 GetBulletPreviousPos(int bullet_index);
    void DeactivateBullet(TBullet* const tmp);
    TBullet* GetBullet(int bullet_index);
}

#endif