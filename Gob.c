#include"functions.h"
#include<SDL2/SDL_image.h>
int main(){
    srand(time(NULL));
    int width = 1100;
    int height = 660;

    board game;
    player p = PLAYER_1;
    bool machine = true;

    SDL_Event event;
    bool quit = false;
    bool dragging = false;
    bool menu = true;
    bool end = true;
    SDL_Rect dest;
    int cx,cy;

    int invalidMove;
    int mx,my;
    int scaleX,scaleY;
    int chosenPiece;
    int pieceHolder;
    action action;
    game = new_game();
    if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stderr,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }
    if(TTF_Init() == -1) fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());

    SDL_Window *win = SDL_CreateWindow("Projet SDL2",SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,width,height,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(!win) fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());

    TTF_Font *Arial = TTF_OpenFont("Arial.ttf", 30);
    if(Arial == NULL) fprintf(stderr,"Erreur de chargement de la police: %s\n",TTF_GetError());


    SDL_Renderer *renderer = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);
    if (!renderer) fprintf(stderr,"Échec de création du renderer (%s)\n",SDL_GetError());

    SDL_Surface* gobOr = IMG_Load("gobeletO.png");
    if (!gobOr) fprintf(stderr,"Échec de chargement du sprite (%s)\n",SDL_GetError());

    SDL_Surface* gobBl = IMG_Load("gobeletB.png");
    if (!gobBl) fprintf(stderr,"Échec de chargement du sprite (%s)\n",SDL_GetError());

    SDL_Surface* gobG = IMG_Load("gobG.png");
    if (!gobOr) fprintf(stderr,"Échec de chargement du sprite (%s)\n",SDL_GetError());

    SDL_Surface *plat = IMG_Load("PLATEAU.png");
    if (!plat) fprintf(stderr,"Échec de chargement du sprite (%s)\n",SDL_GetError());

    SDL_Texture *pla = SDL_CreateTextureFromSurface(renderer,plat);
    if (!pla) fprintf(stderr,"Échec de création de la texture (%s)\n",SDL_GetError());

    SDL_Texture *gobeletOrange = SDL_CreateTextureFromSurface(renderer,gobOr);
    if (!gobeletOrange) fprintf(stderr,"Échec de création de la texture (%s)\n",SDL_GetError());

    SDL_Texture *gobeletBleu = SDL_CreateTextureFromSurface(renderer,gobBl);
    if (!gobeletBleu) fprintf(stderr,"Échec de création de la texture (%s)\n",SDL_GetError());

    SDL_Texture *gobeletGris = SDL_CreateTextureFromSurface(renderer,gobG);
    if (!gobeletGris) fprintf(stderr,"Échec de création de la texture (%s)\n",SDL_GetError());

    SDL_FreeSurface(gobG);
    SDL_FreeSurface(gobOr);
    SDL_FreeSurface(gobBl);
    SDL_FreeSurface(plat);

    affMenu(renderer,Arial,&quit,&machine,gobeletGris);

    while(get_winner(game) == NO_PLAYER && !quit){
        if(p == PLAYER_1 || machine == false){
            invalidMove = 1;
            while(SDL_PollEvent(&event)){

                switch(event.type)
                {
                    case SDL_QUIT:
                        quit = true;
                        break;

                    case SDL_MOUSEBUTTONDOWN :
                        if(!dragging){
                            SDL_GetMouseState(&cx,&cy);
                            SDL_GetRendererOutputSize(renderer,&width,&height);
                            if(cx > 0 && cx < width && cy > 0 && cy < height){
                                if(cx < width/5 && p == PLAYER_1 ){
                                    chosenPiece = 1+cy/(height/3);
                                    if(get_nb_piece_in_house(game,PLAYER_1,chosenPiece)){
                                        action = PLACEMENT;
                                        pieceHolder = p;
                                        dragging = true;
                                        pieceHolder = PLAYER_1;
                                    }
                                }
                                if(cx > width-width/5 && p == PLAYER_2){
                                    chosenPiece = 1+cy/(height/3);
                                    if(get_nb_piece_in_house(game,PLAYER_2,chosenPiece)){
                                        action = PLACEMENT;
                                        pieceHolder = p;
                                        dragging = true;
                                        pieceHolder = PLAYER_2;
                                    }
                                }
                                else if(cx > width/5 && cx< width-width/5){
                                    chosenPiece = get_piece_size(game,cy/(height/3),(cx-width/5)/(width/5));
                                    my = cy/(height/3);
                                    mx = (cx-width/5)/(width/5);
                                    action = DEPLACEMENT;
                                    pieceHolder = get_place_holder(game,cy/(height/3),(cx-width/5)/(width/5));
                                    dragging = true;
                                }
                            }
                        }
                        break;

                    case SDL_MOUSEBUTTONUP :
                        if(dragging && (cx > width/5 && cx < width-width/5 && cy >0 && cy <height)){
                            if(action == PLACEMENT){
                                invalidMove = place_piece(game,p,chosenPiece,cy/(height/3),(cx-width/5)/(width/5));
                            }
                            else if(action == DEPLACEMENT && get_place_holder(game,my,mx) == p){
                                invalidMove = move_piece(game,my,mx,cy/(height/3),(cx-width/5)/(width/5));
                            }
                            if(!invalidMove) p = next_player(p);
                        }
                        dragging = false;
                        break;
                }
            }
        }
        else{
            botMove(game,5,0,-1000,1000,p);
            p = next_player(p);
        }
        affBoard(renderer,game,gobeletBleu,gobeletOrange,pla,Arial);
        if(dragging){
            SDL_GetMouseState(&cx,&cy);
            scaleX = width/15; // 5 divisions de cellules, * 3 tailles de pièces
            scaleY = height/9;
            dest.h = scaleY*chosenPiece;
            dest.w = scaleX*chosenPiece;
            dest.x = cx-dest.w/2;
            dest.y = cy-dest.h/2;
            if(pieceHolder == PLAYER_1) SDL_RenderCopy(renderer,gobeletOrange,NULL,&dest);
            if(pieceHolder == PLAYER_2) SDL_RenderCopy(renderer,gobeletBleu,NULL,&dest);
        }
        SDL_RenderPresent(renderer);
    }
    SDL_Delay(1000);
    SDL_Color black = {0,0,0};
    char txt[62];
    sprintf(txt, "Le Joueur %d a gagne, cliquez pour fermer la fenetre", get_winner(game));
    SDL_Texture *endText= createTexturedText(renderer,txt,black,Arial);

    while(end && !quit){
        SDL_QueryTexture(endText,NULL,NULL,&dest.w,&dest.h);
        SDL_GetRendererOutputSize(renderer,&width,&height);
        dest.x = width/2-dest.w/2;
        dest.y = height/2-dest.h/2;
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,endText,NULL,&dest);
        while(SDL_PollEvent(&event)){

            switch(event.type)
            {
                case SDL_MOUSEBUTTONDOWN :
                    quit = true;
                break;
                case SDL_QUIT :
                    quit = true;
                break;
            }
        }
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(gobeletGris);
    SDL_DestroyTexture(gobeletBleu);
    SDL_DestroyTexture(gobeletOrange);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    TTF_CloseFont(Arial);
    TTF_Quit();
    destroy_game(game);
}
