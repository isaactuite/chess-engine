#include <stdio.h>
#include <stdlib.h>


typedef enum Piece
    {EMPTY, wp, wn, wb, wq, wk, wr, bp, bn, bb, bq, bk, br}Piece;
enum {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE};
enum {RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE};
enum {WHITE, BLACK, BOTH};

enum{
    A1 = 0, B1, C1, D1, E1, F1, G1, H1,
    A2 = 8, B2, C2, D2, E2, F2, G2, H2,
    A3 = 16, B3, C3, D3, E3, F3, G3, H3,
    A4 = 24, B4, C4, D4, E4, F4, G4, H4,
    A5 = 32, B5, C5, D5, E5, F5, G5, H5,
    A6 = 40, B6, C6, D6, E6, F6, G6, H6,
    A7 = 48, B7, C7, D7, E7, F7, G7, H7,
    A8 = 56, B8, C8, D8, E8, F8, G8, H8
};
// Create a board array with 120 squares (to include invalid areas)
int board[8][8] = {0};



int knight_logic(int x, int y, int *legal_knight_moves_x, int *legal_knight_moves_y){


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
                legal_knight_moves_x[counter] = x+i;
                legal_knight_moves_y[counter] = x+j;
                counter++;
            }
        }
    }
    printf("\nPossible Moves:\n");
    for (int i=0; i<counter; i++){
        printf("(%d, %d)",legal_knight_moves_x[i],legal_knight_moves_y[i]);
    }
}
int rook_logic(int x, int y, int *legal_rook_moves_x, int *legal_rook_moves_y){
    int counter = 0;
    //left
    for (int i=-1; x+i>-1; i--){
        legal_rook_moves_x[counter]=x+i;
        legal_rook_moves_y[counter]=y;
        counter+=1;
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
int bishop_logic(int x, int y, int *legal_bishop_moves_x, int *legal_bishop_moves_y){
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
int queen_logic(int x, int y, int *legal_queen_moves_x, int *legal_queen_moves_y){
    int counter = 0;
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
int king_logic(int x, int y, int *legal_queen_moves_x, int *legal_queen_moves_y)



int calculateLegalMoves(){
    int* legal_x_moves = malloc(sizeof(int)*100);
}
int main() {
    int legal_queen_moves_x[40];
    int legal_queen_moves_y[40];
    int x;
    int y;
    int board;
    printf("\nWhat x:\n");
    scanf("%d", &x);
    printf("\nWhat y:\n");
    scanf("%d", &y);
    printf("%d, %d", x, y);
    queen_logic(x, y, legal_queen_moves_x, legal_queen_moves_y);
        return 0;
}
