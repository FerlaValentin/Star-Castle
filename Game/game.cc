#include "game.h"

#include "ship.h"

SHP::TShip* g_ship = nullptr;

void GAME::Init(){
    SHP::Init(&g_ship);
}

void GAME::GetInput(){
    SHP::GetInput(g_ship);
}

void GAME::Update(const double* const dt){
    SHP::Update(g_ship, dt);
}

void GAME::Draw(){
    SHP::Draw(g_ship);
}

void GAME::Free(){
    SHP::Free(g_ship);
}