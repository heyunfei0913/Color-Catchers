#ifndef ___SFX_SM_h__
#define ___SFX_SM_h__

#include "SFX_SM.h"

#define C3  130.81
#define Db3  138.59
#define D3  146.83
#define Eb3  155.56
#define E3  164.81
#define F3  174.61
#define Gb3  185.00
#define G3  196.00
#define Ab3  207.65
#define A3  220.00
#define Bb3  233.08
#define B3  246.94

#define C4  261.63
#define Db4  277.18
#define D4  293.66
#define Eb4  311.13
#define E4  329.63
#define F4  349.23
#define Gb4  369.99
#define G4  392.00
#define Ab4  415.30
#define A4  440.00
#define Bb4  466.16
#define B4  493.88

#define C5  523.25
#define Db5  554.37
#define D5  587.33
#define Eb5  622.25
#define E5  659.25
#define F5  698.46
#define Gb5  739.99
#define G5  783.99
#define Ab5  830.61
#define A5  880.00
#define Bb5  932.33
#define B5  987.77

enum SFXstates {
    Init_SFX, StartSFX, GameStartSFX, EndSFX
    } sfxState;
    
double AM[3] = { A4, Db5, E5 }; // gameStart
double CM[3] = { C5, E5, G5 }; // playGameStart
double phaseChangeSFX[1] = { A4 }; // phaseChange
double BM[4] = { B4, Eb5, Gb5, Gb5 }; // victory
double oneUpSFX[3] = { E4, G4, E5 }; // one up
double Am[4] = { A4, C4, E4, E4}; // game 

int sfxTick (int state) {
    static unsigned char startSFXFlag = 0;
    static unsigned char gameSFXFlag = 0;
    static unsigned char changeSFXFlag = 0;
    static unsigned char endSFXFlag = 0;
    static unsigned char i = 0;
    static unsigned char j = 0;
    static unsigned char k = 0;
    
    switch(state) {
        case(Init_SFX):
            state = StartSFX;
            gameSFXFlag = 0;
            i = 0; j = 0; k =0;
            break;
        case(StartSFX):
            if (game_ctrl == PLAY_GAME) {
                PWM_on();
                i = 0; j = 0;
                state = GameStartSFX;
                break;
            }
            if (i >= 3) {
                PWM_off();
            }
            else if (i < 3) {
                set_PWM(AM[i]);
                ++i;
            }
            state = StartSFX;
            break;
        case(GameStartSFX):
            if (game_ctrl == END_GAME) {
                PWM_on();
                i = 0; j = 0;
                state = EndSFX;
                break;
            }
            if (!gameSFXFlag) {
                if (i >= 3) {
                    PWM_off();
                    i = 0;
                    gameSFXFlag = 1;
                }
                set_PWM(CM[i]);
                ++i;
            }
            else if (changeSignal && gameSFXFlag && !oneUpSignal) {
                if (j >= 1) {
                    changeSignal = 0;
                    j = 0;
                    PWM_off();
                }
                else {
                    PWM_on();
                    set_PWM(phaseChangeSFX[i]);
                    ++j;
                }
            }
            else if (oneUpSignal && gameSFXFlag) {
                if (k >= 3) {
                    oneUpSignal = 0;
                    k = 0;
                    PWM_off();
                }
                else {
                    PWM_on();
                    set_PWM(oneUpSFX[k]);
                    ++k;
                }
            }
            state = GameStartSFX;
            break;
        case(EndSFX):
            if (game_ctrl == START_GAME) {
                PWM_on();
                i = 0; j = 0; k = 0;
                gameSFXFlag = 0;
                state = START_GAME;
                break;
            }
            if (i >= 4) {
                PWM_off();
            }
            else if (score >= MIN_SCORE && i < 4) {
                set_PWM(BM[i]);
                ++i;
                
            }
            else if (score < MIN_SCORE && i < 4) {
                set_PWM(Am[i]);
                ++i;
            }
            state = EndSFX;
            break;           
        default:
            state = Init_SFX;
            break;        
    }
    return state;
}




#endif