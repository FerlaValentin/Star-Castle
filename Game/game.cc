#include "game.h"

#include "ship.h"

SHP::TShip* g_ship = nullptr;

void GAME::Init(){
    SHP::Init(&g_ship);
}

void GAME::GetInput(){
    SHP::GetInput();
}

void GAME::Update(){
    SHP::Update();
}

void GAME::Draw(){
    SHP::Draw(g_ship);
}

void GAME::Free(){
    SHP::Free(g_ship);
}