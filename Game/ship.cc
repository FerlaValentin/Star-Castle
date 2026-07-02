//? Make transform matrix getter function and assign world points to ship

#include "ship.h"

#include <stdlib.h>

#include <esat\math.h>

#include "game_utils.h"

esat::Vec2* g_ship_base_points = nullptr;
esat::Vec2* g_ship_cannon_points = nullptr;

struct SHP::TShip{
    esat::Vec2 *base_world_points, *cannon_world_points;
};

static void AssignBasePoints(){
    *g_ship_base_points = {0, 0.25f};
    *(g_ship_base_points + 1) = {0, -0.25f};
    *(g_ship_base_points + 2) = {4, -3};
    *(g_ship_base_points + 3) = {1.5f, 0};
    *(g_ship_base_points + 4) = {4, 3};
}

static void AssignCannonPoints(){
    *g_ship_cannon_points = {-1.5, 0.5f};
    *(g_ship_cannon_points + 1) = {3.75f, 1.75f};
    *(g_ship_cannon_points + 2) = {0, 0.25f};
    *(g_ship_cannon_points + 3) = {0, -0.25f};
    *(g_ship_cannon_points + 4) = {3.75f, -1.75f};
    *(g_ship_cannon_points + 5) = {-1.5f, -0.5f};
}

static void InitShipLocalPoints(){
    const unsigned char base_vertices = 5, cannon_vertices = 6;
    g_ship_base_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * base_vertices);
    g_ship_cannon_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * cannon_vertices);
    AssignBasePoints();
    AssignCannonPoints();
    UTL::DebugPoints("BASE LOCAL POINTS", g_ship_base_points, base_vertices);
    UTL::DebugPoints("CANNON LOCAL POINTS", g_ship_cannon_points, cannon_vertices);
    UTL::NormalizePoints(g_ship_base_points, base_vertices);
    UTL::NormalizePoints(g_ship_cannon_points, cannon_vertices);
    UTL::DebugPoints("BASE NORMALIZED POINTS", g_ship_base_points, base_vertices);
    UTL::DebugPoints("CANNON NORMALIZED POINTS", g_ship_cannon_points, cannon_vertices);
}

static void InitShipWorldPoints(SHP::TShip* ship){
    const unsigned char base_vertices = 5, cannon_vertices = 6;
    (*ship).base_world_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * base_vertices);
    (*ship).cannon_world_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * cannon_vertices);
}

void SHP::Init(SHP::TShip* ship){
    InitShipLocalPoints();
    ship = (SHP::TShip*)malloc(sizeof(SHP::TShip));
    InitShipWorldPoints(ship);
}




void SHP::Free(SHP::TShip* ship){
    free(g_ship_base_points);
    free(g_ship_cannon_points);
    free((*ship).base_world_points);
    free((*ship).cannon_world_points);
    free(ship);
}