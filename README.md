# Color-Catchers
UCR CS120B Project

## Setup
- Color-Catchers uses an ATMega1284 microcontroller, three shift registers, a speaker, LCD display, and an 8x8 RGB LED matrix.
- A0 - A3 controls the green LEDs in the matrix using a shift register, A4 - A7 controls the blue LEDs in the matrix using a shift register
- B0 - B3 controls the selection of LED's in the matrix
- B6 controls the sound effects played in-game for denoting color changes, being damaged, victory, and defeat.
- D2 controls the RGB LED used to denote the current phase of the game
- D3 - D4 are button inputs
- D5, D6 and C0 - C7 are used for the LCD display

## How to Play
The player is in the bottom row of the LED matrix and must avoid the colored orbs they are not while collecting
whichever colored orbs they are. The player will change colors throughout the game and the fall speed of the orbs
will also increase.

## Bugs
To be added


