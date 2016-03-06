#ifndef ___LCD_SM_h__
#define ___LCD_SM_h__

#include "LCD_SM.h"

enum LCD_DisplayState {
    Init_LCD, 
    Start_LCD, Game_LCD, End_LCD, Reset_LCD
    } lcdState;

void init_CustomChar() {
    LCDdefinechar(heart, 0); // 0b00001000
    LCDdefinechar(trophyLeft, 1); // 0b00001001
    LCDdefinechar(trophyRight, 2); // 0b00001010
    LCDdefinechar(skull, 3); // 0b00001011
    LCDdefinechar(leftSmallWave, 4); // 0b00001100
    LCDdefinechar(rightSmallWave, 5); // 0b00001101
    LCDdefinechar(halfBall, 7); // 0b00001111
}

void displayStart() {
    LCD_Cursor(2); LCD_WriteData(0b00001100);
    LCD_Cursor(3); LCD_WriteData(0b00001111);
    LCD_Cursor(14); LCD_WriteData(0b00001111);
    LCD_Cursor(15); LCD_WriteData(0b00001101);
    
    const unsigned char play_string[] = "PLAY";
    unsigned char msg_column = 7;
    for (unsigned char i = 0; i < sizeof(play_string)-1; ++i) {
        LCD_Cursor(msg_column); LCD_WriteData(play_string[i]);
        ++msg_column;
    }
    
    const unsigned char color_catchers_string[] = "COLOR CATCHERS";
    msg_column = 18;
    for (unsigned char i = 0; i < sizeof(color_catchers_string)-1; ++i) {
        LCD_Cursor(msg_column); LCD_WriteData(color_catchers_string[i]);
        ++msg_column;
    }
}

void displayScore(unsigned long m_score) {
    const unsigned char score_string[] = " Score: ";
    unsigned char msg_column = 18;
    for (unsigned char i = 0; i < sizeof(score_string)-1; ++i) {
        LCD_Cursor(msg_column); LCD_WriteData(score_string[i]);
        ++msg_column;
    }
    
    LCD_WriteData(m_score % 1000000 % 100000 / 10000 + '0' );
    LCD_WriteData(m_score % 1000000 % 100000 % 10000 / 1000 + '0' );
    LCD_WriteData(m_score % 1000000 % 100000 % 10000 % 1000 / 100 + '0' );
    LCD_WriteData(m_score % 1000000 % 100000 % 10000 % 1000 % 100 / 10 + '0' );
    LCD_WriteData(m_score % 1000000 % 100000 % 10000 % 1000 % 100 % 10 + '0' );
}

void displayLives(unsigned char m_lives) {
    if (m_lives >= 1) {
        LCD_Cursor(1);
        LCD_WriteData(0b00001000);
    }
    if (m_lives >= 2) {
        LCD_Cursor(3);
        LCD_WriteData(0b00001000);
    }
    if (m_lives >= 3) {
        LCD_Cursor(5);
        LCD_WriteData(0b00001000);
    }
    if (m_lives >= 4) {
        LCD_Cursor(7);
        LCD_WriteData(0b00001000);
    }
    if (m_lives >= 5) {
        LCD_Cursor(9);
        LCD_WriteData(0b00001000);
    }
}

void displayVictory() {
    LCD_Cursor(1); LCD_WriteData(0b00001001);
    LCD_Cursor(2); LCD_WriteData(0b00001010);
    LCD_Cursor(15); LCD_WriteData(0b00001001);
    LCD_Cursor(16); LCD_WriteData(0b00001010);
    
    const unsigned char victory_string[] = "VICTORY!";
    unsigned char msg_column = 5;
    for (unsigned char i = 0; i < sizeof(victory_string)-1; ++i) {
        LCD_Cursor(msg_column); LCD_WriteData(victory_string[i]);
        ++msg_column;
    }
}

void displayGameOver() {
    LCD_Cursor(2); LCD_WriteData(0b00001011);
    LCD_Cursor(15); LCD_WriteData(0b00001011);
    
    const unsigned char gameover_string[] = "GAME OVER!";
    unsigned char msg_column = 4;
    for (unsigned char i = 0; i < sizeof(gameover_string)-1; ++i) {
        LCD_Cursor(msg_column); LCD_WriteData(gameover_string[i]);
        ++msg_column;
    }
}

int lcdTick (int state) {
    static unsigned short lcdTimer = 0;
    switch(state) {
        case(Init_LCD):
            update_LCD = 1;
            score = 0;
            lives = DEFAULT_LIVES;
            LCD_ClearScreen();
            state = Start_LCD;
            break;
        case(Start_LCD):
            if (game_ctrl == PLAY_GAME) {
                update_LCD = 1;
                LCD_ClearScreen();
                score = 0;
                lives = DEFAULT_LIVES;
                state = Game_LCD;
                break;
            }
            else if (update_LCD) {
                update_LCD = 0;
                displayStart();
            }                
            state = Start_LCD;
            break;              
        case(Game_LCD):
            if (game_ctrl == END_GAME) {
                LCD_ClearScreen();
                update_LCD = 1;
                state = End_LCD;
                break;
            }
            else if (update_LCD) {
                update_LCD = 0;
                LCD_ClearScreen();
                displayScore(score);
                displayLives(lives);
            }                
            state = Game_LCD;             
            break;              
        case(End_LCD):
            if (game_ctrl == RESET_GAME) {
                update_LCD = 1;
                state = Reset_LCD;
                break;
            }
            else if (update_LCD) {
                update_LCD = 0;
                LCD_ClearScreen();
                if (score >= MIN_SCORE) {
                    displayVictory();
                    displayScore(score);
                }
                else {
                    displayGameOver();
                    displayScore(score);
                }
            }                
            state = End_LCD;
            break;          
        case(Reset_LCD):
            update_LCD = 1;
            score = 0;
            lives = DEFAULT_LIVES;
            LCD_ClearScreen();
            state = Start_LCD;
            break;
        default:
            state = Init_LCD;
            break;
    }
    return state;
}
#endif