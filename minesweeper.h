/* minesweeper.c HEADER FOR FUNC DECLARATIONS */
#ifndef MSWP_H
#define MSWP_H

#include <raylib.h>
#include "mswpUI.h"

/* GLOBAL VARIABLES */

extern int cellInitialSize;
extern int graphicalScale;
extern int cellSize;

extern int cellCols;
extern int cellRows;
extern int cellNum;

extern int windowWidth;
extern int windowHeight;

extern short isBoardRandomized;
extern short isGameDone;

extern Cell* board;

/* FUNCTION DECLARATIONS */
void createMinefield();
void allocateBoard();
void freeBoard();
void randomizeBoard();
void setBoardCells();
void rngBoardWithSafeguard(int safeCellID);
void startingThings();
void revealedBomb(int cum);
void setTextureForNumberCell(int bombAmount, int idx);
void revealedNumber(int cum);
void revealedEmpty(int idx);
short checkBombsInRadius(int idx);
void freeCheckedSpaces();
void zeroOutCheckedSpaces();
void checkForWin();
void restartGame(int randomize);
void revealedEmptyLogic(int cell);
void checkCIRE(int idx);
void checkCIRE(int idx);
void setIterationBuff(int idx);
void chordHold(int idx);
void chordReset(int idx);
void chordRelease(int idx);

#endif