#ifndef ___PLAYER_MATRIX_SM_h__
#define ___PLAYER_MATRIX_SM_h__

#include "Matrix_SM.h"

enum playerMatrixStates {
    Init_Player_Matrix, Start_Player, Game_Player, End_Player, Reset_Player
    } playerMatrixState;

void displayPlayer() {
    if (BULLET_HELL) {
        transmit_data(TOP_SR, player_x);
        transmit_data(BOT_SR, player_x);
        transmit_data(B_SR, player_y);
    }
    else {
        if (playerColor == BLUE) {
            transmit_data(TOP_SR, player_x);
        }
        else if (playerColor == GREEN) {
            transmit_data(BOT_SR, player_x);
        }
        transmit_data(B_SR, player_y);
    }
    /* lowers brightness */
    clearMatrix();
}

void changeColor() {
    if (playerColor == BLUE) {
        playerColor = GREEN;
        changeSignal = 1;
    }
    else if (playerColor == GREEN) {
        playerColor = BLUE;
        changeSignal = 1;
    }
}

void detectCollision() {
    for (unsigned char i = 0; i < MAX_ORBS; ++i) {
        if (blueOrbs_arr[i] == player_x && yControl_arr[i] == player_y) {
            blueOrbs_arr[i] = 0x00;
            yControl_arr[i] = 0x00;
            --orbCount;
            if (playerColor == BLUE) {
                score += 5;
                update_LCD = 1;
                return;
            }
            else {
                --lives;
                update_LCD = 1;
                return;
            }
        }
    }
    for (unsigned char i = 0; i < MAX_ORBS; ++i) {
        if (greenOrbs_arr[i] == player_x && yControl_arr[i] == player_y) {
            greenOrbs_arr[i] = 0x00;
            yControl_arr[i] = 0x00;
            --orbCount;
            if (playerColor == GREEN) {
                score += 5;
                update_LCD = 1;
                return;
            }
            else {
                --lives;
                update_LCD = 1;
                return;
            }
        }
    }
    
}

void movePlayerLeft() {
    if (player_x != 0x80) {
        player_x = player_x << 1;
    }
}

void movePlayerRight() {
    if (player_x != 0x01) {
        player_x = player_x >> 1;
    }
}

int playerMatrixTick (int state) {
    const unsigned short phaseChangePeriod = 3000;
    static unsigned short phaseChangeTimer = 0;
    switch(state) {
        case(Init_Player_Matrix):
            player_x = 0x08;
            playerColor = BLUE;
            state = Start_Player;
            break;
        case(Start_Player):
            if (game_ctrl == PLAY_GAME) {
                state = Game_Player;
                break;
            }
            state = Start_Player;
            break;
        case(Game_Player):
            if (game_ctrl == END_GAME) {
                state = End_Player;
                break;
            }
            if (phaseChangeTimer == phaseChangePeriod) {
                phaseChangeTimer = 0;
                changeColor();
            }
            else {
                ++phaseChangeTimer;
            }
            detectCollision();
            displayPlayer();
            if (moveLeftSignal) {
                movePlayerLeft();
                moveLeftSignal = 0;
            }
            else if (moveRightSignal) {
                movePlayerRight();
                moveRightSignal = 0;
            }
            state = Game_Player;
            break;
        case(End_Player):
            if (game_ctrl == RESET_GAME) {
                state = Reset_Player;
                break;
            }
            state = End_Player;
            break;
        case(Reset_Player):
            player_x = 0x08;
            playerColor = BLUE;
            phaseChangeTimer = 0;
            state = Start_Player;
            break;
        default:
            state = Init_Player_Matrix;
            break;
    }
    return state;
}

#endif