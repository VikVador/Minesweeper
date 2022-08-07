//----------------------------------
// 
//           Minesweeper 
//
//----------------------------------
// @ Ali Sezgin     Victor Mangeleer

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "Grid.h"


//------------------
// Notre structure :
//------------------

struct Grid_t
{
    int **element; 
    int nb_lignes;
    int nb_colonnes;
    int nb_bombes;
} ;

//--------------
// Prototypes  :
//--------------

static void titre_grille(int nb_colonnes);

static void tiret_grille(int nb_colonnes);

static void placebombs(Grid *grille, int x, int y);

static void gridValue_recursiv(Grid *grid, int x, int y);

static int gridValueInexplored(Grid *grid, int x, int y);

static int verif_indice(Grid *grid, int x, int y, int a, int b);

//------------------------
// Fonctions principales : 
//------------------------

Grid *gridInit(int width, int height, int nbrBombs)
{
    // Hypothèse :
    if(nbrBombs <= 0 || width <= 0 || height <= 0)
    {
        printf("ERREUR (gridInit) - Les 3 arguments doivent être supérieurs à 0\n\n");
        exit(-2);
    }

    // Initialisation de la grille :
    Grid *Grille = malloc(sizeof(Grid));
    if(!Grille)
    {
        printf("ERREUR (gridInit) - L'allocation d'espace ne s'est pas faite correctement\n\n");
        exit(-1);
    }
    
    Grille->element = (int**) malloc(height * sizeof(int *));
    if (!Grille->element)
    {
        printf("ERREUR (gridInit) - L'allocation d'espace ne s'est pas faite correctement\n\n");
        exit(-1);
    }

    for (int i = 0 ; i < height ; i++)
    {
        Grille->element[i] = (int *) malloc(width * sizeof(int));
        if (!Grille->element[i])
        {
            printf("ERREUR (gridInit) - L'allocation d'espace ne s'est pas faite correctement\n\n");
            free(Grille->element);
            exit(-1);
        }
    }

    // Initialisation des variables :
    Grille->nb_lignes = height;
    Grille->nb_colonnes = width;
    Grille->nb_bombes = nbrBombs;

    // Initialisation des cases :
    for (int j = 0 ; j < height ; j++)
    {
        for (int i = 0 ; i < width ; i++)
        {
            Grille->element[j][i] = -1;
        }
    }

    return Grille;
}

//-------------------------------------

void gridFree(Grid *grid)
{
    if(grid == NULL)
    {
        printf("ERREUR (gridFree) - Un pointeur NULL a été passé en argument\n\n");
        exit(-2);
    }
    
    for (int i = 0 ; i < grid->nb_lignes ; i++)
    {
        free(grid->element[i]);
    }

    free(grid->element);

    free(grid);
}

//-------------------------------------

void gridPrint(Grid *grid)
{
    if(grid == NULL)
    {
        printf("ERREUR (gridPrint) - Un pointeur NULL a été passé en argument\n\n");
        exit(-2);
    }

    int nb_lignes = grid->nb_lignes;
    int nb_colonnes = grid->nb_colonnes;

    // Création du titre :
    tiret_grille(nb_colonnes);
    titre_grille(nb_colonnes);
    tiret_grille(nb_colonnes);

    // Affichage de chacune des cases :
    int vgridwon = gridWon(grid); // Indique si c'est la fin de partie
    
    for (int j = 0 ; j < nb_lignes ; j++)
    {
        for(int i = 0 ; i < nb_colonnes ; i++)
        {
            printf("|");
            
            switch (grid->element[j][i])
            {
                case -1 : // Case vide
                    printf(" ");
                    break;

                case -2 : // Bombe
                    if(vgridwon)
                    {
                        printf("X");
                        break;
                    }
                    printf(" ");
                    break;
                    
                case -3 : // Flag sur une case vide
                    if(vgridwon)
                    {
                        printf(" ");
                        break;
                    }
                    
                    printf("F");
                    break;
                    
                case -4 : // Flag sur une bombe
                    if(vgridwon)
                    {
                        printf("X");
                        break;
                    }
                    
                    printf("F");
                    break;

                case -6 : // Case qui peut être révélée
                    printf(" ");
                    break;
                
                case -7 : // Bombe explosée
                    printf("X");
                    break;

                default:
                    printf("%d",grid->element[j][i]);
            }
        }
        printf("|\n");
        tiret_grille(nb_colonnes);
    }
}

//-------------------------------------

int gridReveal(Grid *grid, int x, int y)
{
    // Hypothèses :
    if(grid == NULL)
    {
        printf("ERREUR (gridReveal) - Un pointeur NULL a été passé en argument\n\n");
        exit(-2);
    }
    else if(x < 0 || x >= grid->nb_colonnes)
    {
        printf("ERREUR (gridReveal) - L'abcisse donnée sort du plateau de jeu\n\n");
        exit(-2);
    }
    else if(y < 0 || y >= grid->nb_lignes)
    {
            printf("ERREUR (gridReveal) - L'ordonnée donnée sort du plateau de jeu\n\n");
            exit(-2);
    }

    // Initialisation des variables :
    int debut_partie = 1;
    int nb_lignes = grid->nb_lignes;
    int nb_colonnes = grid->nb_colonnes;

    // Vérification du début de partie :
    for(int i = 0 ; i < nb_lignes ; i++)
    {
        for(int j = 0 ; j < nb_colonnes ; j++)
        {
            if(grid->element[i][j] != -1)
            {
                debut_partie = 0;
                break;
            }
        }

        if(!debut_partie)
            break;
    }

    // C'est le début de partie :
    if(debut_partie)
    {
        int case_inter = 0;

        for (int b = -2 ; b < 3 ; b++)
        {
            for (int a = -2 ; a < 3 ; a++)
            {
                if(verif_indice(grid, x + a, y + b, a, b))
                {
                    continue;
                }

                case_inter++;
            }
        }

        // Hypothèse de placement des bombes :
        int place_dispo = (nb_lignes*nb_colonnes) - case_inter;

        if(place_dispo < grid->nb_bombes)
        {
            printf("ERREUR (gridReveal) - Pas assez de place pour mettre les bombes\n\n");
            exit(-2);
        }

        // Placement des bombes :
        placebombs(grid, x, y);
        return 0;
    }

    // Le joueur a révélé une bombe ?
    if (grid->element[y][x] == -2 || grid->element[y][x] == -4)
    {
        // -7 = Bombe qui a explosé
        grid->element[y][x] = -7;
        return 1;
    }
    
    // La case est déja explorée ?
    if(gridIsExplored(grid, x, y))
    {
        return 0;
    }

    // Le joueur peut révélér la case :
    grid->element[y][x] = -6;

    int valeur = gridValueInexplored(grid, x, y);
    // Explication de gridValueInexplored dans la section
    // fonctions secondaires

    grid->element[y][x] = valeur;

    // Si la case est un 0, on révèle les autres
    if(!valeur)
    {
        gridValue_recursiv(grid, x, y);
    }

    return 0;
}

//-------------------------------------

int gridIsExplored(Grid *grid, int x, int y)
{
    if(grid == NULL)
    {
        printf("ERREUR (gridIsExplored) - Un pointeur NULL a été passé en argument\n\n");
        exit(-2);
    }
    else if(x < 0 || x >= grid->nb_colonnes)
    {
        printf("ERREUR (gridIsExplored) - L'abcisse donnée sort du plateau de jeu\n\n");
        exit(-2);
    }
    else if(y < 0 || y >= grid->nb_lignes)
    {
        printf("ERREUR (gridIsExplored) - L'ordonnée donnée sort du plateau de jeu\n\n");
        exit(-2);
    }  
    else if(grid->element[y][x] >= 0 && grid->element[y][x] <= 8)
    {
        return 1;
    }
    else 

        return 0;
}

//-------------------------------------
// Cette version de gridValue n'est utilisée que dans le solveur. Celle-ci est moins 
// restrictive que gridValueInexplored qui prend en compte la notion de "case-révélée ou non".

int gridValue(Grid *grid, int x, int y)
{
    // Hypothèses :
    if(grid == NULL)
    {
        printf("ERREUR (gridValue) - Un pointeur NULL a été passé en argument\n\n");
        exit(-2);
    }
    else if(x < 0 || x >= grid->nb_colonnes)
    {
        printf("ERREUR (gridValue) - L'abcisse donnée sort du plateau de jeu\n\n");
        exit(-2);
    }
    else if(y < 0 || y >= grid->nb_lignes)
    {
        printf("ERREUR (gridValue) - L'ordonnée donnée sort du plateau de jeu\n\n");
        exit(-2);
    }

    // Comptage des bombes :
    int compteur_bombe = 0;
    
    for (int b = -1 ; b < 2 ; b++)
    {
        for (int a = -1 ; a < 2 ; a++)
        {
            if(verif_indice(grid, x + a, y + b, a, b))
            {
                continue;
            }

            else if(grid->element[y+b][x+a] == -2 || grid->element[y+b][x+a] == -4)
            {
                compteur_bombe++;
            }
        }
    }

    return compteur_bombe;
}

//-------------------------------------

int gridWidth(Grid *g)
{
    if(g == NULL)
    {
        printf("ERREUR (gridWidth) - Un pointeur NULL a été passé en argument\n\n");
        exit(-2);
    }
    
    int a = g->nb_colonnes ;
    
    return a;
}

//-------------------------------------

int gridHeight(Grid *g)
{
    if(g == NULL)
    {
        printf("ERREUR (gridHeight) - Un pointeur NULL a été passé en argument\n\n");
        exit(-2);
    }
    
    int b = g->nb_lignes ;

    return b;
}

//-------------------------------------

int gridIsFlagged(Grid *grid, int x, int y)
{
    if(grid == NULL)
    {
       printf("ERREUR (gridIsFlagged) - Un pointeur NULL a été passé en argument\n\n");
       exit(-2);
    }
    else if(x < 0 || x >= grid->nb_colonnes)
    {
       printf("ERREUR (gridIsFlagged) - L'abcisse donnée sort du plateau de jeu\n\n");
       exit(-2);
    }
    else if(y < 0 || y >= grid->nb_lignes)
    {
        printf("ERREUR (gridIsFlagged) - L'ordonnée donnée sort du plateau de jeu\n\n");
        exit(-2);
    }  
    else if(grid->element[y][x] == -3 || grid->element[y][x] == -4)
    {
        return 1;
    }
    else 

        return 0;
}

//-------------------------------------

void gridSetFlag(Grid *grid, int x, int y)
{
    if(grid == NULL)
    {
        printf("ERREUR (gridSetFlag) - Un pointeur NULL a été passé en argument\n\n");
        exit(-2);
    }
    else if(x < 0 || x >= grid->nb_colonnes)
    {
        printf("ERREUR (gridSetFlag) - L'abcisse donnée sort du plateau de jeu\n\n");
        exit(-2);
    }
    else if(y < 0 || y >= grid->nb_lignes)
    {
        printf("ERREUR (gridSetFlag) - L'ordonnée donnée sort du plateau de jeu\n\n");
        exit(-2);
    }
    else if(grid->element[y][x] == -1)
    {
        grid->element[y][x] = -3;
    }
    else if(grid->element[y][x] == -2)
    {
        grid->element[y][x] = -4;
    }
}

//-------------------------------------

int gridWon(Grid *grid)
{
    if(grid == NULL)
    {
        printf("ERREUR (gridWon) - Un pointeur NULL a été passé en argument\n\n");
        exit(-2);
    }

    int compteur = 0;
    
    for (int j = 0 ; j < grid->nb_lignes ; j++)
    {
        for (int i = 0 ; i < grid->nb_colonnes ; i++)
        {
            if(grid->element[j][i] == -7) // Une bombe a explosé
            {
                return -1; // Fin de partie
            }
            else if(grid->element[j][i] == -3 || grid->element[j][i] == -1 )
            {
                // On détecte si il existe encore des cases non-révélées
                compteur++;
            }
        }
    }

    // Aucune cases non-révélées n'ont été détectées ?
    if(!compteur)
    {
        return 1; // La partie est gagnée
    }
    
    return 0; // Partie en cours
}

//---------------------------
// Fonctions intermédiaires : 
//---------------------------
// Ajuste le nombre de tirets en fonction de la taille
// de la grille

static void tiret_grille(int nb_colonnes)
{
    if(0 >= nb_colonnes)
    {
        printf("ERREUR (tiret_grille) - Le nombre de colonnes doit être positif\n\n");
        exit(-2);
    }
    
    for (int i = 0 ; i < nb_colonnes ; i++ )
    {
        printf("--");
    }
    
    printf("-\n");
}

//-------------------------------------
// Affiche le titre du jeu en fonction de la taille
// de la grille

static void titre_grille(int nb_colonnes)
{
    if(0 >= nb_colonnes)
    {
        printf("ERREUR (titre_grille) - Le nombre de colonnes doit être positif\n\n");
        exit(-2);
    }
    
    int i;
    int pt_depart = ((nb_colonnes * 2)-24)/2;

    printf("|");
    
    for (i = 1 ; i < pt_depart ; i++)
    {
        printf(" ");
    }
    
    printf("Welcome to the minefield!");
    
    for(i = 0 ; i < pt_depart ; i++)
    {
        printf(" ");
    }
    
    printf("|\n");
}

//-------------------------------------
// Vérifie dans les boucles "for" que les
// indices x, y ne sortent pas du tableau et
// passe la vérification inutile de la case 
// (x + 0, y + 0) dont on connait déja la valeur

static int verif_indice(Grid *grid, int x, int y, int a, int b)
{
    if(y < 0 || y >= grid->nb_lignes)
    {
        return 1;
    }
    else if(x < 0 || x >= grid->nb_colonnes)
    {
        return 1;
    }
    else if(a == 0 && b == 0)
    {
        return 1;
    }
    else 

        return 0;
}

//-------------------------------------
// Le gridValue utilisé par gridReveal qui ne fonctionnera que
// si gridReveal a placé l'indice "-6" dans la case pour spécifier
// que gridValue y est applicable

static int gridValueInexplored(Grid *grid, int x, int y)
{
    // Hypothèses :
    if(grid == NULL)
    {
        printf("ERREUR (gridValueInexplored) - Un pointeur NULL a été passé en argument\n\n");
        exit(-2);
    }
    else if(x < 0 || x >= grid->nb_colonnes)
    {
        printf("ERREUR (gridValueInexplored) - L'abcisse donnée sort du plateau de jeu\n\n");
        exit(-2);
    }
    else if(y < 0 || y >= grid->nb_lignes)
    {
        printf("ERREUR (gridValueInexplored) - L'ordonnée donnée sort du plateau de jeu\n\n");
        exit(-2);
    }
    // Hypothèse supplémentaire :
    else if(grid->element[y][x] != -6)
    {
        printf("ATTENTION - gridValueInexplored ne peut être appelé que sur des cases révélées par gridReveal");
        return -1;
    }

    // Comptage des bombes :
    int compteur_bombe = 0;
    
    for (int b = -1 ; b < 2 ; b++)
    {
        for (int a = -1 ; a < 2 ; a++)
        {
            if(verif_indice(grid, x + a, y + b, a, b))
            {
                continue;
            }

            else if(grid->element[y+b][x+a] == -2 || grid->element[y+b][x+a] == -4)
            {
                compteur_bombe++;
            }
        }
    }

    return compteur_bombe;
}

//-------------------------------------
// Effectue un gridValue de façon récursive sur les cases
// autour d'une case de valeur 0

static void gridValue_recursiv(Grid *grid, int x, int y)
{
    // Hypothèses :
    if(grid == NULL)
    {
        printf("ERREUR (gridValue_recursiv) - Un pointeur NULL a été passé en argument\n\n");
        exit(-2);
    }
    else if(x < 0 || x >= grid->nb_colonnes)
    {
        printf("ERREUR (gridValue_recursiv) - L'abcisse donnée sort du plateau de jeu\n\n");
        exit(-2);
    }
    else if(y < 0 || y >= grid->nb_lignes)
    {
        printf("ERREUR (gridValue_recursiv) - L'ordonnée donnée sort du plateau de jeu\n\n");
        exit(-2);
    }
       

    for (int b = -1 ; b < 2 ; b++)
    {
        for (int a = -1 ; a < 2 ; a++)
        {
           if(verif_indice(grid, x + a, y + b, a, b))
            {
                continue;
            }

            else if(gridIsExplored(grid, x + a, y + b))
            {
                continue;
            }
            
            // La case peut être révélée
            grid->element[y+b][x+a] = -6;

            int valeur = gridValueInexplored(grid, x + a, y + b);

            grid->element[y+b][x+a] = valeur;

            // Si la case = 0, on rappel gridValueInexplored sur la case de position (x + a, y + b)
            if(!valeur)
            {
                gridValue_recursiv(grid, x + a, y + b);
            }
        }
    }      
}

//-------------------------------------
//Place les bombes aléatoirement en dehors de la zone premièrement révélée

static void placebombs(Grid *grille, int x, int y)
{
    if(grille == NULL)
    {
        printf("ERREUR (placebombs) - Un pointeur NULL a été passé en argument");
        exit(-2);
    }
    else if(x < 0 || x >= grille->nb_colonnes)
    {
        printf("ERREUR (placebombs) - L'abcisse donnée sort du plateau de jeu");
        exit(-2);
    }
    else if(y < 0 || y >= grille->nb_lignes)
    {
            printf("ERREUR (placebombs) - L'ordonnée donnée sort du plateau de jeu");
            exit(-2);
    }

    // Initialisation des variables :
    int nb_lignes = grille->nb_lignes;
    int nb_bombes = grille->nb_bombes;  
    int nb_colonnes = grille->nb_colonnes;

    // Initialise le carré 5X5 où le placement de bombes est interdit
    for (int b = -2 ; b < 3 ; b++)
    {
        for (int a = -2 ; a < 3 ; a++)
        {
            if(verif_indice(grille, x + a, y + b, a, b))
            {
                continue;
            }
            // "-5" = Placement d'une bombe y est interdit
            grille->element[y + b][x + a] = -5;
        }
    }

    // Placements aléatoires des bombes
    srand(time(NULL));

    for (int placed_bombs = 0 ; placed_bombs < nb_bombes ; )
    {
        int indice_h = rand() % nb_lignes;
        int indice_w = rand() % nb_colonnes;

        if(grille->element[indice_h][indice_w] != -5 && grille->element[indice_h][indice_w] != -2)
        {
            grille->element[indice_h][indice_w] = -2;
            placed_bombs++;
        }
    }
    
    // On révèle toutes les cases initialement interdites
    grille->element[y][x] = 0; // 1er case pointée vaut toujours 0
    gridValue_recursiv(grille, x , y);
}

//-------------------------------------

