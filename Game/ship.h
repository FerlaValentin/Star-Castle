#ifndef SHIP_H
#define SHIP_H

namespace esat{
    struct Vec2;
}

namespace SHP{
    struct TShip;

    void Init(TShip** ship);
    void GetInput(SHP::TShip* const ship);
    void Update(SHP::TShip* const ship, const double& dt);
    void Draw(const SHP::TShip* const ship);
    void Free(TShip** ship);
    esat::Vec2 GetPosition(const TShip* const ship);
}

#endif