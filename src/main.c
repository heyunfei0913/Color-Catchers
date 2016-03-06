#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

/* non-original code */
#include "bit.h"
#include "timer.h"
#include "io.c"
#include "scheduler.h"
#include "pwm.c"

/* modified non-original code */
#include "custom_char.c"
#include "transmit_data.h"

/* original code */
#include "Color_Catchers.h"
#include "Matrix_SM.c"
#include "Player_Matrix_SM.c"
#include "LCD_SM.c"
#include "Button_SM.c"
#include "SFX_SM.c"

int main(void) {  
    /* A is used for top and bottom SR */
    DDRA = 0xFF; PORTA = 0x00;
    /* B is used for controlling LED matrix */
    DDRB = 0xFF; PORTB = 0x00;
    /* C is LCD display */
    DDRC = 0xFF; PORTC = 0x00;
    /* Want D2 to be PWM, D5 to D3 are button inputs, D6 and D7 are LCD */
    DDRD = 0xC0; PORTD = 0xFF;

    /* LCD Display Task Scheduler */
    const unsigned long myPeriod = 1;
    const unsigned long periodPollButtons = 50;
    const unsigned long periodLCD = 100;
    const unsigned long periodMatrix = 1;
    const unsigned long periodPlayer = 1;
    const unsigned long periodSFX = 500;
    
    /* Initialize scheduler */
    const unsigned short NUM_TASKS = 5;
    task tasks[NUM_TASKS];
    unsigned short i = 0;
    
    tasks[i].state = Init_Matrix;
    tasks[i].period = periodMatrix;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &matrixTick;
    ++i;
    
    tasks[i].state = Init_Player_Matrix;
    tasks[i].period = periodPlayer;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &playerMatrixTick;
    ++i;
    
    tasks[i].state = Init_Poll;
    tasks[i].period = periodPollButtons;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &pollButtonsTick;
    ++i;
    
    tasks[i].state = Init_SFX;
    tasks[i].period = periodSFX;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &sfxTick;
    ++i;
    
    tasks[i].state = Init_LCD;
    tasks[i].period = periodLCD;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &lcdTick;
    ++i;
    
    game_ctrl = INIT_GAME;
    init_CustomChar();
    LCD_init();
    TimerSet(myPeriod);
    TimerOn();
    PWM_on();
    
    while (1) {
        for (i = 0; i < NUM_TASKS; i++) {
            if (tasks[i].elapsedTime >= tasks[i].period){
                tasks[i].state = tasks[i].TickFct(tasks[i].state);
                tasks[i].elapsedTime = 0;
            }
            tasks[i].elapsedTime += myPeriod;
        }
        while(!TimerFlag);
        TimerFlag = 0;
    }
}



