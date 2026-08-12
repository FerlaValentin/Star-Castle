#include "collisions.h"

#include <stdio.h>

#include <esat\math.h>

#include "bullet.h"
#include "config.h"
#include "ring.h"
#include "jonathan.h"

static bool DoesPointCollidePolygon(esat::Vec2 point_pos, esat::Vec2 point_prev_pos, esat::Vec2* polygon_points, int num_of_vertices){
    bool does_collide = false;
    esat::Vec2 point_displacement = {point_pos.x - point_prev_pos.x, point_pos.y - point_prev_pos.y};

    for(int i = 0; i < num_of_vertices; ++i){
        esat::Vec2 polygon_point = *(polygon_points + i);
        esat::Vec2 polygon_segment = 
    }
} 

static void CheckBulletSegmentCollision(int num_of_bullets){
    for(int i = 0; i < num_of_bullets; ++i){
        if(BLT::IsBulletActive(i)){
            esat::Vec2 bullet_pos = BLT::GetBulletPos(i);
            esat::Vec2 bullet_prev_pos = BLT::GetBulletPreviousPos(i);
            for(int j = 0; j < CFG::kNumRings * CFG::kSegmentsPerRing; ++j){
                esat::Vec2* segment_start_point = RNG::GetSegmentStartPointer(j);

                if(DoesPointCollidePolygon(bullet_pos, bullet_prev_pos, segment_start_point, 1)){
                    printf("DEACTIVE BULLET\n");
                    printf("DEACTIVE SEGMENT\n");
                }
            }
        }
    }
}

void COL::Update(int num_of_bullets){
    CheckBulletSegmentCollision(num_of_bullets);
}