//----------------------------------
// 
//           Minesweeper 
//
//----------------------------------
// @ Ali Sezgin     Victor Mangeleer

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "Solver.h"


//-------------
// Prototypes :
//-------------

static int first_action(Grid *grid);

static void first_move(Grid *grid, Move *action);

static void second_move(Grid *grid, Move *action);

static int verif_indice(Grid *grid, int x, int y, int a, int b);

static int nbr_revealedflags(Grid *grid, int x, int y, int action);

static void first_non_revealed(Grid *grid, Move *action, int x, int y);

//------------------------
// Fonctions principales :
//------------------------

// 1) Joueur HUMAIN : 

Move human(Grid *grid)
{
    if(grid == NULL)
    {
        printf("ERREUR (Move Human) - Un pointeur nul a été passé en argument ");
        exit(-2);
    }
    
    // Récupération des valeurs entrées au clavier :
    int x, y, flag;

    printf("\nAction = <x> <y> <flag> :");
    scanf("%d %d %d", &x, &y, &flag);
    printf("\n");
    
    // Initialisation des variables
    int validite = 0;
    int nb_lignes = gridHeight(grid);
    int nb_colonnes = gridWidth(grid);
    
    // Test de la validité des arguments encodés :
    while(!validite)
    {
        // On test si X et Y appartiennent au tableau 
        if(x < 0 && y < 0)
        {
            printf("Les coordonnées en x et y sortent du plateau de jeu, veuiller ré-encoder votre <x> <y> <flag> :");
            scanf("%d %d %d", &x, &y, &flag);
            continue;
        }

        else if(x >= nb_colonnes && y < 0)
        {
            printf("Les coordonnées en x et y sortent du plateau de jeu, veuiller ré-encoder votre action <x> <y> <flag> :");
            scanf("%d %d %d", &x, &y, &flag);
            continue;
        }
        
        else if(x < 0 && y >= nb_lignes)
        {
            printf("Les coordonnées en x et y sortent du plateau de jeu, veuiller ré-encoder votre action <x> <y> <flag> :");
            scanf("%d %d %d", &x, &y, &flag);
            continue;
        }
        
        else if(x >= nb_colonnes && y >= nb_lignes)
        {
            printf("Les coordonnées en x et y sortent du plateau de jeu, veuiller ré-encoder votre action <x> <y> <flag> :");
            scanf("%d %d %d", &x, &y, &flag);
            continue;
        }
        
        // On test si X appartient au tableau
        else if(x < 0 || x >= nb_colonnes)
        {
            printf("La coordonnée en x sort du plateau de jeu, veuiller ré-encoder votre action <x> <y> <flag> :");
            scanf("%d %d %d", &x, &y, &flag);
            continue;
        }

        // On test si Y appartient au tableau
        else if(y < 0 || y >= nb_lignes)
        {
            printf("La coordonnée en y sort du plateau de jeu, veuiller ré-encoder votre action <x> <y> <flag> :");
            scanf("%d %d %d", &x, &y, &flag);
            continue;
        }

        // On test si FLAG est compris entre -1 et 1
        else if(flag < -1 || flag > 1)
        {
            printf("La valeur de flag doit être comprise entre -1 et 1, veuiller ré-encoder votre action <x> <y> <flag> :");
            scanf("%d %d %d", &x, &y, &flag);
            continue;
        }
        else

            validite = 1;
    }
    
    // Conversion du y pour avoir l'origine en bas à gauche
    y = (nb_lignes - 1) - y;
    
    // Création de la structure d'action
    Move action_joueur;
    action_joueur.x = x;
    action_joueur.y = y;
    action_joueur.flag = flag;
    
    return action_joueur;
}

//-------------------------------------

// 2) Joueur ORDINATEUR :

 Move heuristic(Grid *grid)
{
    if(grid == NULL)
    {
        printf("ERREUR (Move heuristic) - Un pointeur nul a été passé en argument ");
        exit(-2);
    }

    // Initialisation des variables :
    int width = gridWidth(grid);
    int height = gridHeight(grid);

    Move action_ordi;
    Move *pt_action = &action_ordi;
    
    // Révèle une case aléatoire si c'est la première action
    if(first_action(grid))
    {
        srand(time(NULL));
        action_ordi.x = rand() % (width - 1);
        action_ordi.y = rand() % (height - 1);

        action_ordi.flag = 0;

        return action_ordi;
    }
    
    // Actions d'heuristic :

    action_ordi.flag = -1; // Initialement quitte la partie

    //FIRST MOVE :

    first_move(grid, pt_action);
    
    if(action_ordi.flag == -1)
    {
        second_move(grid, pt_action);
    }
     
    return action_ordi;
}

//---------------------------
// Fonctions intermédiaires : 
//---------------------------
// Indique à heuristic si c'est son premier coup ou non

static int first_action(Grid *grid)
{
    int nb_lignes = gridHeight(grid);
    int nb_colonnes = gridWidth(grid);
    
    for (int j = 0 ; j < nb_lignes ; j++)
    {
        for (int i = 0 ; i < nb_colonnes ; i++)
        {
            if(gridIsExplored(grid, i, j))
            {
                return 0; // Pas le premier
            }
        }
    }

    return 1; // Le premier
}

//-------------------------------------
// Calcule le nombre de case non-révélée et (si 1er action) 
// de flags autour d'une case

static int nbr_revealedflags(Grid *grid, int x, int y, int action)
{
    if(grid == NULL)
    {
        printf("ERREUR (nbr_revealdflags) - Un pointeur nul a été passé en argument ");
        exit(-2);
    }

    else if(action < 1 || action > 2)
    {
        printf("ERREUR (nbr_revealdflags) - Action doit valoir 1 ou deux ");
        exit(-2);
    }

    int compteur = 0;

    // Les deux compteurs nous permettent de savoir si il existe AU MOINS
    // une case non-révélée dans ce qui a été compté. Si ce n'est pas le cas,
    // alors même si la condition d'égalité est vérifiée on renvoit 0 car l'ordinateur
    // ne trouvera pas de case non-révélée pour effectuer son action dessus
    int compteur_action1 = 0;
    int compteur_action2 = 0;

    for (int a = -1 ; a < 2 ; a++)
    {     
        for (int b = -1 ; b < 2 ; b++)
        {
            if(verif_indice(grid, x + b, y + a, a, b))
            {
                continue;
            }
            else if(gridIsFlagged(grid, x + b, y + a))
            {
                compteur++;
            }
            else if(!gridIsExplored(grid, x + b, y + a))
            {
                if(action == 1)
                {
                    compteur++;
                    compteur_action1++;
                    continue;
                }
                
                compteur_action2++;     
            }
        }
    }      
    if(action == 1 && compteur_action1 == 0)
    {
        return 0;
    }
    else if(action == 2 && compteur_action2 == 0)
    {
        return 0;
    }

    return compteur;
}

//-------------------------------------
// Détecte la première case non-révélée aux alentours et
// place ses coordonnées dans action

static void first_non_revealed(Grid *grid, Move *action, int x, int y)
{
    if(grid == NULL)
    {
        printf("ERREUR (first_non_revealed) - Un pointeur nul a été passé en argument ");
        exit(-2);
    }

    for (int a = -1 ; a < 2 ; a++)
    {    
        for (int b = -1 ; b < 2 ; b++)
        {
            if(verif_indice(grid, x + b, y + a, a, b))
            {
                continue;
            }
            else if(gridIsFlagged(grid, x + b, y + a))
            {
                continue;
            }
            else if(!gridIsExplored(grid, x + b, y + a))
            {       
                action->x = x + b;
                action->y = y + a;
                return;
            }
        }
    }
}   

//-------------------------------------
// Effectue la 1er action possible (change le flag en 1) de heuristic
// si celle-ci est possible

static void first_move(Grid *grid, Move *action)
{
    if(grid == NULL)
    {
        printf("ERREUR (nbr_revealedflags) - Un pointeur nul a été passé en argument ");
        exit(-2);
    }

    int nb_lignes = gridHeight(grid);
    int nb_colonnes = gridWidth(grid);

    for (int a = 0 ; a < nb_lignes ; a++)
    {
        for (int b = 0 ; b < nb_colonnes ; b++)
        {
            if(gridIsExplored(grid, b, a) && gridValue(grid, b, a) > 0)
            {
                int valeur = nbr_revealedflags(grid, b, a, 1);

                if(valeur == gridValue(grid, b, a))
                {
                    first_non_revealed(grid, action, b, a);
                    action->flag = 1;
                    return;
                }
            }
        }
    }
}

//-------------------------------------
// Effectue la 2ème action possible (change flag en 0) de heuristic
// si celle-ci est possible

static void second_move(Grid *grid, Move *action)
{
     if(grid == NULL)
    {
        printf("ERREUR (nbr_revealedflags) - Un pointeur nul a été passé en argument ");
        exit(-2);
    }

    int nb_lignes = gridHeight(grid);
    int nb_colonnes = gridWidth(grid);

    for (int a = 0 ; a < nb_lignes ; a++)
    {
        for (int b = 0 ; b < nb_colonnes ; b++)
        {
            if(gridIsExplored(grid, b, a) && gridValue(grid, b, a) > 0)
            {
                int valeur = nbr_revealedflags(grid, b, a, 2);

                if(valeur == gridValue(grid, b, a))
                {
                    first_non_revealed(grid, action, b, a);
                    action->flag = 0;
                    return;
                }
            }
        }
    }
}

//-------------------------------------
// Vérifie dans les boucles "for" que les
// indices x, y ne sortent pas du tableau et
// passe la vérification inutile de la case 
// (x + 0, y + 0) dont on connait déja la valeur

static int verif_indice(Grid *grid, int x, int y, int a, int b)
{
    int nb_lignes = gridHeight(grid);
    int nb_colonnes = gridWidth(grid);

    if(y < 0 || y >= nb_lignes)
    {
        return 1;
    }
    else if(x < 0 || x >= nb_colonnes)
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


