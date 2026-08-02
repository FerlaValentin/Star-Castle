#include "ring.h"

#include <stdlib.h>

#include <esat\draw.h>
#include <esat\math.h>

#include "game_utils.h"
#include "config.h"

enum TRingLevel{
    kInnerRing,
    kMiddleRing,
    kOuterRing
};

struct TRing{
    bool* is_segments_active;
    TRingLevel ring_level;
    float rotation;
    esat::Vec2* segments_points;
};

static TRing* g_rings = nullptr;
static esat::Vec2* g_segments_local_points;

static void InitSegmentsPoints(){
    g_segments_local_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * 12);
    UTL::InitCircle(g_segments_local_points, 12);
}

static void InitRingSegments(TRing* const ring){
    for(int i = 0; i < 12; ++i){
        *((*ring).is_segments_active + i) = true;
        *((*ring).segments_points + i) = {0.0f, 0.0f}; 
    }
}

static void InitRings(){
    for(int i = 0; i < 3; ++i){
        (*(g_rings + i)).is_segments_active = (bool*)malloc(sizeof(bool) * 12);
        (*(g_rings + i)).segments_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * 12);
        InitRingSegments(g_rings + i);
        (*(g_rings + i)).rotation = 0.0f;
    }
    (*g_rings).ring_level = TRingLevel::kInnerRing;
    (*(g_rings + 1)).ring_level = TRingLevel::kMiddleRing;
    (*(g_rings + 2)).ring_level = TRingLevel::kOuterRing;
}

void RNG::Init(){
    g_rings = (TRing*)malloc(sizeof(TRing) * 3);
    InitSegmentsPoints();
    InitRings();
}

static void FreeSegments(){
    for(int i = 0; i < 3; ++i)
        free((*(g_rings + i)).is_segments_active);
}

static void TransformRing(TRing* const ring){
    const float base_scale = 125.0f;
    const float scale_addition = 40.0f;

    UTL::TransformWorldPoints((*ring).segments_points, g_segments_local_points, 12, base_scale + scale_addition * (*ring).ring_level, (*ring).rotation, {CFG::kScreenX/2 + 15, CFG::kScreenY/2 + 10});
}

void RNG::Update(){
    for(int i = 0; i < 3; ++i)
        TransformRing(g_rings + i);
}

void RNG::Draw(){
    for(int i = 0; i < 3; ++i){
        switch((*(g_rings + i)).ring_level){
            case TRingLevel::kInnerRing: esat::DrawSetStrokeColor(255, 255, 0); break;
            case TRingLevel::kMiddleRing: esat::DrawSetStrokeColor(255, 165, 0); break;
            case TRingLevel::kOuterRing: esat::DrawSetStrokeColor(255, 0, 0);
        }
        for(int j = 0; j < 12; ++j){
            if(*((*(g_rings + i)).is_segments_active + j))
                esat::DrawLine((*((*(g_rings + i)).segments_points + j)).x, (*((*(g_rings + i)).segments_points + j)).y,
                                (*((*(g_rings + i)).segments_points + ((j + 1) % 12))).x, (*((*(g_rings + i)).segments_points + ((j + 1) % 12))).y);
        }
    }
}

void RNG::Free(){
    FreeSegments();
    free(g_rings);
}