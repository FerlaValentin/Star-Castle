#ifndef SHIP_H
#define SHIP_H

namespace SHP{
    struct TShip;

    void Init(TShip* ship);
    void GetInput();
    void Update();
    void Draw();
    void Free(TShip* ship);
}

#endif