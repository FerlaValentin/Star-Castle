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

static esat::Vec2* g_segments_local_points = nullptr;
static TRing* g_rings = nullptr;

static void InitSegmentsPoints(){
    g_segments_local_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * CFG::kSegmentsPerRing);
    UTL::InitCircle(g_segments_local_points, CFG::kSegmentsPerRing);
}

static void InitRingSegments(TRing* const ring){
    for(int i = 0; i < CFG::kSegmentsPerRing; ++i){
        *((*ring).is_segments_active + i) = true;
        *((*ring).segments_points + i) = {0.0f, 0.0f}; 
    }
}

static void InitRings(){
    for(int i = 0; i < CFG::kNumRings; ++i){
        (*(g_rings + i)).is_segments_active = (bool*)malloc(sizeof(bool) * CFG::kSegmentsPerRing);
        (*(g_rings + i)).segments_points = (esat::Vec2*)malloc(sizeof(esat::Vec2) * CFG::kSegmentsPerRing);
        InitRingSegments(g_rings + i);
        (*(g_rings + i)).rotation = 0.0f;
    }
    (*g_rings).ring_level = TRingLevel::kInnerRing;
    (*(g_rings + 1)).ring_level = TRingLevel::kMiddleRing;
    (*(g_rings + 2)).ring_level = TRingLevel::kOuterRing;
}

void RNG::Init(){
    g_rings = (TRing*)malloc(sizeof(TRing) * CFG::kNumRings);
    InitSegmentsPoints();
    InitRings();
}

static void Rotate(double dt, TRing* const ring){
    const unsigned char angular_velocity = 100;

    switch((*ring).ring_level)
    {
    case TRingLevel::kInnerRing:
    case TRingLevel::kOuterRing:
        (*ring).rotation -= angular_velocity * dt;
        if((*ring).rotation <= -360.0f) (*ring).rotation += 360.0f;
        break;
    case TRingLevel::kMiddleRing:
        (*ring).rotation += angular_velocity * dt;
        if((*ring).rotation >= 360.0f) (*ring).rotation -= 360.0f;
        break;
    default:
        break;
    }
}

static void TransformRing(TRing* const ring){
    const float base_scale = 125.0f;
    const float scale_addition = 40.0f;

    UTL::TransformWorldPoints((*ring).segments_points, g_segments_local_points, CFG::kSegmentsPerRing, {0.0f, 0.0f}, base_scale + scale_addition * (*ring).ring_level, (*ring).rotation, {CFG::kScreenX/2 - 5, CFG::kScreenY/2});
}

void RNG::Update(double dt){
    for(int i = 0; i < CFG::kNumRings; ++i){
        Rotate(dt, g_rings + i);
        TransformRing(g_rings + i);
    }
}

void RNG::Draw(){
    for(int i = 0; i < CFG::kNumRings; ++i){
        switch((*(g_rings + i)).ring_level){
            case TRingLevel::kInnerRing: esat::DrawSetStrokeColor(255, 255, 0); break;
            case TRingLevel::kMiddleRing: esat::DrawSetStrokeColor(255, 165, 0); break;
            case TRingLevel::kOuterRing: esat::DrawSetStrokeColor(255, 0, 0);
        }
        for(int j = 0; j < CFG::kSegmentsPerRing; ++j){
            if(*((*(g_rings + i)).is_segments_active + j))
                esat::DrawLine((*((*(g_rings + i)).segments_points + j)).x, (*((*(g_rings + i)).segments_points + j)).y,
                                (*((*(g_rings + i)).segments_points + ((j + 1) % CFG::kSegmentsPerRing))).x, (*((*(g_rings + i)).segments_points + ((j + 1) % CFG::kSegmentsPerRing))).y);
        }
    }
}

static void FreeSegments(){
    free(g_segments_local_points);
    for(int i = 0; i < CFG::kNumRings; ++i)
        free((*(g_rings + i)).segments_points);
}

void RNG::Free(){
    FreeSegments();
    free(g_rings);
}

bool RNG::IsSegmentActive(int segment_index){
    TRing* tmp = g_rings + segment_index / CFG::kSegmentsPerRing;

    return *((*tmp).is_segments_active + segment_index);
}

esat::Vec2 RNG::GetSegmentPointer(int segment_index){
    TRing* tmp = g_rings + segment_index / CFG::kSegmentsPerRing;

    return *((*tmp).segments_points + segment_index);
}