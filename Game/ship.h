#ifndef SHIP_H
#define SHIP_H

namespace SHP{
    struct TShip;

    void Init(TShip** ship);
    void GetInput(SHP::TShip* const ship);
    void Update(SHP::TShip* const ship, const double* const dt);
    void Draw(const SHP::TShip* const ship);
    void Free(TShip* ship);
}

#endif