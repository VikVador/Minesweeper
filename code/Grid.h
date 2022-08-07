//----------------------------------
// 
//           Minesweeper 
//
//----------------------------------
// @ Ali Sezgin     Victor Mangeleer

#ifndef Grid_h
#define Grid_h

#include <stdio.h>

typedef struct Grid_t Grid;

int gridWidth(Grid *g);

int gridHeight(Grid *g);

int gridWon(Grid *grid);

void gridFree(Grid *grid);

void gridPrint(Grid *grid);

int gridValue(Grid *grid, int x, int y);

int gridReveal(Grid *grid, int x, int y);

void gridSetFlag(Grid *grid, int x, int y);

int gridIsFlagged(Grid *grid, int x, int y);

int gridIsExplored(Grid *grid, int x, int y);

Grid *gridInit(int width, int height, int nbrBombs);

#endif /* Grid_h */
