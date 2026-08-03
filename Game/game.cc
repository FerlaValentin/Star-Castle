#include "game.h"

#include <esat\math.h>

#include "ship.h"
#include "starcastle.h"
#include "ring.h"
#include "bullet.h"

static SHP::TShip* g_ship = nullptr;

void GAME::Init(){
    SHP::Init(&g_ship);
    STCT::Init();
    RNG::Init();
    BLT::Init();
}

void GAME::GetInput(){
    SHP::GetInput(g_ship);
}

void GAME::Update(const double& dt){
    SHP::Update(g_ship, dt);
    STCT::Update(dt, SHP::GetPosition(g_ship));
    RNG::Update(dt);
    BLT::Update(dt);
}

void GAME::Draw(){
    SHP::Draw(g_ship);
    STCT::Draw();
    RNG::Draw();
    BLT::Draw();
}

void GAME::Free(){
    SHP::Free(&g_ship);
    STCT::Free();
    RNG::Free();
    BLT::Free();
}