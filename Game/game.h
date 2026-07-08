#ifndef GAME_H
#define GAME_H

namespace GAME{
    void Init();
    void GetInput();
    void Update(const double* const dt);
    void Draw();
    void Free();
}

#endif