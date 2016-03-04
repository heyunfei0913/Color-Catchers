#ifndef ___transmit_data_h__
#define __transmit_data_h__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define BOT_SR 0
#define TOP_SR 1
#define B_SR 2

/* modified to support two SR */
void transmit_data (unsigned char SR, unsigned char data) {
    /* BOT_SR uses A0 - A3  -- controls  GREEN, use '~' on data */
    if (SR == BOT_SR) {
        data = ~data;
        for (int i = 7; i >= 0; --i) {
            // Sets SRCLR to 1 allowing data to be set
            // Also clears SRCLK in preparation of sending data
            PORTA &= 0xF8; PORTA |= 0x08;
            // set SER = next bit of data to be sent.
            PORTA |= ((data >> i) & 0x01);
            // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
            PORTA |= 0x04;
        }
        // set RCLK = 1. Rising edge copies data from the “Shift” register to the “Storage” register
        PORTA |= 0x02;
        // clears all lines in preparation of a new transmission
        PORTA &= 0xF0;
    }
    /* TOP_SR uses A4 - A7 -- controls BLUE, use '~' on data */
    else if (SR == TOP_SR) {
        data = ~data;
        for (int i = 7; i >= 0; --i) {
            // Sets SRCLR to 1 allowing data to be set
            // Also clears SRCLK in preparation of sending data
            PORTA &= 0x8F; PORTA |= 0x80;
            // set SER = next bit of data to be sent.
            PORTA |= ((data >> i) & 0x01) << 4;
            // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
            PORTA |= 0x40;
        }
        // set RCLK = 1. Rising edge copies data from the “Shift” register to the “Storage” register
        PORTA |= 0x20;
        // clears all lines in preparation of a new transmission
        PORTA &= 0x0F;  
    }
    /* B_SR uses B0 - B3 -- control line, do NOT use '~' on data */
    else if (SR == B_SR) {
        for (int i = 7; i >= 0; --i) {
            // Sets SRCLR to 1 allowing data to be set
            // Also clears SRCLK in preparation of sending data
            PORTB &= 0xF8; PORTB |= 0x08;
            // set SER = next bit of data to be sent.
            PORTB |= ((data >> i) & 0x01);
            // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
            PORTB |= 0x04;
        }
        // set RCLK = 1. Rising edge copies data from the “Shift” register to the “Storage” register
        PORTB |= 0x02;
        // clears all lines in preparation of a new transmission
        PORTB &= 0xF0;
    }
}

#endif