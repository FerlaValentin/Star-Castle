#include "bullet.h"

//!REMOVE AFTER REMOVING DEBUG FUNCTIONS
#include <stdio.h>
#include <stdlib.h>

#include <esat\math.h>
#include <esat\draw.h>
#include <esat\input.h> //! REMOVE AFTER REMOVING DEBUGGING FUNCTIONS

#include "game_utils.h"

struct TBullet{
    BLT::TBulletOwner bullet_owner;
    float travelled_distance;
    esat::Vec2 position, forward, *world_points;
    TBullet* next_bullet;
};

static bool debug = false;
static esat::Vec2 *g_local_points = nullptr;
static TBullet* g_bullets = nullptr;

static void AddBulletsList(BLT::TBulletOwner bullet_owner, const esat::Vec2 start_position, const esat::Vec2 forward){
    TBullet* tmp = (TBullet*)malloc(sizeof(TBullet));
    (*tmp).bullet_owner = bullet_owner;
    (*tmp).travelled_distance = 0.0f;
    (*tmp).position = start_position;
    (*tmp).forward = forward;
    (*tmp).world_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * 16);
    (*tmp).next_bullet = g_bullets;
    g_bullets = tmp;
}

static void InitLocalPoints(){
    const char bullet_vertices = 16;

    g_local_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * bullet_vertices);
    UTL::InitCircle(g_local_points, bullet_vertices);
}

void BLT::Init(){
    AddBulletsList(BLT::TBulletOwner::kNone, {0.0f, 0.0f}, {0.0f, 0.0f});
    InitLocalPoints();
}

static void Move(const double* const dt, TBullet* const tmp){
    const short bullet_speed = 1450;
    
    (*tmp).position.x += (*tmp).forward.x * bullet_speed * (*dt);
    (*tmp).position.y += (*tmp).forward.y * bullet_speed * (*dt);
}

static void UpdateTravelledDistance(const double* const dt, TBullet* const tmp){
    const short bullet_speed = 1450;
    
    (*tmp).travelled_distance += bullet_speed * (*dt);
}

static void DeactivateBullet(TBullet* const tmp){
    const short max_distance = 900;

    if((*tmp).travelled_distance > max_distance) (*tmp).bullet_owner = BLT::TBulletOwner::kNone;
}

static void ActivateDebug(){
    if(debug == false) debug = esat::IsKeyDown('K') || esat::IsKeyDown('k');
}

static void DebugNode(const TBullet* const tmp){
    if(debug){
        static int node_counter = 0;

        printf("NODE: %d\n", ++node_counter);
        printf("===============\n");
        printf("TRAVELLED DISTANCE: %f\n", (*tmp).travelled_distance);
        printf("BULLET OWNER: ");
        switch((*tmp).bullet_owner){
            case BLT::TBulletOwner::kShipBullet: printf("SHIP"); break;
            case BLT::TBulletOwner::kStarCastleBullet: printf("STAR CASTLE"); break;
            case BLT::TBulletOwner::kNone: printf("NONE");
        }
        printf("\n\n\n\n");
        if((*tmp).next_bullet == nullptr){
            debug = false;
            node_counter = 0;
        }
    }
}

void BLT::Update(const double* const dt){
    TBullet* tmp = g_bullets;

    ActivateDebug();
    while(tmp != nullptr){
        if((*tmp).bullet_owner != BLT::TBulletOwner::kNone){
            Move(dt, tmp);
            UpdateTravelledDistance(dt, tmp);
            DeactivateBullet(tmp);
            UTL::ScreenWrapObject((*tmp).position, 0);
            UTL::TransformWorldPoints((*tmp).world_points, g_local_points, 16, 1.25f, 0.0f, (*tmp).position);
        }
        DebugNode(tmp);
        tmp = (*tmp).next_bullet;
    }
}

void BLT::Draw(){
    TBullet* tmp = g_bullets;

    esat::DrawSetFillColor(255,255,255);
    while(tmp != nullptr){
        if((*tmp).bullet_owner != BLT::TBulletOwner::kNone) esat::DrawSolidPath(&(*(*tmp).world_points).x, 16, false);
        tmp = (*tmp).next_bullet;
    }
}

static void FreeBulletsList(){
    while(g_bullets != nullptr){
        TBullet* tmp = g_bullets;
        
        g_bullets = (*g_bullets).next_bullet;
        free(tmp);
    }
}

static void FreeLocalPoints(){
    free(g_local_points);
    g_local_points = nullptr;
}

void BLT::Free(){
    FreeBulletsList();
    FreeLocalPoints();
}

static TBullet* FindInactiveBullet(){
    TBullet* tmp = g_bullets;

    while(tmp != nullptr && (*tmp).bullet_owner != BLT::TBulletOwner::kNone)
        tmp = (*tmp).next_bullet;

    return tmp;
}

static void ReuseExistingBullet(TBullet* const inactive_bullet, BLT::TBulletOwner bullet_owner, esat::Vec2& start_position, esat::Vec2& forward){
    (*inactive_bullet).bullet_owner = bullet_owner;
    (*inactive_bullet).position = start_position;
    (*inactive_bullet).forward = forward;
    (*inactive_bullet).travelled_distance = 0.0f;
}

void BLT::Fire(BLT::TBulletOwner bullet_owner, esat::Vec2 start_position, esat::Vec2 forward){
    TBullet* tmp = FindInactiveBullet();

    if(tmp != nullptr)
        ReuseExistingBullet(tmp, bullet_owner, start_position, forward);
    else
        AddBulletsList(bullet_owner, start_position, forward);
}