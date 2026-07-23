//? Pending to add smooth transition
#include "starcastle.h"

//!REMOVE AFTER REMOVING DEBUG FUNCTIONS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <esat\math.h>
#include <esat\draw.h>

#include "game_utils.h"
#include "config.h"

struct TStarCastle{
    float rotation, last_rotation;
    esat::Vec2 position, forward, *body_world_points, *left_wing_world_points, *right_wing_world_points;
};

static esat::Vec2 *g_body_local_points, *g_left_wing_local_points, *g_right_wing_local_points;
static TStarCastle* g_star_castle;

static void AssignBodyPoints(){
    *g_body_local_points = {-2.0, 0.0f};
    *(g_body_local_points + 1) = {-1.0f, 0.5f};
    *(g_body_local_points + 2) = {4.0f, 1.5f};
    *(g_body_local_points + 3) = {3.0f, 0.0f};
    *(g_body_local_points + 4) = {4.0f, -1.5f};
    *(g_body_local_points + 5) = {-1.0f, -0.5f};
}

static void AssignLeftWingPoints(){
    *g_left_wing_local_points = {0.0f, -1.0f};
    *(g_left_wing_local_points + 1) = {2.5f, -1.5f};
    *(g_left_wing_local_points + 2) = {3.0f, -2.5f};
    *(g_left_wing_local_points + 3) = {0.5f, -2.0f};
}

static void AssignRightWingPoints(){
    *g_right_wing_local_points = {0.0f, 1.0f};
    *(g_right_wing_local_points + 1) = {2.5f, 1.5f};
    *(g_right_wing_local_points + 2) = {3.0f, 2.5f};
    *(g_right_wing_local_points + 3) = {0.5f, 2.0f};
}

static void InitLocalPoints(){
    g_body_local_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * 6);
    g_left_wing_local_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * 4);
    g_right_wing_local_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * 4);
    AssignBodyPoints();
    AssignLeftWingPoints();
    AssignRightWingPoints();
    UTL::NormalizePoints(g_body_local_points, 6);
    UTL::NormalizePoints(g_left_wing_local_points, 4);
    UTL::NormalizePoints(g_right_wing_local_points, 4);
}

static void InitStarCastle(){
    g_star_castle = (TStarCastle*)malloc(sizeof(TStarCastle));
    (*g_star_castle).rotation = 0.0f;
    (*g_star_castle).last_rotation = 0.0f;
    (*g_star_castle).position = {CFG::kScreenX/2 + 40, CFG::kScreenY/2};
    (*g_star_castle).forward = {0.0f, 0.0f};
    (*g_star_castle).body_world_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * 6);
    (*g_star_castle).left_wing_world_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * 4);
    (*g_star_castle).right_wing_world_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * 4);
}

void STCT::Init(){
    InitLocalPoints();
    InitStarCastle();
}

static void UpdateForward(esat::Vec2& ship_position){
    (*g_star_castle).forward = UTL::GetVectorDirectionFromPoints(ship_position, (*g_star_castle).position);
}

//!ROTATION IS ALREADY IN RADIANS
static void Rotate(){
    (*g_star_castle).rotation = atan2f((*g_star_castle).forward.y, (*g_star_castle).forward.x);
}

static void TransformStarCastlePoints(){
    const char star_castle_scale = 60.0f;

    UTL::TransformWorldPoints((*g_star_castle).body_world_points, g_body_local_points, 6, star_castle_scale, (*g_star_castle).rotation, (*g_star_castle).position);
    UTL::TransformWorldPoints((*g_star_castle).left_wing_world_points, g_left_wing_local_points, 4, star_castle_scale, (*g_star_castle).rotation, (*g_star_castle).position);
    UTL::TransformWorldPoints((*g_star_castle).right_wing_world_points, g_right_wing_local_points, 4, star_castle_scale, (*g_star_castle).rotation, (*g_star_castle).position);
}

void STCT::Update(const double& dt, esat::Vec2& ship_position){
    UpdateForward(ship_position);
    Rotate();
    if((*g_star_castle).rotation != (*g_star_castle).last_rotation){
        printf("[DEBUG] Rotation: %f\n", (*g_star_castle).rotation);
        TransformStarCastlePoints();
        (*g_star_castle).last_rotation = (*g_star_castle).rotation;
    }
}

void STCT::Draw(){
    esat::DrawSetFillColor(0,0,0,0);
    esat::DrawSetStrokeColor(255,255,0);
    esat::DrawSolidPath(&(*(*g_star_castle).body_world_points).x, 6);
    esat::DrawSolidPath(&(*(*g_star_castle).left_wing_world_points).x, 4);
    esat::DrawSolidPath(&(*(*g_star_castle).right_wing_world_points).x, 4);
}

void STCT::Free(){
    free(g_body_local_points);
    free(g_right_wing_local_points);
    free((*g_star_castle).body_world_points);
    free((*g_star_castle).left_wing_world_points);
    free((*g_star_castle).right_wing_world_points);
    free(g_star_castle);
}