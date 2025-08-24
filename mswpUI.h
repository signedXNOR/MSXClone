/* mswpUI.c HEADER FOR FUNC DECLARATIONS */
#ifndef MSWPUI_H
#define MSWPUI_H

#include <raylib.h>
#include <stddef.h>

/* GLOBAL VARIABLES */
extern short cellsAffected[];

extern char face;

/* CUSTOM DATA TYPES */
typedef struct
{
    int xPixels;
    int yPixels;
    int texture;
    int bombStatus;
    int id;
} Cell;


/* FUNCTION DECLARATIONS */
void programStartBasics();
void renderGame();
void gameLoop();
void createTexture();
void displayTextures();
void drawingPhase();
void updatingPhase();
void reloadStyle();
void mwspUISetRandomSeed();
void displayCell(Cell c);
void endingThings();
void mouseCellCollision(Vector2 mousePos, int cellUnderMouse);
void setSpriteDetails();
void generalInput(Vector2 mousePos, int cellUnderMouse);
void displayBar(int spr, Rectangle r1);


/* ENUMS */
enum textureNames
{
    CELL_EMPTY,
    CELL_ONE,
    CELL_TWO,
    CELL_THREE,
    CELL_FOUR,
    CELL_FIVE,
    CELL_SIX,
    CELL_SEVEN,
    CELL_EIGHT,
    CELL_DEFAULT,
    CELL_BEING_CLICKED,
    CELL_BOMB,
    CELL_FLAGED,
    CELL_WRONG_FLAG,
    CELL_CLICKED_ON_BOMB,
    CELL_QMARK,
    CELL_QMARK_BEING_CLICKED,
    CELL_HOLDING_CHORD,
    TOP_LEFT_CORNER,
    TOP_CONNECTOR,
    TOP_RIGHT_CORNER,
    TOP_MID_CONNECTOR_L,
    TOP_MID_CONNECTOR_R,
    MID_LEFT_EDGE,
    MID_CONNECTOR,
    MID_RIGHT_EDGE,
    MID_BOTTOM_CONNECTOR_L,
    MID_BOTTOM_CONNECTOR_R,
    BOTTOM_LEFT_CORNER,
    BOTTOM_CONNECTOR,
    BOTTOM_RIGHT_CORNER,
    BACKGROUND,
    NRBG,
    FACENORMAL,
    FACEHOLDINGCELL,
    FACEDEATH,
    FACEWIN,
    FACEHOLDINGFACE
};

#endif