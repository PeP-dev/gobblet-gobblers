#include"functions.h"
void affBoard(SDL_Renderer *renderer,board game,SDL_Texture *gobeletBleu,SDL_Texture *gobeletOrange,SDL_Texture *plateau,TTF_Font *font){
    int w;
    int h;
    SDL_GetRendererOutputSize(renderer,&w,&h);
    SDL_Color black = {0, 0, 0};
    char sizes[12];
    SDL_Rect dest;
    dest.x = 0;
    dest.y = 0;
    dest.h = h;
    dest.w = w;
    SDL_RenderCopy(renderer,plateau,NULL,&dest);
    int scaleX= w/15 -10;
    int scaleY = h/9 - 10;
    int size;
    int nbPieces;
    for(int i = SMALL; i<=NB_SIZE;i++){
        //Affichages des pieces du joueur 1
        nbPieces = get_nb_piece_in_house(game,PLAYER_1,i);
        if(nbPieces){
            dest.w = scaleX*i;
            dest.h = scaleY*i;
            dest.x = w/10-dest.w/2;
            dest.y = (i-1)*h/3 +h/6 - dest.h/2;
            SDL_RenderCopy(renderer,gobeletOrange,NULL,&dest);
        }
        //Affichages des pieces du joueur 2
        nbPieces = get_nb_piece_in_house(game,PLAYER_2,i);
        if(nbPieces){
            dest.w = scaleX*i;
            dest.h = scaleY*i;
            dest.x = w-w/10 - dest.w/2;
            dest.y = (i-1)*h/3 + h/6 - dest.h/2;
            SDL_RenderCopy(renderer,gobeletBleu,NULL,&dest);
        }
    }
    //Affichage des pièces posées sur le plateau
    for(int i = 0;i<DIMENSIONS;i++){
        for(int j = 0; j <DIMENSIONS;j++){
            if(get_place_holder(game,j,i) == PLAYER_1){
                size = get_piece_size(game,j,i);
                dest.w = size*scaleX;
                dest.h = size*scaleY;
                dest.y = j*h/3 + h/6 - dest.h/2;
                dest.x = (i+1)*w/5 + w/10 - dest.w/2;
                SDL_RenderCopy(renderer,gobeletOrange,NULL,&dest);
            }
            else if(get_place_holder(game,j,i) == PLAYER_2){
                size = get_piece_size(game,j,i);
                dest.w = size*scaleX;
                dest.h = size*scaleY;
                dest.y = j*h/3 + h/6 - dest.h/2;
                dest.x = (i+1)*w/5 + w/10 - dest.w/2;
                SDL_RenderCopy(renderer,gobeletBleu,NULL,&dest);
            }
        }
    }
}
Minimax_Move minimax(board game, int maxDepth, int depth,int alpha, int beta,player p){
  int goodMove;
  Minimax_Move bestScore;
  Minimax_Move score;
  bool isMaximizer;
  if(p == PLAYER_2) isMaximizer = true;
  else if(p==PLAYER_1) isMaximizer = false;

  if(get_winner(game) == PLAYER_1){
    bestScore.score = -maxDepth +depth -2;
    return bestScore;
  }
  else if(get_winner(game) == PLAYER_2){
    bestScore.score = maxDepth - depth +2;
    return bestScore;
  }
  else{
    bestScore.score = 0;
  }
  if(depth <=maxDepth){
    if(!isMaximizer){
      bestScore.score = 10000;
    }
    else{
      bestScore.score =-10000;
    }
    for(int i = 0; i < DIMENSIONS; i++){
      for(int j = 0; j < DIMENSIONS; j++){
        for(size s = SMALL ; s <= LARGE; s++){
          goodMove = place_piece(game,p,s,j,i);
          if(goodMove == 0) {
            score = minimax(game,maxDepth,depth+1,alpha,beta,next_player(p));
            score.x1 =i;
            score.y1 =j;
            score.a = PLACEMENT;
            score.size = s;
            remove_piece(game,j,i);
            if(!isMaximizer){
              if(bestScore.score > score.score) bestScore = score;
              if(beta > score.score) beta = score.score;
              if(beta <=alpha){
                  return bestScore;
              }
            }
            else{
              if(bestScore.score < score.score) bestScore = score;

              if(alpha < score.score) alpha = score.score;
              if(beta <=alpha){
                  return bestScore;
              }

            }
          }
         if(get_place_holder(game,j,i) == p)
          {
            for(int k = 0; k < DIMENSIONS; k++){
              for(int l = 0; l < DIMENSIONS; l++){
                goodMove = move_piece(game,j,i,l,k);
                if(goodMove == 0){
                  score = minimax(game,maxDepth,depth+1,alpha,beta,next_player(p));
                  score.x1 = i;
                  score.y1 = j;
                  score.x2 = k;
                  score.y2 = l;
                  score.a = DEPLACEMENT;
                  move_piece(game,l,k,j,i);
                  if(!isMaximizer){
                    if(bestScore.score > score.score) bestScore = score;
                    if(beta > score.score) beta = score.score;
                    if(beta <=alpha){
                        return bestScore;
                    }
                  }
                  else{
                    if(bestScore.score < score.score) bestScore = score;

                    if(alpha < score.score) alpha = score.score;
                    if(beta <=alpha){
                        return bestScore;
                    }

                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return bestScore;
}
void affMenu(SDL_Renderer *renderer,TTF_Font *font, bool *quit, bool *machine,SDL_Texture *gob){
    int width = 1100;
    int height = 660;
    bool menu = true;
    bool menuRegles = false;
    SDL_Event event;
    SDL_Rect dBotJoueur, dRegles,dPartie,dFleche,dInfo;
    SDL_Color white = {255,255,255};
    SDL_Color black = {0,0,0};
    int cx,cy;
    //Bien qu'affichées plus tard en noir et blanc, on crée le texte en blanc pour appliquer un masque desus ensuite
    SDL_Texture *bot = createTexturedText(renderer,"Jouer contre l'IA",white,font);
    SDL_Texture *regles = createTexturedText(renderer,"Afficher les regles",white,font);
    SDL_Texture *joueur = createTexturedText(renderer,"Jouer contre un joueur",white,font);
    SDL_Texture *partie = createTexturedText(renderer,"Lancer la partie",white,font);
    SDL_Texture *fleche = createTexturedText(renderer,"<- Retour au menu",white,font);
    SDL_Texture *info = createTexturedText(renderer,"Fenetre redimensionnable",white,font);
    SDL_Texture *tabRegles[6];

    gobMenu g[NB_GOB];
    initGob(g,NB_GOB,renderer);
    tabRegles[0] = createTexturedText(renderer,"Les regles du jeu sont simples",black,font);
    tabRegles[1] = createTexturedText(renderer,"Vous avez 6 gobelets, 2 de chaque taille",black,font);
    tabRegles[2] = createTexturedText(renderer,"Vous pouvez jouer un gobelet depuis ceux dans votre maison",black,font);
    tabRegles[3] = createTexturedText(renderer,"Ou bien deplacer vos gobelets deja poses",black,font);
    tabRegles[4] = createTexturedText(renderer,"Il est possible de jouer un gobelet plus grand sur un gobelet plus petit",black,font);
    tabRegles[5] = createTexturedText(renderer,"Le premier joueur qui aligne 3 gobelets gagne",black,font);

    SDL_SetRenderDrawColor(renderer,200, 200, 255, 255);

    while(menu && !(*quit)){
        SDL_RenderClear(renderer);
        updateGob(g,NB_GOB,renderer,gob);
        SDL_GetRendererOutputSize(renderer,&width,&height);
        SDL_GetMouseState(&cx,&cy);
        //Affichage du menu principal
        if(!menuRegles){

            SDL_QueryTexture(partie,NULL,NULL,&dPartie.w,&dPartie.h);

            dPartie.x = width/2 - dPartie.w/2;
            dPartie.y = height/5 - dPartie.h/2;
            //Changement de couleur au survol du curseur
            if(cx > dPartie.x && cx < dPartie.x+dPartie.w && cy > dPartie.y && cy < dPartie.y+dPartie.h){
                SDL_SetTextureColorMod(partie,0,0,255);
            }
            else{
                SDL_SetTextureColorMod(partie,0,0,0);
            }
            SDL_RenderCopy(renderer,partie,NULL,&dPartie);

            SDL_QueryTexture(regles,NULL,NULL,&dRegles.w,&dRegles.h);
            dRegles.x = width/2 - dRegles.w/2;
            dRegles.y = 2*height/5 - dRegles.h/2;
            //Changement de couleur au survol du curseur
            if(cx > dRegles.x && cx < dRegles.x+dRegles.w && cy > dRegles.y && cy < dRegles.y+dRegles.h){
                SDL_SetTextureColorMod(regles,0,0,255);
            }
            else{
                SDL_SetTextureColorMod(regles,0,0,0);
            }
            SDL_RenderCopy(renderer,regles,NULL,&dRegles);


            if((*machine)){
                SDL_QueryTexture(bot,NULL,NULL,&dBotJoueur.w,&dBotJoueur.h);
                dBotJoueur.x = width/2 - dBotJoueur.w/2;
                dBotJoueur.y = 3*height/5 - dBotJoueur.h/2;
                //Changement de couleur au survol du curseur
                if(cx > dBotJoueur.x && cx < dBotJoueur.x+dBotJoueur.w && cy > dBotJoueur.y && cy < dBotJoueur.y+dBotJoueur.h){
                    SDL_SetTextureColorMod(bot,0,0,255);
                }
                else{
                    SDL_SetTextureColorMod(bot,0,0,0);
                }
                SDL_RenderCopy(renderer,bot,NULL,&dBotJoueur);
            }
            else{
                SDL_QueryTexture(joueur,NULL,NULL,&dBotJoueur.w,&dBotJoueur.h);
                dBotJoueur.x = width/2 - dBotJoueur.w/2;
                dBotJoueur.y = 3*height/5 - dBotJoueur.h/2;
                //Changement de couleur au survol du curseur
                if(cx > dBotJoueur.x && cx < dBotJoueur.x+dBotJoueur.w && cy > dBotJoueur.y && cy < dBotJoueur.y+dBotJoueur.h){
                    SDL_SetTextureColorMod(joueur,0,0,255);
                }
                else{
                    SDL_SetTextureColorMod(joueur,0,0,0);
                }
                SDL_RenderCopy(renderer,joueur,NULL,&dBotJoueur);
            }
            SDL_QueryTexture(info,NULL,NULL,&dInfo.w,&dInfo.h);
            dInfo.x = width/2 - dInfo.w/2;
            dInfo.y = 4*height/5 - dInfo.h/2;
            SDL_RenderCopy(renderer,info,NULL,&dInfo);
        }
        //Affichage des règles
        else{
            SDL_QueryTexture(fleche,NULL,NULL, &dFleche.w,&dFleche.h);
            dFleche.x = width/5 - dFleche.w/2;
            dFleche.y = height/10 - dFleche.h/2;
            //Changement de couleur au survol du curseur
            if(cx > dFleche.x && cx < dFleche.x+dFleche.w && cy > dFleche.y && cy < dFleche.y+dFleche.h){
                SDL_SetTextureColorMod(fleche,0,0,255);
            }
            else{
                SDL_SetTextureColorMod(fleche,0,0,0);
            }
            SDL_RenderCopy(renderer,fleche,NULL,&dFleche);
            //Affichage du texte
            for(int i = 0; i< 6;i++){
                SDL_QueryTexture(tabRegles[i],NULL,NULL,&dRegles.w,&dRegles.h);
                dRegles.x = width/2 - dRegles.w/2;
                dRegles.y = (2+i)*height/8 - dRegles.h/2;
                SDL_RenderCopy(renderer,tabRegles[i],NULL,&dRegles);
            }
        }


        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_MOUSEBUTTONDOWN :
                    if(!menuRegles){
                        if(cx < width/2 + dPartie.w/2 && cx > width/2-dPartie.w/2 && cy > height/5-dPartie.h/2 && cy < height/5 + dPartie.h/2){
                            menu = false;
                        }
                        else if(cx >width/2-dRegles.w/2 && cx < width/2+dRegles.w/2 && cy < 2*height/5 + dRegles.h/2 && cy > 2*height/5 - dRegles.h/2){
                            menuRegles = true;
                        }
                        else if(cx > dBotJoueur.x && cx < dBotJoueur.x+dBotJoueur.w && cy > dBotJoueur.y && cy < dBotJoueur.y+dBotJoueur.h){
                            *machine = !(*machine);
                        }
                    }
                    else{
                        if(cx > dFleche.x && cx < dFleche.x+dFleche.w && cy > dFleche.y && cy < dFleche.y+dFleche.h){
                            menuRegles = false;
                        }
                    }
                    break;
                case SDL_QUIT :
                    *quit = true;
                    break;
            }
        }
        SDL_RenderPresent(renderer);
    }
    for(int i = 0; i<6;i++){
        SDL_DestroyTexture(tabRegles[i]);
    }
    SDL_DestroyTexture(info);
    SDL_DestroyTexture(fleche);
    SDL_DestroyTexture(partie);
    SDL_DestroyTexture(regles);
    SDL_DestroyTexture(joueur);
    SDL_DestroyTexture(bot);
}
void botMove(board game, int maxDepth, int depth,int alpha, int beta,player p){
    Minimax_Move mv = minimax(game,maxDepth,depth,alpha,beta,p);
    if(mv.a == PLACEMENT){place_piece(game,p,mv.size,mv.y1,mv.x1);}
    else if(mv.a == DEPLACEMENT){move_piece(game,mv.y1,mv.x1, mv.y2, mv.x2);}
}
SDL_Texture *createTexturedText(SDL_Renderer *renderer,char *str,SDL_Color col,TTF_Font *font){

    SDL_Surface *surf = TTF_RenderText_Blended(font,str,col);
    if(!surf) fprintf(stderr,"Échec de création du texte (%s)\n",TTF_GetError());

    SDL_Texture *texture = NULL;
    texture = SDL_CreateTextureFromSurface(renderer,surf);
    if(!texture) fprintf(stderr,"Échec de création de la texture du texte (%s)\n",SDL_GetError());
    SDL_FreeSurface(surf);
    return texture;
}
void initGob(gobMenu g[],int size,SDL_Renderer *renderer){
    int w;
    SDL_GetRendererOutputSize(renderer,&w,NULL);
    for(int i = 0; i< size;i++){
        g[i].col.r = 50+rand()%155;
        g[i].col.g = 50+rand()%155;
        g[i].col.b = 50+rand()%155;
        g[i].x = 50+rand()%(w - 100);
        g[i].y = 0;
        g[i].dy = 1+3 * (float)rand() / (float)RAND_MAX;
        g[i].dx =  2*((float)rand() / (float)RAND_MAX) -1;
        g[i].w = 30+rand()%40;
        g[i].h = g[i].w;
    }
}
void updateGob(gobMenu g[],int size,SDL_Renderer *renderer,SDL_Texture *texture){
    int w,h;
    SDL_Rect dest;
    SDL_GetRendererOutputSize(renderer,&w,&h);
    for(int i = 0; i< size;i++){
        g[i].y += g[i].dy;
        g[i].x += g[i].dx;
        if(g[i].y > h) {
            g[i].col.r = 50+rand()%155;
            g[i].col.g = 50+rand()%155;
            g[i].col.b = 50+rand()%155;
            g[i].x = 50+rand()%(w - 100);
            g[i].y = 0;
            g[i].dy = 1+2 * (float)rand() / (float)RAND_MAX;
            g[i].dx =  2*((float)rand() / (float)RAND_MAX) -1;
            g[i].w = 30+rand()%40;
            g[i].h = g[i].w;
        }
        if(g[i].x < 0){
            g[i].x = w;
        }
        if(g[i].x > w){
            g[i].x = 0;
        }
        dest.w = g[i].w;
        dest.h = g[i].h;
        dest.x = g[i].x;
        dest.y = g[i].y;
        SDL_SetTextureColorMod(texture,g[i].col.r,g[i].col.g,g[i].col.b);
        SDL_RenderCopy(renderer,texture,NULL,&dest);
    }
}
