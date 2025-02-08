#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "game_logic.h"




void highlight_legal_moves(int x, int y) {

    int board_x = x * SQUARE_SIZE + board_start_x;
    int board_y = y * SQUARE_SIZE;


    if (board_x < 0 || board_y < 0 || board_x >= WINDOW_WIDTH || board_y >= WINDOW_HEIGHT) {
        return;
    }

    for (int dy = board_y; dy < board_y + SQUARE_SIZE && dy < WINDOW_HEIGHT; dy++) {
        for (int dx = board_x; dx < board_x + SQUARE_SIZE && dx < WINDOW_WIDTH; dx++) {

            gFrameBuffer[dy * WINDOW_WIDTH + dx] = 0x80FF80FF;
        }
    }
}

int check_piece_color(int x, int y){
    if (((x<0) || (y<0))||(x>7)||(y>7)){
        return -1;
    }
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
}

//function to check if a target is valid
int is_valid_attack(int x, int y, int color){

    int piece_color = check_piece_color(x, y);
    return piece_color == -1 ? 0 : ((piece_color != color) && (piece_color !=2));

}


//individual piece logic
int pawn_logic(int x, int y, int color){
    int legal_pawn_moves_x[8];
    int legal_pawn_moves_y[8];
    int counter = 0;
    if (color == 1){
        //black
        if (y==1){
            //checks rank to see if black pawn has not moved yet
            for(int j = 1; j<3; j++){

                if (check_piece_color(x, y+j) != 2){
                    //if there is something blocking the pawns file movement, break
                    break;
                } else {
                    legal_pawn_moves_x[counter] = x;
                    legal_pawn_moves_y[counter] = y+j;
                    counter++;
                }
            
            }
        } else{
            if (check_piece_color(x, y+1) == 2){
                //If it's not the pawns first move, check the square directly in front of it for empty square
                legal_pawn_moves_x[counter] = x;
                legal_pawn_moves_y[counter] = y+1;
                counter++;
            }
        }

        //checks for pawn captures
        if ((check_piece_color(x-1, y+1) == 0) && ((x-1>=0)&&(y+1<8))){
            legal_pawn_moves_x[counter] = x-1;
            legal_pawn_moves_y[counter] = y+1;
            counter++;
        }
        if ((check_piece_color(x+1, y+1) == 0) && ((x+1>=0)&&(y+1<8))){
            legal_pawn_moves_x[counter] = x+1;
            legal_pawn_moves_y[counter] = y+1;
            counter++;
        }

    }else if (color == 0){
        //white
        if (y==6){
            //checks rank to see if white pawn has not moved yet
            for(int j = -1; j>-3; j--){

                if (check_piece_color(x, y+j) != 2){
                    //if there is something blocking the pawns file movement, break

                    break;
                } else {
                    legal_pawn_moves_x[counter] = x;
                    legal_pawn_moves_y[counter] = y+j;
                    counter++;
                }
            
            }
        } else{
            if (check_piece_color(x, y-1) == 2){
                //If it's not the pawns first move, check the square directly in front of it for empty square
                legal_pawn_moves_x[counter] = x;
                legal_pawn_moves_y[counter] = y-1;
                counter++;
            }
        }

        //checks for pawn captures
        if ((check_piece_color(x-1, y-1) == 1) && ((x-1>=0)&&(y-1<8))){
            legal_pawn_moves_x[counter] = x-1;
            legal_pawn_moves_y[counter] = y-1;
            counter++;
        }
        if ((check_piece_color(x+1, y-1) == 1) && ((x+1>=0)&&(y-1<8))){
            legal_pawn_moves_x[counter] = x+1;
            legal_pawn_moves_y[counter] = y-1;
            counter++;
        }
        

        
    }
    memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
    memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y)); //sets every value in the array to -1

    printf("\nCounter: %d", counter);
    printf("\nPossible Moves:\n");

    highlighted_squares_x[0] = counter;
    highlighted_squares_y[0] = counter;

    for (int i=0; i<counter; i++){
        printf("(%d, %d)",legal_pawn_moves_x[i],legal_pawn_moves_y[i]);
        highlight_legal_moves(legal_pawn_moves_x[i],legal_pawn_moves_y[i]);
        highlighted_squares_x[i+1] = legal_pawn_moves_x[i];
        highlighted_squares_y[i+1] = legal_pawn_moves_y[i];
    }
    
}
int knight_logic(int x, int y, int color){
    int legal_knight_moves_x[8]; //8 possible knight moves
    int legal_knight_moves_y[8];
    int counter=0;

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
                if (is_valid_attack(x + i, y + j, color)){
                    legal_knight_moves_x[counter] = x+i;
                    legal_knight_moves_y[counter] = y+j; //changed x to y, don't know if itll work
                    counter++;
                } else if (check_piece_color(x, y)==2){
                    legal_knight_moves_x[counter] = x+i;
                    legal_knight_moves_y[counter] = y+j; 
                    counter++;
                }

            }
        }
    }
    memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
    memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));
    highlighted_squares_x[0] = counter;
    highlighted_squares_y[0] = counter;

    printf("\nPossible Moves:\n");
    for (int i=0; i<counter; i++){
        printf("(%d, %d)",legal_knight_moves_x[i],legal_knight_moves_y[i]);
        highlight_legal_moves(legal_knight_moves_x[i],legal_knight_moves_y[i]);
        
        highlighted_squares_x[i+1] = legal_knight_moves_x[i];
        highlighted_squares_y[i+1] = legal_knight_moves_y[i];
    }
}
int rook_logic(int x, int y, int color){
    int counter = 0;
    int legal_rook_moves_x[14];
    int legal_rook_moves_y[14];
    //left
    for (int i=-1; x+i>-1; i--){
        if(check_piece_color(x+i, y) == color){
            break;
        }
        if (is_valid_attack(x + i, y, color)){
            legal_rook_moves_x[counter]=x+i;
            legal_rook_moves_y[counter]=y;
            counter+=1;
            break;
        }else if (check_piece_color(x+i, y) == 2){
            legal_rook_moves_x[counter]=x+i;
            legal_rook_moves_y[counter]=y;
            counter+=1;
        }
    }
    //up
    for (int j=-1; y+j>-1;j--){
        if(check_piece_color(x, y+j) == color){
            break;
        }
        if (is_valid_attack(x, y+j, color)){
            legal_rook_moves_x[counter]=x;
            legal_rook_moves_y[counter]=y+j;
            counter+=1;
            break;
        }else if (check_piece_color(x, y+j) == 2){
            legal_rook_moves_x[counter]=x;
            legal_rook_moves_y[counter]=y+j;
            counter+=1;
        }
    }
    //right
    for (int i=1; x+i<8; i++){
        if(check_piece_color(x+i, y) == color){
            break;
        }
        if (is_valid_attack(x + i, y, color)){
            legal_rook_moves_x[counter]=x+i;
            legal_rook_moves_y[counter]=y;
            counter+=1;
            break;
        }else if (check_piece_color(x+i, y) == 2){
            legal_rook_moves_x[counter]=x+i;
            legal_rook_moves_y[counter]=y;
            counter+=1;
        }
    }   
    //down
    for (int j=1; y+j<8;j++){
        if(check_piece_color(x, y+j) == color){
            break;
        }
        if (is_valid_attack(x, y+j, color)){
            legal_rook_moves_x[counter]=x;
            legal_rook_moves_y[counter]=y+j;
            counter+=1;
            break;
        }else if (check_piece_color(x, y+j) == 2){
            legal_rook_moves_x[counter]=x;
            legal_rook_moves_y[counter]=y+j;
            counter+=1;
        }
    }
    memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
    memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));
    highlighted_squares_x[0] = counter;
    highlighted_squares_y[0] = counter;

    printf("\nPossible Moves:\n");
    for (int i=0; i<counter; i++){
        printf("(%d, %d)",legal_rook_moves_x[i], legal_rook_moves_y[i]);
        highlight_legal_moves(legal_rook_moves_x[i],legal_rook_moves_y[i]);
        highlighted_squares_x[i+1] = legal_rook_moves_x[i];
        highlighted_squares_y[i+1] = legal_rook_moves_y[i];
    }
}
int bishop_logic(int x, int y, int color){
    int legal_bishop_moves_x[13];
    int legal_bishop_moves_y[13];
    int counter = 0;
    //left up
    for (int i=-1; x+i>-1 && y+i>-1; i--){
        if(check_piece_color(x+i, y+i) == color){
            break;
        }
        if (is_valid_attack(x+i, y+i, color)){
            legal_bishop_moves_x[counter]=x+i;
            legal_bishop_moves_y[counter]=y+i;
            counter+=1;
            break;
        }else if (check_piece_color(x+i, y+i) == 2){
            legal_bishop_moves_x[counter]=x+i;
            legal_bishop_moves_y[counter]=y+i;
            counter+=1;
        }
    }
    //right up
    for (int i=1; x+i<8 && y-i>-1; i++){
        if(check_piece_color(x+i, y-i) == color){
            break;
        }
        if (is_valid_attack(x+i, y-i,  color)){
            legal_bishop_moves_x[counter]=x+i;
            legal_bishop_moves_y[counter]=y-i;
            counter+=1;
            break;
        }else if (check_piece_color(x+i, y-i) == 2){
            legal_bishop_moves_x[counter]=x+i;
            legal_bishop_moves_y[counter]=y-i;
            counter+=1;
        }
    }
    //right down
    for (int i=1; x+i<8 && y+i<8; i++){
        if(check_piece_color(x+i, y+i) == color){
            break;
        }
        if (is_valid_attack(x+i, y+i, color)){
            legal_bishop_moves_x[counter]=x+i;
            legal_bishop_moves_y[counter]=y+i;
            counter+=1;
            break;
        }else if (check_piece_color(x+i, y+i) == 2){
            legal_bishop_moves_x[counter]=x+i;
            legal_bishop_moves_y[counter]=y+i;
            counter+=1;
        }
    }
    //left down
    for (int i=1; x-i>-1 && y+i<8; i++){
        if(check_piece_color(x-i, y+i) == color){
            break;
        }
        if (is_valid_attack(x-i, y+i, color)){
            legal_bishop_moves_x[counter]=x-i;
            legal_bishop_moves_y[counter]=y+i;
            counter+=1;
            break;
        }else if (check_piece_color(x-i, y+i) == 2){
            legal_bishop_moves_x[counter]=x-i;
            legal_bishop_moves_y[counter]=y+i;
            counter+=1;
        }
    }
    memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
    memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));
    highlighted_squares_x[0] = counter;
    highlighted_squares_y[0] = counter;

    printf("\nPossible Moves:\n");
    for (int i=0; i<counter; i++){
        printf("(%d, %d)",legal_bishop_moves_x[i],legal_bishop_moves_y[i]);
        highlight_legal_moves(legal_bishop_moves_x[i],legal_bishop_moves_y[i]);
        highlighted_squares_x[i+1] = legal_bishop_moves_x[i];
        highlighted_squares_y[i+1] = legal_bishop_moves_y[i];
    }
}
int queen_logic(int x, int y, int color){
    int counter = 0;
    int legal_queen_moves_x[27];
    int legal_queen_moves_y[27];
     //left up
     for (int i=-1; x+i>-1 && y+i>-1; i--){
        if(check_piece_color(x+i, y+i) == color){
            break;
        }
        if (is_valid_attack(x+i, y+i, color)){
            legal_queen_moves_x[counter]=x+i;
            legal_queen_moves_y[counter]=y+i;
            counter+=1;
            break;
        }else if (check_piece_color(x+i, y+i) == 2){
            legal_queen_moves_x[counter]=x+i;
            legal_queen_moves_y[counter]=y+i;
            counter+=1;
        }
    }
    //right up
    for (int i=1; x+i<8 && y-i>-1; i++){
        if(check_piece_color(x+i, y-i) == color){
            break;
        }
        if (is_valid_attack(x+i, y-i, color)){
            legal_queen_moves_x[counter]=x+i;
            legal_queen_moves_y[counter]=y-i;
            counter+=1;
            break;
        }else if (check_piece_color(x+i, y-i) == 2){
            legal_queen_moves_x[counter]=x+i;
            legal_queen_moves_y[counter]=y-i;
            counter+=1;
        }
    }
    //right down
    for (int i=1; x+i<8 && y+i<8; i++){
        if(check_piece_color(x+i, y+i) == color){
            break;
        }
        if (is_valid_attack(x+i, y+i, color)){
            legal_queen_moves_x[counter]=x+i;
            legal_queen_moves_y[counter]=y+i;
            counter+=1;
            break;
        }else if (check_piece_color(x+i, y+i) == 2){
            legal_queen_moves_x[counter]=x+i;
            legal_queen_moves_y[counter]=y+i;
            counter+=1;
        }
    }
    //left down
    for (int i=1; x-i>-1 && y+i<8; i++){
        if(check_piece_color(x-i, y+i) == color){
            break;
        }
        if (is_valid_attack(x-i, y+i, color)){
            legal_queen_moves_x[counter]=x-i;
            legal_queen_moves_y[counter]=y+i;
            counter+=1;
            break;
        }else if (check_piece_color(x-i, y+i) == 2){
            legal_queen_moves_x[counter]=x-i;
            legal_queen_moves_y[counter]=y+i;
            counter+=1;
        }
    }

    for (int i=-1; x+i>-1; i--){
        if(check_piece_color(x+i, y) == color){
            break;
        }
        if (is_valid_attack(x + i, y, color)){
            legal_queen_moves_x[counter]=x+i;
            legal_queen_moves_y[counter]=y;
            counter+=1;
            break;
        }else if (check_piece_color(x+i, y) == 2){
            legal_queen_moves_x[counter]=x+i;
            legal_queen_moves_y[counter]=y;
            counter+=1;
        }
    }
    //up
    for (int j=-1; y+j>-1;j--){
        if(check_piece_color(x, y+j) == color){
            break;
        }
        if (is_valid_attack(x, y+j, color)){
            legal_queen_moves_x[counter]=x;
            legal_queen_moves_y[counter]=y+j;
            counter+=1;
            break;
        }else if (check_piece_color(x, y+j) == 2){
            legal_queen_moves_x[counter]=x;
            legal_queen_moves_y[counter]=y+j;
            counter+=1;
        }
    }
    //right
    for (int i=1; x+i<8; i++){
        if(check_piece_color(x+i, y) == color){
            break;
        }
        if (is_valid_attack(x + i, y, color)){
            legal_queen_moves_x[counter]=x+i;
            legal_queen_moves_y[counter]=y;
            counter+=1;
            break;
        }else if (check_piece_color(x+i, y) == 2){
            legal_queen_moves_x[counter]=x+i;
            legal_queen_moves_y[counter]=y;
            counter+=1;
        }
    }
    //down
    for (int j=1; y+j<8;j++){
        if(check_piece_color(x, y+j) == color){
            break;
        }
        if (is_valid_attack(x, y+j, color)){
            legal_queen_moves_x[counter]=x;
            legal_queen_moves_y[counter]=y+j;
            counter+=1;
            break;
        }else if (check_piece_color(x, y+j) == 2){
            legal_queen_moves_x[counter]=x;
            legal_queen_moves_y[counter]=y+j;
            counter+=1;
        }
    }

    memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
    memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));
    highlighted_squares_x[0] = counter;
    highlighted_squares_y[0] = counter;

    printf("\nPossible Moves:\n");
    for (int i=0; i<counter; i++){
        printf("(%d, %d)",legal_queen_moves_x[i],legal_queen_moves_y[i]);
        highlight_legal_moves(legal_queen_moves_x[i],legal_queen_moves_y[i]);
        highlighted_squares_x[i+1] = legal_queen_moves_x[i];
        highlighted_squares_y[i+1] = legal_queen_moves_y[i];
    }

}
int king_logic(int x, int y, int color){

    int counter = 0;
    int legal_king_moves_x[8];
    int legal_king_moves_y[8];

    for(int i = -1;i<2; i++){
        if (x+i>8 ||x+i<0){
            continue;
        }
        for (int j =-1; j<2;j++){
            if (x+j>8 ||x+j<0){
                continue;
            }else if((j == i) &&(j == 0)){
                continue;
            } else {
                legal_king_moves_x[counter]=x+i;
                legal_king_moves_y[counter]=y+j;
                counter++;
            }

        }
    }
    memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
    memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));
    highlighted_squares_x[0] = counter;
    highlighted_squares_y[0] = counter;

    printf("\nPossible Moves:\n");
    for (int i=0; i<counter; i++){
        printf("(%d, %d)",legal_king_moves_x[i],legal_king_moves_y[i]);
        highlight_legal_moves(legal_king_moves_x[i],legal_king_moves_y[i]);
        highlighted_squares_x[i+1] = legal_king_moves_x[i];
        highlighted_squares_y[i+1] = legal_king_moves_y[i];
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
