#include <stdio.h>
#include "defs.h"
#include "game_logic.h"
#include <limits.h>
#include "bot.h"
#include <math.h>



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
                    bot_knight_logic(i, j, color, 2, castling);
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
        "En Passant: (%d,%d)\nScore: %d",depth, s.from_x,s.from_y,s.to_x,s.to_y,s.moved_piece,s.captured_piece,
        s.castling[0],s.castling[1],s.castling[2],s.castling[3],s.en_pass_x, s.en_pass_y, s.score);
    printf("\n-------------------\n");
    
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
int piece_value(int i,int j){
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
            if (bot_check_piece_color(i, j)  == bot_color){
                eval+=piece_value(i, j);
            }
            if (bot_check_piece_color(i, j)  == player_color){
                eval+=piece_value(i, j);
            }
        }
    }
    return eval;
}


void make_move(Move s, int depth) {
    // Handle en passant
    if (s.en_pass_x != -1) {
        bot_board[s.en_pass_x][s.from_y] = 'o';
    }

    // Handle castling
    if (s.moved_piece == 'K') {
        if (s.from_x - s.to_x == 2) { // Queen-side castling
            bot_board[3][7] = 'R';
            bot_board[0][7] = 'o';
        } else if (s.from_x - s.to_x == -2) { // King-side castling
            bot_board[5][7] = 'R';
            bot_board[7][7] = 'o';
        }
    } else if (s.moved_piece == 'k') {
        if (s.from_x - s.to_x == 2) {
            bot_board[3][0] = 'r';
            bot_board[0][0] = 'o';
        } else if (s.from_x - s.to_x == -2) {
            bot_board[5][0] = 'r';
            bot_board[7][0] = 'o';
        }
    }

    //handle promotions
    if (s.moved_piece == 'P' && s.to_y == 0){
        bot_board[s.to_x][s.to_y] = 'Q';
        bot_board[s.from_x][s.from_y] = 'o';
        return;
    }else if (s.moved_piece == 'p' && s.to_y == 7){
        bot_board[s.to_x][s.to_y] = 'q';
        bot_board[s.from_x][s.from_y] = 'o';
        return;
    }

    // Normal move
    bot_board[s.to_x][s.to_y] = bot_board[s.from_x][s.from_y];
    bot_board[s.from_x][s.from_y] = 'o';
    print_bot_board(s,depth);
}

void unmake_move(Move s) {
    // Restore en passant
    if (s.en_pass_x != -1) {
        bot_board[s.en_pass_x][s.from_y] = (s.color == 0) ? 'p' : 'P';
    }

    // Restore castling
    if (s.moved_piece == 'K') {
        if (s.from_x - s.to_x == 2) { // Queen-side
            bot_board[0][7] = 'R';
            bot_board[3][7] = 'o';
        } else if (s.from_x - s.to_x == -2) { // King-side
            bot_board[7][7] = 'R';
            bot_board[5][7] = 'o';
        }
    } else if (s.moved_piece == 'k') {
        if (s.from_x - s.to_x == 2) {
            bot_board[0][0] = 'r';
            bot_board[3][0] = 'o';
        } else if (s.from_x - s.to_x == -2) {
            bot_board[7][0] = 'r';
            bot_board[5][0] = 'o';
        }
    }

    if (s.moved_piece == 'P' && s.to_y == 0){
        bot_board[s.to_x][s.to_y] = s.captured_piece;
        bot_board[s.from_x][s.from_y] = 'P';
        return;
    }else if (s.moved_piece == 'p' && s.to_y == 7){
        bot_board[s.to_x][s.to_y] = s.captured_piece;
        bot_board[s.from_x][s.from_y] = 'p';
        return;
    }

    // Restore pieces
    bot_board[s.from_x][s.from_y] = s.moved_piece;
    bot_board[s.to_x][s.to_y] = s.captured_piece;
    
    

}


void pushMove(Move m) {
    history.moves[history.top++] = m;
}
Move popMove() {
    return history.moves[--history.top];
}
int bot_is_valid_attack(int x, int y, int color){

    int piece_color = bot_check_piece_color(x, y);
    return piece_color == -1 ? 0 : ((piece_color != color) && (piece_color !=2));
}
void play_move(Move s){
    printf("\n-------------------------------\nMove Played:\nFrom: (%d,%d)\nTo:(%d,%d)\n"
        "Moved Piece: '%c'\nCaptured Piece: '%c'\nCastling: (%d,%d,%d,%d)\n"
        "En Passant: (%d,%d)\nScore: %d\n------------------------------- ",
        s.from_x,s.from_y,s.to_x,s.to_y,s.moved_piece,s.captured_piece,
        s.castling[0],s.castling[1],s.castling[2],s.castling[3],s.en_pass_x, s.en_pass_y, s.score);
        // Handle en passant
        if (s.en_pass_x != -1) {
            board[s.en_pass_x][s.from_y] = 'o';
        }
    
        // Handle castling
        if (s.moved_piece == 'K') {
            if (s.from_x - s.to_x == 2) { // Queen-side castling
                board[3][7] = 'R';
                board[0][7] = 'o';
            } else if (s.from_x - s.to_x == -2) { // King-side castling
                board[5][7] = 'R';
                board[7][7] = 'o';
            }
        } else if (s.moved_piece == 'k') {
            if (s.from_x - s.to_x == 2) {
                board[3][0] = 'r';
                board[0][0] = 'o';
            } else if (s.from_x - s.to_x == -2) {
                board[5][0] = 'r';
                board[7][0] = 'o';
            }
        }
    
        //handle promotions
        if (s.moved_piece == 'P' && s.to_y == 0){
            board[s.to_x][s.to_y] = 'Q';
            board[s.from_x][s.from_y] = 'o';
            return;
        }else if (s.moved_piece == 'p' && s.to_y == 7){
            board[s.to_x][s.to_y] = 'q';
            board[s.from_x][s.from_y] = 'o';
            return;
        }
    
        // Normal move
        board[s.to_x][s.to_y] = board[s.from_x][s.from_y];
        board[s.from_x][s.from_y] = 'o';
}
void update_castling_rights(Move move, int *castling) {
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
}
void play_best_move(int color, int *castling) {
    printf("\nplay_best_move(%d, castling)", color);
    int depth = 5; // Set search depth (tune for performance vs. strength)
    
    // Debug the initial state
    printf("\nInitial board state before move generation:");
    
    // Save castling rights
    int saved_castling[4];
    memcpy(saved_castling, castling, 4 * sizeof(int));
    
    // Clear move list and generate moves first
    move_list[0].count = 0;
    generate_moves(color, castling);
    printf("\nGenerated %d moves for color %d", move_list[0].count, color);
    
    // Check if there are any moves
    if (move_list[0].count == 0) {
        printf("\nNo valid moves found!");
        // Check if in checkmate or stalemate
        if (bot_is_checkmate(castling)) {
            printf("\nCheckmate! Game over.");
        } else {
            printf("\nStalemate! Game drawn.");
        }
        
        return;
    }
    
    // Choose best move
    Move best_move = alphaBetaMax(-2000000, 2000000, depth, color, castling);
    
    // Restore castling rights before playing the move
    memcpy(castling, saved_castling, 4 * sizeof(int));
    
    // Debug move info
    printf("\nBest move:\nFrom: (%d,%d)\nTo:(%d,%d)\n"
           "Moved Piece: '%c'\nCaptured Piece: '%c'\nScore: %d",
           best_move.from_x, best_move.from_y,
           best_move.to_x, best_move.to_y,
           best_move.moved_piece, best_move.captured_piece, 
           best_move.score);
    
    // Play the move
    play_move(best_move);
    
    // Update castling rights based on the move
    update_castling_rights(best_move, castling);
    
    // Switch turns
    turn_color = !turn_color;
}


Move alphaBetaMax(int alpha, int beta, int depthleft, int color, int *castling) {
    Move best_move;
    best_move.from_x = -1;
    best_move.from_y = -1;
    best_move.to_x = -1;
    best_move.to_y = -1;
    best_move.moved_piece = 'o';
    best_move.captured_piece = 'o';
    best_move.score = -2000000;
    best_move.en_pass_x = -1;
    best_move.en_pass_y = -1;
    
    // Terminal node - evaluate position
    if (depthleft == 0) {
        best_move.score = evaluate();
        return best_move;
    }
    
    // Save castling rights
    int saved_castling[4];
    memcpy(saved_castling, castling, 4 * sizeof(int));
    
    // Generate legal moves
    move_list[0].count = 0;
    generate_moves(color, castling);
    
    // No moves - either checkmate or stalemate
    if (move_list[0].count == 0) {
        if (bot_is_checkmate(castling)) {
            best_move.score = -1000000;  // Checkmate is very bad for max player
        } else {
            best_move.score = 0;         // Stalemate is a draw
        }
        return best_move;
    }
    
    // Search through all legal moves
    for (int i = 0; i < move_list[0].count; i++) {
        Move current_move = move_list[i];
        
        // Make the move on the board
        make_move(current_move, depthleft);
        pushMove(current_move);
        
        // Recursively search for opponent's best response
        Move response = alphaBetaMin(alpha, beta, depthleft - 1, !color, castling);
        int score = response.score;
        
        // Unmake the move
        Move last_move = popMove(); // Pop the move
        unmake_move(last_move);
        
        // Restore castling rights
        memcpy(castling, saved_castling, 4 * sizeof(int));
        
        // Update best move if this move is better
        if (score > best_move.score) {
            best_move = current_move;
            best_move.score = score;
            
            // Update alpha
            if (score > alpha) {
                alpha = score;
            }
        }
        
        // Beta cutoff
        if (alpha >= beta) {
            break;
        }
    }
    
    return best_move;
}

Move alphaBetaMin(int alpha, int beta, int depthleft, int color, int *castling) {
    Move best_move;
    best_move.from_x = -1;
    best_move.from_y = -1;
    best_move.to_x = -1;
    best_move.to_y = -1;
    best_move.moved_piece = 'o';
    best_move.captured_piece = 'o';
    best_move.score = 2000000;
    best_move.en_pass_x = -1;
    best_move.en_pass_y = -1;
    
    // Terminal node - evaluate position
    if (depthleft == 0) {
        best_move.score = evaluate();
        return best_move;
    }
    
    // Save castling rights
    int saved_castling[4];
    memcpy(saved_castling, castling, 4 * sizeof(int));
    
    // Generate legal moves
    move_list[0].count = 0;
    generate_moves(color, castling);
    
    // No moves - either checkmate or stalemate
    if (move_list[0].count == 0) {
        if (bot_is_checkmate(castling)) {
            best_move.score = 1000000;  // Checkmate is very good for min player
        } else {
            best_move.score = 0;        // Stalemate is a draw
        }
        return best_move;
    }
    
    // Search through all legal moves
    for (int i = 0; i < move_list[0].count; i++) {
        Move current_move = move_list[i];
        
        // Make the move on the board
        make_move(current_move,depthleft);
        pushMove(current_move);
        
        // Recursively search for opponent's best response
        Move response = alphaBetaMax(alpha, beta, depthleft - 1, !color, castling);
        int score = response.score;
        
        // Unmake the move
        Move last_move = popMove(); // Pop the move
        unmake_move(last_move);
        
        // Restore castling rights
        memcpy(castling, saved_castling, 4 * sizeof(int));
        
        // Update best move if this move is better (lower score for min player)
        if (score < best_move.score) {
            best_move = current_move;
            best_move.score = score;
            
            // Update beta
            if (score < beta) {
                beta = score;
            }
        }
        
        // Alpha cutoff
        if (alpha >= beta) {
            break;
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

        case 'o':
            return 2;
            break;
        default:
            return -1;
    }
    
}
void add_to_move_list(int x, int y, int to_x, int to_y, int en_pass, int *castling, int color, char current_piece, char captured_piece){
    int index;
    
    index = move_list[0].count;
    move_list[index].from_x = x;
    move_list[index].from_y = y;
    move_list[index].to_x = to_x;
    move_list[index].to_y = to_y;
    move_list[index].captured_piece = captured_piece;
    move_list[index].moved_piece = current_piece;

    if (en_pass){
        move_list[index].en_pass_x = to_x;
        move_list[index].en_pass_y = to_y;
    } else{
        move_list[index].en_pass_x = -1;
        move_list[index].en_pass_y = -1;
    }

    for (int i = 0; i<4; i++){
        move_list[index].castling[i] = castling[i];
    }
    move_list[index].color = color;
    move_list[0].count++;
    
}

int sim_move(int x, int y, int x_to, int y_to, int color, int en_pass, int *castling, int mode){
    
    char current_piece = bot_board[x][y];
    char captured_piece = bot_board[x_to][y_to];
    char passant_piece;
    bot_board[x_to][y_to] = current_piece;
    bot_board[x][y] = 'o';
    if (en_pass){
        passant_piece = bot_board[x_to][y];
        bot_board[x_to][y] = 'o';
    }
    if (bot_search_from_king(color) == 0){
        if (mode == 1){
            return 1;
        }
        add_to_move_list(x, y, x_to, y_to, en_pass, castling, color, current_piece, captured_piece);
    }
    if (en_pass){
        bot_board[x_to][y] = passant_piece;
    }
    bot_board[x_to][y_to] = captured_piece;
    bot_board[x][y] = current_piece;
}
//individual piece logic
int bot_pawn_logic(int x, int y, int color, int mode, int *castling){
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
                    checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode);
                }
            
            }
        }
        else if (y!=1){
            if (bot_check_piece_color(x, y+1) == 2){

                checkmate = sim_move(x, y, x, y+1, color, 0, castling, mode);
            }
        }
        

        //checks for pawn captures
        if ((bot_check_piece_color(x-1, y+1) == 0) && ((x-1>=0)&&(y+1<8))){
            checkmate = sim_move(x, y, x-1, y+1, color, 0, castling, mode);
        }
        if ((bot_check_piece_color(x+1, y+1) == 0) && ((x+1<8)&&(y+1<8))){
            checkmate = sim_move(x, y, x+1, y+1, color, 0, castling, mode);
        }
        
        if ((bot_en_passant_x == x+1)  && (bot_en_passant_y == y)){
            checkmate = sim_move(x, y, x+1, y+1, color, 1, castling, mode);
        }
        if ((bot_en_passant_x == x-1)  && (bot_en_passant_y == y)){
            checkmate = sim_move(x, y, x-1, y+1, color, 1, castling, mode);
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
                    checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode);
                }
            
            }
        }else if (y!=6){
            if (bot_check_piece_color(x, y-1) == 2){
                checkmate = sim_move(x, y, x, y-1, color, 0, castling, mode);
            }
        }

        //checks for pawn captures
        if ((bot_check_piece_color(x-1, y-1) == 1) && ((x-1>=0)&&(y-1>=0))){
            checkmate = sim_move(x, y, x-1, y-1, color, 0, castling, mode);
        }
        if ((bot_check_piece_color(x+1, y-1) == 1) && ((x+1<8)&&(y-1>=0))){
            checkmate = sim_move(x, y, x+1, y-1, color, 0, castling, mode);
            
        }
        if ((bot_en_passant_x == x+1)  && (bot_en_passant_y == y)){
            checkmate = sim_move(x, y, x+1, y-1, color, 1, castling, mode);
        }
        if ((bot_en_passant_x == x-1)  && (bot_en_passant_y == y)){
            checkmate = sim_move(x, y, x-1, y-1, color, 1, castling, mode);
        }
    }
    if (mode == 1){
        return checkmate;
    } else if (mode == 2){
        
        return 0;
    }

}
int bot_knight_logic(int x, int y, int color, int mode, int *castling){
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
                checkmate = sim_move(x, y, x+i, y+j, color, 0, castling, mode);
            }
        }
    }

    

    if (mode == 1){
        return checkmate;
    }else if (mode == 2){
        
        return 0;
    }
}
int bot_rook_logic(int x, int y, int color, int mode,int *castling){
    int checkmate = 0;
    
    
    //left
    for (int i=-1; x+i>-1; i--){
    
        if(bot_check_piece_color(x+i, y) == color){
            break;
        }
        if (bot_is_valid_attack(x+i, y, color)){
            checkmate = sim_move(x, y, x+i, y, color, 0, castling, mode);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y, color, 0, castling, mode);
        }
    }
    //up
    for (int j=-1; y+j>-1;j--){
        if(bot_check_piece_color(x, y+j) == color){
            break;
        }
        if (bot_is_valid_attack(x, y+j, color)){
            checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode);
            break;
        }else{
            checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode);
        }    }
    //right
    for (int i=1; x+i<8; i++){
        if(bot_check_piece_color(x+i, y) == color){
            break;
        }
        if (bot_is_valid_attack(x+i, y, color)){
            checkmate = sim_move(x, y, x+i, y, color, 0, castling, mode);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y, color, 0, castling, mode);
        }
    }   
    //down
    for (int j=1; y+j<8;j++){
        if(bot_check_piece_color(x, y+j) == color){
            break;
        }
        if (bot_is_valid_attack(x, y+j, color)){
            checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode);
            break;
        }else{
            checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode);
        }    
    }

    if (mode == 1){
        return checkmate;
    }else if (mode == 2){
        
        return 0;
    }
}
int bot_bishop_logic(int x, int y, int color, int mode, int *castling){
    int checkmate = 0;
    //left up
    for (int i=-1; x+i>-1 && y+i>-1; i--){
        
        if(bot_check_piece_color(x+i, y+i) == color){
            break;
        }
        if (bot_is_valid_attack(x+i, y+i, color)){
            checkmate = sim_move(x, y, x+i, y+i, color, 0, castling, mode);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y+i, color, 0, castling, mode);
        }
    }
    //right up
    for (int i=1; x+i<8 && y-i>-1; i++){
        if(bot_check_piece_color(x+i, y-i) == color){
            break;
        }
        

        if (bot_is_valid_attack(x+i, y-i, color)){
            checkmate = sim_move(x, y, x+i, y-i, color, 0, castling, mode);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y-i, color, 0, castling, mode);
        }
    }
    //right down
    for (int i=1; x+i<8 && y+i<8; i++){
        if(bot_check_piece_color(x+i, y+i) == color){
            break;
        }
        if (bot_is_valid_attack(x+i, y+i, color)){
            checkmate = sim_move(x, y, x+i, y+i, color, 0, castling, mode);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y+i, color, 0, castling, mode);
        }
    }
    //left down
    for (int i=1; x-i>-1 && y+i<8; i++){
        if(bot_check_piece_color(x-i, y+i) == color){
            break;
        }
        if (bot_is_valid_attack(x-i, y+i, color)){
            checkmate = sim_move(x, y, x-i, y+i, color, 0, castling, mode);
            break;
        }else{
            checkmate = sim_move(x, y, x-i, y+i, color, 0, castling, mode);
        }
    }
    if (mode == 1){
        return checkmate;
    }else if (mode == 2){
        
        return 0;
    }
}
int bot_queen_logic(int x, int y, int color, int mode, int *castling){
    int checkmate = 0;
    //left up
    for (int i=-1; x+i>-1 && y+i>-1; i--){
        
        if(bot_check_piece_color(x+i, y+i) == color){
            break;
        }
        if (bot_is_valid_attack(x+i, y+i, color)){
            checkmate = sim_move(x, y, x+i, y+i, color, 0, castling, mode);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y+i, color, 0, castling, mode);
        }
    }
    //right up
    for (int i=1; x+i<8 && y-i>-1; i++){
        if(bot_check_piece_color(x+i, y-i) == color){
            break;
        }
        

        if (bot_is_valid_attack(x+i, y-i, color)){
            checkmate = sim_move(x, y, x+i, y-i, color, 0, castling, mode);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y-i, color, 0, castling, mode);
        }
    }
    //right down
    for (int i=1; x+i<8 && y+i<8; i++){
        if(bot_check_piece_color(x+i, y+i) == color){
            break;
        }
        if (bot_is_valid_attack(x+i, y+i, color)){
            checkmate = sim_move(x, y, x+i, y+i, color, 0, castling, mode);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y+i, color, 0, castling, mode);
        }
    }
    //left down
    for (int i=1; x-i>-1 && y+i<8; i++){
        if(bot_check_piece_color(x-i, y+i) == color){
            break;
        }
        if (bot_is_valid_attack(x-i, y+i, color)){
            checkmate = sim_move(x, y, x-i, y+i, color, 0, castling, mode);
            break;
        }else{
            checkmate = sim_move(x, y, x-i, y+i, color, 0, castling, mode);
        }
    }
     //left
     for (int i=-1; x+i>-1; i--){
    
        if(bot_check_piece_color(x+i, y) == color){
            break;
        }
        if (bot_is_valid_attack(x+i, y, color)){
            checkmate = sim_move(x, y, x+i, y, color, 0, castling, mode);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y, color, 0, castling, mode);
        }
    }
    //up
    for (int j=-1; y+j>-1;j--){
        if(bot_check_piece_color(x, y+j) == color){
            break;
        }
        if (bot_is_valid_attack(x, y+j, color)){
            checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode);
            break;
        }else{
            checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode);
        }    }
    //right
    for (int i=1; x+i<8; i++){
        if(bot_check_piece_color(x+i, y) == color){
            break;
        }
        if (bot_is_valid_attack(x+i, y, color)){
            checkmate = sim_move(x, y, x+i, y, color, 0, castling, mode);
            break;
        }else{
            checkmate = sim_move(x, y, x+i, y, color, 0, castling, mode);
        }
    }   
    //down
    for (int j=1; y+j<8;j++){
        if(bot_check_piece_color(x, y+j) == color){
            break;
        }
        if (bot_is_valid_attack(x, y+j, color)){
            checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode);
            break;
        }else{
            checkmate = sim_move(x, y, x, y+j, color, 0, castling, mode);
        }    
    }
    if (mode == 1){
        return checkmate;
    }else if (mode == 2){
        
        return 0;
    }
}
int bot_king_logic(int x, int y, int color, int mode, int *castling){
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
        
    bot_check_for_castle(color, castling);

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
                
            } else if (color == 1){
                bot_bking_x +=i;
                bot_bking_y+=j;
            }
            checkmate = sim_move(x, y, x+i, y+j, color, 0, castling, mode);
            if (color == 0){
                bot_wking_x = bot_temp_king_x;
                bot_wking_y=bot_temp_king_y;
            } else if (color == 1){
                bot_bking_x = bot_temp_king_x;
                bot_bking_y = bot_temp_king_y;
            }
        }
    }
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
int bot_check_for_castle(int color, int *castling){
    if (color == 0){
        int temp_bot_wking_x = bot_wking_x;
        

        if (castling[1]){
            if (bot_board[3][7] == 'o' && bot_board[2][7] == 'o' && bot_board[1][7] == 'o' &&bot_board[0][7] == 'R'){
                if (!bot_search_from_king(color)){
                    bot_board[3][7] = 'K';
                    bot_board[4][7] = 'o';
                    bot_wking_x = 3;
                    
                    if (!bot_search_from_king(color)){
                        bot_board[2][7] = 'K';
                        bot_board[3][7] = 'o';
                        bot_wking_x = 2;
                        if (!bot_search_from_king(color)){
                            add_to_move_list(4, 7, 2, 7, 0, castling, color,'K', 'o');
                        }
                    }
                }
                bot_board[3][7] = 'o';
                bot_board[2][7] = 'o';
                bot_board[4][7] = 'K';
            }
        }
        if (castling[0]){
            if (bot_board[5][7] == 'o' && bot_board[6][7] == 'o' && bot_board[7][7] == 'R'){
                if (!bot_search_from_king(color)){
                    bot_board[5][7] = 'K';
                    bot_board[4][7] = 'o';
                    bot_wking_x = 5;
                    if (!bot_search_from_king(color)){
                        bot_board[6][7] = 'K';
                        bot_board[5][7] = 'o';
                        bot_wking_x = 6;
                        if (!bot_search_from_king(color)){
                            add_to_move_list(4, 7, 6, 7, 0, castling, color,'K', 'o');
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

        if (castling[3]){
            if (bot_board[3][0] == 'o' && bot_board[2][0] == 'o' && bot_board[1][0] == 'o' &&bot_board[0][0] == 'r'){
                if (!bot_search_from_king(color)){
                    bot_board[3][0] = 'k';
                    bot_board[4][0] = 'o';
                    bot_bking_x = 3;
                    if (!bot_search_from_king(color)){
                        bot_board[2][0] = 'k';
                        bot_board[3][0] = 'o';
                        bot_bking_x = 2;
                        if (!bot_search_from_king(color)){
                            add_to_move_list(4, 0, 2, 0, 0, castling, color,'k', 'o');
                        }
                    }
                }
                bot_board[3][0] = 'o';
                bot_board[2][0] = 'o';
                bot_board[4][0] = 'k';
            }
        }
        if (castling[2]){
            if (bot_board[5][0] == 'o' && bot_board[6][0] == 'o' && bot_board[7][0] == 'r'){
                if (!bot_search_from_king(color)){
                    bot_board[5][0] = 'k';
                    bot_board[4][0] = 'o';
                    bot_bking_x = 5;
                    if (!bot_search_from_king(color)){
                        bot_board[6][0] = 'k';
                        bot_board[5][0] = 'o';
                        bot_bking_x = 6;
                        if (!bot_search_from_king(color)){
                            add_to_move_list(4, 0, 6, 0, 0, castling, color,'k', 'o');
                        }
                    }
                }
                bot_board[3][0] = 'o';
                bot_board[2][0] = 'o';
                bot_board[4][0] = 'k';
            }
        }
        bot_bking_x = temp_bot_bking_x;
    }
    
}
int bot_is_checkmate(int *castling){
    
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