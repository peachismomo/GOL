#include <stdio.h>
#include "cprocessing.h"

#define GOL_GRID_COLS 30
#define GOL_GRID_ROWS 30
#define GOL_GRID_BUFFERS 2

#define GOL_ALIVE 1
#define GOL_DEAD 0

#define TRUE 1
#define FALSE 0

int gIsPaused;
int gGrids[GOL_GRID_BUFFERS][GOL_GRID_ROWS][GOL_GRID_COLS];
int gridNo;

float cellWidth;
float cellHeight;

CP_Color black;
CP_Color white;

/* Feel free to declare your own variables here */

void game_init(void)
{
    /* Set every grids' cells as 'dead' */
    for (int row = 0; row < GOL_GRID_ROWS; ++row) {
        for (int col = 0; col < GOL_GRID_COLS; ++col) {
            for (int i = 0; i < GOL_GRID_BUFFERS; ++i) {
                gGrids[i][row][col] = GOL_DEAD;
            }
        }
    }

    /*********************************************************
    *  Let's start with gGrids[0] as the 'reference grid'
    *  and gGrids[1] as the 'displaying grid'.
    *
    *  We initialize gGrids[0] with a simple 'glider'.
    *********************************************************/

    gGrids[0][1][3] = GOL_ALIVE;
    gGrids[0][2][1] = GOL_ALIVE;
    gGrids[0][2][3] = GOL_ALIVE;
    gGrids[0][3][2] = GOL_ALIVE;
    gGrids[0][3][3] = GOL_ALIVE;

    /* We start unpaused */
    gIsPaused = FALSE;

    /* Initialization of your other variables here */
    //CP_System_SetFrameRate(2);
    CP_Settings_StrokeWeight(0.8f);
    CP_Settings_RectMode(CP_POSITION_CORNER);

    black = CP_Color_Create(0, 0, 0, 255);
    white = CP_Color_Create(255, 255, 255, 255);

    //sGrids[gridNo] => display grid
    //sGrids[!gridNo] => refernce grid
    gridNo = 1;

    //Get height and width of each cell
    cellWidth = CP_System_GetWindowWidth() / (float)GOL_GRID_ROWS;
    cellHeight = CP_System_GetWindowHeight() / (float)GOL_GRID_COLS;
}

void game_update(void)
{    
    if (CP_Input_KeyTriggered(KEY_ANY)) {
        gIsPaused = !gIsPaused;
    }

    if (!gIsPaused) {
        gridNo = CP_System_GetFrameCount() % 2;

        for (int row = 0; row <= GOL_GRID_ROWS; row++) {
            for (int col = 0; col <= GOL_GRID_COLS; col++) {

                /*NUMBER OF NEIGHBOURS ALIVE*/
                int neighbours[8];
                if (row > 0) {
                    neighbours[0] = gGrids[!gridNo][row - 1][col + 1];
                    neighbours[3] = gGrids[!gridNo][row - 1][col];
                }
                else {
                    neighbours[0] = GOL_DEAD;
                    neighbours[3] = GOL_DEAD;
                }

                neighbours[1] = gGrids[!gridNo][row][col + 1];
                neighbours[2] = gGrids[!gridNo][row + 1][col + 1];
                neighbours[4] = gGrids[!gridNo][row + 1][col];

                if (row > 0 && col > 0) {
                    neighbours[5] = gGrids[!gridNo][row - 1][col - 1];
                }
                else {
                    neighbours[5] = 0;

                }

                if (col > 0) {
                    neighbours[6] = gGrids[!gridNo][row][col - 1];
                    neighbours[7] = gGrids[!gridNo][row + 1][col - 1];
                }
                else {
                    neighbours[6] = GOL_DEAD;
                    neighbours[7] = GOL_DEAD;
                }

                int count = 0;
                for (int i = 0; i < 8; i++) {
                    if (neighbours[i] == GOL_ALIVE) {
                        count++;
                    }
                }
                /*-----------------------------------------------------------*/
                printf("Row: %d | Col: %d | Neighbours: %d |\n", row, col, count);
                /*SIMULATION LOGIC*/
                if (gGrids[!gridNo][row][col] == GOL_ALIVE) {
                    if (count < 2 || count > 3) {
                        gGrids[gridNo][row][col] = GOL_DEAD;
                    }
                    else {
                        gGrids[gridNo][row][col] = GOL_ALIVE;
                    }
                }

                if (gGrids[!gridNo][row][col] == GOL_DEAD)
                {
                    if (count == 3) {
                        gGrids[gridNo][row][col] = GOL_ALIVE;
                    }
                    else {
                        gGrids[gridNo][row][col] = GOL_DEAD;
                    }
                }
                /*-----------------------------------------------------------*/
            }
        }
    }

    else {
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
            CP_Vector mousePos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());

            for (int row = 0; row <= GOL_GRID_ROWS; row++) {
                for (int col = 0; col <= GOL_GRID_COLS; col++) {

                    float cellX = cellWidth * col;
                    float cellY = cellHeight * row;

                    if (!(mousePos.x < cellX || mousePos.x > cellX + cellWidth || mousePos.y < cellY || mousePos.y > cellY + cellHeight)) {
                        gGrids[gridNo][row][col] = !gGrids[gridNo][row][col];
                    }
                }
            }
        }
    }

    /*DRAW DISPLAY TABLE*/
    for (int row = 0; row <= GOL_GRID_ROWS; row++) {
        for (int col = 0; col <= GOL_GRID_COLS; col++) {
            float cellX = cellWidth * col;
            float cellY = cellHeight * row;

            if (gGrids[gridNo][row][col] == GOL_ALIVE) {
                CP_Settings_Fill(black);
                CP_Graphics_DrawRect(cellX, cellY, cellWidth, cellHeight);
                //update reference table
                gGrids[!gridNo][row][col] = GOL_ALIVE;
            }
            else
            {
                CP_Settings_Fill(white);
                CP_Graphics_DrawRect(cellX, cellY, cellWidth, cellHeight);
                //update reference table
                gGrids[!gridNo][row][col] = GOL_DEAD;

            }
        }
    }
    /*-----------------------------------------------------------*/
}

void game_exit(void)
{

}