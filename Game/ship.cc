//? Check if pivot point is at the right position and move the ship

#include "ship.h"

#include <stdlib.h>

#include <esat\draw.h>
#include <esat\math.h>
#include <esat\input.h>

#include "game_utils.h"
#include "config.h"

esat::Vec2 *g_ship_base_points = nullptr, *g_ship_cannon_points = nullptr, *g_ship_flame_points = nullptr, debug_pivot[16], debug_local_pivot[16];

struct SHP::TShip{
    bool is_propelling, is_rotating_right, is_rotating_left;
    unsigned char flames_current_frame;
    float rotation;
    esat::Vec2 position, forward, *base_world_points, *cannon_world_points, *flames_world_points;
};

static void InitShip(SHP::TShip** ship){
    *ship = (SHP::TShip*)malloc(sizeof(SHP::TShip));
    (**ship).is_propelling = false;
    (**ship).is_rotating_left = false;
    (**ship).is_rotating_right = false;
    (**ship).flames_current_frame = 1;
    (**ship).rotation = 0.0f;
    (**ship).position = {CFG::kScreenX/2, CFG::kScreenY/2};
    (**ship).forward = {-1, 0};
    (**ship).base_world_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * 5);
    (**ship).cannon_world_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * 6);
    (**ship).flames_world_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * 17);

}

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

static void AssignFlamesPoints(){
    *g_ship_flame_points = {2.25f, 0.5f};
    *(g_ship_flame_points + 1) = {2.85f, 1.15f};
    *(g_ship_flame_points + 2) = {2.75f, 0.75f};
    *(g_ship_flame_points + 3) = {3.25f, 0.85f};
    *(g_ship_flame_points + 4) = {2.75f, 0.5f};
    *(g_ship_flame_points + 5) = {3.15f, 0.5f};
    *(g_ship_flame_points + 6) = {2.25f, 0.25f};
    *(g_ship_flame_points + 7) = {2.25f, -0.5f};
    *(g_ship_flame_points + 8) = {2.85f, -1.15f};
    *(g_ship_flame_points + 9) = {2.75f, -0.75f};
    *(g_ship_flame_points + 10) = {3.25f, -0.85f};
    *(g_ship_flame_points + 11) = {2.75f, -0.5f};
    *(g_ship_flame_points + 12) = {3.15f, -0.5f};
    *(g_ship_flame_points + 13) = {2.25f, -0.25f};
    *(g_ship_flame_points + 14) = {3.0f, 0.25f};
    *(g_ship_flame_points + 15) = {3.75f, 0.0f};
    *(g_ship_flame_points + 16) = {3.0f, -0.25f};
}

static void InitShipLocalPoints(){
    const unsigned char base_vertices = 5, cannon_vertices = 6, flames_vertices = 17;

    g_ship_base_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * base_vertices);
    g_ship_cannon_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * cannon_vertices);
    g_ship_flame_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * flames_vertices);
    AssignBasePoints();
    AssignCannonPoints();
    AssignFlamesPoints();
    UTL::NormalizePoints(g_ship_base_points, base_vertices);
    UTL::NormalizePoints(g_ship_cannon_points, cannon_vertices);
    UTL::NormalizePoints(g_ship_flame_points, flames_vertices);
}

static void TransformShipWorldPoints(SHP::TShip* const ship, bool is_ship_propelling){
    const unsigned char base_vertices = 5, cannon_vertices = 6, flames_vertices = 17;
    const float ship_scale = 27.5f;

    UTL::TransformWorldPoints((*ship).base_world_points, g_ship_base_points, base_vertices, ship_scale, (*ship).rotation, (*ship).position);
    UTL::TransformWorldPoints((*ship).cannon_world_points, g_ship_cannon_points, cannon_vertices, ship_scale, (*ship).rotation, (*ship).position);
    if(is_ship_propelling){
        const unsigned char flames_scale = 27.5f * (*ship).flames_current_frame;
        const float flames_displacement = ((*ship).flames_current_frame - 1) / 25;
        const esat::Vec2 flames_position = {(*ship).position.x - (*ship).forward.x * flames_displacement, (*ship).position.y - (*ship).forward.y * flames_displacement};

        UTL::TransformWorldPoints((*ship).flames_world_points, g_ship_flame_points, flames_vertices, flames_scale, (*ship).rotation, flames_position);
    }  
    UTL::TransformWorldPoints(debug_pivot, debug_local_pivot, 16, 3.5f, 0.0f, (*ship).position);
}

void SHP::Init(SHP::TShip** ship){
    InitShip(ship);
    InitShipLocalPoints();
    UTL::InitCircle(debug_local_pivot, 16);
    TransformShipWorldPoints(*ship, (**ship).is_propelling);
}

void SHP::GetInput(SHP::TShip* const ship){
    (*ship).is_propelling = esat::IsSpecialKeyPressed(esat::SpecialKey::kSpecialKey_Up);
    (*ship).is_rotating_left = esat::IsSpecialKeyPressed(esat::SpecialKey::kSpecialKey_Left);
    (*ship).is_rotating_right = esat::IsSpecialKeyPressed(esat::SpecialKey::kSpecialKey_Right);
}

static void Propel(SHP::TShip* const ship, const double* const dt){
    const unsigned char propelling_speed = 50;

    if((*ship).is_propelling){
        (*ship).position.x += (*ship).forward.x * propelling_speed * (*dt);
        (*ship).position.y += (*ship).forward.y * propelling_speed * (*dt);
    }
}

static void Rotate(SHP::TShip* const ship, const double* const dt){
    const unsigned char rotation_speed = 250;

    if((*ship).is_rotating_left)    (*ship).rotation -= rotation_speed * (*dt);
    if((*ship).is_rotating_right)    (*ship).rotation += rotation_speed * (*dt);
}

static void UpdateFlamesAnimation(SHP::TShip* const ship, const double* const dt){
    const float frame_change_timestamp = 0.125f;
    static float timer = 0.0f;

    if((*ship).is_propelling){
        (*ship).flames_current_frame = ((int)(timer / frame_change_timestamp) % 3) + 1;
        timer += *dt;
    }
    else
        timer = 0.0f;
}

void SHP::Update(SHP::TShip* const ship, const double* const dt){
    Propel(ship, dt);
    Rotate(ship, dt);
    UpdateFlamesAnimation(ship, dt);
    if((*ship).is_propelling || (*ship).is_rotating_left || (*ship).is_rotating_right) TransformShipWorldPoints(ship, (*ship).is_propelling); 
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

static void DrawFlames(const esat::Vec2* const flames_points){
    const unsigned char flame_vertices = 6, flames_vertices = 16;

    for(int i = 0; i < flame_vertices; ++i)
        esat::DrawLine((*(flames_points + i)).x, (*(flames_points + i)).y, (*(flames_points + i + 1)).x, (*(flames_points + i + 1)).y);
    for(int i = flame_vertices + 1; i < flame_vertices * 2 + 1; ++i)
        esat::DrawLine((*(flames_points + i)).x, (*(flames_points + i)).y, (*(flames_points + i + 1)).x, (*(flames_points + i + 1)).y);
    for(int i = flame_vertices * 2 + 2; i < flames_vertices; ++i)
        esat::DrawLine((*(flames_points + i)).x, (*(flames_points + i)).y, (*(flames_points + i + 1)).x, (*(flames_points + i + 1)).y);
}

static void DebugPivot(){
    esat::DrawSetFillColor(0, 255, 0);
    esat::DrawSolidPath(&debug_pivot->x, 16);
}

void SHP::Draw(const SHP::TShip* const ship){
    esat::DrawSetStrokeColor(0, 0, 255);
    DrawBase((*ship).base_world_points);
    DrawCannon((*ship).cannon_world_points);
    DrawFlames((*ship).flames_world_points);
    DebugPivot();
}

void SHP::Free(SHP::TShip* ship){
    free(g_ship_base_points);
    free(g_ship_cannon_points);
    free((*ship).base_world_points);
    free((*ship).cannon_world_points);
    free(ship);
}