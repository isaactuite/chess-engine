#include <stdio.h>
#include <stdlib.h>
#include "defs.h"


int check_piece_color(int x, int y, char board[8][8]){
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
            return 0;
        case 'P':
        case 'R':
        case 'N':
        case 'B':
        case 'Q':
        case 'K':
            //white
            return 1;
        case 'o':
            return 2;
            printf("Checked empty square colour");
        default:
            return -1;
            printf("checked invalid char");
    }
}

//function to check if a target is valid
int is_valid_attack(int x, int y, char board[8][8], int color){

    int piece_color = check_piece_color(x, y, board);
    return piece_color == -1 ? 0 : ((piece_color != color) && (piece_color !=2));

}


//individual piece logic
int pawn_logic(int x, int y, char board[8][8], int color){
    int legal_pawn_moves_x[4];
    int legal_pawn_moves_y[4];
    int counter = 0;
    if (color == 0){
        //black
        if (y==1){
            //checks rank to see if black pawn has not moved yet
            for(int j = 1; j<3; j++){

                if (check_piece_color(x, y+j, board) != 2){
                    //if there is something blocking the pawns file movement, break
                    break;
                } else {
                    legal_pawn_moves_x[counter] = x;
                    legal_pawn_moves_y[counter] = y+j;
                    counter++;
                }
            
            }
        } else{
            if (check_piece_color(x, y+1, board) == 2){
                //If it's not the pawns first move, check the square directly in front of it for empty square
                legal_pawn_moves_x[counter] = x;
                legal_pawn_moves_y[counter] = y+1;
                counter++;
            }
        }

        //checks for pawn captures
        if ((check_piece_color(x-1, y+1, board) == 1) && ((x-1<=0)&&(y+1<8))){
            legal_pawn_moves_x[counter] = x-1;
            legal_pawn_moves_y[counter] = y+1;
            counter++;
        }
        if ((check_piece_color(x+1, y+1, board) == 1) && ((x+1<=0)&&(y+1<8))){
            legal_pawn_moves_x[counter] = x+1;
            legal_pawn_moves_y[counter] = y+1;
            counter++;
        }

    }else if (color == 1){
        //white
        if (y==6){
            //checks rank to see if white pawn has not moved yet
            for(int j = -1; j>-3; j--){

                if (check_piece_color(x, y+j, board) != 2){
                    //if there is something blocking the pawns file movement, break
                    break;
                } else {
                    legal_pawn_moves_x[counter] = x;
                    legal_pawn_moves_y[counter] = y+j;
                    counter++;
                }
            
            }
        } else{
            if (check_piece_color(x, y-1, board) == 2){
                //If it's not the pawns first move, check the square directly in front of it for empty square
                legal_pawn_moves_x[counter] = x;
                legal_pawn_moves_y[counter] = y-1;
                counter++;
            }
        }

        //checks for pawn captures
        if ((check_piece_color(x-1, y-1, board) == 0) && ((x-1<=0)&&(y-1<8))){
            legal_pawn_moves_x[counter] = x-1;
            legal_pawn_moves_y[counter] = y+1;
            counter++;
        }
        if ((check_piece_color(x+1, y-1, board) == 0) && ((x+1<=0)&&(y-1<8))){
            legal_pawn_moves_x[counter] = x+1;
            legal_pawn_moves_y[counter] = y+1;
            counter++;
        }
        
        

    }
}
int knight_logic(int x, int y, char board[8][8], int color){
    int legal_knight_moves_x[8]; //8 possible knight moves
    int legal_knight_moves_y[8];
    int counter=0;

    for (int i = -2; i<3; i++){
        
        if (i==0){
            i++;
        }

        for (int j=-2; j<3; j++){
            
            if (j==0){
                j++;
            }
            if (abs(j)==abs(i)){
                continue;
            }
            if ((x+i>=0 && x+i<=7)&&(y+j>=0 && y+j<=7)){
                if (is_valid_attack(x + i, y + j, board, color)){
                    legal_knight_moves_x[counter] = x+i;
                    legal_knight_moves_y[counter] = y+j; //changed x to y, don't know if itll work
                    counter++;
                } else if (check_piece_color(x, y, board)==2){
                    legal_knight_moves_x[counter] = x+i;
                    legal_knight_moves_y[counter] = y+j; //changed x to y, don't know if itll work
                    counter++;
                }

            }
        }
    }
    printf("\nPossible Moves:\n");
    for (int i=0; i<counter; i++){
        printf("(%d, %d)",legal_knight_moves_x[i],legal_knight_moves_y[i]);
    }
}
int rook_logic(int x, int y, char board[8][8], int color){
    int counter = 0;
    int legal_rook_moves_x[14];
    int legal_rook_moves_y[14];
    //left
    for (int i=-1; x+i>-1; i--){
        if(check_piece_color(x, y, board[8][8]) == color){
            break;
        }
        if (is_valid_attack(x + i, y, board, color)){
            legal_rook_moves_x[counter]=x+i;
            legal_rook_moves_y[counter]=y;
            counter+=1;
            break;
        }else if (check_piece_color(x, y, board[8][8]) == 2){
            legal_rook_moves_x[counter]=x+i;
            legal_rook_moves_y[counter]=y;
            counter+=1;
            }
    }
    //up
    for (int j=-1; y+j>-1;j--){
        legal_rook_moves_x[counter]=x;
        legal_rook_moves_y[counter]=y+j;
        counter+=1;
    }
    //right
    for (int i=1; x+i<8; i++){
        legal_rook_moves_x[counter]=x+i;
        legal_rook_moves_y[counter]=y;
        counter+=1;
    }   
    //down
    for (int j=1; y+j<8;j++){
        legal_rook_moves_x[counter]=x;
        legal_rook_moves_y[counter]=y+j;
        counter+=1;
    }

    printf("\nPossible Moves:\n");
    for (int i=0; i<counter; i++){
        printf("(%d, %d)",legal_rook_moves_x[i],legal_rook_moves_y[i]);
    }
}
int bishop_logic(int x, int y, char board[8][8], int color){
    int legal_bishop_moves_x[13];
    int legal_bishop_moves_y[13];
    int counter = 0;
    //left up
    for (int i=-1; x+i>-1 && y+i>-1; i--){
        legal_bishop_moves_x[counter] = x+i;
        legal_bishop_moves_y[counter] = y+i;
        counter++;
    }
    //right up
    for (int i=1; x+i<8 && y-i>-1; i++){
        legal_bishop_moves_x[counter] = x+i;
        legal_bishop_moves_y[counter] = y-i;
        counter++;
    }
    //right down
    for (int i=1; x+i<8 && y+i<8; i++){
        legal_bishop_moves_x[counter] = x+i;
        legal_bishop_moves_y[counter] = y+i;
        counter++;
    }
    //left down
    for (int i=1; x-i>-1 && y+i<8; i++){
        legal_bishop_moves_x[counter] = x-i;
        legal_bishop_moves_y[counter] = y+i;
        counter++;
    }
    printf("\nPossible Moves:\n");
    for (int i=0; i<counter; i++){
        printf("(%d, %d)",legal_bishop_moves_x[i],legal_bishop_moves_y[i]);
    }
}
int queen_logic(int x, int y, char board[8][8], int color){
    int counter = 0;
    int legal_queen_moves_x[27];
    int legal_queen_moves_y[27];
     //left up
    for (int i=-1; x+i>-1 && y+i>-1; i--){
        legal_queen_moves_x[counter] = x+i;
        legal_queen_moves_y[counter] = y+i;
        counter++;
    }
    //right up
    for (int i=1; x+i<8 && y-i>-1; i++){
        legal_queen_moves_x[counter] = x+i;
        legal_queen_moves_y[counter] = y-i;
        counter++;
    }
    //right down
    for (int i=1; x+i<8 && y+i<8; i++){
        legal_queen_moves_x[counter] = x+i;
        legal_queen_moves_y[counter] = y+i;
        counter++;
    }
    //left down
    for (int i=1; x-i>-1 && y+i<8; i++){
        legal_queen_moves_x[counter] = x-i;
        legal_queen_moves_y[counter] = y+i;
        counter++;
    }  

    //left
    for (int i=-1; x+i>-1; i--){
        legal_queen_moves_x[counter]=x+i;
        legal_queen_moves_y[counter]=y;
        counter+=1;
    }
    //up
    for (int j=-1; y+j>-1;j--){
        legal_queen_moves_x[counter]=x;
        legal_queen_moves_y[counter]=y+j;
        counter+=1;
    }
    //right
    for (int i=1; x+i<8; i++){
        legal_queen_moves_x[counter]=x+i;
        legal_queen_moves_y[counter]=y;
        counter+=1;
    }   
    //down
    for (int j=1; y+j<8;j++){
        legal_queen_moves_x[counter]=x;
        legal_queen_moves_y[counter]=y+j;
        counter+=1;
    }
    printf("\nPossible Moves:\n");
    for (int i=0; i<counter; i++){
        printf("(%d, %d)",legal_queen_moves_x[i],legal_queen_moves_y[i]);
    }

}
int king_logic(int x, int y, char board[8][8], int color){

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
    printf("\nPossible Moves:\n");
    for (int i=0; i<counter; i++){
        printf("(%d, %d)",legal_king_moves_x[i],legal_king_moves_y[i]);
    }
}



int legal_moves(int x, int y, int color){
    /*
    Function takes in the coordinates of a piece, the current state of the board, and what color the piece is.
    x can range from (0,7)
    y can range from (0,7)
    Board is a 8x8 2d matrix.
    Color can be -1, 0, 2, 3. Invalid char is -1, black is 0, white is 1, empty is 2.
    */
    switch (board[x][y]){
        case 'P': case 'p':
            printf("Pawn Selected");
            pawn_logic(x, y, board, color);
            break;
        case 'N': case 'n':
            printf("Knight selected");
            knight_logic(x, y, board, color);
            break;
        case 'B': case 'b':
            printf("Bishop selected");
            bishop_logic(x, y, board, color);
            break;
        case 'R': case 'r':
            printf("Rook selected");
            rook_logic(x, y, board, color);
            break;
        
        case 'Q': case 'q':
            printf("Queen selected");
            queen_logic(x, y, board, color);
            break;
        case 'K': case 'k':
            printf("King selected");
            king_logic(x, y, board, color);
            break;
        default:
            print("legal_moves called, invalid");
    }
}
