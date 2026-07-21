#include "game.h"

#include "ship.h"
#include "bullet.h"

static SHP::TShip* g_ship = nullptr;

void GAME::Init(){
    SHP::Init(&g_ship);
    BLT::Init();
}

void GAME::GetInput(){
    SHP::GetInput(g_ship);
}

void GAME::Update(const double* const dt){
    SHP::Update(g_ship, dt);
    BLT::Update(dt);
}

void GAME::Draw(){
    SHP::Draw(g_ship);
    BLT::Draw();
}

void GAME::Free(){
    SHP::Free(g_ship);
    BLT::Free();
}