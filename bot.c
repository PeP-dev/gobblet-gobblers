int remove_piece(board game, int line, int column){
    if(line > DIMENSIONS-1 || line <0 || column >DIMENSIONS-1 || column < 0){return 2;}
    else if(get_place_holder(game,line,column) == NO_PLAYER || get_piece_size(game,line,column) == NONE){return 1;}
    else{
        int set = game->cells[column][line].nbPiecesSet;
          game->nbPiecesPerPlayer[get_place_holder(game,line,column)-1][get_piece_size(game,line,column)-1] ++;
          game->cells[column][line].sizeHolder[set-1] = NONE;
          game->cells[column][line].placeHolder[set-1] = NO_PLAYER;
          game->cells[column][line].nbPiecesSet--;
    }
    return 0;
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
void botMove(board game, int maxDepth, int depth,int alpha, int beta,player p){
    Minimax_Move mv = minimax(game,maxDepth,depth,alpha,beta,p);
    if(mv.a == PLACEMENT){place_piece(game,p,mv.size,mv.y1,mv.x1);}
    else if(mv.a == DEPLACEMENT){move_piece(game,mv.y1,mv.x1, mv.y2, mv.x2);}
}
