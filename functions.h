#include"board.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
/**
* @brief Action possibles lors du tour d'un joueur
*/
typedef enum{PLACEMENT =0,DEPLACEMENT} action;
/**
* @brief Nombre d'actions possibles pour le joueur
*/
#define NB_ACTIONS 2
#define NB_GOB 10
typedef struct{
    SDL_Color col;
    float dx,dy;
    float x,y;
    int w,h;
}gobMenu;
/**
* @brief Structure de l'algo Minimax
*/
typedef struct{
    int score;
    int x1,x2;
    int y1,y2;
    size size;
    action a;
} Minimax_Move;
/**
* @brief Retire une pièce
* @param board le plateau de jeu
* @param int la ligne selectionnée
* @param int la colonne selectionnée
* @return 2 : saisie illégale, 1 : case vide, 0 : succès
*/
int remove_piece(board game, int line, int column);
/**
* @brief Algorithme minimax avec élagage alpha-beta pour trouver le meilleur déplacement possible
* @param board le plateau de jeu
* @param int la profondeur maximale
* @param int la profondeur actuelle
* @param player le joueur qui effectue une action
* @return le mouvement que le bot va effectuer
*/
Minimax_Move minimax(board game, int maxDepth, int depth,int alpha, int beta,player p);
/**
* @brief Fonction utilisant la fonction minimax() pour ensuite faire jouer le bot
* @param board le plateau de jeu
* @param int la profondeur maximale
* @param int la profondeur actuelle
* @param player le joueur qui effectue une action
*/
void botMove(board game, int maxDepth, int depth,int alpha, int beta,player p);
/**
* @brief Affiche le plateau de jeu
* @param SDL_Renderer le renderer utilisée
* @param board le plateau de jeu
* @param SDL_Texture texture du gobelet orange
* @param SDL_Texture texture du gobelet bleu
* @param TTF_Font la police utilisée
*/
void affBoard(SDL_Renderer *renderer,board game,SDL_Texture *gobeletBleu,SDL_Texture *gobeletOrange,SDL_Texture *plateau,TTF_Font *font);
/**
* @brief Affiche le menu
* @param SDL_Renderer le renderer utilisée
* @param TTF_Font la police utilisée
* @param bool booléen pour quitter le jeu
* @param bool booléen qui permet de jouer contre une machine ou un joueur
*/
void affMenu(SDL_Renderer *renderer,TTF_Font *font, bool *quit, bool *machine,SDL_Texture *gob);
/**
* @brief Crée du texte sous forme de texture
* @param SDL_Renderer le renderer utilisé
* @param char la chaine de caractères à texturiser
* @param SDL_Color la couleur du texte
* @param TTF_Font la police utilisée
* @return la texture créée
*/
SDL_Texture *createTexturedText(SDL_Renderer *renderer,char *str,SDL_Color col,TTF_Font *font);
/**
* @brief Initialise un tableau de gobelet flottants
* @param gobMenu le tableau de gobelets
* @param int la taille du tableau
* @param SDL_Renderer le renderer dans lequel les gobelets seront affichés
*/
void initGob(gobMenu g[],int size,SDL_Renderer *renderer);
/**
* @brief Affiche des gobelets flottant dans le menu
* @param gobMenu le tableau de gobelets
* @param int la taille du tableau
* @param SDL_Renderer le renderer dans lequel les gobelets seront affichés
* @param SDL_Texture la texture du gobelet à afficher
*/
void updateGob(gobMenu g[],int size,SDL_Renderer *renderer,SDL_Texture *texture);
