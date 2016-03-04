#ifndef ___BUTTON_SM_h__
#define ___BUTTON_SM_h__

#include "Button_SM.h"

enum pollButtonsState {
    Init_Poll, 
    Poll_Start_RELEASE, Poll_Start_HOLD,
    Poll_Game_RELEASE, Poll_Game_HOLD, 
    Poll_End_RELEASE, Poll_End_HOLD,
    Poll_Reset_HOLD
    } pollButtons;
    
int pollButtonsTick (int state) {
    button_L = ~PIND & 0x20; // D5
    button_M = ~PIND & 0x10; // D4
    button_R = ~PIND & 0x08; // D3
    
    switch(state) {
        case(Init_Poll):
            game_ctrl = INIT_GAME;
            state = Poll_Start_RELEASE;
            break;
        case(Poll_Start_RELEASE):
            game_ctrl = START_GAME;
            if (button_M) {
                state = Poll_Start_HOLD;
                break;
            }
            else {
                state = Poll_Start_RELEASE;
               break; 
            }
        case(Poll_Start_HOLD):
            game_ctrl = START_GAME;
            if (!button_M) {
                state = Poll_Game_RELEASE;
                break;
            }
            else {
                state = Poll_Start_HOLD;
                break;
            }
        case(Poll_Game_RELEASE):
            game_ctrl = PLAY_GAME;
            if (button_L) {
                if (DEBUG_MODE) {
                    score += 5;
                    update_LCD = 1;
                }   
                else {
                    moveLeftSignal = 1;
                }
                state = Poll_Game_HOLD;
                break;
            }
            else if (button_R) {     
                moveRightSignal = 1;
                state = Poll_Game_HOLD;
                break;
            }        
            else if (button_M) {
                --lives;
                if (lives == 0) {
                    state = Poll_End_HOLD;
                    break;
                }
                update_LCD = 1;
                state = Poll_Game_HOLD;
                break;
            }
            else {
                state = Poll_Game_RELEASE;
                break;
            }
        case(Poll_Game_HOLD):
            game_ctrl = PLAY_GAME;
            if (button_L || button_M || button_R) {
                state = Poll_Game_HOLD;
                break;
            }
            else {
                state = Poll_Game_RELEASE;
                break;
            }
        case(Poll_End_RELEASE):
            game_ctrl = END_GAME;
            if (button_M) {
                state = Poll_Reset_HOLD;
                break;
            }
            else {
                state = Poll_End_RELEASE;
                break;
            }
        case(Poll_End_HOLD):
            game_ctrl = END_GAME;
            if (!button_M) {
                state = Poll_End_RELEASE;
                break;
            }
            else {
                state = Poll_End_HOLD;
                break;
            }
        case(Poll_Reset_HOLD):
            game_ctrl = RESET_GAME;
            if (button_M) {
                state = Poll_Reset_HOLD;
                break;
            }
            else {
                state = Poll_Start_RELEASE;
                break;
            }
        default:
            state = Init_Poll;
            break;
    }
    return state;
}

#endif