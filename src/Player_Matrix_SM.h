#ifndef ___PLAYER_MATRIX_SM_h__
#define ___PLAYER_MATRIX_SM_h__

void changePhase();
void detectCollision();
void checkPlayerBounds();
void movePlayerLeft();
void movePlayerRight();
void displayPlayer();

int playerMatrixTick (int state);

#endif