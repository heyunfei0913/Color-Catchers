#ifndef ___color_catchers_h__
#define __color_catchers_h__

#define DEFAULT_LIVES 5
#define MIN_SCORE 15

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
    
    
enum color_phase {
    Phase_Blue, Phase_Green
    };
    
unsigned char update_LCD = 0;
unsigned char moveLeftSignal = 0;
unsigned char moveRightSignal = 0;

unsigned short phase_timer = 0;
#define CHANGE_PERIOD 100

#define BLINK_PERIOD 15

/* shared variables */
unsigned long score = 0;
unsigned char lives = 0;

/* player */
struct player_struct {
    enum color_phase phase;
    unsigned char playerLeft;
    unsigned char playerRight;
    };

struct player_struct my_player;

unsigned long randomizer_seed = 12345;
unsigned char random_num = 0;

unsigned char DEBUG_MODE = 0;

#endif