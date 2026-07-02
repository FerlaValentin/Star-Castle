#include "game_utils.h"

#include <stdio.h> //! DELETE AFTER REMOVING DEBUG FUNCTIONS
#include <float.h>

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

void UTL::NormalizePoints(esat::Vec2* points, int num_of_points){
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