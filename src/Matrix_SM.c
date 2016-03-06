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
    } startMatrixSubstate = Init_Start_SS;

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
    const unsigned short SOLID_PERIOD = 1500;
    const unsigned short CONVERGE_PERIOD = 1500;
    const unsigned short SOLID_STATE = 375;
    const unsigned short CONV_STATE = 375;
    static unsigned char i = 0;
    static unsigned short startTimer = 0;
    static unsigned char color_toggle = 0;
    switch (startMatrixSubstate) {
        case(Init_Start_SS):
            clearMatrix();
            startTimer = 0;
            color_toggle = 0;
            i = 0;
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
                clearMatrix();
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
                clearMatrix();
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

enum gameMatrixSubstates {
    Init_Game_SS,
    Play_Game_SS
    } gameMatrixSubstate = Init_Game_SS;

#define NUM_COLUMN 8
#define NUM_COLORS 2
#define BLUE_ORB 0
#define GREEN_ORB 1
static unsigned char blueCount = 0;
static unsigned char greenCount = 0;
static unsigned long randSeed = 5294;

unsigned char pickColumn(unsigned char x) {
    unsigned char column;
    if (x == 0) {
        column = 0x01;
    }
    else if (x == 1) {
        column = 0x02;
    }
    else if (x == 2) {
        column = 0x04;
    }
    else if (x == 3) {
        column = 0x08;
    }
    else if (x == 4) {
        column = 0x10;
    }
    else if (x == 5) {
        column = 0x20;
    }
    else if (x == 6) {
        column = 0x40;
    }
    else if (x == 7) {
        column = 0x80;
    }
    return column;
}

unsigned char pickColor(unsigned char c) {
    unsigned char color;
    if (c == 0) {
        color = BLUE_ORB;
    }
    else if (c == 1) {
        color = GREEN_ORB;
    }
    return color;
}

/* pick random (x) to generate one orb at -- populates array */
/* tfw no vectors in C. feels bad man */
void generateOrbs() {
    static unsigned char color = 0;
    if (orbCount < MAX_ORBS) {
        const unsigned char topRow = 0x01;
        unsigned char randomX = rand() % NUM_COLUMN;
        unsigned char columnX = pickColumn(randomX);
        if (ALT_SPAWN) {
            if (color == 0) {
                color = 1;
            }
            else {
                color = 0;
            }
        }
        else {
            unsigned char randomColor = rand() % NUM_COLORS;
            color = pickColor(randomColor);
        }
        
        // find flagged Y to write to
        for (unsigned i = 0; i < MAX_ORBS; ++i) {
            if (yControl_arr[i] == 0x00 && color == BLUE_ORB) {
                blueOrbs_arr[i] = columnX;
                yControl_arr[i] = topRow;
                ++orbCount;
                break;
            }
            else if (yControl_arr[i] == 0x00 && color == GREEN_ORB) {
                greenOrbs_arr[i] = columnX;
                yControl_arr[i] = topRow;
                ++orbCount;
                break;
            }
        }
    }  
}

/* shift all orbs down (y); checks if orb goes out of bounds and removes it */
void moveOrbDown() {
    // if oob orb flag it, shift non-oob orbs
    for (unsigned short i = 0; i < MAX_ORBS; ++i) {
        if (yControl_arr[i] >= Y_BOUND) {
            if (orbCount > 0) {
                --orbCount;
                yControl_arr[i] = 0x00;
                blueOrbs_arr[i] = 0x00;
                greenOrbs_arr[i] = 0x00;
            }
            break;
        }
    }
    for (unsigned short i = 0; i < MAX_ORBS; ++i) {
        if (yControl_arr[i] != 0x00) {
            yControl_arr[i] = yControl_arr[i] << 1;
        }            
    }
}

void displayOrbs() {
    static unsigned short i = 0;
    if (i >= MAX_ORBS) {
        i = 0;
    }
    for (; i < MAX_ORBS; ++i) {
        if (blueOrbs_arr[i] != 0x00) {
            clearMatrix();
            transmit_data(TOP_SR, blueOrbs_arr[i]);
            transmit_data(B_SR, yControl_arr[i]);
            ++i;
            break;
        }
        if (greenOrbs_arr[i] != 0x00) {
            clearMatrix();
            transmit_data(BOT_SR, greenOrbs_arr[i]);
            transmit_data(B_SR, yControl_arr[i]);
            ++i;
            break;
        }
    }
    /* lowers brightness */
    clearMatrix();
}

void displayMatrixGame() {
    /* prime periods to avoid awkward generation/shifting */
    unsigned short GENERATE_PERIOD = 457;
    unsigned short SHIFT_PERIOD = 263;
    if (BULLET_HELL) {
        GENERATE_PERIOD = GENERATE_PERIOD / 4;
        SHIFT_PERIOD = SHIFT_PERIOD / 4;
    }
    static unsigned short generateTimer = 0;
    static unsigned short shiftTimer = 0;
    switch(gameMatrixSubstate) {
        case(Init_Game_SS):
            generateTimer = 0;
            shiftTimer = 0;
            orbCount = 0;
            gameMatrixSubstate = Play_Game_SS;
            break;
        case(Play_Game_SS):
            displayOrbs();
            if (generateTimer == GENERATE_PERIOD) {
                generateOrbs();
                generateTimer = 0;   
            }
            if (shiftTimer == SHIFT_PERIOD) {
                moveOrbDown();
                shiftTimer = 0;
            }
            ++generateTimer;
            ++shiftTimer;      
            break;
         default:
            gameMatrixSubstate = Init_Game_SS;
            break;  
    }
}

void clearMatrix() {
    transmit_data(B_SR, 0x00);
    transmit_data(TOP_SR, 0x00);
    transmit_data(BOT_SR, 0x00);
}

void clearGame() {
    startMatrixSubstate = Init_Start_SS;
    gameMatrixSubstate = Init_Game_SS;
    for (unsigned short i = 0; i < MAX_ORBS; ++i) {
        greenOrbs_arr[i] = 0;
        blueOrbs_arr[i] = 0;
        yControl_arr[i] = 0;
    }
}

int matrixTick (int state) {
    switch(state) {
        case(Init_Matrix):
            srand(randSeed);
            clearMatrix();
            state = Start_Matrix;
            break;
        case(Start_Matrix):
            if (game_ctrl == PLAY_GAME) {
                srand(randSeed);
                clearMatrix();
                state = Game_Matrix;
                break;
            }
            else {
               ++randSeed;
               displayMatrixStart();
            }
            state = Start_Matrix;
            break;
         case(Game_Matrix):
            if (game_ctrl == END_GAME) {
                state = End_Matrix;
                break;
            }
            displayMatrixGame();
            state = Game_Matrix;
            break;
         case(End_Matrix):
            if (game_ctrl == RESET_GAME) {
                state = Reset_Matrix;
                break;
            }
            displayMatrixGame();
            state = End_Matrix;
            break;
         case(Reset_Matrix):
            clearMatrix();
            clearGame();
            state = Start_Matrix;
            break;
         default:
            state = Init_Matrix;
            break;    
    }
    return state;
}

#endif