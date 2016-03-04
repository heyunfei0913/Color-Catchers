#ifndef ___MATRIX_SM_h__
#define ___MATRIX_SM_h__

#include "Matrix_SM.h"

enum matrixStates {
    Init_Matrix, 
    Start_Matrix, Game_Matrix, End_Matrix, Reset_Matrix
    } matrixState;

enum startMatrixSubstates {
    Init_Start_SS,
    Display_Solid_SS,
    Display_Converge_SS
    } startMatrixSubstate;

#define BALL_SIZE 8
static unsigned char ball = 0;
static unsigned char ball_arr[BALL_SIZE] = {
    0x00,0x24,0x5a,0x3c,0x3c,0x5a,0x24,0x00
};

static unsigned char ballSelect = 0;
static unsigned char ballSelect_arr[BALL_SIZE] = {
    0x00,0x42,0x24,0x18,0x18,0x24,0x42,0x00
};

#define CONV_SIZE 4
static unsigned char convergeBall = 0;
static unsigned char convergeBall_arr[CONV_SIZE] = {
    0x81, 0x42, 0x24, 0x18
};

static unsigned char convergeBallSelect = 0;
static unsigned char convergeBallSelect_arr[CONV_SIZE] = {
    0x81, 0x42, 0x24, 0x18
};

void displayMatrixStart() {
    static const unsigned short SOLID_PERIOD = 1500;
    static const unsigned short CONVERGE_PERIOD = 1500;
    static const unsigned short SOLID_STATE = 375;
    static const unsigned short CONV_STATE = 375;

    static unsigned char startMatrixSubstate = Init_Start_SS;
    
    static unsigned char i = 0;
    static unsigned short startTimer = 0;
    static unsigned char color_toggle = 0;
    
    switch (startMatrixSubstate) {
        case(Init_Start_SS):
            transmit_data(B_SR, 0x00);
            transmit_data(TOP_SR, 0x00);
            transmit_data(BOT_SR, 0x00);
            startMatrixSubstate = Display_Converge_SS;
            break;
        case(Display_Converge_SS):
            if (startTimer >= SOLID_PERIOD) {
                startTimer = 0;
                i = 0;
                startMatrixSubstate = Display_Solid_SS;
                break;
            }
            if (startTimer % CONV_STATE == 0) {
                convergeBallSelect = convergeBallSelect_arr[i];
                convergeBall = convergeBall_arr[i];
                
                /* to avoid "bleed-over" */
                transmit_data(B_SR, 0x00);
                transmit_data(TOP_SR, 0x00);
                transmit_data(BOT_SR, 0x00);
                
                transmit_data(B_SR, convergeBallSelect);
                if (color_toggle) {
                    transmit_data(BOT_SR, convergeBall);
                }
                else {
                    transmit_data(TOP_SR, convergeBall);
                }
                
                if (i < BALL_SIZE) {
                    ++i;
                }
                else {
                    i = 0;
                }
            }
            ++startTimer;             
            startMatrixSubstate = Display_Converge_SS;
            break;
        case(Display_Solid_SS): 
            if (startTimer >= CONVERGE_PERIOD) {
                startTimer = 0;
                i = 0;
                color_toggle = ~color_toggle;
                startMatrixSubstate = Display_Converge_SS;
                break;
            }
            if (startTimer <= SOLID_STATE ||
                     startTimer >= SOLID_STATE * 2 &&
                     startTimer <= SOLID_STATE * 3) {
                ballSelect = ballSelect_arr[i];
                ball = ball_arr[i];
                
                /* to avoid "bleed-over" */
                transmit_data(B_SR, 0x00);
                transmit_data(TOP_SR, 0x00);
                transmit_data(BOT_SR, 0x00);
                
                transmit_data(B_SR, ballSelect);
                if (color_toggle) {
                    transmit_data(BOT_SR, ball);
                }
                else {
                    transmit_data(TOP_SR, ball);
                }
                
                if (i < BALL_SIZE) {
                    ++i;
                }
                else {
                    i = 0;
                }
            }
            else {
                transmit_data(B_SR, 0x00);
            }
            ++startTimer;
            startMatrixSubstate = Display_Solid_SS;
            break;
         default:
            startMatrixSubstate = Init_Start_SS;
            break;           
     }                   
}

void displayMatrixGame() {
    
}

void displayMatrixVictory() {
    
}

void displayMatrixGameOver() {
    
}

int matrixTick (int state) {
    switch(state) {
        case(Init_Matrix):
            srand(randomizer_seed);
            transmit_data(B_SR, 0x00);
            transmit_data(TOP_SR, 0x00);
            transmit_data(BOT_SR, 0x00);
            state = Start_Matrix;
            break;
        case(Start_Matrix):
            if (game_ctrl == PLAY_GAME) {
                state = Game_Matrix;
                break;
            }
            else {
               displayMatrixStart();
            }
            state = Start_Matrix;
            break;
         case(Game_Matrix):
            if (game_ctrl == END_GAME) {
                state = End_Matrix;
                break;
            }
            transmit_data(B_SR, 0x55);
            transmit_data(TOP_SR, 0x55);
            transmit_data(BOT_SR, 0x55);
            displayMatrixGame();
            state = Game_Matrix;
            break;
         case(End_Matrix):
            if (game_ctrl == RESET_GAME) {
                state = Reset_Matrix;
                break;
            }
            if (score >= MIN_SCORE) {
                displayMatrixVictory();
            }
            else {
                displayMatrixGameOver();
            }
            state = End_Matrix;
            break;
         case(Reset_Matrix):
            transmit_data(B_SR, 0x00);
            transmit_data(TOP_SR, 0x00);
            transmit_data(BOT_SR, 0x00);
            state = Start_Matrix;
            break;
         default:
            state = Init_Matrix;
            break;    
    }
    return state;
}

#endif