#include <stdio.h>
#include "defs.h"
#include "bot.h"
#include <math.h>
#include <stdlib.h>



void generate_moves(int color, int *castling, MoveList* move_list){
    for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
            if (color == 1){
                switch (bot_board[i][j]){
                    case 'k':
                    bot_king_logic(i, j, color, 2, castling, move_list);
                        break;
                    case 'p':
                    bot_pawn_logic(i, j, color, 2, castling, move_list);
                        break;
                    case 'n':
                    bot_knight_logic(i, j, color, 2, castling, move_list);
                        break;
                    case 'b':
                    bot_bishop_logic(i, j, color, 2, castling, move_list);
                        break;
                    case 'r':
                    bot_rook_logic(i, j,color, 2, castling, move_list);
                        break;
                    case 'q':
                    bot_queen_logic(i, j, color, 2, castling, move_list);
                        break;
                    default:
                        break;
                }
            } else if (color == 0){
                switch (bot_board[i][j]){
                    case 'K':
                    bot_king_logic(i, j, color, 2, castling, move_list);

                        break;
                    case 'P':
                    bot_pawn_logic(i, j, color, 2, castling, move_list);
                        break;
                    case 'N':
                    bot_knight_logic(i, j, color, 2, castling, move_list);
                        break;
                    case 'B':
                    bot_bishop_logic(i, j, color, 2, castling, move_list);
                        break;
                    case 'R':
                    bot_rook_logic(i, j,color, 2, castling, move_list);
                        break;
                    case 'Q':
                    bot_queen_logic(i, j, color, 2, castling, move_list);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}
void store_killer_move(Move move, int depth) {
    if (killer_moves[depth][0].from_x != move.from_x ||
        killer_moves[depth][0].to_x != move.to_x) {
        // Shift the older killer move
        killer_moves[depth][1] = killer_moves[depth][0];
        // Save the new killer move
        killer_moves[depth][0] = move;
    }
}

void update_history(Move move, int depth) {
    history_heuristic[move.from_x][move.from_y][move.to_x][move.to_y] += depth * depth;
}


int score_move(Move move, int depth){
    int score = 0;
    int piece_values[128] = {0};  
    
    // Set specific values
    piece_values['P'] = 100;
    piece_values['p'] = 100;
    piece_values['N'] = 320;
    piece_values['n'] = 320;
    piece_values['B'] = 330;
    piece_values['b'] = 330;
    piece_values['R'] = 500;
    piece_values['r'] = 500;
    piece_values['Q'] = 900;
    piece_values['q'] = 900;
    piece_values['K'] = 20000; 
    piece_values['k'] = 20000;


    if ((move.moved_piece == 'K' || move.moved_piece == 'k') && 
        (abs(move.from_x - move.to_x) == 2) && !is_endgame()) {
        score += 200;  
    }
    if (move.captured_piece == '.' && !(move.moved_piece == 'P' && (move.to_y == 0 || move.to_y == 7))) {
        score -= 10;  
    }
    if (move.captured_piece != '.') {
        score += piece_values[move.captured_piece] - piece_values[move.moved_piece] + 1000;
    }
    if (move.moved_piece == 'P' && move.to_y == 0) {
        score += 800; 
    }else if (move.moved_piece == 'p' && move.to_y == 7) {
        score += 800; 
    }

    score += history_heuristic[move.from_x][move.from_y][move.to_x][move.to_y];

    if ((killer_moves[depth][0].from_x == move.from_x &&
        killer_moves[depth][0].to_x == move.to_x) ||
       (killer_moves[depth][1].from_x == move.from_x &&
        killer_moves[depth][1].to_x == move.to_x)) {
       score += 900; 
   }

  
    if ((move.moved_piece == 'P' || move.moved_piece == 'p') &&
        (move.to_x >= 2 && move.to_x <= 5 && move.to_y >= 2 && move.to_y <= 5)) {
        score += 50;
    }

    return score;
}
static int current_depth = 0;

int compare_moves(const void* a, const void* b) {
    const Move* move_a = (const Move*)a;
    const Move* move_b = (const Move*)b;

    int score_a = score_move(*move_a, current_depth);
    int score_b = score_move(*move_b, current_depth);

    return score_b - score_a;  // Higher scores first
}

void sort_moves(MoveList* move_list, int depth) {
    current_depth = depth;
    qsort(move_list->moves, move_list->count, sizeof(Move), compare_moves);
    current_depth = 0;  // Reset to avoid side effects
}
void match_bot_board(){
    for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
            bot_board[i][j] = board[i][j];
        }
    }


}
void print_bot_board(Move s, int depth){
    printf("\n\n-------------------");
    for (int i=0; i<8; i++){
        printf("\n");
        for (int j=0; j<8; j++){
            printf("%c", bot_board[j][i]);
        }
    }
    printf("\nMove Played:\nDepth: %d\nFrom: (%d,%d)\nTo:(%d,%d)\n"
        "Moved Piece: '%c'\nCaptured Piece: '%c'\nCastling: (%d,%d,%d,%d)\n"
        "En Passant: (%d,%d)\nScore: %d, color: %d\nWKing: (%d,%d)\nBking: (%d,%d)",depth, s.from_x,s.from_y,s.to_x,s.to_y,s.moved_piece,s.captured_piece,
        s.castling[0],s.castling[1],s.castling[2],s.castling[3],s.en_pass_x, s.en_pass_y, s.score, s.color, bot_wking_x, bot_wking_y, bot_bking_x,bot_bking_y);
    printf("\n-------------------\n");
    
}
int king_count_legal_moves(int color){
    int x;
    int y;
    int mobility = 0;
    if (color == 0){
        x = bot_wking_x;
        y = bot_wking_y;
    }else{
        x = bot_bking_x;
        y = bot_bking_y;
    }
    int bot_temp_wking_x = bot_wking_x;
    int bot_temp_wking_y = bot_wking_y;
    int bot_temp_bking_x = bot_bking_x;
    int bot_temp_bking_y = bot_bking_y;
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
                bot_wking_x = bot_temp_wking_x;
                bot_wking_y=bot_temp_wking_y;
                bot_wking_x +=i;
                bot_wking_y+=j;
                
            } else if (color == 1){
                bot_bking_x = bot_temp_bking_x;
                bot_bking_y = bot_temp_bking_y;
                bot_bking_x +=i;
                bot_bking_y+=j;
            }
            char current_piece = bot_board[x][y];
            char captured_piece = bot_board[x+i][y+j];
            bot_board[x+i][y+j] = current_piece;
            bot_board[x][y] = '.';
            if (bot_search_from_king(color) == 0){
                mobility++;
            }
            bot_board[x+i][y+j] = captured_piece;
            bot_board[x][y] = current_piece;

            if (color == 0){
                bot_wking_x = bot_temp_wking_x;
                bot_wking_y=bot_temp_wking_y;
            } else if (color == 1){
                bot_bking_x = bot_temp_bking_x;
                bot_bking_y = bot_temp_bking_y;
            }
        }
    }
    bot_bking_x = bot_temp_bking_x;
    bot_bking_y = bot_temp_bking_y;
    bot_wking_x = bot_temp_wking_x;
    bot_wking_y = bot_temp_wking_y;
    if (mobility == 0) return -400;  // In check with no moves (checkmate will be detected elsewhere)
    if (mobility == 1) return -200;  // Only one legal move
    if (mobility == 2) return -100;  // Two legal moves
    return 0;
}
int evaluate_checkmate_patterns(MoveList* move_list, int color, int endgame){
    int attackers = 0;
    int eval = 0;
    int x;
    int y;
    if (color == 0){
        x = bot_wking_x;
        y = bot_wking_y;
    }else{
        x = bot_bking_x;
        y = bot_bking_y;
    }
    for(int i=0; i<move_list->count; i++){
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

                if (x+i == move_list->moves[move_list->count].to_x && y+j == move_list->moves[move_list->count].to_y){
                    attackers+=1;
                }
            }
        }
    }
    if (attackers>0){
        printf("Attackers: %d", attackers);
    }
    eval-= attackers*attackers*15;
    if (endgame){
        eval += king_count_legal_moves(color);
    }
    return eval;

}
int open_file(int x, int y, char piece){
    int eval =0;
    if (piece == 'k'){
        for(int i=-1; i<2; i++){
            if (x+i>=0 && x+i<8){
                for (int j = 0; j<8;j++){
                    if (bot_board[x+i][j]!='.' && bot_board[x+i][j]!='Q' && bot_board[x+i][j]!='R'){
                        break;
                    }else{
                        eval-=(15-abs(i*5));
                    }
                }
            }
        }
        return eval;
    } else if (piece == 'K'){
        for(int i=-1; i<2; i++){
            if (x+i>=0 && x+i<8){
                for (int j = 7; j>=0;j--){
                    if (bot_board[x+i][j]!='.' && bot_board[x+i][j]!='q' && bot_board[x+i][j]!='r'){
                        break;
                    }else{
                        eval-=(15-abs(i*5));
                    }
                }
            }
        }
        return eval;
    }
}
int doubled_pawn(int x, int y, char piece){
    if (piece == 'P'){    
        for (int j = y+1; y+j<7; j++){
            if (bot_board[x][y+j] == 'P'){
                return -20;
            }
        }
    } else if (piece == 'p'){
        for (int j = y-1; y+j>0; j--){
            if (bot_board[x][y+j] == 'p'){
                return -20;
            }
        }
    }
    return 0;
}
int isolated_pawn(int x, int y, char piece){
    if (piece == 'P'){    
        for (int j = 1; j<7; j++){
            if(x-1>=0){
                if (bot_board[x-1][j] == 'P'){
                    return 0;
                }
            }if(x+1<8){
                if (bot_board[x+1][j] == 'P'){
                    return 0;
                }
            }
            
        }
        return -20;
    } else if (piece == 'p'){    
        for (int j = 1; j<7; j++){
            if(x-1>=0){
                if (bot_board[x-1][j] == 'p'){
                    return 0;
                }
            }if(x+1<8){
                if (bot_board[x+1][j] == 'p'){
                    return 0;
                }
            }
            
        }
        return -20;
    }
    return 0;
}
int passed_pawn(int x, int y, char piece){
    if(piece == 'P'){
        for (int j = y-1; j>0; j--){
            if(x-1>=0){
                if (bot_board[x-1][j] == 'p'){
                    return 0;
                }
            }if(x+1<8){
                if (bot_board[x+1][j] == 'p'){
                    return 0;
                }
            }
            
        }

        return 8*(7-y);
    } else if(piece == 'p'){
        for (int j = y+1; j<7; j++){
            if(x-1>=0){
                if (bot_board[x-1][j] == 'P'){
                    return 0;
                }
            }if(x+1<8){
                if (bot_board[x+1][j] == 'P'){
                    return 0;
                }
            }
            
        }
        return 8*y;
    }
}
int are_rooks_doubled(int x, int y){
    //up
    for (int j=-1; y+j>-1;j--){
        if (bot_board[x][y+j] == bot_board[x][y]){
            return 15;
        }
    }
    //down
    for (int j=1; y+j<8;j++){
        if (bot_board[x][y+j] == bot_board[x][y]){
            return 15;
        }
    }
    return 0;
}
int is_endgame(){
    int piece_count = 0;
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (bot_board[i][j]!='.'){
                piece_count++;
            }
        }
    }
    return piece_count<=16;
}
int piece_value(int i,int j, int endgame){
    int bishop_pair = 0;
    int eval=0;
    if (bot_color==0){
        switch(bot_board[i][j]){
            case 'P':
            eval+=(100+pawn_pts[i][j]);
            eval+=doubled_pawn(i, j, 'P');
            eval+=isolated_pawn(i, j, 'P');
            eval+=passed_pawn(i, j, 'P');
            break;
            case 'p':
            eval-=(100+pawn_pts[i][7-j]);
            eval-=doubled_pawn(i, j, 'p');
            eval-=isolated_pawn(i, j, 'p');
            eval-=passed_pawn(i, j, 'p');
            break;
            case 'N':
            eval+=(320+knight_pts[i][j]);
            break;
            case 'B':
            eval+=(330+bishop_pts[i][j]);
            break;
            case 'R':
            eval+=(500+rook_pts[i][j]);
            eval+=are_rooks_doubled(i, j);
            break;
            case 'Q':
            eval+=(900+queen_pts[i][7-j]);
            break;
            case 'n':
            eval-=(320+knight_pts[i][7-j]);
            break;
            case 'b':
            eval-=(330+bishop_pts[i][7-j]);
            break;
            case 'r':
            eval-=(500+rook_pts[i][7-j]);
            eval-=are_rooks_doubled(i, j);
            break;
            case 'q':
            eval-=(900+queen_pts[i][7-j]);
            break;
            case 'K':
                if (endgame){
                    eval+= (king_pts_late[i][j]);
                } else{
                    eval+= (king_pts_mid[i][j]);
                }
                break;
            case 'k':
                if (endgame){
                    eval-= (king_pts_late[i][7-j]);
                } else{
                    eval-= (king_pts_mid[i][7-j]);
                }
                break;
            default:
            break;
        }

        
        return eval;
    } else{
    
        switch(bot_board[i][j]){
            case 'P':
            eval-=(100+pawn_pts[i][j]);
            eval-=doubled_pawn(i, j, 'P');
            eval-=isolated_pawn(i, j, 'P');
            eval-=passed_pawn(i, j, 'P');
            break;
            case 'p':
            eval+=(100+pawn_pts[i][7-j]);
            eval+=doubled_pawn(i, j, 'p');
            eval+=isolated_pawn(i, j, 'p');
            eval+=passed_pawn(i, j, 'p');
            break;
            case 'N':
            eval-=(300+knight_pts[i][j]);
            break;
            case 'B':
            eval-=(300+bishop_pts[i][j]);
            break;
            case 'R':
            eval-=(500+rook_pts[i][j]);
            eval-=are_rooks_doubled(i, j);
            break;
            case 'Q':
            eval-=(900+queen_pts[i][7-j]);
            break;
            case 'n':
            eval+=(300+knight_pts[i][7-j]);
            break;
            case 'b':
            eval+=(300+bishop_pts[i][7-j]);
            break;
            case 'r':
            eval+=(500+rook_pts[i][7-j]);
            eval+=are_rooks_doubled(i, j);
            break;
            case 'q':
            eval+=(900+queen_pts[i][7-j]);
            break;
            case 'K':
                if (endgame){
                    eval-= (king_pts_late[i][j]);
                } else{
                    eval-= (king_pts_mid[i][j]);
                }
                break;
            case 'k':
                if (endgame){
                    eval+= (king_pts_late[i][7-j]);
                } else{
                    eval+= (king_pts_mid[i][7-j]);
                }
                break;
            default:
            break;
        }

        
        return eval;
        
    }
}
int evaluate(MoveList* move_list, int color){
    int eval = 0;
    int endgame = is_endgame();
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            if (bot_board[i][j] != '.') {  // If there's a piece on this square
                eval += piece_value(i, j,endgame);
            }
        }
    }
    eval+=evaluate_checkmate_patterns(move_list,color, endgame);
    return eval;
}


void make_move(Move s, int depth) {
    // Handle en passant
    if (s.en_pass_x != -1) {
        bot_board[s.en_pass_x][s.from_y] = '.';
    }

    // Handle castling
    if (s.moved_piece == 'K') {
        if (s.from_x - s.to_x == 2) { // Queen-side castling
            bot_board[3][7] = 'R';
            bot_board[0][7] = '.';
            bot_board[2][7] = 'K';
            bot_board[4][7] = '.';
        }
        else if (s.from_x - s.to_x == -2) { // King-side castling
            bot_board[5][7] = 'R';
            bot_board[7][7] = '.';
            bot_board[6][7] = 'K';
            bot_board[4][7] = '.';
        }
        bot_wking_x = s.to_x;
        bot_wking_y = s.to_y;
    }
    else if (s.moved_piece == 'k') {
        if (s.from_x - s.to_x == 2) {
            bot_board[3][0] = 'r';
            bot_board[0][0] = '.';
            bot_board[2][0] = 'k';
            bot_board[4][0] = '.';
        }
        else if (s.from_x - s.to_x == -2) {
            bot_board[5][0] = 'r';
            bot_board[7][0] = '.';
            bot_board[6][0] = 'k';
            bot_board[4][0] = '.';
        }
        bot_bking_x = s.to_x;
        bot_bking_y = s.to_y;
    }

    //handle promotions
    if (s.moved_piece == 'P' && s.to_y == 0) {
        bot_board[s.to_x][s.to_y] = 'Q';
        bot_board[s.from_x][s.from_y] = '.';

    }
    else if (s.moved_piece == 'p' && s.to_y == 7) {
        bot_board[s.to_x][s.to_y] = 'q';
        bot_board[s.from_x][s.from_y] = '.';
    }
    else {

        // Normal move (excluding castling already handled above)
        if (!(s.moved_piece == 'K' && abs(s.from_x - s.to_x) == 2) &&
            !(s.moved_piece == 'k' && abs(s.from_x - s.to_x) == 2)) {
            bot_board[s.to_x][s.to_y] = bot_board[s.from_x][s.from_y];
            bot_board[s.from_x][s.from_y] = '.';
            }
    }
    //print_bot_board(s, depth);

    if ((s.moved_piece == 'p' ||s.moved_piece == 'P') && abs(s.from_y-s.to_y) == 2){
        bot_en_passant_x = s.to_x;
        bot_en_passant_y = s.to_y;
    }else{
        bot_en_passant_x = -1;
        bot_en_passant_y = -1;
    }
    
}
int unmake_move(Move s, int depth) {
    // First, restore the moved piece to its original position
    bot_board[s.from_x][s.from_y] = s.moved_piece;
    
    // Then restore the captured piece (if any)
    if (s.captured_piece != '.') {
        bot_board[s.to_x][s.to_y] = s.captured_piece;
    } else {
        bot_board[s.to_x][s.to_y] = '.';
    }

    // Handle special cases
    
    // 1. En passant capture restoration
    if ((s.moved_piece == 'p' || s.moved_piece == 'P') && s.en_pass_x != -1 && s.to_x == s.en_pass_x) {
        int cap_y = (s.moved_piece == 'p') ? s.to_y - 1 : s.to_y + 1;
        bot_board[s.en_pass_x][cap_y] = (s.color == 0) ? 'p' : 'P';
    }

    // 2. Castling restoration
    if (s.moved_piece == 'K' && abs(s.from_x - s.to_x) == 2) {
        // White king castling
        if (s.to_x == 2) {  // Queen-side
            bot_board[0][7] = 'R';  // Restore rook
            bot_board[3][7] = '.';  // Clear rook's castling position
        } else if (s.to_x == 6) {  // King-side
            bot_board[7][7] = 'R';  // Restore rook
            bot_board[5][7] = '.';  // Clear rook's castling position
        }
    } 
    else if (s.moved_piece == 'k' && abs(s.from_x - s.to_x) == 2) {
        // Black king castling
        if (s.to_x == 2) {  // Queen-side
            bot_board[0][0] = 'r';  // Restore rook
            bot_board[3][0] = '.';  // Clear rook's castling position
        } else if (s.to_x == 6) {  // King-side
            bot_board[7][0] = 'r';  // Restore rook
            bot_board[5][0] = '.';  // Clear rook's castling position
        }
    }

    // Update king positions
    if (s.moved_piece == 'K') {
        bot_wking_x = s.from_x;
        bot_wking_y = s.from_y;
    } 
    else if (s.moved_piece == 'k') {
        bot_bking_x = s.from_x;
        bot_bking_y = s.from_y;
    }

    // Restore en passant target square
    bot_en_passant_x = s.en_pass_x;
    bot_en_passant_y = s.en_pass_y;

    return 0;
}


void pushMove(Move m) {
    history.moves[history.top++] = m;
}

Move popMove() {
    return history.moves[--history.top];
}
int bot_is_valid_attack(int x, int y, int color) {

    int piece_color = bot_check_piece_color(x, y);
    return piece_color == -1 ? 0 : ((piece_color != color) && (piece_color != 2));
}
void play_move(Move s) {
    //printf("\n-------------------------------\nMove Played:\nFrom: (%d,%d)\nTo:(%d,%d)\n"
    //    "Moved Piece: '%c'\nCaptured Piece: '%c'\nCastling: (%d,%d,%d,%d)\n"
    //    "En Passant: (%d,%d)\nScore: %d\nWKing: (%d,%d)\nBking: (%d,%d)\n------------------------------- ",
    //    s.from_x, s.from_y, s.to_x, s.to_y, s.moved_piece, s.captured_piece,
    //    s.castling[0], s.castling[1], s.castling[2], s.castling[3], s.en_pass_x, s.en_pass_y, s.score, bot_wking_x, bot_wking_y, bot_bking_x,bot_bking_y);
    // Handle en passant
    if (s.en_pass_x != -1) {
        board[s.en_pass_x][s.en_pass_y] = '.';
    }

    // Handle castling
    if (s.moved_piece == 'K') {
        if (s.from_x - s.to_x == 2) { // Queen-side castling
            board[3][7] = 'R';
            board[0][7] = '.';

        }
        else if (s.from_x - s.to_x == -2) { // King-side castling
            board[5][7] = 'R';
            board[7][7] = '.';
        }
    }
    else if (s.moved_piece == 'k') {
        if (s.from_x - s.to_x == 2) {
            board[3][0] = 'r';
            board[0][0] = '.';
        }
        else if (s.from_x - s.to_x == -2) {
            board[5][0] = 'r';
            board[7][0] = '.';
        }
    }
    if (s.moved_piece == 'k'){
        bot_bking_x = s.to_x;
        bot_bking_y = s.to_y;
        bking_x = bot_bking_x;
        bking_y = bot_bking_y;

    }
    if (s.moved_piece == 'K'){
        bot_wking_x = s.to_x;
        bot_wking_y = s.to_y;
        wking_x = bot_wking_x;
        wking_y = bot_wking_y;

    }
    if ((s.moved_piece == 'p' ||s.moved_piece == 'P') && abs(s.from_y-s.to_y) == 2){
        bot_en_passant_x = s.to_x;
        bot_en_passant_y = s.to_y;
    }else{
        bot_en_passant_x = -1;
        bot_en_passant_y = -1;
    }
    //handle promotions
    if (s.moved_piece == 'P' && s.to_y == 0) {
        board[s.to_x][s.to_y] = 'Q';
        board[s.from_x][s.from_y] = '.';
        return;
    }
    else if (s.moved_piece == 'p' && s.to_y == 7) {
        board[s.to_x][s.to_y] = 'q';
        board[s.from_x][s.from_y] = '.';
        return;
    }

    // Normal move
    board[s.to_x][s.to_y] = board[s.from_x][s.from_y];
    board[s.from_x][s.from_y] = '.';
    match_bot_board();
    print_bot_board(s, 0);
    printf("\nKnight Castling: (%d,%d,%d,%d)", s.castling[0],s.castling[1],s.castling[2],s.castling[3]);

}
void update_castling_rights(Move move, int* castling) {
    // White king moved - lose both castling rights
    if (move.moved_piece == 'K') {
        castling[0] = 0; // Queen-side
        castling[1] = 0; // King-side
    }
    // Black king moved - lose both castling rights
    else if (move.moved_piece == 'k') {
        castling[2] = 0; // Queen-side
        castling[3] = 0; // King-side
    }
    // White rook moved or captured
    else if (move.moved_piece == 'R' || move.captured_piece == 'R') {
        // Queen-side rook moved from a1 (0,7)
        if (move.from_x == 0 && move.from_y == 7) {
            castling[0] = 0;
        }
        // King-side rook moved from h1 (7,7)
        else if (move.from_x == 7 && move.from_y == 7) {
            castling[1] = 0;
        }
    }
    // Black rook moved or captured
    else if (move.moved_piece == 'r' || move.captured_piece == 'r') {
        // Queen-side rook moved from a8 (0,0)
        if (move.from_x == 0 && move.from_y == 0) {
            castling[2] = 0;
        }
        // King-side rook moved from h8 (7,0)
        else if (move.from_x == 7 && move.from_y == 0) {
            castling[3] = 0;
        }
    }

    castling_rights_KS = castling[0];
    castling_rights_QS = castling[1];
    castling_rights_ks = castling[2];
    castling_rights_qs = castling[3];

}
void play_best_move(int color, int* castling) {
    printf("Castling: (%d,%d,%d,%d)", castling[0], castling[1], castling[2], castling[3]);
    int depth = 6; // Set search depth (tune for performance vs. strength)
    MoveList move_list1;
    // Choose best move
    Move best_move = alphaBetaMax(-2000000, 2000000, depth, color, castling, &move_list1);


    
    // Play the move
    play_move(best_move);

    // Update castling rights based on the move
    update_castling_rights(best_move, castling);

    // Switch turns

}


Move alphaBetaMax(int alpha, int beta, int depthleft, int color, int* castling, MoveList* move_list1) {
    Move best_move;
    best_move.from_x = -1;
    best_move.from_y = -1;
    best_move.to_x = -1;
    best_move.to_y = -1;
    best_move.moved_piece = '.';
    best_move.captured_piece = '.';
    best_move.score = -2000000;
    best_move.en_pass_x = -1;
    best_move.en_pass_y = -1;

    // Terminal node - evaluate position
    if (depthleft == 0) {

        best_move.score = evaluate(move_list1, color);
    return best_move;
    }
    int saved_bot_wking_x = bot_wking_x;
    int saved_bot_wking_y = bot_wking_y;
    int saved_bot_bking_x = bot_bking_x;
    int saved_bot_bking_y = bot_bking_y;

    // Save en passant target
    int saved_en_pass_x = bot_en_passant_x;
    int saved_en_pass_y = bot_en_passant_y;
    // Save castling rights
    int saved_castling[4];
    memcpy(saved_castling, castling, 4 * sizeof(int));

    // Generate legal moves
    MoveList local_move_list = { .count = 0 };
    generate_moves(color, castling, &local_move_list);

    sort_moves(&local_move_list, depthleft);


    // Search through all legal moves
    for (int i = 0; i < local_move_list.count; i++) {
        Move current_move = local_move_list.moves[i];


        // Make the move on the board
        make_move(current_move, depthleft);
        pushMove(current_move);

        // Recursively search for opponent's best response
        Move response = alphaBetaMin(alpha, beta, depthleft - 1, !color, castling, &local_move_list);

        if (local_move_list.count == 0) {
            if (bot_is_checkmate(castling, color, &local_move_list) ==0) {
                response.score = -1000001 - depthleft*1000;  // Checkmate is very bad for max player
            }
            else if (bot_is_checkmate(castling, color, &local_move_list) ==2){
                response.score = 0;         // Stalemate is a draw
            }
            return response;
        }

        int score = response.score;

        // Unmake the move
        Move last_move = popMove(); // Pop the move
        unmake_move(last_move, depthleft);

        // Restore castling rights
        memcpy(castling, saved_castling, 4 * sizeof(int));
        // Restore en passant target
        bot_en_passant_x = saved_en_pass_x;
        bot_en_passant_y = saved_en_pass_y;

        bot_wking_x = saved_bot_wking_x;
        bot_wking_y = saved_bot_wking_y;
        bot_bking_x = saved_bot_bking_x;
        bot_bking_y= saved_bot_bking_y;

        // Update best move if this move is better
        if (score > best_move.score) {
            best_move.from_x = current_move.from_x;
            best_move.from_y = current_move.from_y;
            best_move.to_x = current_move.to_x;
            best_move.to_y = current_move.to_y;
            best_move.moved_piece = current_move.moved_piece;
            best_move.captured_piece = current_move.captured_piece;
            best_move.en_pass_x = current_move.en_pass_x;
            best_move.en_pass_y = current_move.en_pass_y;
            best_move.score = score;
            best_move.castling[0] = castling[0];
            best_move.castling[1] = castling[1];
            best_move.castling[2] = castling[2];
            best_move.castling[3] = castling[3];


            // Update alpha
            if (score > alpha) {
                alpha = score;
            }
        }
        //Alpha-beta pruning
        if (alpha >= beta) {
            store_killer_move(local_move_list.moves[i], depthleft);
            update_history(local_move_list.moves[i], depthleft);
            break;  // Beta cutoff
        }

    }

    return best_move;
}

Move alphaBetaMin(int alpha, int beta, int depthleft, int color, int* castling, MoveList* move_list1) {
    Move best_move;
    best_move.from_x = -1;
    best_move.from_y = -1;
    best_move.to_x = -1;
    best_move.to_y = -1;
    best_move.moved_piece = '.';
    best_move.captured_piece = '.';
    best_move.score = 2000000;
    best_move.en_pass_x = -1;
    best_move.en_pass_y = -1;

    // Terminal node - evaluate position
    if (depthleft == 0) {
        best_move.score = evaluate(move_list1, color);
        return best_move;
    }
    int saved_bot_wking_x = bot_wking_x;
    int saved_bot_wking_y = bot_wking_y;
    int saved_bot_bking_x = bot_bking_x;
    int saved_bot_bking_y = bot_bking_y;

    // Save en passant target
    int saved_en_pass_x = bot_en_passant_x;
    int saved_en_pass_y = bot_en_passant_y;

    // Save castling rights
    int saved_castling[4];
    memcpy(saved_castling, castling, 4 * sizeof(int));

    // Generate legal moves
    MoveList local_move_list = { .count = 0 };
    generate_moves(color, castling, &local_move_list);
    sort_moves(&local_move_list, depthleft);

    

    // Search through all legal moves
    for (int i = 0; i < local_move_list.count; i++) {
        Move current_move =local_move_list.moves[i];

        // Make the move on the board
        make_move(current_move, depthleft);
        pushMove(current_move);

        // Recursively search for opponent's best response
        Move response = alphaBetaMax(alpha, beta, depthleft - 1, !color, castling, &local_move_list);

        if (local_move_list.count == 0) {
            if (bot_is_checkmate(castling, color, &local_move_list) ==0) {
                response.score = 1000001 + depthleft * 1000;
            }

            else if (bot_is_checkmate(castling, color, &local_move_list) ==2){
                response.score = 0;         // Stalemate is a draw
            }
            return response;
        }

        int score = response.score;

        // Unmake the move
        Move last_move = popMove(); // Pop the move
        unmake_move(last_move, depthleft);

        // Restore castling rights
        memcpy(castling, saved_castling, 4 * sizeof(int));
        // Restore en passant target
        bot_en_passant_x = saved_en_pass_x;
        bot_en_passant_y = saved_en_pass_y;

        bot_wking_x = saved_bot_wking_x;
        bot_wking_y = saved_bot_wking_y;
        bot_bking_x = saved_bot_bking_x;
        bot_bking_y= saved_bot_bking_y;

        // Update best move if this move is better (lower score for min player)
        if (score < best_move.score) {
            best_move.from_x = current_move.from_x;
            best_move.from_y = current_move.from_y;
            best_move.to_x = current_move.to_x;
            best_move.to_y = current_move.to_y;
            best_move.moved_piece = current_move.moved_piece;
            best_move.captured_piece = current_move.captured_piece;
            best_move.en_pass_x = current_move.en_pass_x;
            best_move.en_pass_y = current_move.en_pass_y;
            best_move.score = score;
            best_move.castling[0] = castling[0];
            best_move.castling[1] = castling[1];
            best_move.castling[2] = castling[2];
            best_move.castling[3] = castling[3];
            
            // Update beta
            if (score < beta) {
                beta = score;
                //print_bot_board(best_move, depthleft);
            }
        }
        if (alpha >= beta) {
            store_killer_move(local_move_list.moves[i], depthleft);
            update_history(local_move_list.moves[i], depthleft);
            break;  // Alpha cutoff
        }

    }

    return best_move;
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
            break;
        case 'P':
        case 'R':
        case 'N':
        case 'B':
        case 'Q':
        case 'K':
            return 0;
            break;

        case '.':
            return 2;
            break;
        default:
            return -1;
    }
    
}
void add_to_move_list(int x, int y, int to_x, int to_y, int en_pass, int *castling, int color, char current_piece, char captured_piece, MoveList *move_list) {
    // Get the correct index for the new move
    int index = move_list->count;

    // Add move details
    move_list->moves[index].from_x = x;
    move_list->moves[index].from_y = y;
    move_list->moves[index].to_x = to_x;
    move_list->moves[index].to_y = to_y;
    move_list->moves[index].moved_piece = current_piece;
    move_list->moves[index].captured_piece = captured_piece;
    move_list->moves[index].color = color;

    // Handle en passant
    if (en_pass) {
        move_list->moves[index].en_pass_x = to_x;
        move_list->moves[index].en_pass_y = y;
    } else {
        move_list->moves[index].en_pass_x = -1;
        move_list->moves[index].en_pass_y = -1;
    }

    // Copy castling rights
    memcpy(move_list->moves[index].castling, castling, 4 * sizeof(int));


    move_list->count++;
}

int sim_move(int x, int y, int x_to, int y_to, int color, int en_pass, int *castling, int mode, MoveList *move_list){
    
    char current_piece = bot_board[x][y];
    char captured_piece = bot_board[x_to][y_to];
    char passant_piece;
    bot_board[x_to][y_to] = current_piece;
    bot_board[x][y] = '.';
    
    if (en_pass==1){
        passant_piece = bot_board[x_to][y];
        bot_board[x_to][y] = '.';
    }
    if (bot_search_from_king(color) == 0){
        if (mode == 1){
            if (en_pass ==1){
                bot_board[x_to][y] = passant_piece;
            }
            bot_board[x_to][y_to] = captured_piece;
            bot_board[x][y] = current_piece;
            return 1;
        }
        add_to_move_list(x, y, x_to, y_to, en_pass, castling, color, current_piece, captured_piece, move_list);
    }
    if (en_pass == 1){
        bot_board[x_to][y] = passant_piece;
    }
    bot_board[x_to][y_to] = captured_piece;
    bot_board[x][y] = current_piece;
    return 0;
}

//individual piece logic
int bot_pawn_logic(int x, int y, int color, int mode, int *castling, MoveList* move_list){
    int checkmate = 0;
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
                    checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode, move_list);
                    
                    
                }
            
            }
        }
        else if (y!=1){
            if (bot_check_piece_color(x, y+1) == 2){

                checkmate = sim_move(x, y, x, y+1, color, 0, castling, mode, move_list);
            }
        }
        

        //checks for pawn captures
        if ((bot_check_piece_color(x-1, y+1) == 0) && ((x-1>=0)&&(y+1<8))){
            checkmate = sim_move(x, y, x-1, y+1, color, 0, castling, mode, move_list);
        }
        if ((bot_check_piece_color(x+1, y+1) == 0) && ((x+1<8)&&(y+1<8))){
            checkmate = sim_move(x, y, x+1, y+1, color, 0, castling, mode, move_list);
        }
        
        if ((bot_en_passant_x == x+1)  && (bot_en_passant_y == y)){
            checkmate = sim_move(x, y, x+1, y+1, color, 1, castling, mode, move_list);
        }
        if ((bot_en_passant_x == x-1)  && (bot_en_passant_y == y)){
            checkmate = sim_move(x, y, x-1, y+1, color, 1, castling, mode, move_list);
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
                } else {
                    checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode, move_list);
                    
                }
            
            }
        }else if (y!=6){
            if (bot_check_piece_color(x, y-1) == 2){
                checkmate = sim_move(x, y, x, y-1, color, 0, castling, mode, move_list);
            }
        }

        //checks for pawn captures
        if ((bot_check_piece_color(x-1, y-1) == 1) && ((x-1>=0)&&(y-1>=0))){
            checkmate = sim_move(x, y, x-1, y-1, color, 0, castling, mode, move_list);
        }
        if ((bot_check_piece_color(x+1, y-1) == 1) && ((x+1<8)&&(y-1>=0))){
            checkmate = sim_move(x, y, x+1, y-1, color, 0, castling, mode, move_list);
            
        }
        if ((bot_en_passant_x == x+1)  && (bot_en_passant_y == y)){
            checkmate = sim_move(x, y, x+1, y-1, color, 1, castling, mode, move_list);
        }
        if ((bot_en_passant_x == x-1)  && (bot_en_passant_y == y)){
            checkmate = sim_move(x, y, x-1, y-1, color, 1, castling, mode, move_list);
        }
    }
    if (mode == 1){
        return checkmate;
    } else if (mode == 2){
        
        return 0;
    }

}

int bot_knight_logic(int x, int y, int color, int mode, int *castling, MoveList* move_list){
    int checkmate = 0;


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
                checkmate = sim_move(x, y, x+i, y+j, color, 0, castling, mode, move_list);
            }
        }
    }

    

    if (mode == 1){
        return checkmate;
    }else if (mode == 2){
        
        return 0;
    }
}
int bot_rook_logic(int x, int y, int color, int mode,int *castling, MoveList* move_list){
    int checkmate = 0;

    
    //left
    for (int i=-1; x+i>-1; i--){
    
        if(bot_check_piece_color(x+i, y) == color){
            break;
        }
        if (bot_is_valid_attack(x+i, y, color)){
            checkmate = sim_move(x, y, x+i, y, color, 0, castling, mode, move_list);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y, color, 0, castling, mode, move_list);
        }
    }
    //up
    for (int j=-1; y+j>-1;j--){
        if(bot_check_piece_color(x, y+j) == color){
            break;
        }
        if (bot_is_valid_attack(x, y+j, color)){
            checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode, move_list);
            break;
        }else{
            checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode, move_list);
        }    }
    //right
    for (int i=1; x+i<8; i++){
        if(bot_check_piece_color(x+i, y) == color){
            break;
        }
        if (bot_is_valid_attack(x+i, y, color)){
            checkmate = sim_move(x, y, x+i, y, color, 0, castling, mode, move_list);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y, color, 0, castling, mode, move_list);
        }
    }   
    //down
    for (int j=1; y+j<8;j++){
        if(bot_check_piece_color(x, y+j) == color){
            break;
        }
        if (bot_is_valid_attack(x, y+j, color)){
            checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode, move_list);
            break;
        }else{
            checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode, move_list);
        }    
    }

    if (mode == 1){
        return checkmate;
    }else if (mode == 2){
        
        return 0;
    }
}
int bot_bishop_logic(int x, int y, int color, int mode, int *castling, MoveList* move_list){
    int checkmate = 0;
    //left up
    for (int i=-1; x+i>-1 && y+i>-1; i--){
        
        if(bot_check_piece_color(x+i, y+i) == color){
            break;
        }
        if (bot_is_valid_attack(x+i, y+i, color)){
            checkmate = sim_move(x, y, x+i, y+i, color, 0, castling, mode, move_list);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y+i, color, 0, castling, mode, move_list);
        }
    }
    //right up
    for (int i=1; x+i<8 && y-i>-1; i++){
        if(bot_check_piece_color(x+i, y-i) == color){
            break;
        }
        

        if (bot_is_valid_attack(x+i, y-i, color)){
            checkmate = sim_move(x, y, x+i, y-i, color, 0, castling, mode, move_list);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y-i, color, 0, castling, mode, move_list);
        }
    }
    //right down
    for (int i=1; x+i<8 && y+i<8; i++){
        if(bot_check_piece_color(x+i, y+i) == color){
            break;
        }
        if (bot_is_valid_attack(x+i, y+i, color)){
            checkmate = sim_move(x, y, x+i, y+i, color, 0, castling, mode, move_list);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y+i, color, 0, castling, mode, move_list);
        }
    }
    //left down
    for (int i=1; x-i>-1 && y+i<8; i++){
        if(bot_check_piece_color(x-i, y+i) == color){
            break;
        }
        if (bot_is_valid_attack(x-i, y+i, color)){
            checkmate = sim_move(x, y, x-i, y+i, color, 0, castling, mode, move_list);
            break;
        }else{
            checkmate = sim_move(x, y, x-i, y+i, color, 0, castling, mode, move_list);
        }
    }
    if (mode == 1){
        return checkmate;
    }else if (mode == 2){
        
        return 0;
    }
}
int bot_queen_logic(int x, int y, int color, int mode, int *castling, MoveList* move_list){
    int checkmate = 0;
    //left up
    for (int i=-1; x+i>-1 && y+i>-1; i--){
        
        if(bot_check_piece_color(x+i, y+i) == color){
            break;
        }
        if (bot_is_valid_attack(x+i, y+i, color)){
            checkmate = sim_move(x, y, x+i, y+i, color, 0, castling, mode, move_list);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y+i, color, 0, castling, mode, move_list);
        }
    }
    //right up
    for (int i=1; x+i<8 && y-i>-1; i++){
        if(bot_check_piece_color(x+i, y-i) == color){
            break;
        }
        

        if (bot_is_valid_attack(x+i, y-i, color)){
            checkmate = sim_move(x, y, x+i, y-i, color, 0, castling, mode, move_list);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y-i, color, 0, castling, mode, move_list);
        }
    }
    //right down
    for (int i=1; x+i<8 && y+i<8; i++){
        if(bot_check_piece_color(x+i, y+i) == color){
            break;
        }
        if (bot_is_valid_attack(x+i, y+i, color)){
            checkmate = sim_move(x, y, x+i, y+i, color, 0, castling, mode, move_list);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y+i, color, 0, castling, mode, move_list);
        }
    }
    //left down
    for (int i=1; x-i>-1 && y+i<8; i++){
        if(bot_check_piece_color(x-i, y+i) == color){
            break;
        }
        if (bot_is_valid_attack(x-i, y+i, color)){
            checkmate = sim_move(x, y, x-i, y+i, color, 0, castling, mode, move_list);
            break;
        }else{
            checkmate = sim_move(x, y, x-i, y+i, color, 0, castling, mode, move_list);
        }
    }
     //left
     for (int i=-1; x+i>-1; i--){
    
        if(bot_check_piece_color(x+i, y) == color){
            break;
        }
        if (bot_is_valid_attack(x+i, y, color)){
            checkmate = sim_move(x, y, x+i, y, color, 0, castling, mode, move_list);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y, color, 0, castling, mode, move_list);
        }
    }
    //up
    for (int j=-1; y+j>-1;j--){
        if(bot_check_piece_color(x, y+j) == color){
            break;
        }
        if (bot_is_valid_attack(x, y+j, color)){
            checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode, move_list);
            break;
        }else{
            checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode, move_list);
        }    
    }
    //right
    for (int i=1; x+i<8; i++){
        if(bot_check_piece_color(x+i, y) == color){
            break;
        }
        if (bot_is_valid_attack(x+i, y, color)){
            checkmate = sim_move(x, y, x+i, y, color, 0, castling, mode, move_list);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y, color, 0, castling, mode, move_list);
        }
    }   
    //down
    for (int j=1; y+j<8;j++){
        if(bot_check_piece_color(x, y+j) == color){
            break;
        }
        if (bot_is_valid_attack(x, y+j, color)){
            checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode, move_list);
            break;
        }else{
            checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode, move_list);
        }    
    }
    if (mode == 1){
        return checkmate;
    }else if (mode == 2){
        
        return 0;
    }
}
int bot_king_logic(int x, int y, int color, int mode, int *castling, MoveList* move_list){
    int checkmate = 0;
    int bot_temp_wking_x = bot_wking_x;
    int bot_temp_wking_y = bot_wking_y;
    int bot_temp_bking_x = bot_bking_x;
    int bot_temp_bking_y = bot_bking_y;

        
    bot_check_for_castle(color, castling, move_list);

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
                bot_wking_x = bot_temp_wking_x;
                bot_wking_y=bot_temp_wking_y;
                bot_wking_x +=i;
                bot_wking_y+=j;
                
            } else if (color == 1){
                bot_bking_x = bot_temp_bking_x;
                bot_bking_y = bot_temp_bking_y;
                bot_bking_x +=i;
                bot_bking_y+=j;
            }
            checkmate = sim_move(x, y, x+i, y+j, color, 0, castling, mode, move_list);
            if (color == 0){
                bot_wking_x = bot_temp_wking_x;
                bot_wking_y=bot_temp_wking_y;
            } else if (color == 1){
                bot_bking_x = bot_temp_bking_x;
                bot_bking_y = bot_temp_bking_y;
            }
        }
    }
    bot_bking_x = bot_temp_bking_x;
    bot_bking_y = bot_temp_bking_y;
    bot_wking_x = bot_temp_wking_x;
    bot_wking_y = bot_temp_wking_y;

    if (mode == 1){
        return checkmate;
    }else if (mode == 2){
        
        return 0;
    }
}
int bot_search_from_king(int color){
    int decision;
    if (color==0){
        //left up
        for (int i=-1; bot_wking_x+i>-1 && bot_wking_y+i>-1; i--){
            decision = bot_king_search_help(bot_wking_x+i, bot_wking_y+i, color, 'b', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //right up
        for (int i=1; bot_wking_x+i<8 && bot_wking_y-i>-1; i++){
            decision = bot_king_search_help(bot_wking_x+i, bot_wking_y-i, color, 'b', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //right down
        for (int i=1; bot_wking_x+i<8 && bot_wking_y+i<8; i++){
            decision = bot_king_search_help(bot_wking_x+i, bot_wking_y+i, color, 'b', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //left down
        for (int i=1; bot_wking_x-i>-1 && bot_wking_y+i<8; i++){
            decision = bot_king_search_help(bot_wking_x-i, bot_wking_y+i, color, 'b', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //left
        for (int i=-1; bot_wking_x+i>-1; i--){
            decision = bot_king_search_help(bot_wking_x+i, bot_wking_y, color, 'r', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }
        //up
        for (int j=-1; bot_wking_y+j>-1;j--){
            decision = bot_king_search_help(bot_wking_x, bot_wking_y+j, color, 'r', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }
        //right
        for (int i=1; bot_wking_x+i<8; i++){
            decision = bot_king_search_help(bot_wking_x+i, bot_wking_y, color, 'r', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //down
        for (int j=1; bot_wking_y+j<8;j++){
            decision = bot_king_search_help(bot_wking_x, bot_wking_y+j, color, 'r', 'q');
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
                    decision = bot_king_search_help(bot_wking_x+i, bot_wking_y+j, color, 'n', 'n');
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
                    decision = bot_king_search_help(bot_wking_x+i, bot_wking_y+j, color, 'k', 'k');
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
            decision = bot_king_search_help(bot_bking_x+i, bot_bking_y+i, color, 'B', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //right up
        for (int i=1; bot_bking_x+i<8 && bot_bking_y-i>-1; i++){
            decision = bot_king_search_help(bot_bking_x+i, bot_bking_y-i, color, 'B', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //right down
        for (int i=1; bot_bking_x+i<8 && bot_bking_y+i<8; i++){
            decision = bot_king_search_help(bot_bking_x+i, bot_bking_y+i, color, 'B', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //left down
        for (int i=1; bot_bking_x-i>-1 && bot_bking_y+i<8; i++){
            decision = bot_king_search_help(bot_bking_x-i, bot_bking_y+i, color, 'B', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //left
        for (int i=-1; bot_bking_x+i>-1; i--){
            decision = bot_king_search_help(bot_bking_x+i, bot_bking_y, color, 'R', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }
        //up
        for (int j=-1; bot_bking_y+j>-1;j--){
            decision = bot_king_search_help(bot_bking_x, bot_bking_y+j, color, 'R', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }
        //right
        for (int i=1; bot_bking_x+i<8; i++){
            decision = bot_king_search_help(bot_bking_x+i, bot_bking_y, color, 'R', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //down
        for (int j=1;bot_bking_y+j<8;j++){
            decision = bot_king_search_help(bot_bking_x, bot_bking_y+j, color, 'R', 'Q');
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
                    decision = bot_king_search_help(bot_bking_x+i, bot_bking_y+j, color, 'N', 'N');
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
                    decision = bot_king_search_help(bot_bking_x+i, bot_bking_y+j, color, 'K', 'K');
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
int bot_king_search_help(int x, int y, int color, char piece_to_check1, char piece_to_check2){
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
int bot_check_for_castle(int color, int *castling, MoveList* move_list){
    if (color == 0){
        int temp_bot_wking_x = bot_wking_x;
        

        if (castling[1]){
            if (bot_board[3][7] == '.' && bot_board[2][7] == '.' && bot_board[1][7] == '.' &&bot_board[0][7] == 'R'){
                if (!bot_search_from_king(color)){
                    bot_board[3][7] = 'K';
                    bot_board[4][7] = '.';
                    bot_wking_x = 3;
                    
                    if (!bot_search_from_king(color)){
                        bot_board[2][7] = 'K';
                        bot_board[3][7] = '.';
                        bot_wking_x = 2;
                        if (!bot_search_from_king(color)){
                            castling[1] = 0;
                            add_to_move_list(4, 7, 2, 7, 0, castling, color,'K', '.', move_list);
                        }
                    }
                }
                bot_board[3][7] = '.';
                bot_board[2][7] = '.';
                bot_board[4][7] = 'K';
            }
        }
        if (castling[0]){
            if (bot_board[5][7] == '.' && bot_board[6][7] == '.' && bot_board[7][7] == 'R'){
                if (!bot_search_from_king(color)){
                    bot_board[5][7] = 'K';
                    bot_board[4][7] = '.';
                    bot_wking_x = 5;
                    if (!bot_search_from_king(color)){
                        bot_board[6][7] = 'K';
                        bot_board[5][7] = '.';
                        bot_wking_x = 6;
                        if (!bot_search_from_king(color)){
                            castling[0] = 0;
                            add_to_move_list(4, 7, 6, 7, 0, castling, color,'K', '.', move_list);
                        }
                    }
                }
                bot_board[5][7] = '.';
                bot_board[6][7] = '.';
                bot_board[4][7] = 'K';
            }
        }
        bot_wking_x = temp_bot_wking_x;
    } else if (color == 1){

        int temp_bot_bking_x = bot_bking_x;

        if (castling[3]){
            if (bot_board[3][0] == '.' && bot_board[2][0] == '.' && bot_board[1][0] == '.' &&bot_board[0][0] == 'r'){
                if (!bot_search_from_king(color)){
                    bot_board[3][0] = 'k';
                    bot_board[4][0] = '.';
                    bot_bking_x = 3;
                    if (!bot_search_from_king(color)){
                        bot_board[2][0] = 'k';
                        bot_board[3][0] = '.';
                        bot_bking_x = 2;
                        if (!bot_search_from_king(color)){
                            castling[3] = 0;
                            add_to_move_list(4, 0, 2, 0, 0, castling, color,'k', '.', move_list);
                        }
                    }
                }
                bot_board[3][0] = '.';
                bot_board[2][0] = '.';
                bot_board[4][0] = 'k';
            }
        }
        if (castling[2]){
            if (bot_board[5][0] == '.' && bot_board[6][0] == '.' && bot_board[7][0] == 'r'){
                if (!bot_search_from_king(color)){
                    bot_board[5][0] = 'k';
                    bot_board[4][0] = '.';
                    bot_bking_x = 5;
                    if (!bot_search_from_king(color)){
                        bot_board[6][0] = 'k';
                        bot_board[5][0] = '.';
                        bot_bking_x = 6;
                        if (!bot_search_from_king(color)){
                            castling[2] = 0;
                            add_to_move_list(4, 0, 6, 0, 0, castling, color,'k', '.', move_list);
                        }
                    }
                }
                bot_board[5][0] = '.';
                bot_board[6][0] = '.';
                bot_board[4][0] = 'k';
            }
        }
        bot_bking_x = temp_bot_bking_x;
    }
    
}
int bot_is_checkmate(int *castling, int color, MoveList* move_list){
    
    int checkmate = 1;
    for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
            if (color == 1){
                switch (bot_board[i][j]){
                    case 'k':
                        if (!bot_king_logic(i, j, color, 1, castling, move_list)){
                            return 1;
                        }
                        break;
                    case 'p':
                        if (!bot_pawn_logic(i, j, color, 1, castling, move_list)){
                            return 1;
                        }
                        break;
                    case 'n':
                        if (!bot_knight_logic(i, j, color, 1, castling, move_list)){
                            return 1;
                        }
                        break;
                    case 'b':
                        if (!bot_bishop_logic(i, j, color, 1, castling, move_list)){
                            return 1;
                        }
                        break;
                    case 'r':
                        if (!bot_rook_logic(i, j,color, 1, castling, move_list)){
                            return 1;
                        }
                        break;
                    case 'q':
                        if (!bot_queen_logic(i, j, color, 1, castling, move_list)){
                            return 1;
                        }
                        break;
                    default:
                        break;
                }
            } else if (turn_color == 0){
                switch (bot_board[i][j]){
                    case 'K':
                        if (!bot_king_logic(i, j, color, 1, castling, move_list)){
                            return 1;
                        }
                        break;
                    case 'P':
                        if (!bot_pawn_logic(i, j, color, 1, castling, move_list)){
                            return 1;
                        }
                        break;
                    case 'N':
                        if (!bot_knight_logic(i, j, color, 1, castling, move_list)){
                            return 1;
                        }
                        break;
                    case 'B':
                        if (!bot_bishop_logic(i, j, color, 1, castling, move_list)){
                            return 1;
                        }
                        break;
                    case 'R':
                        if (!bot_rook_logic(i, j,color, 1, castling, move_list)){
                            return 1;
                        }
                        break;
                    case 'Q':
                        if (!bot_queen_logic(i, j, color, 1, castling, move_list)){
                            return 1;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
    if(bot_search_from_king(color)){
        return 0;
    } else{
        return 2;
    }
}