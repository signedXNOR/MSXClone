/* GAME LOGIC FILE */

#include <stdio.h>
#include <stdlib.h>

#include "mswpUI.h"
#include "minesweeper.h"

int cellInitialSize = 16;
int graphicalScale = 2;
int cellSize;

int cellCols = 30;
int cellRows = 16;
int cellNum;

int windowWidth;
int windowHeight;

int mineDensity = 99;

short isBoardRandomized = false;
short isGameDone = false;

Cell* board;

short* checkedSpaces;

short CIRE[] = {0, 0, 0, 0, 0, 0, 0, 0}; /* Cells In Radius E???*/

int iterationBuff[8];

short textureBuff[8] = {0,0,0,0,0,0,0,0};

int main(void) 
{
    startingThings();
    renderGame();
}

void startingThings()
{
    mwspUISetRandomSeed();
    cellSize = cellInitialSize*graphicalScale;
    cellNum = cellCols*cellRows;
    windowWidth = (24*graphicalScale)+cellSize*cellCols;
    windowHeight = (67*graphicalScale)+cellSize*cellRows;
    checkedSpaces = malloc(cellNum * sizeof(int));
    zeroOutCheckedSpaces();
}

void zeroOutCheckedSpaces()
{
    for (int i = 0; i<cellNum; i++)
    {
        checkedSpaces[i] = 0;
    }
}

void freeCheckedSpaces()
{
    free(checkedSpaces);
    checkedSpaces = NULL;
}

void createMinefield()
{
    allocateBoard();
    setBoardCells();
}

void allocateBoard()
{
    board = malloc(cellNum * sizeof(Cell));
}

void freeBoard()
{
    free(board);
    board = NULL;
}

void setBoardCells()
{
    for (int i = 0; i < cellRows; i++)
    {
        for(int j = 0; j < cellCols; j++)
        {
            board[i*cellCols+j].id = i*cellCols+j;
            board[i*cellCols+j].xPixels = j*cellSize;
            board[i*cellCols+j].yPixels = i*cellSize;
            board[i*cellCols+j].texture = CELL_DEFAULT;
        }
    }
}

void rngBoardWithSafeguard(int safeCellID)
{
    randomizeBoard();
    board[safeCellID].bombStatus = 0;
    int rng;
    while(true)
    {
        rng = GetRandomValue(0, cellNum-1);
        if (board[rng].bombStatus == 1 && rng != safeCellID) { board[rng].bombStatus = 0; break;}
    }
}

void restartGame(int randomize)
{
    face = 33;
    zeroOutCheckedSpaces();
    setBoardCells();
    isBoardRandomized = !randomize;
    isGameDone = 0;
}

void randomizeBoard()
{
    int* bombOrEmpty = malloc(cellNum * sizeof(int));

    for (int i = 0; i<cellNum; i++) { bombOrEmpty[i] = 0; }

    int remainingBombs = mineDensity;
    int rng;

    while(remainingBombs)
    {
        rng = GetRandomValue(0, cellNum-1);
        if (bombOrEmpty[rng] == 0)
        {
            remainingBombs--;
        }
        bombOrEmpty[rng] = 1;
    }

    for (int l = 0; l < cellNum; l++) {
        board[l].bombStatus = bombOrEmpty[l];
        /* printf("%d bombstatus: %d\n", l, board[l].bombStatus); DEBUG */
    }

    free(bombOrEmpty);
    bombOrEmpty = NULL;
    isBoardRandomized = 1;
}

void revealedBomb(int idx)
{
    for (int i = 0; i < cellNum; i++)
    {
        switch(board[i].texture)
        {
            case CELL_FLAGED:
                if (board[i].bombStatus == false)
                {
                    board[i].texture = CELL_WRONG_FLAG;
                }
            default:
                if (board[i].bombStatus == true && board[i].texture != CELL_FLAGED)
                { board[i].texture = CELL_BOMB; }
        }
    }
    board[idx].texture = CELL_CLICKED_ON_BOMB;
}

void revealedNumber(int idx)
{
    short bombsInRadius = checkBombsInRadius(idx);
    if (bombsInRadius == 0)
    {
        revealedEmpty(idx);
    }
    setTextureForNumberCell(bombsInRadius, idx);
}

void checkCIRE(int idx)
{
    for (int i=0;i<8;i++) { CIRE[i] = 0; }
    if (idx-cellCols >= 0 && idx%cellCols != 0 && idx != 0)                 { CIRE[0] = 1; }
    if (idx-cellCols >= 0)                                                  { CIRE[1] = 1; }
    if (idx-cellCols >= 0 && idx%cellCols != cellCols-1 && idx != 0)        { CIRE[2] = 1; }
    if (idx%cellCols != 0 && idx != 0)                                      { CIRE[3] = 1; }
    if (idx%cellCols != cellCols-1)                                         { CIRE[4] = 1; }
    if (idx+cellCols < cellNum && idx%cellCols != 0 && idx != 0)            { CIRE[5] = 1; }
    if (idx+cellCols < cellNum)                                             { CIRE[6] = 1; }
    if (idx+cellCols < cellNum && idx%cellCols != cellCols-1)               { CIRE[7] = 1; }
}

short checkBombsInRadius(int idx)
{
    checkCIRE(idx);
    short bombsInRadius = 0;
    setIterationBuff(idx);

    for (int i = 0;i<8;i++)
    {
        if (CIRE[i])
        {
            bombsInRadius += board[iterationBuff[i]].bombStatus;
        }
    }

    return bombsInRadius;
}

short checkFlagsInRadius(int idx)
{
    checkCIRE(idx);
    short flagsInRadius = 0;
    setIterationBuff(idx);

    for (int i = 0;i<8;i++)
    {
        if (CIRE[i] && board[iterationBuff[i]].texture == CELL_FLAGED)
        {
            flagsInRadius++;
        }
    }

    return flagsInRadius;
}

void setIterationBuff(int idx)
{
    iterationBuff[0] = idx-cellCols-1;
    iterationBuff[1] = idx-cellCols;
    iterationBuff[2] = idx-cellCols+1;
    iterationBuff[3] = idx-1;
    iterationBuff[4] = idx+1;
    iterationBuff[5] = idx+cellCols-1;
    iterationBuff[6] = idx+cellCols;
    iterationBuff[7] = idx+cellCols+1;
}

void setTextureForNumberCell(int bombAmount, int idx)
{
    switch(bombAmount)
    {
        case 1: board[idx].texture = CELL_ONE; break;
        case 2: board[idx].texture = CELL_TWO; break;
        case 3: board[idx].texture = CELL_THREE; break;
        case 4: board[idx].texture = CELL_FOUR; break;
        case 5: board[idx].texture = CELL_FIVE; break;
        case 6: board[idx].texture = CELL_SIX; break;
        case 7: board[idx].texture = CELL_SEVEN; break;
        case 8: board[idx].texture = CELL_EIGHT; break;
        default: board[idx].texture = CELL_EMPTY;
    }
}

void revealedEmpty(int idx)
{
    board[idx].texture = CELL_EMPTY;
    /* TOP LEFT */ 
    if (idx-cellCols >= 0 && idx%cellCols != 0 && idx != 0 && !checkedSpaces[idx-cellCols-1])
        { revealedEmptyLogic(idx-cellCols-1); }

    /* TOP MIDDLE */ 
    if (idx-cellCols >= 0 && !checkedSpaces[idx-cellCols]) 
        { revealedEmptyLogic(idx-cellCols); }

    /* TOP RIGHT */ 
    if (idx-cellCols >= 0 && idx%cellCols != cellCols-1 && idx != 0 && !checkedSpaces[idx-cellCols+1])
        { revealedEmptyLogic(idx-cellCols+1); }
    
    /* LEFT */
    if (idx%cellCols != 0 && idx != 0 && !checkedSpaces[idx-1])
        { revealedEmptyLogic(idx-1); }

    /* RIGHT */
    if (idx%cellCols != cellCols-1 && !checkedSpaces[idx+1])
        { revealedEmptyLogic(idx+1); }

    /* BOTTOM LEFT */ 
    if (idx+cellCols < cellNum && idx%cellCols != 0 && idx != 0 && !checkedSpaces[idx+cellCols-1])
        { revealedEmptyLogic(idx+cellCols-1); }

    /* BOTTOM MIDDLE */
    if (idx+cellCols < cellNum && !checkedSpaces[idx+cellCols])
        { revealedEmptyLogic(idx+cellCols); }

    /* BOTTOM RIGHT */
    if (idx+cellCols < cellNum && idx%cellCols != cellCols-1 && !checkedSpaces[idx+cellCols+1])
        { revealedEmptyLogic(idx+cellCols+1); }
}

void revealedEmptyLogic(int cell)
{
    checkedSpaces[cell] = 1;
    short bombsInRadius = checkBombsInRadius(cell);
    if (board[cell].texture != CELL_FLAGED)
    {
        if (bombsInRadius == 0)
        {
            revealedEmpty(cell);
        } else {
            setTextureForNumberCell(bombsInRadius, cell);
        }
    }
}

void checkForWin()
{
    int emptysTotal = 0;
    int emptysRevealed = 0;
    for (int i = 0; i < cellNum; i++)
    {
        if (board[i].bombStatus == false)
        {
            emptysTotal++;
            if (
                board[i].texture != CELL_BEING_CLICKED && 
                board[i].texture != CELL_DEFAULT && 
                board[i].texture != CELL_FLAGED)
            {
                emptysRevealed++;
            }
        }
    }
    if (emptysRevealed == emptysTotal)
    {
        for (int j = 0; j < cellNum; j++)
        {
            if (board[j].bombStatus == 1 && board[j].texture == CELL_DEFAULT)
            {
                board[j].texture = CELL_FLAGED;
            }
        }
        face = 36;
    }
}

void chordHold(int idx)
{
    checkCIRE(idx);
    setIterationBuff(idx);
    for (int i = 0;i<8;i++)
    {
        if (CIRE[i] && board[iterationBuff[i]].texture == CELL_DEFAULT)
        {
            textureBuff[i] = board[iterationBuff[i]].texture;
            board[iterationBuff[i]].texture = CELL_HOLDING_CHORD;
        }
    }
}

void chordReset(int idx)
{
    checkCIRE(idx);
    setIterationBuff(idx);
    for (int i = 0;i<8;i++)
    {
        if (CIRE[i] && board[iterationBuff[i]].texture == CELL_HOLDING_CHORD)
        {
            board[iterationBuff[i]].texture = textureBuff[i];
        }
    }
}

void chordRelease(int idx)
{
    short flagsInRadius = checkFlagsInRadius(idx);
    short bombsInRadius = checkBombsInRadius(idx);
    printf("flagsInRadius: %d  -  bombsInRadius: %d\n", flagsInRadius, bombsInRadius);
    checkCIRE(idx);

    setIterationBuff(idx);
    for (int j = 0; j<8; j++)
    {
        printf("%d,", iterationBuff[j]);
    }
    printf("\n");
    for (int k = 0; k<8; k++)
    {
        printf("%d,", CIRE[k]);
    }
    printf("\n");

    if (flagsInRadius == bombsInRadius)
    {
        for (int i = 0;i<8;i++)
        {
            setIterationBuff(idx);
            if (CIRE[i] && board[iterationBuff[i]].texture == CELL_DEFAULT)
            {
                printf("iterationBuff[%d]: %d\n", i, iterationBuff[i]);
                revealedEmptyLogic(iterationBuff[i]);
            }
        }
    }
}