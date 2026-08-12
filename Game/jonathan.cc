#include "jonathan.h"

#include <stdio.h>

#include <esat\math.h>


struct LineEquation{
    float cx, cy, ci;
};

struct LineEquationSystem{
    LineEquation eq[2];
};

//Equations
void PrintLineEquation(LineEquation line){
    printf("%6.2fx %6.2fy = %6.2f\n",line.cx,line.cy,line.ci);
}

void PrintLineEqSystem(LineEquationSystem system){
    printf("Equation System 2x2\n");
    for(unsigned char i = 0; i < 2; i++){   
        PrintLineEquation(system.eq[i]);
    }
}

float NegateX(LineEquationSystem system){
    LineEquationSystem system_aux;

    system_aux.eq[0] = {system.eq[0].cx*system.eq[1].cx, system.eq[0].cy*system.eq[1].cx, system.eq[0].ci*system.eq[1].cx};
    system_aux.eq[1] = {system.eq[1].cx*system.eq[0].cx, system.eq[1].cy*system.eq[0].cx, system.eq[1].ci*system.eq[0].cx};

    // PrintLineEqSystem(system_aux);

    if(system_aux.eq[0].cx == system_aux.eq[1].cx){
        system_aux.eq[0].cx *= -1;
        system_aux.eq[0].cy *= -1;
        system_aux.eq[0].ci *= -1;
    }

    system_aux.eq[0].cx += system_aux.eq[1].cx;
    system_aux.eq[0].cy += system_aux.eq[1].cy;
    system_aux.eq[0].ci += system_aux.eq[1].ci;

    if(system_aux.eq[0].cy == 0){
        return -1.0f; //SIN INTERSECCION
    }else{
        return system_aux.eq[0].ci/system_aux.eq[0].cy;
    }
}

float ReplaceY(LineEquation line, float y){
    return ((line.ci-(line.cy*y))/line.cx);
}

esat::Vec2 CalcLineEqSystem(LineEquationSystem system){
    esat::Vec2 res;
    res.y = NegateX(system);
    res.x = ReplaceY(system.eq[0],res.y);

    return res;
}

//Interpolation

esat::Vec2 JMN::CalcInterpolation(esat::Vec2 p1, esat::Vec2 v1, esat::Vec2 p2, esat::Vec2 v2){
    LineEquationSystem system = {
        {
            {v1.x, -v2.x, p2.x-p1.x},
            {v1.y, -v2.y, p2.y-p1.y}
        }
    };
    return CalcLineEqSystem(system);
}