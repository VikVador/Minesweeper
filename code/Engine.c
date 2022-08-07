//----------------------------------
// 
//           Minesweeper 
//
//----------------------------------
// @ Ali Sezgin     Victor Mangeleer

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "Engine.h"


//----------------------
// Fonction principale :
//----------------------

int play(int width, int height, int nbrBombs, Move (*computeMove)(Grid *), int show)
{
    // 1 : Création, initialisation et présentation de la grille
    Grid *grille = gridInit(width, height, nbrBombs);
    if(show)
        {
            gridPrint(grille);
        }

    // 2 : Récupération de la première action
    Move mouvement = computeMove(grille);
    
        // Le joueur met fin à la partie
        if(mouvement.flag == -1)
        {
            printf("---------------------------------\n");
            printf("|          FIN DE PARTIE        |\n");
            printf("---------------------------------\n");
            return 0;
        }
    
        while(mouvement.flag == 1)
        {
            printf("ATTENTION - La première action est de révéler une case !\n\n");
            if(show)
            {
                gridPrint(grille);
            }

            mouvement = computeMove(grille);
            
            if(mouvement.flag == -1)
            {
                printf("---------------------------------\n");
                printf("|          FIN DE PARTIE        |\n");
                printf("---------------------------------\n");
                return 0;
            }
        }
    
    // 3 : 1er appel de gridReveal (Placement des bombes)
    gridReveal(grille, mouvement.x, mouvement.y);

    // 4 : Initialisation des variables et la partie commence 
    
    int x, y;
    
    while(!gridWon(grille))
    {
        // Affichage de la grille après chaque actions - Si SHOW = 1
        if(show)
        {
            gridPrint(grille);
        }
        
        //Récupération de la prochaine action
        mouvement = computeMove(grille);
        
        // CAS 1 : Le joueur met fin à la partie
        if(mouvement.flag == -1)
        {
            printf("---------------------------------\n");
            printf("|          FIN DE PARTIE        |\n");
            printf("---------------------------------\n");
            return 0;
        }
        
        // CAS 2 : Le joueur veut placer un drapeau
        x = mouvement.x;
        y = mouvement.y;

        if(mouvement.flag == 1)
        {
            // Le joueur a déja placé un drapeau sur la case ?
            if((gridIsFlagged(grille, x, y)))
            {
                printf("ATTENTION - Cette case possède déja un flag\n\n");
                continue;
            }

            // Le joueur a déja révélé la case ?
            else if(gridIsExplored(grille, x, y))
            {
                printf("ATTENTION - Cette case a déja été révélée\n\n");
                continue;
            }
            else 
            {
               gridSetFlag(grille, x, y);
                continue; 
            }
        }
        
        // CAS 3 : Le joueur veut révéler une case

         // Le joueur a déja révélé la case ?
        if(gridIsExplored(grille, x, y))
        {
            printf("ATTENTION - Cette case a déja été révélée\n\n");
            continue;
        }
        
        // La case est révélée
        gridReveal(grille, x, y);
    }

    // Fin de partie, dernier affichage de la grille et return
    gridPrint(grille);

    int retour = gridWon(grille);

    gridFree(grille);

    return retour;
}


