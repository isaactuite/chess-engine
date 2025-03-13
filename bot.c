#include <stdio.h>
#include "defs.h"
#include "game_logic.h"
#include <limits.h>




void generate_moves(){

    for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
            if (turn_color == 1){
                switch (bot_board[i][j]){
                    case 'k':
                        if (!king_logic(i, j, turn_color, 1)){
                            return 1;
                        }
                        break;
                    case 'p':
                        if (!pawn_logic(i, j, turn_color, 1)){
                            return 1;
                        }
                        break;
                    case 'n':
                        if (!knight_logic(i, j, turn_color, 1)){
                            return 1;
                        }
                        break;
                    case 'b':
                        if (!bishop_logic(i, j, turn_color, 1)){
                            return 1;
                        }
                        break;
                    case 'r':
                        if (!rook_logic(i, j,turn_color, 1)){
                            return 1;
                        }
                        break;
                    case 'q':
                        if (!queen_logic(i, j, turn_color, 1)){
                            return 1;
                        }
                        break;
                    default:
                        break;
                }
            } else if (turn_color == 0){
                switch (fake_board[i][j]){
                    case 'K':
                        if (!king_logic(i, j, turn_color, 1)){
                            return 1;
                        }
                        break;
                    case 'P':
                        if (!pawn_logic(i, j, turn_color, 1)){
                            return 1;
                        }
                        break;
                    case 'N':
                        if (!knight_logic(i, j, turn_color, 1)){
                            return 1;
                        }
                        break;
                    case 'B':
                        if (!bishop_logic(i, j, turn_color, 1)){
                            return 1;
                        }
                        break;
                    case 'R':
                        if (!rook_logic(i, j,turn_color, 1)){
                            return 1;
                        }
                        break;
                    case 'Q':
                        if (!queen_logic(i, j, turn_color, 1)){
                            return 1;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
}
int is_endgame(){
    int piece_count = 0;
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (bot_board[i][j]!='o'){
                piece_count++;
            }
        }
    }
    return piece_count<=14;
}
int piece_value(i,j){
    int eval=0;
    switch(bot_board[i][j]){
        case 'P':
        eval+=(100+pawn_pts[i][j]);
        break;
        case 'p':
        eval-=(100+pawn_pts[i][7-j]);
        break;
        case 'N':
        eval+=(300+knight_pts[i][j]);
        break;
        case 'B':
        eval+=(300+bishop_pts[i][j]);
        break;
        case 'R':
        eval+=(500+rook_pts[i][j]);
        break;
        case 'Q':
        eval+=(900+queen_pts[i][7-j]);
        break;
        case 'n':
        eval-=(300+knight_pts[i][7-j]);
        break;
        case 'b':
        eval-=(300+bishop_pts[i][7-j]);
        break;
        case 'r':
        eval-=(500+rook_pts[i][7-j]);
        break;
        case 'q':
        eval-=(900+queen_pts[i][7-j]);
        break;
        case 'K':
            if (is_endgame()){
                eval+= king_pts_late[i][j];
            } else{
                eval+= king_pts_mid[i][j];
            }
            break;
        case 'k':
            if (is_endgame()){
                eval+= king_pts_late[i][7-j];
            } else{
                eval-= king_pts_mid[i][7-j];
            }
            break;
        default:
        break;
    }

    
    return eval;
}
int evaluate(){
    int eval = 0;
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (check_piece_color(i, j, 2)  == 0){
                eval+=piece_value(i, j);
            }
        }
    }
    return eval;
}

int alphaBetaMax( int alpha, int beta, int depthleft){
    if (depthleft == 0){
        return evaluate();
    }

    int best_value = INT_MIN;

    for (int i = 0; i<8; i++){
        for (int j=0; j<8; j++){
            
        }
    }
}
















