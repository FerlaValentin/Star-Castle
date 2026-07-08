#include <stdio.h>

#include <esat\window.h>
#include <esat\draw.h>
#include <esat\input.h>
#include <esat\time.h>

#include "config.h"
#include "game.h"

static unsigned char fps=25; //Control de frames por segundo
static double current_time,last_time=0.0,dt;

void InitFrame(){
    // Calculate time elapsed since the last frame
    current_time = esat::Time();
    dt = (current_time - last_time) / 1000.0;
    // Limit delta_time to avoid large jumps
    if (dt > 0.1) dt = 0.1;
    last_time = current_time;
}

void FinishFrame(){
    do{
        current_time = esat::Time();
    }while((current_time-last_time)<=1000.0/fps);
}

int esat::main(int argc, char **argv){
	esat::WindowInit(CFG::kScreenX, CFG::kScreenY);
	WindowSetMouseVisibility(true);
    GAME::Init();
    while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
        InitFrame();
    	esat::DrawBegin();
    	esat::DrawClear(0,0,0);

        GAME::GetInput();
        GAME::Update(&dt);
        GAME::Draw();

    	esat::DrawEnd();  	
    	esat::WindowFrame();
        FinishFrame();
    }
    esat::WindowDestroy();
    GAME::Free();

    return 0;  
}