#ifndef ___MATRIX_SM_h__
#define ___MATRIX_SM_h__

void displayMatrixStart();
void displayMatrixGame();

void displayOrbs();
void generateOrbs();
unsigned char pickColumn(unsigned char x);
unsigned char pickColor(unsigned char c);
void moveOrbDown();
void clearMatrix();
void clearGame();

int matrixTick (int state);

#endif