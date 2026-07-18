#include "game_utils.h"

#include <stdio.h> //! DELETE AFTER REMOVING DEBUG FUNCTIONS
#include <float.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <esat\math.h>

#include "config.h"

static float GetAbsoluteValue(float number){
    return number < 0 ? number * -1 : number;
}

static float FindBiggestComponent(esat::Vec2* points, int num_of_points){
    float biggest_component = FLT_MIN;
    for(int i = 0; i < num_of_points; ++i){
        if(GetAbsoluteValue((*(points + i)).x) > biggest_component) biggest_component = GetAbsoluteValue((*(points + i)).x);
        if(GetAbsoluteValue((*(points + i)).y) > biggest_component) biggest_component = GetAbsoluteValue((*(points + i)).y);
    }

    return biggest_component;
}

void UTL::NormalizePoints(esat::Vec2* const points, const int num_of_points){
    const float biggest_comp = FindBiggestComponent(points, num_of_points);

    for(int i = 0; i < num_of_points; ++i){
        (*(points + i)).x /= biggest_comp;
        (*(points + i)).y /= biggest_comp;
    }
}

void UTL::DebugPoints(const char* const debug_header, esat::Vec2* points, int num_of_points){
    if(debug_header){
        printf("%s\n", debug_header);
        for(int i = 0; i < 40; ++i)
            printf("=");
        printf("\n");
    }
    for(int i = 0; i < num_of_points; ++i)
        printf("POINT %d: [X] %f [Y] %f\n", i, (*(points + i)).x, (*(points + i)).y);
    printf("\n");
}

float UTL::AngleToRadians(float angle){
    return angle * M_PI / 180;
}

static esat::Mat3 GetTransformationMatrix(float scale, float angle, esat::Vec2 translate){
    esat::Mat3 tmp = esat::Mat3Identity();

    tmp = esat::Mat3Multiply(esat::Mat3Scale(scale, scale), tmp);
    tmp = esat::Mat3Multiply(esat::Mat3Rotate(UTL::AngleToRadians(angle)), tmp);
    tmp = esat::Mat3Multiply(esat::Mat3Translate(translate.x, translate.y), tmp);

    return tmp;
}

void UTL::TransformWorldPoints(esat::Vec2* const world_points, const esat::Vec2* const local_points, const int num_of_points, float scale, float angle, esat::Vec2 translate){
    esat::Mat3 transf_matrix = GetTransformationMatrix(scale, angle, translate);

    for(int i = 0; i < num_of_points; ++i)
        *(world_points + i) = esat::Mat3TransformVec2(transf_matrix, *(local_points + i));
}

void UTL::InitCircle(esat::Vec2 *circle, int num_of_points){
    for(int i = 0; i < num_of_points; ++i){
        const float angle = 360 / num_of_points * i;
        *(circle + i) = {cosf(UTL::AngleToRadians(angle)), sinf(UTL::AngleToRadians(angle))};
    }
}

float UTL::GetMagnitude(const esat::Vec2* const vec){
    return sqrtf((*vec).x * (*vec).x + (*vec).y + (*vec).y);
}

esat::Vec2 UTL::NormalizeVector(const esat::Vec2* const vec){
    esat::Vec2 tmp = {(*vec).x / UTL::GetMagnitude(vec), (*vec).y / UTL::GetMagnitude(vec)};

    return tmp;
}

static void AssignLoopLimits(const float* const comp1, const float* const comp2, char* i, char* j, int last_point_index){
    if(*comp1 > *comp2){
        *i = -last_point_index;
        *j = 0;
    }
    else{
        *i = 0;
        *j = last_point_index;
    }
}

static void MovePoints(esat::Vec2* const world_points, int last_point_index, int new_position, bool is_x_axis){
    char i, j;

    if(new_position){
        if(is_x_axis)
            AssignLoopLimits(&(*world_points).x, &(*(world_points + last_point_index)).x, &i, &j, last_point_index);
        else
            AssignLoopLimits(&(*world_points).y, &(*(world_points + last_point_index)).y, &i, &j, last_point_index);
    }
    else{
        if(is_x_axis)
            AssignLoopLimits(&(*(world_points + last_point_index)).x, &(*world_points).x, &i, &j, last_point_index);
        else
            AssignLoopLimits(&(*(world_points + last_point_index)).y, &(*world_points).y, &i, &j, last_point_index);
    }
    for(i; i <= j; ++i){
        if(is_x_axis){
            const float points_difference = (*(world_points + (int)GetAbsoluteValue(i))).x - (*(world_points + j)).x;
            (*(world_points + (int)GetAbsoluteValue(i))).x = new_position ? new_position + points_difference : new_position - points_difference;
        }
        else{
            const float points_difference = (*(world_points + (int)GetAbsoluteValue(i))).y - (*(world_points + j)).y;
            (*(world_points + (int)GetAbsoluteValue(i))).y = new_position ? new_position + points_difference : new_position - points_difference;
        }
    }
}

void UTL::ScreenWrapObject(esat::Vec2* const world_points, int last_point_index){
    if((*(world_points + last_point_index)).x < 0.0f)  MovePoints(world_points, last_point_index, CFG::kScreenX, true);
    if((*(world_points + last_point_index)).y < 0.0f)  MovePoints(world_points, last_point_index, CFG::kScreenY, false);
    if((*world_points).x > CFG::kScreenX)  MovePoints(world_points, last_point_index, 0, true);
    if((*world_points).y > CFG::kScreenY)  MovePoints(world_points, last_point_index, 0, false);
}