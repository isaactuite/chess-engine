#include <stdio.h>
#include "defs.h"
#include "game_logic.h"
#include <limits.h>




void generate_moves(int color, int *castling){
    reset_moves();
    for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
            if (color == 1){
                switch (bot_board[i][j]){
                    case 'k':
                    bot_king_logic(i, j, color, 2, castling);
                        break;
                    case 'p':
                    bot_pawn_logic(i, j, color, 2, castling);
                        break;
                    case 'n':
                        knight_logic(i, j, color, 2, castling);
                        break;
                    case 'b':
                        bot_bishop_logic(i, j, color, 2, castling);
                        break;
                    case 'r':
                    bot_rook_logic(i, j,color, 2, castling);
                        break;
                    case 'q':
                    bot_queen_logic(i, j, color, 2, castling);
                        break;
                    default:
                        break;
                }
            } else if (color == 0){
                switch (bot_board[i][j]){
                    case 'K':
                    bot_king_logic(i, j, color, 2, castling);

                        break;
                    case 'P':
                    bot_pawn_logic(i, j, color, 2, castling);
                        break;
                    case 'N':
                    bot_knight_logic(i, j, color, 2, castling);
                        break;
                    case 'B':
                        bot_bishop_logic(i, j, color, 2, castling);
                        break;
                    case 'R':
                    bot_rook_logic(i, j,color, 2, castling);
                        break;
                    case 'Q':
                    bot_queen_logic(i, j, color, 2, castling);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}
void reset_moves(){
    memset(move_list, -1, sizeof(move_list));
    return;
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
            if (bot_check_piece_color(i, j)  == 0){
                eval+=piece_value(i, j);
            }
        }
    }
    return eval;
}

int alphaBetaMax( int alpha, int beta, int depthleft, int color, int castling){
    if (depthleft == 0){
        return evaluate();
    }

    int best_value = INT_MIN;

    generate_moves(color, castling);
    for (int i = 0; i<move_list[0].count; i++){
        int score = alphaBetaMin(alpha, beta, depthleft-1, !color, castling);
        if (score>best_value){
            best_value = score;
            if(score>alpha){
                alpha = score;
            }

        }
        if (score>=beta){
            return score;
        }

        
    }
    return best_value;
}
void make_move(Move s){
    bot_board[s.to_x][s.to_y] = bot_board[s.from_x][s.from_y];
    bot_board[s.from_x][s.from_y] = 'o';
}
int unmake_move(Move s){
    bot_board[s.from_x][s.from_y] = s.moved_piece;
    bot_board[s.to_x][s.to_y] = s.captured_piece;
}

void pushMove(Move m) {
    history.moves[history.top++] = m;
}
Move popMove() {
    return history.moves[--history.top];
}

int alphaBetaMin( int alpha, int beta, int depthleft, int color, int castling){
    if (depthleft == 0){
        return evaluate();
    }

    int best_value = INT_MAX;
    generate_moves(color, castling);
    for (int i = 0; i<move_list[0].count; i++){
        int score = alphaBetaMax(alpha, beta, depthleft-1, !color, castling);
        if (score<best_value){
            best_value = score;
            if(score<beta){
                beta = score;
            }

        }
        if (score<=alpha){
            return score;
        }

        
    }
    return best_value;
}


















int bot_check_piece_color(int x, int y){
    if (((x<0) || (y<0))||(x>7)||(y>7)){
        return -1;
    }
    
    switch(bot_board[x][y]){
        case 'p':
        case 'r':
        case 'n':
        case 'b':
        case 'q':
        case 'k':

            return 1;
        case 'P':
        case 'R':
        case 'N':
        case 'B':
        case 'Q':
        case 'K':
            return 0;

        case 'o':
            return 2;
        default:
            return -1;
    }
    
}
void add_to_move_list(int x, int y, int to_x, int to_y, int en_pass, int *castling){
    int index;
    
    index = move_list[0].count;
    move_list[index].from_x = x;
    move_list[index].from_y = y;
    move_list[index].to_x = to_x;
    move_list[index].to_y = to_y;
    move_list[index].captured_piece = bot_board[to_x][to_y];
    move_list[index].moved_piece = bot_board[x][y];
    if (en_pass){
        move_list[index].en_pass_x = to_x;
        move_list[index].en_pass_y = to_y;
    } else{
        move_list[index].en_pass_x = 0;
        move_list[index].en_pass_y = 0;
    }
    for (int i = 0; i<4; i++){
        move_list[index].castling[i] = castling[i];
    }

    move_list[0].count++;
    
}
//function to check if a target is valid
int is_valid_attack(int x, int y, int color){

    int piece_color = bot_check_piece_color(x, y);
    return piece_color == -1 ? 0 : ((piece_color != color) && (piece_color !=2));
}
//individual piece logic
int pawn_logic(int x, int y, int color, int mode, int *castling){
    int checkmate = 0;
    int counter = 0;
    if (color == 1){
        //black
        if (y==1){
            //checks rank to see if black pawn has not moved yet
            for(int j = 1; j<3; j++){
                
                if (bot_check_piece_color(x, y+j) != 2){
                    
                    //if there is something blocking the pawns file movement, break
                    break;
                //don't have to check for check here, because it's not a capture
                } else {
                    bot_board[x][y+j] = bot_board[x][y];
                    bot_board[x][y] = 'o';
                    if (search_from_king(color) == 0){
                        add_to_move_list(x, y, x, y+j, 0, castling);
 
                    }
                    bot_board[x][y+j] = board[x][y+j];
                    bot_board[x][y] = board[x][y];
                    
                }
            
            }
        }
        else if (y!=1){
            if (bot_check_piece_color(x, y+1) == 2){
                bot_board[x][y+1] = bot_board[x][y];
                bot_board[x][y] = 'o';
                //If it's not the pawns first move, check the square directly in front of it for empty square
                if (search_from_king(color) == 0){
                    add_to_move_list(x, y, x, y+1, 0, castling);
                    counter++;
                }
                bot_board[x][y+1] = board[x][y+1];
                bot_board[x][y] = board[x][y];
            }
        }
        

        //checks for pawn captures
        if ((bot_check_piece_color(x-1, y+1) == 0) && ((x-1>=0)&&(y+1<8))){
            bot_board[x-1][y+1] = bot_board[x][y];
            bot_board[x][y] = 'o';
            if (search_from_king(color) == 0){
                add_to_move_list(x, y, x-1, y+1, 0, castling);
                counter++;
            }
            bot_board[x-1][y+1] = board[x-1][y+1];
            bot_board[x][y] = board[x][y];
        }
        if ((bot_check_piece_color(x+1, y+1) == 0) && ((x+1<8)&&(y+1<8))){
            bot_board[x+1][y+1] = bot_board[x][y];
            bot_board[x][y] = 'o';
            if (search_from_king(color) == 0){
                add_to_move_list(x, y, x+1, y+1, 0, castling);
                counter++;
            }
            bot_board[x+1][y+1] = board[x+1][y+1];
            bot_board[x][y] = board[x][y];
            
        }
        
        if ((bot_en_passant_x == x+1)  && (bot_en_passant_y == y)){
            bot_board[x+1][y+1] = bot_board[x][y];
            bot_board[x+1][y] = 'o';
            bot_board[x][y] = 'o';
            if (search_from_king(color) == 0){
                add_to_move_list(x, y, x+1, y+1,1, castling);
                counter++;
            }
            bot_board[x+1][y+1] = board[x+1][y+1];
            bot_board[x+1][y] = board[x+1][y];
            bot_board[x][y] = board[x][y];
        }
        if ((bot_en_passant_x == x-1)  && (bot_en_passant_y == y)){
            bot_board[x-1][y+1] = bot_board[x][y];
            bot_board[x-1][y] = 'o';
            bot_board[x][y] = 'o';
            if (search_from_king(color) == 0){
                add_to_move_list(x, y, x-1, y+1, 1, castling);
                counter++;
            }
            bot_board[x-1][y+1] = board[x-1][y+1];
            bot_board[x-1][y] = board[x-1][y];
            bot_board[x][y] = board[x][y];
        }
        
    
    }
    else if (color == 0){
        //white
        if (y==6){
            //checks rank to see if white pawn has not moved yet
            for(int j = -1; j>-3; j--){

                if (bot_check_piece_color(x, y+j) != 2){
                    
                    //if there is something blocking the pawns file movement, break
                    break;
                //don't have to check for check here, because it's not a capture
                } else {
                    bot_board[x][y+j] = bot_board[x][y];
                    bot_board[x][y] = 'o';
                    if (search_from_king(color) == 0){
                        add_to_move_list(x, y, x, y+j, 0, castling);
                        counter++;
                    }
                    bot_board[x][y+j] = board[x][y+j];
                    bot_board[x][y] = board[x][y];
                }
            
            }
        }else if (y!=6){
            if (bot_check_piece_color(x, y-1) == 2){
                bot_board[x][y-1] = bot_board[x][y];
                bot_board[x][y] = 'o';
                //If it's not the pawns first move, check the square directly in front of it for empty square
                if (search_from_king(color) == 0){
                    add_to_move_list(x, y, x, y-1, 0, castling);
                    counter++;
                }
                bot_board[x][y-1] = board[x][y-1];
                bot_board[x][y] = board[x][y];
            }
        }

        //checks for pawn captures
        if ((bot_check_piece_color(x-1, y-1) == 1) && ((x-1>=0)&&(y-1>=0))){
            bot_board[x-1][y-1] = bot_board[x][y];
            bot_board[x][y] = 'o';
            if (search_from_king(color) == 0){
                add_to_move_list(x, y, x-1, y-1, 0, castling);
                counter++;
            }
            bot_board[x-1][y-1] = board[x-1][y-1];
            bot_board[x][y] = board[x][y];
        }
        if ((bot_check_piece_color(x+1, y-1) == 1) && ((x+1<8)&&(y-1>=0))){
            bot_board[x+1][y-1] = bot_board[x][y];
            bot_board[x][y] = 'o';
            if (search_from_king(color) == 0){
                add_to_move_list(x, y, x+1, y-1, 0, castling);
                counter++;
            }
            bot_board[x+1][y-1] = board[x+1][y-1];
            bot_board[x][y] = board[x][y];
            
        }
        if ((bot_en_passant_x == x+1)  && (bot_en_passant_y == y)){
            bot_board[x+1][y-1] = bot_board[x][y];
            bot_board[x+1][y] = 'o';
            bot_board[x][y] = 'o';
            if (search_from_king(color) == 0){
                add_to_move_list(x, y, x+1, y-1, 1, castling);
                counter++;
            }
            bot_board[x+1][y-1] = board[x+1][y-1];
            bot_board[x+1][y] = board[x+1][y];
            bot_board[x][y] = board[x][y];
        }
        if ((bot_en_passant_x == x-1)  && (bot_en_passant_y == y)){
            bot_board[x-1][y-1] = bot_board[x][y];
            bot_board[x-1][y] = 'o';
            bot_board[x][y] = 'o';
            if (search_from_king(color) == 0){
                add_to_move_list(x, y, x-1, y-1, 1, castling);
                counter++;
            }
            bot_board[x-1][y-1] = board[x-1][y-1];
            bot_board[x-1][y] = board[x-1][y];
            bot_board[x][y] = board[x][y];
        }
    }
    if (mode == 1){
        return checkmate;
    } else if (mode == 2){
        
        return 0;
    }

}
int knight_logic(int x, int y, int color, int mode, int *castling){
    int checkmate = 0;
   
    int counter=1;

    for (int i = -2; i<3; i++){
        
        if (i==0){
            continue;
        }

        for (int j=-2; j<3; j++){
            
            if (j==0){
                continue;
            }
            if (abs(j)==abs(i)){
                continue;
            }
            
            if ((x+i>=0 && x+i<=7)&&(y+j>=0 && y+j<=7)){
                
                if (color == bot_check_piece_color(x+i, y+j)){
                    continue;
                }
                bot_board[x+i][y+j] = bot_board[x][y];
                bot_board[x][y] = 'o';
                if (search_from_king(color) ==0){
                    if (is_valid_attack(x + i, y + j, color)){
                        add_to_move_list(x, y, x+i, y+j, 0, castling);
                        counter++;
                    } else if (bot_check_piece_color(x+i, y+j)==2){
                        add_to_move_list(x, y, x+i, y+j, 0, castling);
                        counter++;
                    }
                }
                bot_board[x+i][y+j] = board[x+i][y+j];
                bot_board[x][y] = board[x][y];
                

                
            }
        }
    }

    

    if (mode == 1){
        return checkmate;
    }else if (mode == 2){
        add_to_move_list(x, y, x, y, 0, castling);
        return 0;
    }
}
int rook_logic(int x, int y, int color, int mode,int *castling){
    int counter = 1;
    int checkmate = 0;
    
    
    //left
    for (int i=-1; x+i>-1; i--){
    
        if(bot_check_piece_color(x+i, y) == color){
            break;
        }
        

        bot_board[x+i][y] = bot_board[x][y];
        bot_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i,y, color)){
            
                add_to_move_list(x, y, x+i, y, 0, castling);
                counter+=1;
                bot_board[x+i][y] = board[x+i][y];
                bot_board[x][y] = board[x][y];
                break;
                
            }else if (bot_check_piece_color(x+i, y) == 2){
                
                add_to_move_list(x, y, x+i, y, 0, castling);
                counter+=1;
            
            }
        }
        bot_board[x+i][y] = board[x+i][y];
        bot_board[x][y] = board[x][y];
    }
    //up
    for (int j=-1; y+j>-1;j--){
        if(bot_check_piece_color(x, y+j) == color){
            break;
        }
        

        bot_board[x][y+j] = bot_board[x][y];
        bot_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x, y+j, color)){
            
                add_to_move_list(x, y, x, y+j, 0, castling);
                counter+=1;
                bot_board[x][y+j] = board[x][y+j];
                bot_board[x][y] = board[x][y];
                break;
                
            }else if (bot_check_piece_color(x, y+j) == 2){
                
                add_to_move_list(x, y, x, y+j, 0, castling);
                counter+=1;
            
            }
        }
        bot_board[x][y+j] = board[x][y+j];
        bot_board[x][y] = board[x][y];
    }
    //right
    for (int i=1; x+i<8; i++){
        if(bot_check_piece_color(x+i, y) == color){
            break;
        }
        

        bot_board[x+i][y] = bot_board[x][y];
        bot_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i, y, color)){
            
                add_to_move_list(x, y, x+i, y, 0, castling);
                counter+=1;
                bot_board[x+i][y] = board[x+i][y];
                bot_board[x][y] = board[x][y];
                break;
                
            }else if (bot_check_piece_color(x+i, y) == 2){
                
                add_to_move_list(x, y, x+i, y, 0, castling);
                counter+=1;
            
            }
        }
        bot_board[x+i][y] = board[x+i][y];
        bot_board[x][y] = board[x][y];
    }   
    //down
    for (int j=1; y+j<8;j++){
        if(bot_check_piece_color(x, y+j) == color){
            break;
        }
        

        bot_board[x][y+j] = bot_board[x][y];
        bot_board[x][y] = 'o';

        if (search_from_king(color)==0){
            if (is_valid_attack(x, y+j, color)){
            
                add_to_move_list(x, y, x, y+j, 0, castling);
                counter+=1;
                bot_board[x][y+j] = board[x][y+j];
                bot_board[x][y] = board[x][y];
                break;
                
            }else if (bot_check_piece_color(x, y+j) == 2){
                
                add_to_move_list(x, y, x, y+j, 0, castling);
                counter+=1;
            
            }
        }
        bot_board[x][y+j] = board[x][y+j];
        bot_board[x][y] = board[x][y];
    }

    if (mode == 1){
        return checkmate;
    }else if (mode == 2){
        add_to_move_list(x, y, x, y, 0, castling);
        return 0;
    }
}
int bot_bishop_logic(int x, int y, int color, int mode, int *castling){
    int checkmate = 0;
    
    int counter = 1;
    //left up
    for (int i=-1; x+i>-1 && y+i>-1; i--){
        
        if(bot_check_piece_color(x+i, y+i) == color){
            break;
        }
        
        bot_board[x+i][y+i] = bot_board[x][y];
        bot_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i, y+i, color)){
            
                add_to_move_list(x, y, x+i, y+i, 0, castling);
                counter+=1;
                bot_board[x+i][y+i] = board[x+i][y+i];
                bot_board[x][y] = board[x][y];
                break;
                
            }else if (bot_check_piece_color(x+i, y+i) == 2){
                
                add_to_move_list(x, y, x+i, y+i, 0, castling);
                counter+=1;
            
            }
        }
        bot_board[x+i][y+i] = board[x+i][y+i];
        bot_board[x][y] = board[x][y];
        
    }
    //right up
    for (int i=1; x+i<8 && y-i>-1; i++){
        if(bot_check_piece_color(x+i, y-i) == color){
            break;
        }
        

        bot_board[x+i][y-i] = bot_board[x][y];
        bot_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i, y-i, color)){
            
                add_to_move_list(x, y, x+i, y-i, 0, castling);
                counter+=1;
                bot_board[x+i][y-i] = board[x+i][y-i];
                bot_board[x][y] = board[x][y];
                break;
                
            }else if (bot_check_piece_color(x+i, y-i) == 2){
                
                add_to_move_list(x, y, x+i, y-i, 0, castling);
                counter+=1;
            
            }
        }
        bot_board[x+i][y-i] = board[x+i][y-i];
        bot_board[x][y] = board[x][y];
    }
    //right down
    for (int i=1; x+i<8 && y+i<8; i++){
        if(bot_check_piece_color(x+i, y+i) == color){
            break;
        }
        

        bot_board[x+i][y+i] = bot_board[x][y];
        bot_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i, y+i, color)){
            
                add_to_move_list(x, y, x+i, y+i, 0, castling);
                counter+=1;
                bot_board[x+i][y+i] = board[x+i][y+i];
                bot_board[x][y] = board[x][y];
                break;
                
            }else if (bot_check_piece_color(x+i, y+i) == 2){
                
                add_to_move_list(x, y, x+i, y+i, 0, castling);
                counter+=1;
            
            }
        }
        bot_board[x+i][y+i] = board[x+i][y+i];
        bot_board[x][y] = board[x][y];
    }
    //left down
    for (int i=1; x-i>-1 && y+i<8; i++){
        if(bot_check_piece_color(x-i, y+i) == color){
            break;
        }
        

        bot_board[x-i][y+i] = bot_board[x][y];
        bot_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x-i, y+i, color)){
            
                add_to_move_list(x, y, x-i, y+i, 0, castling);
                counter+=1;
                bot_board[x-i][y+i]  = board[x-i][y+i];
                bot_board[x][y] = board[x][y];
                break;
                
            }else if (bot_check_piece_color(x-i, y+i) == 2){
                
                add_to_move_list(x, y, x-i, y+i, 0, castling);
                counter+=1;
            
            }
        }
        bot_board[x-i][y+i]  = board[x-i][y+i];
        bot_board[x][y] = board[x][y];
    }
    if (mode == 1){
        return checkmate;
    }
}
int bot_queen_logic(int x, int y, int color, int mode, int *castling){
    int checkmate = 0;
   
    int counter = 1;

    for (int i=-1; x+i>-1 && y+i>-1; i--){
        
        if(bot_check_piece_color(x+i, y+i) == color){
            break;
        }
        
        bot_board[x+i][y+i] = bot_board[x][y];
        bot_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i, y+i, color)){
            
                add_to_move_list(x, y, x+i, y+i, 0, castling);
                counter+=1;
                bot_board[x+i][y+i] = board[x+i][y+i];
                bot_board[x][y] = board[x][y];
                break;
                
            }else if (bot_check_piece_color(x+i, y+i) == 2){
                
                add_to_move_list(x, y, x+i, y+i, 0, castling);
                counter+=1;
            
            }
        }
        bot_board[x+i][y+i] = board[x+i][y+i];
        bot_board[x][y] = board[x][y];
        
    }
    //right up
    for (int i=1; x+i<8 && y-i>-1; i++){
        if(bot_check_piece_color(x+i, y-i) == color){
            break;
        }
        

        bot_board[x+i][y-i] = bot_board[x][y];
        bot_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i, y-i, color)){
            
                add_to_move_list(x, y, x+i, y-i, 0, castling);
                counter+=1;
                bot_board[x+i][y-i] = board[x+i][y-i];
                bot_board[x][y] = board[x][y];
                break;
                
            }else if (bot_check_piece_color(x+i, y-i) == 2){
                
                add_to_move_list(x, y, x+i, y-i, 0, castling);
                counter+=1;
            
            }
        }
        bot_board[x+i][y-i] = board[x+i][y-i];
        bot_board[x][y] = board[x][y];
    }
    //right down
    for (int i=1; x+i<8 && y+i<8; i++){
        if(bot_check_piece_color(x+i, y+i) == color){
            break;
        }
        

        bot_board[x+i][y+i] = bot_board[x][y];
        bot_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i, y+i, color)){
            
                add_to_move_list(x, y, x+i, y+i, 0, castling);
                counter+=1;
                bot_board[x+i][y+i] = board[x+i][y+i];
                bot_board[x][y] = board[x][y];
                break;
                
            }else if (bot_check_piece_color(x+i, y+i) == 2){
                
                add_to_move_list(x, y, x+i, y+i, 0, castling);
                counter+=1;
            
            }
        }
        bot_board[x+i][y+i] = board[x+i][y+i];
        bot_board[x][y] = board[x][y];
    }
    //left down
    for (int i=1; x-i>-1 && y+i<8; i++){
        if(bot_check_piece_color(x-i, y+i) == color){
            break;
        }
        

        bot_board[x-i][y+i] = bot_board[x][y];
        bot_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x-i, y+i, color)){
            
                add_to_move_list(x, y, x-i, y+i, 0, castling);
                counter+=1;
                bot_board[x-i][y+i]  = board[x-i][y+i];
                bot_board[x][y] = board[x][y];
                break;
                
            }else if (bot_check_piece_color(x-i, y+i) == 2){
                
                add_to_move_list(x, y, x-i, y+i, 0, castling);
                counter+=1;
            
            }
        }
        bot_board[x-i][y+i]  = board[x-i][y+i];
        bot_board[x][y] = board[x][y];
    }
    //left
    for (int i=-1; x+i>-1; i--){
        
        if(bot_check_piece_color(x+i, y) == color){
            break;
        } 
        

        bot_board[x+i][y] = bot_board[x][y];
        bot_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i,y, color)){
            
                add_to_move_list(x, y, x+i, y, 0, castling);
                counter+=1;
                bot_board[x+i][y] = board[x+i][y];
                bot_board[x][y] = board[x][y];
                break;
                
            }else if (bot_check_piece_color(x+i, y) == 2){
                
                add_to_move_list(x, y, x+i, y, 0, castling);
                counter+=1;
            
            }
        }
        bot_board[x+i][y] = board[x+i][y];
        bot_board[x][y] = board[x][y];
    }
    //up
    for (int j=-1; y+j>-1;j--){
        if(bot_check_piece_color(x, y+j) == color){
            break;
        }
        

        bot_board[x][y+j] = bot_board[x][y];
        bot_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x, y+j, color)){
            
                add_to_move_list(x, y, x, y+j, 0, castling);
                counter+=1;
                bot_board[x][y+j] = board[x][y+j];
                bot_board[x][y] = board[x][y];
                break;
                
            }else if (bot_check_piece_color(x, y+j) == 2){
                
                add_to_move_list(x, y, x, y+j, 0, castling);
                counter+=1;
            
            }
        }
        bot_board[x][y+j] = board[x][y+j];
        bot_board[x][y] = board[x][y];
    }
    //right
    for (int i=1; x+i<8; i++){
        if(bot_check_piece_color(x+i, y) == color){
            break;
        }
        

        bot_board[x+i][y] = bot_board[x][y];
        bot_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i, y, color)){
            
                add_to_move_list(x, y, x+i, y, 0, castling);
                counter+=1;
                bot_board[x+i][y] = board[x+i][y];
                bot_board[x][y] = board[x][y];
                break;
                
            }else if (bot_check_piece_color(x+i, y) == 2){
                
                add_to_move_list(x, y, x+i, y, 0, castling);
                counter+=1;
            
            }
        }
        bot_board[x+i][y] = board[x+i][y];
        bot_board[x][y] = board[x][y];
    }   
    //down
    for (int j=1; y+j<8;j++){
        if(bot_check_piece_color(x, y+j) == color){
            break;
        }
        

        bot_board[x][y+j] = bot_board[x][y];
        bot_board[x][y] = 'o';

        if (search_from_king(color)==0){
            if (is_valid_attack(x, y+j, color)){
            
                add_to_move_list(x, y, x, y+j, 0, castling);
                counter+=1;
                bot_board[x][y+j] = board[x][y+j];
                bot_board[x][y] = board[x][y];
                break;
                
            }else if (bot_check_piece_color(x, y+j) == 2){
                
                add_to_move_list(x, y, x, y+j, 0, castling);
                counter+=1;
            
            }
        }
        bot_board[x][y+j] = board[x][y+j];
        bot_board[x][y] = board[x][y];
    }
    if (mode == 1){
        return checkmate;
    }
}
int king_logic(int x, int y, int color, int mode, int *castling){
    int checkmate = 0;
 
    int bot_temp_king_x;
    int bot_temp_king_y;
    if (color == 0){
        bot_temp_king_x = bot_wking_x;
        bot_temp_king_y = bot_wking_y;
    } else if (color == 1){
        bot_temp_king_x = bot_bking_x;
        bot_temp_king_y = bot_bking_y;
    }
    

    
    int counter = 1;
    
    counter = check_for_castle(color, counter, castling);

    for(int i = -1;i<2; i++){
        if (x+i>=8 ||x+i<0){
            continue;
        }
        for (int j =-1; j<2;j++){
            if (y+j>=8 ||y+j<0){
                continue;
            }
            
            if((j == i) &&(j == 0)){
                continue;
            
            }
            if (bot_check_piece_color(x+i, y+j)==color){
                continue;
            }
            if (color == 0){
                bot_wking_x +=i;
                bot_wking_y+=j;
                bot_board[bot_wking_x][bot_wking_y] = 'K';
                bot_board[bot_temp_king_x][bot_temp_king_y] = 'o';
            } else if (color == 1){
                bot_bking_x +=i;
                bot_bking_y+=j;
                bot_board[bot_bking_x][bot_bking_y] = 'k';
                bot_board[bot_temp_king_x][bot_temp_king_y] = 'o';
            }
            if (search_from_king(color) == 0){
                if (is_valid_attack(x + i, y + j, color)){
                    add_to_move_list(x, y, x+i, y+j, 0, castling);
                    counter++;
                } else if (bot_check_piece_color(x+i, y+j)==2){
                    add_to_move_list(x, y, x+i, y+j, 0, castling);
                    counter++;
                }
            }
            if (color == 0){
                bot_board[bot_wking_x][bot_wking_y] = board[bot_wking_x][bot_wking_y];
                bot_board[bot_temp_king_x][bot_temp_king_y] = board[bot_temp_king_x][bot_temp_king_y];
                bot_wking_x = bot_temp_king_x;
                bot_wking_y=bot_temp_king_y;
                
            } else if (color == 1){
                bot_board[bot_bking_x][bot_bking_y] = board[bot_bking_x][bot_bking_y];
                bot_board[bot_temp_king_x][bot_temp_king_y] = board[bot_temp_king_x][bot_temp_king_y];
                bot_bking_x =bot_temp_king_x;
                bot_bking_y=bot_temp_king_y;
            }
        }
    }
    if (mode == 1){
        return checkmate;
    }
}
int search_from_king(int color){
    int decision;
    for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
        }
    } 
    for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
        }
    } 
    if (color==0){
        //left up
        for (int i=-1; bot_wking_x+i>-1 && bot_wking_y+i>-1; i--){
            decision = king_search_help(bot_wking_x+i, bot_wking_y+i, color, 'b', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //right up
        for (int i=1; bot_wking_x+i<8 && bot_wking_y-i>-1; i++){
            decision = king_search_help(bot_wking_x+i, bot_wking_y-i, color, 'b', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //right down
        for (int i=1; bot_wking_x+i<8 && bot_wking_y+i<8; i++){
            decision = king_search_help(bot_wking_x+i, bot_wking_y+i, color, 'b', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //left down
        for (int i=1; bot_wking_x-i>-1 && bot_wking_y+i<8; i++){
            decision = king_search_help(bot_wking_x-i, bot_wking_y+i, color, 'b', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //left
        for (int i=-1; bot_wking_x+i>-1; i--){
            decision = king_search_help(bot_wking_x+i, bot_wking_y, color, 'r', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }
        //up
        for (int j=-1; bot_wking_y+j>-1;j--){
            decision = king_search_help(bot_wking_x, bot_wking_y+j, color, 'r', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }
        //right
        for (int i=1; bot_wking_x+i<8; i++){
            decision = king_search_help(bot_wking_x+i, bot_wking_y, color, 'r', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //down
        for (int j=1; bot_wking_y+j<8;j++){
            decision = king_search_help(bot_wking_x, bot_wking_y+j, color, 'r', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }
        //Knight
        for (int i = -2; i<3; i++){
            
            if (i==0){
                continue;
            }

            for (int j=-2; j<3; j++){
                
                if (j==0){
                    continue;
                }
                if (abs(j)==abs(i)){
                    continue;
                }
                
                if ((bot_wking_x+i>=0 && bot_wking_x+i<=7)&&(bot_wking_y+j>=0 && bot_wking_y+j<=7)){
                    decision = king_search_help(bot_wking_x+i, bot_wking_y+j, color, 'n', 'n');
                    if (decision == 0 || decision == 1){
                        continue;
                    } else if (decision == 3){
                        return 1;
                    }
                }
                
            }
        }
        if (bot_board[bot_wking_x-1][bot_wking_y-1] == 'p'){
            return 1;
        } 
        if (bot_board[bot_wking_x+1][bot_wking_y-1] == 'p'){
            return 1;
        }
         //king
         for(int i = -1;i<2; i++){
            if (bot_wking_x+i>=8 ||bot_wking_x+i<0){
                continue;
            }
            for (int j =-1; j<2;j++){
                if (bot_wking_x+j>=8 ||bot_wking_x+j<0){
                    continue;
                }
                        
                if((j == i) &&(j == 0)){
                    continue;
                
                }
                if (bot_check_piece_color(bot_wking_x+i, bot_wking_y+j)==color){
                    continue;
                }
                if ((bot_wking_x+i>=0 && bot_wking_x+i<=7)&&(bot_wking_y+j>=0 && bot_wking_y+j<=7)){
                    decision = king_search_help(bot_wking_x+i, bot_wking_y+j, color, 'k', 'k');
                    if (decision == 0 || decision == 1){
                        continue;
                    } else if (decision == 3){
                        return 1;
                    }
                }
            }
        }
    }
    
    
    
    
    else if (color==1){
        //left up
        for (int i=-1; bot_bking_x+i>-1 && bot_bking_y+i>-1; i--){
            decision = king_search_help(bot_bking_x+i, bot_bking_y+i, color, 'B', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //right up
        for (int i=1; bot_bking_x+i<8 && bot_bking_y-i>-1; i++){
            decision = king_search_help(bot_bking_x+i, bot_bking_y-i, color, 'B', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //right down
        for (int i=1; bot_bking_x+i<8 && bot_bking_y+i<8; i++){
            decision = king_search_help(bot_bking_x+i, bot_bking_y+i, color, 'B', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //left down
        for (int i=1; bot_bking_x-i>-1 && bot_bking_y+i<8; i++){
            decision = king_search_help(bot_bking_x-i, bot_bking_y+i, color, 'B', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //left
        for (int i=-1; bot_bking_x+i>-1; i--){
            decision = king_search_help(bot_bking_x+i, bot_bking_y, color, 'R', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }
        //up
        for (int j=-1; bot_bking_y+j>-1;j--){
            decision = king_search_help(bot_bking_x, bot_bking_y+j, color, 'R', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }
        //right
        for (int i=1; bot_bking_x+i<8; i++){
            decision = king_search_help(bot_bking_x+i, bot_bking_y, color, 'R', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //down
        for (int j=1;bot_bking_y+j<8;j++){
            decision = king_search_help(bot_bking_x, bot_bking_y+j, color, 'R', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }
        //Knight
        for (int i = -2; i<3; i++){
            
            if (i==0){
                continue;
            }

            for (int j=-2; j<3; j++){
                
                if (j==0){
                    continue;
                }
                if (abs(j)==abs(i)){
                    continue;
                }
                
                if ((bot_bking_x+i>=0 && bot_bking_x+i<=7)&&(bot_bking_y+j>=0 && bot_bking_y+j<=7)){
                    decision = king_search_help(bot_bking_x+i, bot_bking_y+j, color, 'N', 'N');
                    if (decision == 0 || decision == 1){
                        continue;
                    } else if (decision == 3){
                        return 1;
                    }
                }
                
            }
        }
        //pawns
        if (bot_board[bot_bking_x-1][bot_bking_y+1] == 'P'){
            return 1;
        } 
        if (bot_board[bot_bking_x+1][bot_bking_y+1] == 'P'){
            return 1;
        }
        //king
        for(int i = -1;i<2; i++){
            if (bot_bking_x+i>=8 ||bot_bking_x+i<0){
                continue;
            }
            for (int j =-1; j<2;j++){
                if (bot_bking_x+j>=8 ||bot_bking_x+j<0){
                    continue;
                }
                        
                if((j == i) &&(j == 0)){
                    continue;
                
                }
                if (bot_check_piece_color(bot_bking_x+i, bot_bking_y+j)==color){
                    continue;
                }
                if ((bot_bking_x+i>=0 && bot_bking_x+i<=7)&&(bot_bking_y+j>=0 && bot_bking_y+j<=7)){
                    decision = king_search_help(bot_bking_x+i, bot_bking_y+j, color, 'K', 'K');
                    if (decision == 0 || decision == 1){
                        continue;
                    } else if (decision == 3){
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
int king_search_help(int x, int y, int color, char piece_to_check1, char piece_to_check2){
    piece_logic_for_moving = 0;
    
    if (color==0){

        if (bot_check_piece_color(x, y) == 0){
            piece_logic_for_moving = 1;
            
            return 0;
        }
        if (bot_check_piece_color(x, y) == 2){
            piece_logic_for_moving = 1;
            
            return 2;
        }
        if (bot_check_piece_color(x, y) == 1){
            
            if (bot_board[x][y] == piece_to_check1 || bot_board[x][y] == piece_to_check2){
                
                piece_logic_for_moving = 1;
                return 3;
            }
            piece_logic_for_moving = 1;
            
            return 1;
        }
        
    }

    else if (color==1){
        if (bot_check_piece_color(x, y) == 1){
            piece_logic_for_moving = 1;
            
            return 0;
        }
        if (bot_check_piece_color(x, y) == 2){
            piece_logic_for_moving = 1;
           
            return 2;
        }
        if (bot_check_piece_color(x, y) == 0){
            
            if (bot_board[x][y] == piece_to_check1 || bot_board[x][y] == piece_to_check2){
                piece_logic_for_moving = 1;
                
                return 3;
            }
            piece_logic_for_moving = 1;
           
            return 1;
        }
    }
    piece_logic_for_moving = 1;

    return -1;
}
int check_for_castle(int color, int counter, int *castling){
    if (color == 0){
        int temp_bot_wking_x = bot_wking_x;
        

        if (sim_castling_rights_QS){
            if (bot_board[3][7] == 'o' && bot_board[2][7] == 'o' && bot_board[1][7] == 'o' &&bot_board[0][7] == 'R'){
                if (!search_from_king(color)){
                    bot_board[3][7] = 'K';
                    bot_board[4][7] = 'o';
                    bot_wking_x = 3;
                    
                    if (!search_from_king(color)){
                        bot_board[2][7] = 'K';
                        bot_board[3][7] = 'o';
                        bot_wking_x = 2;
                        if (!search_from_king(color)){
                            add_to_move_list(4, 7, 2, 7, 0, castling);
                            counter++;
                        }
                    }
                }
                bot_board[3][7] = 'o';
                bot_board[2][7] = 'o';
                bot_board[4][7] = 'K';
            }
        }
        if (sim_castling_rights_KS){
            if (bot_board[5][7] == 'o' && bot_board[6][7] == 'o' && bot_board[7][7] == 'R'){
                if (!search_from_king(color)){
                    bot_board[5][7] = 'K';
                    bot_board[4][7] = 'o';
                    bot_wking_x = 5;
                    if (!search_from_king(color)){
                        bot_board[6][7] = 'K';
                        bot_board[5][7] = 'o';
                        bot_wking_x = 6;
                        if (!search_from_king(color)){
                            add_to_move_list(4, 7, 6, 7, 0, castling);
                            counter++;
                        }
                    }
                }
                bot_board[3][7] = 'o';
                bot_board[2][7] = 'o';
                bot_board[4][7] = 'K';
            }
        }
        bot_wking_x = temp_bot_wking_x;
    } else if (color == 1){

        int temp_bot_bking_x = bot_bking_x;

        if (sim_castling_rights_qs){
            if (bot_board[3][0] == 'o' && bot_board[2][0] == 'o' && bot_board[1][0] == 'o' &&bot_board[0][0] == 'r'){
                if (!search_from_king(color)){
                    bot_board[3][0] = 'K';
                    bot_board[4][0] = 'o';
                    bot_bking_x = 3;
                    if (!search_from_king(color)){
                        bot_board[2][0] = 'K';
                        bot_board[3][0] = 'o';
                        bot_bking_x = 2;
                        if (!search_from_king(color)){
                            add_to_move_list(4, 0, 2, 0, 0, castling);
                            counter++;
                        }
                    }
                }
                bot_board[3][0] = 'o';
                bot_board[2][0] = 'o';
                bot_board[4][0] = 'K';
            }
        }
        if (sim_castling_rights_ks){
            if (bot_board[5][0] == 'o' && bot_board[6][0] == 'o' && bot_board[7][0] == 'r'){
                if (!search_from_king(color)){
                    bot_board[5][0] = 'K';
                    bot_board[4][0] = 'o';
                    bot_bking_x = 5;
                    if (!search_from_king(color)){
                        bot_board[6][0] = 'K';
                        bot_board[5][0] = 'o';
                        bot_bking_x = 6;
                        if (!search_from_king(color)){
                            add_to_move_list(4, 0, 6, 0, 0, castling);
                            counter++;
                        }
                    }
                }
                bot_board[3][0] = 'o';
                bot_board[2][0] = 'o';
                bot_board[4][0] = 'K';
            }
        }
        bot_bking_x = temp_bot_bking_x;
    }
    return counter;
    
}
int is_checkmate(castling){
    
    int checkmate = 1;
    for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
            if (turn_color == 1){
                switch (bot_board[i][j]){
                    case 'k':
                        if (!bot_king_logic(i, j, turn_color, 1, castling)){
                            return 1;
                        }
                        break;
                    case 'p':
                        if (!bot_pawn_logic(i, j, turn_color, 1, castling)){
                            return 1;
                        }
                        break;
                    case 'n':
                        if (!bot_knight_logic(i, j, turn_color, 1, castling)){
                            return 1;
                        }
                        break;
                    case 'b':
                        if (!bot_bishop_logic(i, j, turn_color, 1, castling)){
                            return 1;
                        }
                        break;
                    case 'r':
                        if (!bot_rook_logic(i, j,turn_color, 1, castling)){
                            return 1;
                        }
                        break;
                    case 'q':
                        if (!bot_queen_logic(i, j, turn_color, 1, castling)){
                            return 1;
                        }
                        break;
                    default:
                        break;
                }
            } else if (turn_color == 0){
                switch (bot_board[i][j]){
                    case 'K':
                        if (!bot_king_logic(i, j, turn_color, 1, castling)){
                            return 1;
                        }
                        break;
                    case 'P':
                        if (!bot_pawn_logic(i, j, turn_color, 1, castling)){
                            return 1;
                        }
                        break;
                    case 'N':
                        if (!bot_knight_logic(i, j, turn_color, 1, castling)){
                            return 1;
                        }
                        break;
                    case 'B':
                        if (!bot_bishop_logic(i, j, turn_color, 1, castling)){
                            return 1;
                        }
                        break;
                    case 'R':
                        if (!bot_rook_logic(i, j,turn_color, 1, castling)){
                            return 1;
                        }
                        break;
                    case 'Q':
                        if (!bot_queen_logic(i, j, turn_color, 1, castling)){
                            return 1;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return 0;
}