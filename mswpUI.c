/* UI FILE */

#include <raylib.h>
#include <time.h>
/* includes I might need for quick debugging:*/
#include <stdio.h>

#include "mswpUI.h"
#include "minesweeper.h"

Texture2D style;

static Rectangle spriteDetails[38];

static int prevCell = 0;

short cellsAffected[] = {0, 0, 0, 0, 0, 0, 0, 0};

char isChording = 0;

char face = FACENORMAL; /* 33 34 35 36 37 */

void renderGame()
{
    programStartBasics();
    gameLoop();
}

void programStartBasics()
{
    InitWindow(windowWidth, windowHeight, "Minesweeper");
    createMinefield();
    /* SetTargetFPS(60); Not necessary */
    setSpriteDetails();
    createTexture();
}

void createTexture()
{
    Image styleIMG = LoadImage("resources/purpur.bmp");
    ImageFormat(&styleIMG, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    style = LoadTextureFromImage(styleIMG);
}

void setSpriteDetails()
{
    int idx = 0;
    for (int i = 0; i<2; i++)
    {
        for (int j = 0; j<9; j++)
        {
            spriteDetails[idx] = (Rectangle)
            {
                cellInitialSize*j,
                cellInitialSize*i,
                cellInitialSize,
                cellInitialSize
            };
            idx++;
            if (i == 1 && j == 7) { break; } 
        }
    }

    spriteDetails[CELL_HOLDING_CHORD] = spriteDetails[CELL_BEING_CLICKED]; // duplicate for chording
    spriteDetails[TOP_LEFT_CORNER] = (Rectangle){0, 82, 12, 11};
    spriteDetails[TOP_CONNECTOR] = (Rectangle){13, 82, 1, 11};
    spriteDetails[TOP_RIGHT_CORNER] = (Rectangle){15, 82, 12, 11};

    spriteDetails[TOP_MID_CONNECTOR_L] = (Rectangle){0, 94, 12, 1};
    spriteDetails[TOP_MID_CONNECTOR_R] = (Rectangle){15, 94, 12, 1};
    
    spriteDetails[MID_LEFT_EDGE] = (Rectangle){0, 96, 12, 11};
    spriteDetails[MID_CONNECTOR] = (Rectangle){13, 96, 1, 11};
    spriteDetails[MID_RIGHT_EDGE] = (Rectangle){15, 96, 12, 11};
    
    spriteDetails[MID_BOTTOM_CONNECTOR_L] = (Rectangle){0, 108, 12, 1};
    spriteDetails[MID_BOTTOM_CONNECTOR_R] = (Rectangle){15, 108, 12, 1};
    
    spriteDetails[BOTTOM_LEFT_CORNER] = (Rectangle){0, 110, 12, 12};
    spriteDetails[BOTTOM_CONNECTOR] = (Rectangle){13, 110, 1, 12};
    spriteDetails[BOTTOM_RIGHT_CORNER] = (Rectangle){15, 110, 12, 12};
    
    spriteDetails[BACKGROUND] = (Rectangle){70, 82, 1, 1};
    spriteDetails[NRBG] = (Rectangle){28, 82, 41, 25};
    
    spriteDetails[FACENORMAL] = (Rectangle){0, 55, 26, 26};
    spriteDetails[FACEHOLDINGCELL] = (Rectangle){27, 55, 26, 26};
    spriteDetails[FACEDEATH] = (Rectangle){54, 55, 26, 26};
    spriteDetails[FACEWIN] = (Rectangle){81, 55, 26, 26};
    spriteDetails[FACEHOLDINGFACE] = (Rectangle){108, 55, 26, 26};
}

void gameLoop() 
{
    while(!WindowShouldClose())
    {
        /* UPDATING */
        updatingPhase();

        BeginDrawing();

        /* DRAWING */
        drawingPhase();

        EndDrawing();
    }
    endingThings();
    CloseWindow();
}

void updatingPhase()
{
    Vector2 mousePos = GetMousePosition();
    int cellUnderMouse = 
    ((int)mousePos.x-(12*graphicalScale))/(cellSize)
    +
    cellCols*(((int)mousePos.y-(55*graphicalScale))/(cellSize));
    
    if (!isGameDone) {
        checkForWin();
        mouseCellCollision(mousePos, cellUnderMouse);
    }

    generalInput(mousePos, cellUnderMouse);
}

void mouseCellCollision(Vector2 mousePos, int cellUnderMouse)
{
    face = FACENORMAL;
    /* prevCell stuff idk what to call this */
    if (board[prevCell].texture == CELL_BEING_CLICKED && prevCell != cellUnderMouse)
    {
        board[prevCell].texture = CELL_DEFAULT;
    }

    if (isChording)
    {
        chordReset(prevCell);
        isChording--;
    }

    /* Checking if mouse is on the minefield */
    if (
        mousePos.x >= (12*graphicalScale) && mousePos.x < cellCols*cellSize + (12*graphicalScale)
        &&
        mousePos.y >= (55*graphicalScale) && mousePos.y < cellRows*cellSize + (55*graphicalScale))
    {

        /* Main left click check */
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {

            printf("Cell Under Mouse: %d  -  Mouse x, y: %.0f, %.0f  -  CeUnMo bombstatus: %d\n",
                cellUnderMouse, mousePos.x, mousePos.y, board[cellUnderMouse].bombStatus); /* DEBUG */ 

            /* HOLDING LEFT CLICK ON AN UNOPENED CELL*/
            if (board[cellUnderMouse].texture == CELL_DEFAULT)
            {
                board[cellUnderMouse].texture = CELL_BEING_CLICKED;
                face = FACEHOLDINGCELL;

            /* HOLDING LEFT CLICK ON AN OPENED NUMBER CELL*/
            } else if (board[cellUnderMouse].texture > 0 && board[cellUnderMouse].texture < 9)
            {
                chordHold(cellUnderMouse);
                isChording = 1;
            }

            /* FOR FACE CHANGING */
            if (board[cellUnderMouse].texture == CELL_BEING_CLICKED)
            {
                face = FACEHOLDINGCELL;
            }
            
        } 
        else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            printf("Cell Under Mouse: %d  -  Mouse x, y: %.0f, %.0f  -  CeUnMo bombstatus: %d\n",
                cellUnderMouse, mousePos.x, mousePos.y, board[cellUnderMouse].bombStatus); /* DEBUG */

            /* Generates a board if not generated yet */
            if (!isBoardRandomized) { rngBoardWithSafeguard(cellUnderMouse); }

            /* LEFT GO OF LEFT CLICK ON AN UNOPENED CELL */
            if (board[cellUnderMouse].texture == CELL_BEING_CLICKED)
            {
                /* CLICKED ON A BOMB LOGIC */
                if (board[cellUnderMouse].bombStatus == true)
                {
                    revealedBomb(cellUnderMouse);
                    isGameDone = true;
                    face = FACEDEATH;
                }
                /* CLICKED ON NUMBER CELL LOGIC */
                else if (checkBombsInRadius(cellUnderMouse) > 0)
                {
                    revealedNumber(cellUnderMouse);
                }
                /* CLICKED ON EMPTY CELL LOGIC */
                else
                {
                    revealedEmpty(cellUnderMouse);
                }
            /* LET GO OF CLICK ON AN OPENED NUMBER CELL*/
            } else if (board[cellUnderMouse].texture > 0 && board[cellUnderMouse].texture < 9)
            {
                chordRelease(cellUnderMouse);
            }
            
        }
        /* MAIN RIGHT CLICK CHECK */
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            /* Generates a board is not generated yet */
            if (!isBoardRandomized) { rngBoardWithSafeguard(cellUnderMouse); }

            /* FLAGGING FUNCTIONALITY */
            switch(board[cellUnderMouse].texture)
            {
                case CELL_DEFAULT:
                    board[cellUnderMouse].texture = CELL_FLAGED;
                    break;
                case CELL_FLAGED:
                    board[cellUnderMouse].texture = CELL_DEFAULT;
            }
        }
        prevCell = cellUnderMouse;
    }
}

void generalInput(Vector2 mousePos, int cellUnderMouse)
{
    /* RESTART GAME CHECK   -   NEED TO REMOVE THESE THEY ARE OBSOLETE */
    if (IsKeyPressed(KEY_R))
    {
        restartGame(true);
    }
    if (IsKeyPressed(KEY_T))
    {
        restartGame(false);
    }

    /* CHECKING IF MOUSE IS ON THE FACE */
    if (
        mousePos.x >= (239*graphicalScale) && mousePos.x < (265*graphicalScale)
        &&
        mousePos.y >= (15*graphicalScale) && mousePos.y < (41*graphicalScale)
    )
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            restartGame(true);
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && face == FACENORMAL)
        {
            face = FACEHOLDINGFACE;
        }
    }
}

void drawingPhase()
{
    ClearBackground(BLACK);
    displayTextures();
}

void displayTextures()
{
    for (int i = 0; i < cellNum; i++)
    {
        displayCell(board[i]);
    }
    displayBar(
        TOP_LEFT_CORNER,
        (Rectangle){0, 0, 12*graphicalScale, 11*graphicalScale}
    );
    displayBar(
        TOP_CONNECTOR,
        (Rectangle){12*graphicalScale, 0, cellSize*cellCols, 11*graphicalScale}
    );
    displayBar(
        TOP_RIGHT_CORNER,
        (Rectangle){windowWidth-12*graphicalScale, 0, 12*graphicalScale, 11*graphicalScale}
    );
    displayBar(
        TOP_MID_CONNECTOR_L,
        (Rectangle){0, 11*graphicalScale, 12*graphicalScale, 33*graphicalScale}
    );
    displayBar(
        TOP_MID_CONNECTOR_R,
        (Rectangle){windowWidth-12*graphicalScale, 11*graphicalScale, 12*graphicalScale, 34*graphicalScale}
    );
    displayBar(
        MID_LEFT_EDGE,
        (Rectangle){0, 44*graphicalScale, 12*graphicalScale, 11*graphicalScale}
    );
    displayBar(
        MID_CONNECTOR,
        (Rectangle){12*graphicalScale, 44*graphicalScale, cellSize*cellCols, 11*graphicalScale}
    );
    displayBar(
        MID_RIGHT_EDGE,
        (Rectangle){windowWidth-12*graphicalScale, 44*graphicalScale, 12*graphicalScale, 11*graphicalScale}
    );
    displayBar(
        MID_BOTTOM_CONNECTOR_L,
        (Rectangle){0, 55*graphicalScale, 12*graphicalScale, cellSize*cellRows}
    );
    displayBar(
        MID_BOTTOM_CONNECTOR_R,
        (Rectangle){windowWidth-12*graphicalScale, 55*graphicalScale, 12*graphicalScale, cellSize*cellRows}
    );
    displayBar(
        BOTTOM_LEFT_CORNER,
        (Rectangle){0, windowHeight-12*graphicalScale, 12*graphicalScale, 12*graphicalScale}
    );
    displayBar(
        BOTTOM_CONNECTOR,
        (Rectangle){12*graphicalScale, windowHeight-12*graphicalScale, cellSize*cellCols, 12*graphicalScale}
    );
    displayBar(
        BOTTOM_RIGHT_CORNER,
        (Rectangle){windowWidth-12*graphicalScale, windowHeight-12*graphicalScale, 12*graphicalScale, 12*graphicalScale}
    );
    displayBar(
        BACKGROUND,
        (Rectangle){12*graphicalScale, 11*graphicalScale, 480*graphicalScale, 33*graphicalScale}
    );
    displayBar(
        NRBG,
        (Rectangle){16*graphicalScale, 15*graphicalScale, 41*graphicalScale, 25*graphicalScale}
    );
    displayBar(
        NRBG,
        (Rectangle){windowWidth-61*graphicalScale, 15*graphicalScale, 41*graphicalScale, 25*graphicalScale}
    );
    displayBar(
        face,
        (Rectangle){239*graphicalScale, 15*graphicalScale, 26*graphicalScale, 26*graphicalScale}
    );
}

void displayCell(Cell c)
{
    // DrawTexture(textures[c.texture], c.xPixels, c.yPixels, WHITE);
    // DrawTextureRec(
    //     textures[c.texture],
    //     (Rectangle){c.xPixels, c.yPixels, cellSize, cellSize},
    //     (Vector2){c.xPixels, c.yPixels},
    //     WHITE
    // );
    DrawTexturePro(
        style,
        spriteDetails[c.texture], /* do NOT change c.texture to c.id */
        (Rectangle){c.xPixels, c.yPixels, cellSize, cellSize},
        (Vector2){-(12*graphicalScale),-(55*graphicalScale)},
        0.0f,
        WHITE
    );
}

void displayBar(int spr, Rectangle r1)
{
    DrawTexturePro(
        style,
        spriteDetails[spr],
        r1,
        (Vector2){0, 0},
        0.0f,
        WHITE
    );
}

void endingThings()
{
    freeCheckedSpaces();
    freeBoard();
}

void reloadStyle()
{
    /* Nothing yet */
}

void mwspUISetRandomSeed()  
{
    SetRandomSeed(time(NULL));
}