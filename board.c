#include"functions.h"
typedef struct gobblet{
    player placeHolder[NB_SIZE];
    size sizeHolder[NB_SIZE];
    int nbPiecesSet;
}Gobblet;

struct board_s{
    Gobblet cells[DIMENSIONS][DIMENSIONS];
    int nbPiecesPerPlayer[NB_PLAYERS][NB_SIZE];
};

player next_player(player current_player){

    if(current_player == PLAYER_2 || current_player == NO_PLAYER) current_player = PLAYER_1;
    else if(current_player == PLAYER_1) current_player = PLAYER_2;
    return current_player;
}
board new_game(){
    board b = malloc(sizeof(struct board_s));
    for(int i = 0; i<DIMENSIONS;i++){
        for(int j = 0; j < DIMENSIONS;j++){
            b->cells[i][j].nbPiecesSet = 0;

            for(int k = 0; k < NB_SIZE;k++){
                b->cells[i][j].placeHolder[k] = NO_PLAYER;
                b->cells[i][j].sizeHolder[k] = NONE;
            }

        }
    }
    for(int i = 0; i<=NB_SIZE;i++){
        b->nbPiecesPerPlayer[PLAYER_1-1][i] = NB_INITIAL_PIECES;
        b->nbPiecesPerPlayer[PLAYER_2-1][i] = NB_INITIAL_PIECES;
    }
    return b;
}


board copy_game(board original_game){
    board b = new_game(); //On créé un board valide pour effectuer la copie
    for(int i = 0; i<DIMENSIONS;i++) {
        for(int j = 0; j < DIMENSIONS;j++) {
            b->cells[i][j].nbPiecesSet = original_game->cells[i][j].nbPiecesSet;

            for(int k = 0; k < b->cells[i][j].nbPiecesSet;k++){
                b->cells[i][j].placeHolder[k] = original_game->cells[i][j].placeHolder[k];
                b->cells[i][j].sizeHolder[k] = original_game->cells[i][j].sizeHolder[k];
            }

        }
    }
    for(int i = 0; i<=NB_SIZE;i++){
        b->nbPiecesPerPlayer[PLAYER_1-1][i] = original_game->nbPiecesPerPlayer[PLAYER_1-1][i];
        b->nbPiecesPerPlayer[PLAYER_1-1][i] = original_game->nbPiecesPerPlayer[PLAYER_1-1][i];
    }
    return b;
}


void destroy_game(board game){
    free(game);
}


player get_place_holder(board game, int line, int column){
    player holder = NO_PLAYER;
    if(line <= DIMENSIONS-1 && line >=0 && column <= DIMENSIONS-1 && column >=0){
        int set = game->cells[column][line].nbPiecesSet;
        if(set) holder = game->cells[column][line].placeHolder[set-1];
    }
    return holder;
}


size get_piece_size(board game, int line, int column){
    size s = NONE;
    if(line <= DIMENSIONS-1 && line >=0 && column <= DIMENSIONS-1 && column >=0){
        int set = game->cells[column][line].nbPiecesSet;
        if(set) s = game->cells[column][line].sizeHolder[set-1];
    }
    return s;
}
player get_winner(board game){
    //Principe de carré magique
    int sumL = 0;
    int sumC = 0;
    player owner ;
    // Test lignes/colonnes
    for(int i = 0; i < DIMENSIONS; i++){
        for(int j = 0; j < DIMENSIONS; j++){
            owner = get_place_holder(game,i,j);
            if(owner == PLAYER_1){
                sumL++;
            }
            else if(owner == PLAYER_2){
                sumL--;
            }
            owner = get_place_holder(game,j,i);
            if(owner == PLAYER_1){
                sumC++;
            }
            else if(owner == PLAYER_2){
                sumC--;
            }
        }
        if(sumL == DIMENSIONS || sumC == DIMENSIONS){
            if(sumL != -DIMENSIONS && sumC != -DIMENSIONS) return PLAYER_1;
        }
        else if(sumL == -DIMENSIONS || sumC == -DIMENSIONS){
            if(sumL != DIMENSIONS && sumC != DIMENSIONS) return PLAYER_2;
        }
        sumL = 0;
        sumC = 0;
    }
    //
    //Test diagonale Haut gauche -> Bas droite et Bas gauche -> Haut droit
    for(int j = 0; j < DIMENSIONS; j++){
        owner = get_place_holder(game,DIMENSIONS-1-j,j);
        if(owner == PLAYER_1){
            sumL++;
        }
        else if(owner == PLAYER_2){
            sumL--;
        }
        owner = get_place_holder(game,j,j);
        if(owner == PLAYER_1){
            sumC++;
        }
        else if(owner == PLAYER_2){
            sumC--;
        }
    }
    if(sumL == DIMENSIONS || sumC == DIMENSIONS){
        if(sumL != -DIMENSIONS && sumC != -DIMENSIONS) return PLAYER_1;
    }
    else if(sumL == -DIMENSIONS || sumC == -DIMENSIONS){
        if(sumL != DIMENSIONS && sumC != DIMENSIONS) return PLAYER_2;
    }
    return NONE;
}


int get_nb_piece_in_house(board game, player checked_player, size piece_size){
    int nb =0;
    if(checked_player-1 >=0 && piece_size-1 < NB_SIZE && checked_player -1 < NB_PLAYERS && checked_player-1 >=0)
    {
        nb = game->nbPiecesPerPlayer[checked_player-1][piece_size-1];
    }
    return nb;
}


int place_piece(board game, player current_player, size piece_size, int line, int column){
    if(line > DIMENSIONS-1 || line <0 || column >DIMENSIONS-1 || column < 0 || piece_size < SMALL || piece_size > NB_SIZE || current_player > NB_PLAYERS || current_player <= NO_PLAYER) {return 3;}
    else if(piece_size <= get_piece_size(game,line,column)) {return 2;}
    else if(get_nb_piece_in_house(game,current_player,piece_size) <= 0) {return 1;}
    else{
        game->cells[column][line].nbPiecesSet ++;
        int set =game->cells[column][line].nbPiecesSet;
        game->cells[column][line].placeHolder[set-1] = current_player;
        game->cells[column][line].sizeHolder[set-1] = piece_size;
        game->nbPiecesPerPlayer[current_player-1][piece_size-1] --;
    }
    return 0;
}
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
int canMove(board game, int source_line, int source_column, int target_line, int target_column){
    if(source_line > DIMENSIONS-1 || source_line < 0 || source_column > DIMENSIONS-1 || source_column < 0 ||target_line > DIMENSIONS-1 || target_line <0 || target_column >DIMENSIONS-1 || target_column < 0 ) {return 3;}
    else if(get_piece_size(game,source_line,source_column) == NONE) {return 1;}
    else if(get_piece_size(game,source_line,source_column) <= get_piece_size(game,target_line,target_column)) {return 2;}
    else {return 0;}
}
int move_piece(board game, int source_line, int source_column, int target_line, int target_column){
    int ret = canMove(game, source_line, source_column, target_line, target_column);
    if(!ret){
        game->cells[target_column][target_line].nbPiecesSet ++;
        int set1 = game->cells[target_column][target_line].nbPiecesSet;
        int set2 = game->cells[source_column][source_line].nbPiecesSet;
        game->cells[target_column][target_line].placeHolder[set1-1] = game->cells[source_column][source_line].placeHolder[set2-1];
        game->cells[target_column][target_line].sizeHolder[set1-1] = game->cells[source_column][source_line].sizeHolder[set2-1];
        game->cells[source_column][source_line].nbPiecesSet --;
    }
    return ret;
}
