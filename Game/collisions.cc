#include "collisions.h"

#include <stdio.h>

#include <esat\math.h>

#include "bullet.h"
#include "config.h"
#include "ring.h"
#include "jonathan.h"

static bool DoesPointCollideSegment(esat::Vec2 point_pos, esat::Vec2 point_prev_pos, esat::Vec2 segment_start_point, esat::Vec2 segment_end_point){
    bool does_collide = false;
    esat::Vec2 point_displacement = {point_pos.x - point_prev_pos.x, point_pos.y - point_prev_pos.y};
    esat::Vec2 polygon_segment = {segment_end_point.x - segment_start_point.x, segment_end_point.y - segment_start_point.y};
    esat::Vec2 res = JMN::CalcInterpolation(point_pos, point_displacement, segment_start_point, polygon_segment);

    if(res.x >= 0 && res.x <= 1 && res.y >= 0 && res.y <= 1)    does_collide = true;

    return does_collide;
} 

static void CheckBulletSegmentCollision(){
    int num_of_bullets = BLT::GetNumOfBullets();
    for(int i = 0; i < num_of_bullets; ++i){
        if(BLT::IsBulletActive(i)){
            bool has_this_bullet_collided = false;
            esat::Vec2 bullet_pos = BLT::GetBulletPos(i), bullet_prev_pos = BLT::GetBulletPreviousPos(i);

            for(int j = 0; j < CFG::kNumRings * CFG::kSegmentsPerRing && has_this_bullet_collided == false; ++j){
                if(RNG::IsSegmentActive(j)){
                    esat::Vec2 segment_start_point = RNG::GetSegmentPointer(j), segment_end_point = RNG::GetSegmentPointer(j / CFG::kSegmentsPerRing * CFG::kSegmentsPerRing + ((j + 1) % CFG::kSegmentsPerRing));

                    if(DoesPointCollideSegment(bullet_pos, bullet_prev_pos, segment_start_point, segment_end_point)){
                        has_this_bullet_collided = true;
                        BLT::DeactivateBullet(BLT::GetBullet(i));
                        RNG::DeactivateSegment(j);
                    }
                }
            }
        }
    }
}

void COL::Update(){
    CheckBulletSegmentCollision();
}