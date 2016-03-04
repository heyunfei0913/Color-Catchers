#ifndef ___LCD_SM_h__
#define ___LCD_SM_h__

void lcdTick (int state);
    
void init_CustomChar();
void displayStart();
void displayScore(unsigned long score);
void displayLives(unsigned char lives);
void displayVictory();
void displayGameOver();

#endif