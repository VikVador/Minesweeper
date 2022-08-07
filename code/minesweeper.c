//----------------------------------
// 
//           Minesweeper 
//
//----------------------------------
// @ Ali Sezgin     Victor Mangeleer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Solver.h"
#include "Engine.h"


int main(int argc, char **argv) 
{
    // Terminal GUI
    printf("\n           _                                                 \n");                                    
    printf("          (_)                                                  \n");                                 
    printf(" _ __ ___  _ _ __   ___  _____      _____  ___ _ __   ___ _ __ \n");
    printf("| '_ ` _  | | '_   / _  / __    /  / / _  / _   '_   / _   '__|\n");
    printf("| | | | | | | | | |  __/ __    V  V /  __/  __/ |_) |  __/ |   \n");
    printf("|_| |_| |_|_|_| |_| ___||___/  _/ _/  ___| ___| .__/  ___|_|   \n");
    printf("                                              | |              \n");
    printf("                                              |_|              \n\n");
    printf("@ Ali Sezgin & Victor Mangeleer\n\n");

    if (argc != 5) 
    {
        printf("-------\nUSAGE :\n-------\n");
        printf("<executable> <width> <height> <number of bombs> <player>\n\n");
        printf("where,\n\n");
        printf("- <width>            : int in [0, 1000]\n");
        printf("- <height>           : int in [0, 1000]\n");
        printf("- <number of bombs>  : int in [0, (w * h) - 1]\n");
        printf("- <player>           : 0 = Heuristic\n");
        printf("                       1 = Human    \n");
        return -1;
    }
    

    // Initialization
    int width = (int) strtol(argv[1], NULL, 0);
    int height = (int) strtol(argv[2], NULL, 0);
    int nbrBombs = (int) strtol(argv[3], NULL, 0);
    int player = (int) strtol(argv[4], NULL, 0);

    // Playing game
    int success;
    if (player)
    {
        // Terminal GUI
        printf("-------------\n How to play ?\n-------------\n");
        printf("<x> <y> <flag> \n\n");
        printf("where,\n\n");
        printf("- <x>        : horizontal coordinate\n");
        printf("- <y>        : vertical coordinate\n");
        printf("- <flag>     : -1 = ending the game\n");
        printf("                0 = reveal the tile\n");
        printf("                1 = place a flag\n\n");

        success = play(width, height, nbrBombs, human, 1);
    }
    else
        success = play(width, height, nbrBombs, heuristic, 1);
    
    // Result
    if (success == -1) {
        printf("\n\nDamn! You walked on a bomb ! Sorry... !\n\n");
    } else if (success == 1) {
        printf("\n\nYou did it! Well done!\n\n");
    } else {
        printf("\n\nHeuristic couldn't find a play, try again with different game parameters\n\n");
    }
    return 0;
}
