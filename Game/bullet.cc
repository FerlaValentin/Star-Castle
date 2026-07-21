#include "bullet.h"

//!REMOVE AFTER REMOVING DEBUG FUNCTIONS
#include <stdio.h>
#include <stdlib.h>

#include <esat\math.h>
#include <esat\draw.h>

#include "game_utils.h"

struct TBullet{
    BLT::TBulletOwner bullet_owner;
    esat::Vec2 position, forward, *world_points;
    TBullet* next_bullet;
};

static esat::Vec2 *g_local_points = nullptr;
static TBullet* g_bullets = nullptr;

static void AddBulletsList(BLT::TBulletOwner bullet_owner, const esat::Vec2 start_position, const esat::Vec2 forward){
    TBullet* tmp = (TBullet*)malloc(sizeof(TBullet));
    (*tmp).bullet_owner = bullet_owner;
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

void BLT::Update(const double* const dt){
    TBullet* tmp = g_bullets;

    while(tmp != nullptr){
        if((*tmp).bullet_owner != BLT::TBulletOwner::kNone){
            UTL::TransformWorldPoints((*tmp).world_points, g_local_points, 16, 1.5f, 0.0f, (*tmp).position);
        }
        tmp = (*tmp).next_bullet;
    }
}

void BLT::Draw(){
    TBullet* tmp = g_bullets;

    esat::DrawSetFillColor(255,255,255);
    while(tmp != nullptr){
        esat::DrawSolidPath(&(*(*tmp).world_points).x, 16, false);
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
}

void BLT::Fire(BLT::TBulletOwner bullet_owner, esat::Vec2 start_position, esat::Vec2 forward){
    TBullet* tmp = FindInactiveBullet();

    if(tmp != nullptr)
        ReuseExistingBullet(tmp, bullet_owner, start_position, forward);
    else
        AddBulletsList(bullet_owner, start_position, forward);
}