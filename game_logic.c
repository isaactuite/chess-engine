#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "game_logic.h"


void update_fake_board(){
    for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
            fake_board[i][j]=board[i][j];
        }
    }
}



int is_in_check(int current_x, int current_y, int future_x, int future_y, int color) {
    piece_logic_for_moving = 0;
    static int recursion_depth = 0;
    if (recursion_depth > 0) {  // Limit recursion depth
        printf("Recursion Limit reached.");
        return 1;
    }
    recursion_depth++;
    fake_board[future_x][future_y] = board[current_x][current_y];
    fake_board[current_x][current_y] = board[future_x][future_y];
    sim_counter = 1; // Reset sim_counter
    memset(squares_under_attack_x, -1, sizeof(squares_under_attack_x));
    memset(squares_under_attack_y, -1, sizeof(squares_under_attack_y));
    // Calculate squares under attack once
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (color == 1) {
                switch (fake_board[i][j]) {
                    case 'Q': queen_logic(i, j, 0); break;
                    case 'P': pawn_logic(i, j, 0); break;
                    case 'R': rook_logic(i, j, 0); break;
                    case 'N': knight_logic(i, j, 0); break;
                    case 'B': bishop_logic(i, j, 0); break;
                    default: break;
                }
            } else if (color == 0) {
                switch (fake_board[i][j]) {
                    case 'q': queen_logic(i, j, 1); break;
                    case 'p': pawn_logic(i, j, 1); break;
                    case 'r': rook_logic(i, j, 1); break;
                    case 'n': knight_logic(i, j, 1); break;
                    case 'b': bishop_logic(i, j, 1); break;
                    default: break;
                }
            }
        }
    }

    piece_logic_for_moving = 1;
    printf("\nSquares under attack by the opponent:\n");
    for (int i = 1; i < squares_under_attack_x[0]; i++) {
        printf("\n(%d,%d)", squares_under_attack_x[i], squares_under_attack_y[i]);
    }
    if (color == 1) {
        for (int i = 1; i < squares_under_attack_x[0]; i++) {
            if (squares_under_attack_x[i] == bking_x && squares_under_attack_y[i] == bking_y) {
                printf("Black king in check at (%d, %d)", squares_under_attack_x[i], squares_under_attack_y[i]);
                return 1;
            }
        }
    } else if (color == 0) {
        for (int i = 1; i < squares_under_attack_x[0]; i++) {
            if (squares_under_attack_x[i] == wking_x && squares_under_attack_y[i] == wking_y) {
                printf("White king in check (%d, %d)", squares_under_attack_x[i], squares_under_attack_y[i]);
                return 1;
            }
        }
    }
    printf("\nsquares_under_attack_x[0] == %d",squares_under_attack_x[0]);
    printf("\nsquares_under_attack_y[0] == %d",squares_under_attack_y[0]);

    fake_board[future_x][future_y] = board[future_x][future_y];
    fake_board[current_x][current_y] = board[current_x][current_y];
    recursion_depth--;
    return 0;
}
void highlight_legal_moves(int x, int y) {
    if (!piece_logic_for_moving){
        return;
    }
    int board_x = x * SQUARE_SIZE + board_start_x;
    int board_y = y * SQUARE_SIZE;


    if (board_x < 0 || board_y < 0 || board_x >= WINDOW_WIDTH || board_y >= WINDOW_HEIGHT) {
        return;
    }

    for (int dy = board_y; dy < board_y + SQUARE_SIZE && dy < WINDOW_HEIGHT; dy++) {
        for (int dx = board_x; dx < board_x + SQUARE_SIZE && dx < WINDOW_WIDTH; dx++) {

            gFrameBuffer[dy * WINDOW_WIDTH + dx] = 0xFFFF80FF;
        }
    }
}

void highlighted_squares(int x, int y, int counter){
    highlighted_squares_x[counter] = x;
    highlighted_squares_y[counter] = y;
    highlighted_squares_x[0] = counter;
    highlighted_squares_y[0] = counter;

}

void squares_under_attack(int x, int y){
    squares_under_attack_x[sim_counter] = x;
    squares_under_attack_y[sim_counter] = y;
    squares_under_attack_x[0]=sim_counter;
    squares_under_attack_y[0]=sim_counter;
    sim_counter++;
}

int check_piece_color(int x, int y){
    if (((x<0) || (y<0))||(x>7)||(y>7)){
        return -1;
    }
    if (piece_logic_for_moving){
        switch(board[x][y]){
            case 'p':
            case 'r':
            case 'n':
            case 'b':
            case 'q':
            case 'k':
                //black
                printf("\nChecked black square at (%d, %d), piece: %c", x, y, board[x][y]);

                return 1;
            case 'P':
            case 'R':
            case 'N':
            case 'B':
            case 'Q':
            case 'K':
                //white
                printf("\nChecked white square at (%d, %d), piece: %c", x, y, board[x][y]);
                return 0;
            case 'o':
                printf("\nChecked empty square at (%d,%d)", x, y);
                return 2;
            default:

                printf("\nchecked invalid char");
                return -1;
        }
    } else{
        switch(fake_board[x][y]){
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

            case 'o':
                return 2;
            default:


                return -1;
        }
    }
}

//function to check if a target is valid
int is_valid_attack(int x, int y, int color){

    int piece_color = check_piece_color(x, y);
    return piece_color == -1 ? 0 : ((piece_color != color) && (piece_color !=2));
}


//individual piece logic
int pawn_logic(int x, int y, int color){
    printf("\nchecking pawn at (%d, %d)", x, y);
    int counter = 1;
    if(piece_logic_for_moving){
        memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
        memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));
    }
    if (color == 1){
        //black
        if (y==1){
            //checks rank to see if black pawn has not moved yet
            for(int j = 1; j<3; j++){

                if (check_piece_color(x, y+j) != 2){
                    //if there is something blocking the pawns file movement, break
                    break;
                //don't have to check for check here, because it's not a capture
                } else if (piece_logic_for_moving){
                    if (!is_in_check(x, y,x, y+j, color)){
                        highlighted_squares(x, y+j, counter);
                        counter++;
                    }
                }
            
            }
        }

        if(piece_logic_for_moving) {
            if (check_piece_color(x, y+1) == 2){
                //If it's not the pawns first move, check the square directly in front of it for empty square
                if (!is_in_check(x, y,x, y+1, color)){
                    highlighted_squares(x, y+1, counter);
                    counter++;
                }
            }
        }

        //checks for pawn captures
        if ((check_piece_color(x-1, y+1) == 0) && ((x-1>=0)&&(y+1<8))){
            if (!piece_logic_for_moving){
                        squares_under_attack(x-1, y+1);
            }else if (piece_logic_for_moving){
                if (!is_in_check(x, y,x-1, y+1, color)){
                    highlighted_squares(x-1, y+1, counter);
                    counter++;
                }
            }
        }
        if ((check_piece_color(x-1, y+1) == 0) && ((x-1>=0)&&(y+1<8))){
            if (!piece_logic_for_moving){
                squares_under_attack(x+1, y+1);
            }else if (piece_logic_for_moving){
                if (!is_in_check(x, y,x+1, y+1, color)){
                    highlighted_squares(x+1, y+1, counter);
                    counter++;
                }
            }
        }
        if (piece_logic_for_moving){
            if ((en_passant_x == x+1)  && (en_passant_y == y)){
                if (!is_in_check(x, y,x+1, y+1, color)){
                    highlighted_squares(x+1, y+1, counter);
                    counter++;
                }
            }
            if ((en_passant_x == x-1)  && (en_passant_y == y)){
                if (!is_in_check(x, y,x-1, y+1, color)){
                    highlighted_squares(x-1, y+1, counter);
                    counter++;
                }
            }
        }
    
    }
    else if (color == 0){
        //white
        if (y==6){
            //checks rank to see if white pawn has not moved yet
            for(int j = -1; j>-3; j--){

                if (check_piece_color(x, y+j) != 2){
                    //if there is something blocking the pawns file movement, break

                    break;
                }else if (piece_logic_for_moving){
                    if (!is_in_check(x, y,x, y+j, color)){
                        highlighted_squares(x, y+j, counter);
                        counter++;
                    }
                }
            
            }
        }if(piece_logic_for_moving) {
            if (check_piece_color(x, y+1) == 2){
                if (!is_in_check(x, y,x, y-1, color)){
                    //If it's not the pawns first move, check the square directly in front of it for empty square
                    highlighted_squares(x, y-1, counter);
                    counter++;
                }
            }
        }

        //checks for pawn captures
        if ((check_piece_color(x-1, y-1) == 1) && ((x-1>=0)&&(y-1>=0))){
            if (!piece_logic_for_moving){
                squares_under_attack(x-1, y-1);
            }else if (piece_logic_for_moving){
                if(!is_in_check(x, y,x-1, y-1, color)){
                    highlighted_squares(x-1, y-1, counter);

                    counter++;
                }
            }
        }
        if ((check_piece_color(x+1, y-1) == 1) && ((x+1>=0)&&(y-1>=0))){
            if (!piece_logic_for_moving){
                squares_under_attack(x+1, y-1);
            }else if (piece_logic_for_moving){
                if (!is_in_check(x,y,x+1, y-1,color)){
                    highlighted_squares(x+1, y-1, counter);
                    counter++;
                }
            }
        }
        if (piece_logic_for_moving){
            if ((en_passant_x == x+1)  && (en_passant_y == y)){
                if (!is_in_check(x, y,x+1, y-1, color)){
                    highlighted_squares(x+1, y-1, counter);

                    counter++;
                }
            }
            if ((en_passant_x == x-1)  && (en_passant_y == y)){
                if (!is_in_check(x, y,x-1, y-1, color)){
                    highlighted_squares(x-1, y-1, counter);

                    counter++;
                }
            }
        }
    }


    for (int i=1; i<counter+1; i++){
        highlight_legal_moves(highlighted_squares_x[i],highlighted_squares_y[i]);
    }

}

int knight_logic(int x, int y, int color){
    printf("\nchecking knight at (%d, %d)", x, y);
    if(piece_logic_for_moving){
        memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
        memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));
    }
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
            
            if(!piece_logic_for_moving){
                if ((x+i>=0 && x+i<=7)&&(y+j>=0 && y+j<=7)){
                    squares_under_attack(x+i, y+j);
                }
            }else{
                
                if ((x+i>=0 && x+i<=7)&&(y+j>=0 && y+j<=7)){
                    if(!is_in_check(x, y,x+i, y+j, color)){
                        if (is_valid_attack(x + i, y + j, color)){
                            highlighted_squares(x+i, y+j, counter);

                            counter++;
                        } else if (check_piece_color(x+i, y+j)==2){
                            highlighted_squares(x+i, y+j, counter);
                            counter++;
                        }

                    }
                }
            }
        }
    }

    


    for (int i=1; i<counter+1; i++){
        highlight_legal_moves(highlighted_squares_x[i],highlighted_squares_y[i]);
    }
}
int rook_logic(int x, int y, int color){
    int counter = 1;
    printf("\nchecking rook at (%d, %d)", x, y);
    if(piece_logic_for_moving){
        memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
        memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));
    }
    //left
    for (int i=-1; x+i>-1; i--){
        
        if (piece_logic_for_moving){
            if(check_piece_color(x+i, y) == color){
                break;
            }
            if (is_valid_attack(x + i, y, color)){
                if (!is_in_check(x,y,x+i, y,color)){
                    highlighted_squares(x+i, y, counter);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x+i, y) == 2){
                
                if (!is_in_check(x,y,x+i, y,color)){
                    highlighted_squares(x+i, y, counter);
                    counter+=1;
                }
            }
        }else if(!piece_logic_for_moving){
            if(check_piece_color(x+i, y) == color){
                squares_under_attack(x+i, y);
                break;
            }
            if (is_valid_attack(x + i, y, color)){
                squares_under_attack(x+i, y);
                counter+=1;
                break;
            }else if (check_piece_color(x+i, y) == 2){
                squares_under_attack(x+i, y);
                counter+=1; 
            }
        }
    }
    //up
    
    for (int j=-1; y+j>-1;j--){
        
        if (piece_logic_for_moving){
            if(check_piece_color(x, y+j) == color){
                break;
            }
            if (is_valid_attack(x, y+j, color)){
                if (!is_in_check(x,y,x, y+j,color)){
                    highlighted_squares(x, y+j, counter);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x, y+j) == 2){
                
                if (!is_in_check(x,y,x, y+j,color)){
                    highlighted_squares(x, y+j, counter);
                    counter+=1;
                }
            }
        }else if(!piece_logic_for_moving){
            if(check_piece_color(x, y+j) == color){
                squares_under_attack(x, y+j);
                break;
            }
            if (is_valid_attack(x, y+j, color)){
                squares_under_attack(x, y+j);
                counter+=1;
                break;
            }else if (check_piece_color(x, y+j) == 2){
                squares_under_attack(x, y+j);
                counter+=1;
            }
        }
    }
    //right
    
    for (int i=1; x+i<8; i++){
        
        if (piece_logic_for_moving){
            if(check_piece_color(x+i, y) == color){
                break;
            }
            if (is_valid_attack(x + i, y, color)){
                if (!is_in_check(x,y,x+i, y,color)){
                    highlighted_squares(x+i, y, counter);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x+i, y) == 2){
                
                if (!is_in_check(x,y,x+i, y,color)){
                    highlighted_squares(x+i, y, counter);
                    counter+=1;
                }
            }
        }else if(!piece_logic_for_moving){
            if(check_piece_color(x+i, y) == color){
                squares_under_attack(x+i, y);
                break;
            }
            if (is_valid_attack(x + i, y, color)){
                squares_under_attack(x+i, y);
                counter+=1;
                break;
            }else if (check_piece_color(x+i, y) == 2){
                squares_under_attack(x+i, y);
                counter+=1;
            }
        }
    }   
    //down

    for (int j=1; y+j<8;j++){
        
        if (piece_logic_for_moving){
            if(check_piece_color(x, y+j) == color){
                break;
            }
            if (is_valid_attack(x, y+j, color)){
                if (!is_in_check(x,y,x, y+j,color)){
                    highlighted_squares(x, y+j, counter);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x, y+j) == 2){
                
                if (!is_in_check(x,y,x, y+j,color)){
                    highlighted_squares(x, y+j, counter);
                    counter+=1;
                }
            }
        }else if(!piece_logic_for_moving){
            if(check_piece_color(x, y+j) == color){
                squares_under_attack(x, y+j);
                break;
            }
            if (is_valid_attack(x, y+j, color)){
                squares_under_attack(x, y+j);
                counter+=1;
                break;
            }else if (check_piece_color(x, y+j) == 2){
            
                squares_under_attack(x, y+j);
                counter+=1;
                
            }
        }
    }

    
    for (int i=1; i<counter+1; i++){

        highlight_legal_moves(highlighted_squares_x[i],highlighted_squares_y[i]);

    }
}
int bishop_logic(int x, int y, int color){
    printf("\nchecking bishop at (%d, %d)", x, y);
    if(piece_logic_for_moving){
        memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
        memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));
    }
    int counter = 1;
    //left up
    for (int i=-1; x+i>-1 && y+i>-1; i--){
        if (piece_logic_for_moving){
            if(check_piece_color(x+i, y+i) == color){
                break;
            }
            if (is_valid_attack(x+i, y+i, color)){
                if (!is_in_check(x,y,x+i, y+i,color)){
                    highlighted_squares(x+i, y+i, counter);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x+i, y+i) == 2){
                if (!is_in_check(x,y,x+i, y+i,color)){
                    highlighted_squares(x+i, y+i, counter);
                    counter+=1;
                }
            }
        } else if (!piece_logic_for_moving){
            if(check_piece_color(x+i, y+i) == color){
                squares_under_attack(x+i, y+i);
                break;
            }
            if (is_valid_attack(x+i, y+i, color)){
                if (!is_in_check(x,y,x+i, y+i,color)){
                    squares_under_attack(x+i, y+i);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x+i, y+i) == 2){
                if (!is_in_check(x,y,x+i, y+i,color)){
                    squares_under_attack(x+i, y+i);
                    counter+=1;
                }
            }
        }
    }
    //right up
    for (int i=1; x+i<8 && y-i>-1; i++){
        if (piece_logic_for_moving){
            if(check_piece_color(x+i, y-i) == color){
                break;
            }
            if (is_valid_attack(x+i, y-i, color)){
                if (!is_in_check(x,y,x+i, y-i,color)){
                    highlighted_squares(x+i, y-i, counter);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x+i, y-i) == 2){
                if (!is_in_check(x,y,x+i, y-i,color)){
                    highlighted_squares(x+i, y-i, counter);
                    counter+=1;
                }
            }
        } else if (!piece_logic_for_moving){
            if(check_piece_color(x+i, y-i) == color){
                squares_under_attack(x+i, y-i);
                break;
            }
            if (is_valid_attack(x+i, y-i, color)){
                if (!is_in_check(x,y,x+i, y-i,color)){
                    squares_under_attack(x+i, y-i);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x+i, y-i) == 2){
                if (!is_in_check(x,y,x+i, y-i,color)){
                    squares_under_attack(x+i, y-i);
                    counter+=1;
                }
            }
        }
    }
    //right down
    for (int i=1; x+i<8 && y+i<8; i++){
        if (piece_logic_for_moving){
            if(check_piece_color(x+i, y+i) == color){
                break;
            }
            if (is_valid_attack(x+i, y+i, color)){
                if (!is_in_check(x,y,x+i, y+i,color)){
                    highlighted_squares(x+i, y+i, counter);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x+i, y+i) == 2){
                if (!is_in_check(x,y,x+i, y+i,color)){
                    highlighted_squares(x+i, y+i, counter);
                    counter+=1;
                }
            }
        } else if (!piece_logic_for_moving){
            if(check_piece_color(x+i, y+i) == color){
                squares_under_attack(x+i, y+i);
                break;
            }
            if (is_valid_attack(x+i, y+i, color)){
                if (!is_in_check(x,y,x+i, y+i,color)){
                    squares_under_attack(x+i, y+i);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x+i, y+i) == 2){
                if (!is_in_check(x,y,x+i, y+i,color)){
                    squares_under_attack(x+i, y+i);
                    counter+=1;
                }
            }
        }
    }
    //left down
    for (int i=1; x-i>-1 && y+i<8; i++){
        if (piece_logic_for_moving){
            if(check_piece_color(x-i, y+i) == color){
                break;
            }
            if (is_valid_attack(x-i, y+i, color)){
                if (!is_in_check(x,y,x-i, y+i,color)){
                    highlighted_squares(x-i, y+i, counter);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x-i, y+i) == 2){
                if (!is_in_check(x,y,x-i, y+i,color)){
                    highlighted_squares(x-i, y+i, counter);
                    counter+=1;
                }
            }
        } else if (!piece_logic_for_moving){
            if(check_piece_color(x-i, y+i) == color){
                squares_under_attack(x-i, y+i);
                break;
            }
            if (is_valid_attack(x-i, y+i, color)){
                if (!is_in_check(x,y,x-i, y+i,color)){
                    squares_under_attack(x-i, y+i);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x-i, y+i) == 2){
                if (!is_in_check(x,y,x-i, y+i,color)){
                    squares_under_attack(x-i, y+i);
                    counter+=1;
                }
            }
        }
    }

    for (int i=1; i<counter+1; i++){

        highlight_legal_moves(highlighted_squares_x[i],highlighted_squares_y[i]);

    }
}
int queen_logic(int x, int y, int color){
    printf("\nchecking queen at (%d, %d)", x, y);
    int counter = 1;
    if(piece_logic_for_moving){
        memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
        memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));
    }
    for (int i=-1; x+i>-1 && y+i>-1; i--){
        if (piece_logic_for_moving){
            if(check_piece_color(x+i, y+i) == color){
                break;
            }
            if (is_valid_attack(x+i, y+i, color)){
                if (!is_in_check(x,y,x+i, y+i,color)){
                    highlighted_squares(x+i, y+i, counter);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x+i, y+i) == 2){
                if (!is_in_check(x,y,x+i, y+i,color)){
                    highlighted_squares(x+i, y+i, counter);
                    counter+=1;
                }
            }
        } else if (!piece_logic_for_moving){
            if(check_piece_color(x+i, y+i) == color){
                squares_under_attack(x+i, y+i);
                break;
            }
            if (is_valid_attack(x+i, y+i, color)){
                if (!is_in_check(x,y,x+i, y+i,color)){
                    squares_under_attack(x+i, y+i);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x+i, y+i) == 2){
                if (!is_in_check(x,y,x+i, y+i,color)){
                    squares_under_attack(x+i, y+i);
                    counter+=1;
                }
            }
        }
    }
    //right up
    for (int i=1; x+i<8 && y-i>-1; i++){
        if (piece_logic_for_moving){
            if(check_piece_color(x+i, y-i) == color){
                break;
            }
            if (is_valid_attack(x+i, y-i, color)){
                if (!is_in_check(x,y,x+i, y-i,color)){
                    highlighted_squares(x+i, y-i, counter);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x+i, y-i) == 2){
                if (!is_in_check(x,y,x+i, y-i,color)){
                    highlighted_squares(x+i, y-i, counter);
                    counter+=1;
                }
            }
        } else if (!piece_logic_for_moving){
            if(check_piece_color(x+i, y-i) == color){
                squares_under_attack(x+i, y-i);
                break;
            }
            if (is_valid_attack(x+i, y-i, color)){
                if (!is_in_check(x,y,x+i, y-i,color)){
                    squares_under_attack(x+i, y-i);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x+i, y-i) == 2){
                if (!is_in_check(x,y,x+i, y-i,color)){
                    squares_under_attack(x+i, y-i);
                    counter+=1;
                }
            }
        }
    }
    //right down
    for (int i=1; x+i<8 && y+i<8; i++){
        if (piece_logic_for_moving){
            if(check_piece_color(x+i, y+i) == color){
                break;
            }
            if (is_valid_attack(x+i, y+i, color)){
                if (!is_in_check(x,y,x+i, y+i,color)){
                    highlighted_squares(x+i, y+i, counter);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x+i, y+i) == 2){
                if (!is_in_check(x,y,x+i, y+i,color)){
                    highlighted_squares(x+i, y+i, counter);
                    counter+=1;
                }
            }
        } else if (!piece_logic_for_moving){
            if(check_piece_color(x+i, y+i) == color){
                squares_under_attack(x+i, y+i);
                break;
            }
            if (is_valid_attack(x+i, y+i, color)){
                if (!is_in_check(x,y,x+i, y+i,color)){
                    squares_under_attack(x+i, y+i);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x+i, y+i) == 2){
                if (!is_in_check(x,y,x+i, y+i,color)){
                    squares_under_attack(x+i, y+i);
                    counter+=1;
                }
            }
        }
    }
    //left down
    for (int i=1; x-i>-1 && y+i<8; i++){
        if (piece_logic_for_moving){
            if(check_piece_color(x-i, y+i) == color){
                break;
            }
            if (is_valid_attack(x-i, y+i, color)){
                if (!is_in_check(x,y,x-i, y+i,color)){
                    highlighted_squares(x-i, y+i, counter);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x-i, y+i) == 2){
                if (!is_in_check(x,y,x-i, y+i,color)){
                    highlighted_squares(x-i, y+i, counter);
                    counter+=1;
                }
            }
        } else if (!piece_logic_for_moving){
            if(check_piece_color(x-i, y+i) == color){
                squares_under_attack(x-i, y+i);
                break;
            }
            if (is_valid_attack(x-i, y+i, color)){
                if (!is_in_check(x,y,x-i, y+i,color)){
                    squares_under_attack(x-i, y+i);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x-i, y+i) == 2){
                if (!is_in_check(x,y,x-i, y+i,color)){
                    squares_under_attack(x-i, y+i);
                    counter+=1;
                }
            }
        }
    }
    //left
    for (int i=-1; x+i>-1; i--){
        
        if (piece_logic_for_moving){
            if(check_piece_color(x+i, y) == color){
                break;
            }
            if (is_valid_attack(x + i, y, color)){
                if (!is_in_check(x,y,x+i, y,color)){
                    highlighted_squares(x+i, y, counter);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x+i, y) == 2){
                
                if (!is_in_check(x,y,x+i, y,color)){
                    highlighted_squares(x+i, y, counter);
                    counter+=1;
                }
            }
        }else if(!piece_logic_for_moving){
            if(check_piece_color(x+i, y) == color){
                squares_under_attack(x+i, y);
                break;
            }
            if (is_valid_attack(x + i, y, color)){
                squares_under_attack(x+i, y);
                counter+=1;
                break;
            }else if (check_piece_color(x+i, y) == 2){
                squares_under_attack(x+i, y);
                counter+=1; 
            }
        }
    }
    //up
    for (int j=-1; y+j>-1;j--){
        if (piece_logic_for_moving){
            if(check_piece_color(x, y+j) == color){
                break;
            }
            if (is_valid_attack(x, y+j, color)){
                if (!is_in_check(x,y,x, y+j,color)){
                    highlighted_squares(x, y+j, counter);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x, y+j) == 2){
                
                if (!is_in_check(x,y,x, y+j,color)){
                    highlighted_squares(x, y+j, counter);
                    counter+=1;
                }
            }
        }else if(!piece_logic_for_moving){
            if(check_piece_color(x, y+j) == color){
                squares_under_attack(x, y+j);
                break;
            }
            if (is_valid_attack(x, y+j, color)){
                squares_under_attack(x, y+j);
                counter+=1;
                break;
            }else if (check_piece_color(x, y+j) == 2){
                squares_under_attack(x, y+j);
                counter+=1;
            }
        }
    }
    //right
    for (int i=1; x+i<8; i++){
        if (piece_logic_for_moving){
            if(check_piece_color(x+i, y) == color){
                break;
            }
            if (is_valid_attack(x + i, y, color)){
                if (!is_in_check(x,y,x+i, y,color)){
                    highlighted_squares(x+i, y, counter);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x+i, y) == 2){
                
                if (!is_in_check(x,y,x+i, y,color)){
                    highlighted_squares(x+i, y, counter);
                    counter+=1;
                }
            }
        }else if(!piece_logic_for_moving){
            if(check_piece_color(x+i, y) == color){
                squares_under_attack(x+i, y);
                break;
            }
            if (is_valid_attack(x + i, y, color)){
                squares_under_attack(x+i, y);
                counter+=1;
                break;
            }else if (check_piece_color(x+i, y) == 2){
                squares_under_attack(x+i, y);
                counter+=1;
            }
        }
    }   
    //down
    for (int j=1; y+j<8;j++){
        if (piece_logic_for_moving){
            if(check_piece_color(x, y+j) == color){
                break;
            }
            if (is_valid_attack(x, y+j, color)){
                if (!is_in_check(x,y,x, y+j,color)){
                    highlighted_squares(x, y+j, counter);
                    counter+=1;
                    break;
                }
            }else if (check_piece_color(x, y+j) == 2){
                
                if (!is_in_check(x,y,x, y+j,color)){
                    highlighted_squares(x, y+j, counter);
                    counter+=1;
                }
            }
        }else if(!piece_logic_for_moving){
            if(check_piece_color(x, y+j) == color){
                squares_under_attack(x, y+j);
                break;
            }
            if (is_valid_attack(x, y+j, color)){
                squares_under_attack(x, y+j);
                counter+=1;
                break;
            }else if (check_piece_color(x, y+j) == 2){
            
                squares_under_attack(x, y+j);
                counter+=1;
                
            }
        }
    }

    for (int i=1; i<counter+1; i++){
        
        highlight_legal_moves(highlighted_squares_x[i],highlighted_squares_y[i]);
    }

}
int king_logic(int x, int y, int color){
    printf("\nchecking King at (%d, %d)", x, y);
    if(piece_logic_for_moving){
        memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
        memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));
    }
    int counter = 1;
    

    for(int i = -1;i<2; i++){
        if (x+i>8 ||x+i<0){
            continue;
        }
        for (int j =-1; j<2;j++){
            if (x+j>8 ||x+j<0){
                continue;
            }if (piece_logic_for_moving){
                if (!is_in_check(x, y, x+i, y+j, color)){

                    
                    if((j == i) &&(j == 0)){
                        continue;

                    }else if (is_valid_attack(x + i, y + j, color)){
                        highlighted_squares(x+i, y+j, counter);
                        counter++;
                    } else if (check_piece_color(x+i, y+j)==2){
                        highlighted_squares(x+i, y+j, counter);
                        counter++;
                    } 
                }
            } else if (!piece_logic_for_moving){
                squares_under_attack(x+i, y+j);
            }

        }
    }
    

    for (int i=1; i<counter+1; i++){
        highlight_legal_moves(highlighted_squares_x[i],highlighted_squares_y[i]);
    }
}


int legal_moves(int x, int y){
    /*
    Function takes in the coordinates of a piece, the current state of the board, and what color the piece is.
    x can range from (0,7)
    y can range from (0,7)
    Board is a 8x8 2d matrix.
    Color can be -1, 0, 2, 3. Invalid char is -1, black is 1, white is 0, empty is 2.
    */
    
    int color = check_piece_color(x, y);
    printf("\nlegal_moves recieved:(%d,%d)", x, y);
    switch (board[x][y]){
        case 'P': case 'p':
            printf("\nPawn Selected");
            pawn_logic(x, y, color);
            break;
        case 'N': case 'n':
            printf("\nKnight selected");
            knight_logic(x, y, color);
            break;
        case 'B': case 'b':
            printf("\nBishop selected");
            bishop_logic(x, y, color);
            break;
        case 'R': case 'r':
            printf("\nRook selected");
            rook_logic(x, y,color);
            break;
        
        case 'Q': case 'q':
            printf("\nQueen selected");
            queen_logic(x, y, color);
            break;
        case 'K': case 'k':
            printf("\nKing selected");
            king_logic(x, y, color);
            break;
        default:
            printf("\nlegal_moves called, invalid");
            break;
    }
}
