#include "game_utils.h"

#include <stdio.h> //! DELETE AFTER REMOVING DEBUG FUNCTIONS
#include <float.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <esat\math.h>

static float GetAbsoluteValue(float number){
    return number < 0 ? number * -1 : number;
}

static float FindBiggestComponent(esat::Vec2* points, int num_of_points){
    float biggest_component = FLT_MIN;
    for(int i = 0; i < num_of_points; ++i){
        if(GetAbsoluteValue((*(points + i)).x) > biggest_component) biggest_component = (*(points + i)).x;
        if(GetAbsoluteValue((*(points + i)).y) > biggest_component) biggest_component = (*(points + i)).y;
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

static esat::Mat3 GetTransformationMatrix(float scale, float translate_x, float translate_y){
    esat::Mat3 tmp = esat::Mat3Identity();

    tmp = esat::Mat3Multiply(esat::Mat3Scale(scale, scale), tmp);
    tmp = esat::Mat3Multiply(esat::Mat3Translate(translate_x, translate_y), tmp);

    return tmp;
}

void UTL::TransformWorldPoints(esat::Vec2* const world_points, const esat::Vec2* const local_points, const int num_of_points, float scale, float translate_x, float translate_y){
    esat::Mat3 transf_matrix = GetTransformationMatrix(scale, translate_x, translate_y);

    for(int i = 0; i < num_of_points; ++i)
        *(world_points + i) = esat::Mat3TransformVec2(transf_matrix, *(local_points + i));
}

static float AngleToRadians(float angle){
    return angle * M_PI / 180;
}

void UTL::InitCircle(esat::Vec2 *circle, int num_of_points){
    for(int i = 0; i < num_of_points; ++i){
        const float angle = 360 / num_of_points * i;
        *(circle + i) = {cosf(AngleToRadians(angle)), sinf(AngleToRadians(angle))};
    }
}