#ifndef ___color_catchers_h__
#define __color_catchers_h__

#define DEFAULT_LIVES 3
unsigned short MIN_SCORE = 50;

/* button input variables */
unsigned char button_L = 0;
unsigned char button_M = 0;
unsigned char button_R = 0;

/* 
    Synchronize SMs to be in correct states.
    Only pollButtons will write to this!   
*/
enum game_control {
    INIT_GAME, START_GAME, PLAY_GAME, END_GAME, RESET_GAME, 
    } game_ctrl;
    
unsigned char update_LCD = 0;
unsigned char moveLeftSignal = 0;
unsigned char moveRightSignal = 0;
unsigned char changeSignal = 0; // sfx

/* shared variables */
unsigned long score = 0;
unsigned short lives = 0;

/* player's column and row info */
unsigned char player_x = 0;
const unsigned char player_y = 0x80;
/* player color */
unsigned char playerColor = 0;
#define BLUE 1
#define GREEN 2

/* max collection of orbs */
#define MAX_ORBS 24
#define Y_BOUND 0x80
unsigned long orbCount = 0;
/* holds column information of orb[i] */
unsigned char greenOrbs_arr[MAX_ORBS] = { 0 };
unsigned char blueOrbs_arr[MAX_ORBS] = { 0 };
/* holds row information of orb[i] */
unsigned char yControl_arr[MAX_ORBS];

/* game modes */
unsigned char ALT_SPAWN = 0; // alternating orb color generation
unsigned char BULLET_HELL = 0;

unsigned char DEBUG_MODE = 0;

#endif