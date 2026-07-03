//? Check if pivot point is at the right position and move the ship

#include "ship.h"

#include <stdlib.h>

#include <esat\draw.h>
#include <esat\math.h>

#include "game_utils.h"
#include "config.h"

esat::Vec2 *g_ship_base_points = nullptr, *g_ship_cannon_points = nullptr, debug_pivot[16];

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
    UTL::NormalizePoints(g_ship_base_points, base_vertices);
    UTL::NormalizePoints(g_ship_cannon_points, cannon_vertices);
}

static void InitShipWorldPoints(SHP::TShip* const ship){
    const unsigned char base_vertices = 5, cannon_vertices = 6;

    (*ship).base_world_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * base_vertices);
    (*ship).cannon_world_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * cannon_vertices);
    UTL::TransformWorldPoints((*ship).base_world_points, g_ship_base_points, base_vertices, 27.5f, CFG::kScreenX/2, CFG::kScreenY/2);
    UTL::TransformWorldPoints((*ship).cannon_world_points, g_ship_cannon_points, cannon_vertices, 27.5f, CFG::kScreenX/2, CFG::kScreenY/2);
}

void SHP::Init(SHP::TShip** ship){
    InitShipLocalPoints();
    *ship = (SHP::TShip*)malloc(sizeof(SHP::TShip));
    InitShipWorldPoints(*ship);
    UTL::InitCircle(debug_pivot, 16);
    UTL::TransformWorldPoints(debug_pivot, debug_pivot, 16, 3.5f, CFG::kScreenX/2, CFG::kScreenY/2);
}

void SHP::GetInput(){

}

void SHP::Update(){

}

static void DrawBase(esat::Vec2* const base_points){
    const unsigned char base_vertices = 5;

    esat::DrawSetFillColor(0,0,0,0);
    esat::DrawSolidPath(&((*base_points).x), base_vertices);
}

static void DrawCannon(const esat::Vec2* const cannon_points){
    const unsigned char cannon_vertices = 6;

    for(int i = 0; i < cannon_vertices / 2 - 1; ++i)
        esat::DrawLine((*(cannon_points + i)).x, (*(cannon_points + i)).y, (*(cannon_points + i + 1)).x, (*(cannon_points + i + 1)).y);
    for(int i = cannon_vertices / 2; i < cannon_vertices - 1; ++i)
        esat::DrawLine((*(cannon_points + i)).x, (*(cannon_points + i)).y, (*(cannon_points + i + 1)).x, (*(cannon_points + i + 1)).y);
}

static void DebugPivot(){
    esat::DrawSetFillColor(0, 255, 0);
    esat::DrawSolidPath(&debug_pivot->x, 16);
}

void SHP::Draw(const SHP::TShip* const ship){
    esat::DrawSetStrokeColor(0, 0, 255);
    DrawBase((*ship).base_world_points);
    DrawCannon((*ship).cannon_world_points);
    DebugPivot();
}

void SHP::Free(SHP::TShip* ship){
    free(g_ship_base_points);
    free(g_ship_cannon_points);
    free((*ship).base_world_points);
    free((*ship).cannon_world_points);
    free(ship);
}