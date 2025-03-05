#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "game_logic.h"
#include <maths.h>

void update_fake_board(){
    for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
            fake_board[i][j]=board[i][j];
        }
    }
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
    
    int counter = 1;

    memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
    memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));

    if (color == 1){
        //black
        if (y==1){
            //checks rank to see if black pawn has not moved yet
            for(int j = 1; j<3; j++){

                if (check_piece_color(x, y+j) != 2){
                    //if there is something blocking the pawns file movement, break
                    break;
                //don't have to check for check here, because it's not a capture
                } else {
                    fake_board[x][y+j] = fake_board[x][y];
                    fake_board[x][y] = 'o';
                    if (search_from_king(color) == 0){
                        highlighted_squares(x, y+j, counter);
                        counter++;
                    }
                    fake_board[x][y+j] = board[x][y+j];
                    fake_board[x][y] = board[x][y];
                }
            
            }
        }
        else if (y!=1){
            if (check_piece_color(x, y+1) == 2){
                fake_board[x][y+1] = fake_board[x][y];
                fake_board[x][y] = 'o';
                //If it's not the pawns first move, check the square directly in front of it for empty square
                if (search_from_king(color) == 0){
                    highlighted_squares(x, y+1, counter);
                    counter++;
                }
                fake_board[x][y+1] = board[x][y+1];
                fake_board[x][y] = board[x][y];
            }
        }
        

        //checks for pawn captures
        if ((check_piece_color(x-1, y+1) == 0) && ((x-1>=0)&&(y+1<8))){
            fake_board[x-1][y+1] = fake_board[x][y];
            fake_board[x][y] = 'o';
            if (search_from_king(color) == 0){
                highlighted_squares(x-1, y+1, counter);
                counter++;
            }
            fake_board[x-1][y+1] = board[x-1][y+1];
            fake_board[x][y] = board[x][y];
        }
        if ((check_piece_color(x+1, y+1) == 0) && ((x+1>=0)&&(y+1<8))){
            fake_board[x+1][y+1] = fake_board[x][y];
            fake_board[x][y] = 'o';
            if (search_from_king(color) == 0){
                highlighted_squares(x+1, y+1, counter);
                counter++;
            }
            fake_board[x+1][y+1] = board[x+1][y+1];
            fake_board[x][y] = board[x][y];
            
        }
        
        if ((en_passant_x == x+1)  && (en_passant_y == y)){
            fake_board[x+1][y+1] = fake_board[x][y];
            fake_board[x+1][y] = 'o';
            fake_board[x][y] = 'o';
            if (search_from_king(color) == 0){
                highlighted_squares(x+1, y+1, counter);
                counter++;
            }
            fake_board[x+1][y+1] = board[x+1][y+1];
            fake_board[x+1][y] = board[x+1][y];
            fake_board[x][y] = board[x][y];
        }
        if ((en_passant_x == x-1)  && (en_passant_y == y)){
            fake_board[x-1][y+1] = fake_board[x][y];
            fake_board[x-1][y] = 'o';
            fake_board[x][y] = 'o';
            if (search_from_king(color) == 0){
                highlighted_squares(x-1, y+1, counter);
                counter++;
            }
            fake_board[x-1][y+1] = board[x-1][y+1];
            fake_board[x-1][y] = board[x-1][y];
            fake_board[x][y] = board[x][y];
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
                //don't have to check for check here, because it's not a capture
                } else {
                    fake_board[x][y+j] = fake_board[x][y];
                    fake_board[x][y] = 'o';
                    if (search_from_king(color) == 0){
                        highlighted_squares(x, y+j, counter);
                        counter++;
                    }
                    fake_board[x][y+j] = board[x][y+j];
                    fake_board[x][y] = board[x][y];
                }
            
            }
        }else if (y!=6){
            if (check_piece_color(x, y-1) == 2){
                fake_board[x][y-1] = fake_board[x][y];
                fake_board[x][y] = 'o';
                //If it's not the pawns first move, check the square directly in front of it for empty square
                if (search_from_king(color) == 0){
                    highlighted_squares(x, y-1, counter);
                    counter++;
                }
                fake_board[x][y-1] = board[x][y-1];
                fake_board[x][y] = board[x][y];
            }
        }

        //checks for pawn captures
        if ((check_piece_color(x-1, y-1) == 0) && ((x-1>=0)&&(y-1<8))){
            fake_board[x-1][y-1] = fake_board[x][y];
            fake_board[x][y] = 'o';
            if (search_from_king(color) == 0){
                highlighted_squares(x-1, y-1, counter);
                counter++;
            }
            fake_board[x-1][y-1] = board[x-1][y-1];
            fake_board[x][y] = board[x][y];
        }
        if ((check_piece_color(x+1, y-1) == 0) && ((x+1>=0)&&(y-1<8))){
            fake_board[x+1][y-1] = fake_board[x][y];
            fake_board[x][y] = 'o';
            if (search_from_king(color) == 0){
                highlighted_squares(x+1, y-1, counter);
                counter++;
            }
            fake_board[x+1][y-1] = board[x+1][y-1];
            fake_board[x][y] = board[x][y];
            
        }
        if ((en_passant_x == x+1)  && (en_passant_y == y)){
            fake_board[x+1][y-1] = fake_board[x][y];
            fake_board[x+1][y] = 'o';
            fake_board[x][y] = 'o';
            if (search_from_king(color) == 0){
                highlighted_squares(x+1, y-1, counter);
                counter++;
            }
            fake_board[x+1][y-1] = board[x+1][y-1];
            fake_board[x+1][y] = board[x+1][y];
            fake_board[x][y] = board[x][y];
        }
        if ((en_passant_x == x-1)  && (en_passant_y == y)){
            fake_board[x-1][y-1] = fake_board[x][y];
            fake_board[x-1][y] = 'o';
            fake_board[x][y] = 'o';
            if (search_from_king(color) == 0){
                highlighted_squares(x-1, y-1, counter);
                counter++;
            }
            fake_board[x-1][y-1] = board[x-1][y-1];
            fake_board[x-1][y] = board[x-1][y];
            fake_board[x][y] = board[x][y];
        }
    }


    for (int i=1; i<counter+1; i++){
        highlight_legal_moves(highlighted_squares_x[i],highlighted_squares_y[i]);
    }

}

int knight_logic(int x, int y, int color){
    
    memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
    memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));
    
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
            
            else if ((x+i>=0 && x+i<=7)&&(y+j>=0 && y+j<=7)){
                

                fake_board[x+i][y+j] = fake_board[x][y];
                fake_board[x][y] = 'o';
                if (search_from_king(color) ==0){
                    if (is_valid_attack(x + i, y + j, color)){
                        highlighted_squares(x+i, y+j, counter);
                        counter++;
                    } else if (check_piece_color(x+i, y+j)==2){
                        highlighted_squares(x+i, y+j, counter);
                        counter++;
                    }
                } else{
                    continue;
                }
                fake_board[x+i][y+i] = board[x+i][y+j];
                fake_board[x][y] = board[x][y];
                    
                
            }
        }
    }

    


    for (int i=1; i<counter+1; i++){
        highlight_legal_moves(highlighted_squares_x[i],highlighted_squares_y[i]);
    }
}
int rook_logic(int x, int y, int color){
    int counter = 1;

    
    memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
    memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));
    
    //left
        //left
        for (int i=-1; x+i>-1; i--){
        
            if(check_piece_color(x+i, y) == color){
                break;
            } 
            
    
            fake_board[x+i][y] = fake_board[x][y];
            fake_board[x][y] = 'o';
            if (search_from_king(color)==0){
                if (is_valid_attack(x+i,y, color)){
                
                    highlighted_squares(x+i, y, counter);
                    counter+=1;
                    break;
                    
                }else if (check_piece_color(x+i, y) == 2){
                    
                    highlighted_squares(x+i, y, counter);
                    counter+=1;
                
                }
            } else{
                continue;
            }
            fake_board[x+i][y] = board[x+i][y];
            fake_board[x][y] = board[x][y];
        }
        //up
        for (int j=-1; y+j>-1;j--){
            if(check_piece_color(x, y+j) == color){
                break;
            }
            
    
            fake_board[x][y+j] = fake_board[x][y];
            fake_board[x][y] = 'o';
            if (search_from_king(color)==0){
                if (is_valid_attack(x, y+j, color)){
                
                    highlighted_squares(x, y+j, counter);
                    counter+=1;
                    break;
                    
                }else if (check_piece_color(x, y+j) == 2){
                    
                    highlighted_squares(x, y+j, counter);
                    counter+=1;
                
                }
            } else{
                continue;
            }
            fake_board[x][y+j] = board[x][y+j];
            fake_board[x][y] = board[x][y];
        }
        //right
        for (int i=1; x+i<8; i++){
            if(check_piece_color(x+i, y) == color){
                break;
            }
            
    
            fake_board[x+i][y] = fake_board[x][y];
            fake_board[x][y] = 'o';
            if (search_from_king(color)==0){
                if (is_valid_attack(x+i, y, color)){
                
                    highlighted_squares(x+i, y, counter);
                    counter+=1;
                    break;
                    
                }else if (check_piece_color(x+i, y) == 2){
                    
                    highlighted_squares(x+i, y, counter);
                    counter+=1;
                
                }
            } else{
                continue;
            }
            fake_board[x+i][y] = board[x+i][y];
            fake_board[x][y] = board[x][y];
        }   
        //down
        for (int j=1; y+j<8;j++){
            if(check_piece_color(x, y+i) == color){
                break;
            }
            
    
            fake_board[x][y+j] = fake_board[x][y];
            fake_board[x][y] = 'o';
    
            if (search_from_king(color)==0){
                if (is_valid_attack(x, y+j, color)){
                
                    highlighted_squares(x, y+j, counter);
                    counter+=1;
                    break;
                    
                }else if (check_piece_color(x, y+j) == 2){
                    
                    highlighted_squares(x, y+j, counter);
                    counter+=1;
                
                }
            } else{
                continue;
            }
            fake_board[x][y+j] = board[x][y+j];
            fake_board[x][y] = board[x][y];
        }

    
    for (int i=1; i<counter+1; i++){

        highlight_legal_moves(highlighted_squares_x[i],highlighted_squares_y[i]);

    }
}
int bishop_logic(int x, int y, int color){
    
    
    memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
    memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));
    
    int counter = 1;
    //left up
    for (int i=-1; x+i>-1 && y+i>-1; i--){
        
        if(check_piece_color(x+i, y+i) == color){
            break;
        }
        
        fake_board[x+i][y+i] = fake_board[x][y];
        fake_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i, y+i, color)){
            
                highlighted_squares(x+i, y+i, counter);
                counter+=1;
                break;
                
            }else if (check_piece_color(x+i, y+i) == 2){
                
                highlighted_squares(x+i, y+i, counter);
                counter+=1;
            
            }
        } else{
            continue;
        }
        fake_board[x+i][y+i] = board[x+i][y+i];
        fake_board[x][y] = board[x][y];
        
    }
    //right up
    for (int i=1; x+i<8 && y-i>-1; i++){
        if(check_piece_color(x+i, y-i) == color){
            break;
        }
        

        fake_board[x+i][y-i] = fake_board[x][y];
        fake_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i, y-i, color)){
            
                highlighted_squares(x+i, y-i, counter);
                counter+=1;
                break;
                
            }else if (check_piece_color(x+i, y-i) == 2){
                
                highlighted_squares(x+i, y-i, counter);
                counter+=1;
            
            }
        } else{
            continue;
        }
        fake_board[x+i][y-i] = board[x+i][y-i];
        fake_board[x][y] = board[x][y];
    }
    //right down
    for (int i=1; x+i<8 && y+i<8; i++){
        if(check_piece_color(x+i, y+i) == color){
            break;
        }
        

        fake_board[x+i][y+i] = fake_board[x][y];
        fake_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i, y+i, color)){
            
                highlighted_squares(x+i, y+i, counter);
                counter+=1;
                break;
                
            }else if (check_piece_color(x+i, y+i) == 2){
                
                highlighted_squares(x+i, y+i, counter);
                counter+=1;
            
            }
        } else{
            continue;
        }
        fake_board[x+i][y+i] = board[x+i][y+i];
        fake_board[x][y] = board[x][y];
    }
    //left down
    for (int i=1; x-i>-1 && y+i<8; i++){
        if(check_piece_color(x-i, y+i) == color){
            break;
        }
        

        fake_board[x-i][y+i] = fake_board[x][y];
        fake_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x-i, y+i, color)){
            
                highlighted_squares(x-i, y+i, counter);
                counter+=1;
                break;
                
            }else if (check_piece_color(x-i, y+i) == 2){
                
                highlighted_squares(x-i, y+i, counter);
                counter+=1;
            
            }
        } else{
            continue;
        }
        fake_board[x-i][y+i]  = board[x-i][y+i];
        fake_board[x][y] = board[x][y];
    }

    for (int i=1; i<counter+1; i++){

        highlight_legal_moves(highlighted_squares_x[i],highlighted_squares_y[i]);

    }
}
int queen_logic(int x, int y, int color){
    
    int counter = 1;
    
    memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
    memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));

    for (int i=-1; x+i>-1 && y+i>-1; i--){
        
        if(check_piece_color(x+i, y+i) == color){
            break;
        }
        
        fake_board[x+i][y+i] = fake_board[x][y];
        fake_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i, y+i, color)){
            
                highlighted_squares(x+i, y+i, counter);
                counter+=1;
                break;
                
            }else if (check_piece_color(x+i, y+i) == 2){
                
                highlighted_squares(x+i, y+i, counter);
                counter+=1;
            
            }
        } else{
            continue;
        }
        fake_board[x+i][y+i] = board[x+i][y+i];
        fake_board[x][y] = board[x][y];
        
    }
    //right up
    for (int i=1; x+i<8 && y-i>-1; i++){
        if(check_piece_color(x+i, y-i) == color){
            break;
        }
        

        fake_board[x+i][y-i] = fake_board[x][y];
        fake_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i, y-i, color)){
            
                highlighted_squares(x+i, y-i, counter);
                counter+=1;
                break;
                
            }else if (check_piece_color(x+i, y-i) == 2){
                
                highlighted_squares(x+i, y-i, counter);
                counter+=1;
            
            }
        } else{
            continue;
        }
        fake_board[x+i][y-i] = board[x+i][y-i];
        fake_board[x][y] = board[x][y];
    }
    //right down
    for (int i=1; x+i<8 && y+i<8; i++){
        if(check_piece_color(x+i, y+i) == color){
            break;
        }
        

        fake_board[x+i][y+i] = fake_board[x][y];
        fake_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i, y+i, color)){
            
                highlighted_squares(x+i, y+i, counter);
                counter+=1;
                break;
                
            }else if (check_piece_color(x+i, y+i) == 2){
                
                highlighted_squares(x+i, y+i, counter);
                counter+=1;
            
            }
        } else{
            continue;
        }
        fake_board[x+i][y+i] = board[x+i][y+i];
        fake_board[x][y] = board[x][y];
    }
    //left down
    for (int i=1; x-i>-1 && y+i<8; i++){
        if(check_piece_color(x-i, y+i) == color){
            break;
        }
        

        fake_board[x-i][y+i] = fake_board[x][y];
        fake_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x-i, y+i, color)){
            
                highlighted_squares(x-i, y+i, counter);
                counter+=1;
                break;
                
            }else if (check_piece_color(x-i, y+i) == 2){
                
                highlighted_squares(x-i, y+i, counter);
                counter+=1;
            
            }
        } else{
            continue;
        }
        fake_board[x-i][y+i]  = board[x-i][y+i];
        fake_board[x][y] = board[x][y];
    }
    //left
    for (int i=-1; x+i>-1; i--){
        
        if(check_piece_color(x+i, y) == color){
            break;
        } 
        

        fake_board[x+i][y] = fake_board[x][y];
        fake_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i,y, color)){
            
                highlighted_squares(x+i, y, counter);
                counter+=1;
                break;
                
            }else if (check_piece_color(x+i, y) == 2){
                
                highlighted_squares(x+i, y, counter);
                counter+=1;
            
            }
        } else{
            continue;
        }
        fake_board[x+i][y] = board[x+i][y];
        fake_board[x][y] = board[x][y];
    }
    //up
    for (int j=-1; y+j>-1;j--){
        if(check_piece_color(x, y+j) == color){
            break;
        }
        

        fake_board[x][y+j] = fake_board[x][y];
        fake_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x, y+j, color)){
            
                highlighted_squares(x, y+j, counter);
                counter+=1;
                break;
                
            }else if (check_piece_color(x, y+j) == 2){
                
                highlighted_squares(x, y+j, counter);
                counter+=1;
            
            }
        } else{
            continue;
        }
        fake_board[x][y+j] = board[x][y+j];
        fake_board[x][y] = board[x][y];
    }
    //right
    for (int i=1; x+i<8; i++){
        if(check_piece_color(x+i, y) == color){
            break;
        }
        

        fake_board[x+i][y] = fake_board[x][y];
        fake_board[x][y] = 'o';
        if (search_from_king(color)==0){
            if (is_valid_attack(x+i, y, color)){
            
                highlighted_squares(x+i, y, counter);
                counter+=1;
                break;
                
            }else if (check_piece_color(x+i, y) == 2){
                
                highlighted_squares(x+i, y, counter);
                counter+=1;
            
            }
        } else{
            continue;
        }
        fake_board[x+i][y] = board[x+i][y];
        fake_board[x][y] = board[x][y];
    }   
    //down
    for (int j=1; y+j<8;j++){
        if(check_piece_color(x, y+i) == color){
            break;
        }
        

        fake_board[x][y+j] = fake_board[x][y];
        fake_board[x][y] = 'o';

        if (search_from_king(color)==0){
            if (is_valid_attack(x, y+j, color)){
            
                highlighted_squares(x, y+j, counter);
                counter+=1;
                break;
                
            }else if (check_piece_color(x, y+j) == 2){
                
                highlighted_squares(x, y+j, counter);
                counter+=1;
            
            }
        } else{
            continue;
        }
        fake_board[x][y+j] = board[x][y+j];
        fake_board[x][y] = board[x][y];
    }

    for (int i=1; i<counter+1; i++){
        
        highlight_legal_moves(highlighted_squares_x[i],highlighted_squares_y[i]);
    }

}
int king_logic(int x, int y, int color){
    int temp_king_x;
    int temp_king_y;
    if (color == 0){
        temp_king_x = wking_x;
        temp_king_y = wking_y;
    } else if (color == 1){
        temp_king_x = bking_x;
        temp_king_y = bking_y;
    }
    if(piece_logic_for_moving){
        memset(highlighted_squares_x, -1, sizeof(highlighted_squares_x));
        memset(highlighted_squares_y, -1, sizeof(highlighted_squares_y));
    }
    int counter = 1;
    
    counter += check_for_castle(color, counter);

    for(int i = -1;i<2; i++){
        if (x+i<8 ||x+i<0){
            continue;
        }
        for (int j =-1; j<2;j++){
            if (y+j<8 ||y+j<0){
                continue;
            }
                    
            if((j == i) &&(j == 0)){
                continue;
            
            }
            if (check_piece_color(x+i, y+j)==color){
                continue;
            }
            if (color == 0){
                wking_x +=i;
                wking_y+=j;
            } else if (color == 1){
                bking_x +=i;
                bking_y+=j;
            }
            if (search_from_king(color) == 1){
                else if (is_valid_attack(x + i, y + j, color)){
                    highlighted_squares(x+i, y+j, counter);
                    counter++;
                } else if (check_piece_color(x+i, y+j)==2){
                    highlighted_squares(x+i, y+j, counter);
                    counter++;
                }
            }
            if (color == 0){
                wking_x = temp_king_x;
                wking_y=temp_king_y;
            } else if (color == 1){
                bking_x =temp_king_x;
                bking_y=temp_king_y;
            }
        }
    }
    
    for (int i=1; i<counter+1; i++){
        highlight_legal_moves(highlighted_squares_x[i],highlighted_squares_y[i]);
    }
}
void search_from_king(int color){
    int decision;
    
    if (color==0){
        //left up
        for (int i=-1; wking_x+i>-1 && wking_y+i>-1; i--){
            decision = king_search_help(wking_x+i, wking_y+i, color, 'b', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //right up
        for (int i=1; wking_x+i<8 && wking_y-i>-1; i++){
            decision = king_search_help(wking_x+i, wking_y-i, color, 'b', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //right down
        for (int i=1; wking_x+i<8 && wking_y+i<8; i++){
            decision = king_search_help(wking_x+i, wking_y+i, color, 'b', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //left down
        for (int i=1; wking_x-i>-1 && wking_y+i<8; i++){
            decision = king_search_help(wking_x-i, wking_y+i, color, 'b', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //left
        for (int i=-1; wking_x+i>-1; i--){
            decision = king_search_help(wking_x+i, wking_y, color, 'r', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }
        //up
        for (int j=-1; y+j>-1;j--){
            decision = king_search_help(wking_x, wking_y+i, color, 'r', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }
        //right
        for (int i=1; wking_x+i<8; i++){
            decision = king_search_help(wking_x+i, wking_y, color, 'r', 'q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //down
        for (int j=1; y+j<8;j++){
            decision = king_search_help(wking_x, wking_y+i, color, 'r', 'q');
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
                
                if ((wking_x+i>=0 && wking_x+i<=7)&&(wking_y+j>=0 && wking_y+j<=7)){
                    decision = king_search_help(wking_x+i, wking_y+j, color, 'n', 'g');
                    if (decision == 0 || decision == 1){
                        continue;
                    } else if (decision == 3){
                        return 1;
                    }
                }
                
            }
        }
        if (fake_board[wking_x-1][wking_y-1] == 'p'){
            return 1;
        } 
        if (fake_board[wking_x+1][wking_y-1]){
            return 1;
        }
         //king
         for(int i = -1;i<2; i++){
            if (x+i<=8 ||x+i<0){
                continue;
            }
            for (int j =-1; j<2;j++){
                if (x+j<=8 ||x+j<0){
                    continue;
                }
                        
                if((j == i) &&(j == 0)){
                    continue;
                
                }
                if (check_piece_color(x+i, y+j)==color){
                    continue;
                }
                if ((wking_x+i>=0 && wking_x+i<=7)&&(wking_y+j>=0 && wking_y+j<=7)){
                    decision = king_search_help(wking_x+i, wking_y+j, color, 'k', 'G');
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
        for (int i=-1; bking_x+i>-1 && bking_y+i>-1; i--){
            decision = king_search_help(bking_x+i, bking_y+i, color, 'B', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //right up
        for (int i=1; bking_x+i<8 && bking_y-i>-1; i++){
            decision = king_search_help(bking_x+i, bking_y-i, color, 'B', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //right down
        for (int i=1; bking_x+i<8 && bking_y+i<8; i++){
            decision = king_search_help(bking_x+i, bking_y+i, color, 'B', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //left down
        for (int i=1; bking_x-i>-1 && bking_y+i<8; i++){
            decision = king_search_help(bking_x-i, bking_y+i, color, 'B', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //left
        for (int i=-1; bking_x+i>-1; i--){
            decision = king_search_help(bking_x+i, bking_y, color, 'R', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }
        //up
        for (int j=-1; y+j>-1;j--){
            decision = king_search_help(bking_x, bking_y+j, color, 'R', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }
        //right
        for (int i=1; bking_x+i<8; i++){
            decision = king_search_help(bking_x+i, bking_y, color, 'R', 'Q');
            if (decision == 0 || decision == 1){
                break;
            } else if (decision == 3){
                return 1;
            }
        }

        //down
        for (int j=1; y+j<8;j++){
            decision = king_search_help(bking_x, bking_y+j, color, 'R', 'Q');
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
                
                if ((bking_x+i>=0 && bking_x+i<=7)&&(bking_y+j>=0 && bking_y+j<=7)){
                    decision = king_search_help(bking_x+i, bking_y+j, color, 'N', 'G');
                    if (decision == 0 || decision == 1){
                        continue;
                    } else if (decision == 3){
                        return 1;
                    }
                }
                
            }
        }
        //pawns
        if (fake_board[bking_x-1][bking_y-1] == 'P'){
            return 1;
        } 
        if (fake_board[bking_x+1][bking_y-1]){
            return 1;
        }
        //king
        for(int i = -1;i<2; i++){
            if (x+i<=8 ||x+i<0){
                continue;
            }
            for (int j =-1; j<2;j++){
                if (x+j<=8 ||x+j<0){
                    continue;
                }
                        
                if((j == i) &&(j == 0)){
                    continue;
                
                }
                if (check_piece_color(x+i, y+j)==color){
                    continue;
                }
                if ((bking_x+i>=0 && bking_x+i<=7)&&(bking_y+j>=0 && bking_y+j<=7)){
                    decision = king_search_help(bking_x+i, bking_y+j, color, 'K', 'G');
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

        if (check_piece_color(x, y) == 0){
            piece_logic_for_moving = 1;
            return 0;
        }
        if (check_piece_color(x, y) == 2){
            piece_logic_for_moving = 1;
            return 2;
        }
        if (check_piece_color(x, y) == 1){
            
            if (fake_board[x][y] == piece_to_check1 || fake_board[x][y] == piece_to_check2){
                piece_logic_for_moving = 1;
                return 3;
            }
            piece_logic_for_moving = 1;
            return 1;
        }
        
    }

    else if (color==1){
        if (check_piece_color(x, y) == 1){
            piece_logic_for_moving = 1;
            return 0;
        }
        if (check_piece_color(x, y) == 2){
            piece_logic_for_moving = 1;
            return 2;
        }
        if (check_piece_color(x, y) == 0){
            
            if (fake_board[x][y] == piece_to_check1 || fake_board[x][y] == piece_to_check2){
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
int check_for_castle(int color, int counter){
    if (color == 0){

        int temp_wking_x = wking_x;

        if (castling_rights_QS){
            if (board[3][7] == 'o' && board[2][7] == 'o' && board[1][7] == 'o' &&board[3][7] == 'R'){
                if (!search_from_king(color)){
                    fake_board[3][7] = 'K';
                    fake_board[4][7] = 'o';
                    wking_x = 3;
                    
                    if (!search_from_king(color)){
                        fake_board[2][7] = 'K';
                        fake_board[3][7] = 'o';
                        wking_x = 2;
                        if (!search_from_king(color)){
                            highlighted_squares(2, 7, counter);
                            counter++;
                        }
                    }
                }
                fake_board[3][7] = 'o';
                fake_board[2][7] = 'o';
                fake_board[4][7] = 'K';
            }
        }
        if (castling_rights_KS){
            if (board[5][7] == 'o' && board[6][7] == 'o' && board[7][7] == 'R'){
                if (!search_from_king(color)){
                    fake_board[5][7] = 'K';
                    fake_board[4][7] = 'o';
                    wking_x = 5;
                    if (!search_from_king(color)){
                        fake_board[6][7] = 'K';
                        fake_board[5][7] = 'o';
                        wking_x = 6;
                        if (!search_from_king(color)){
                            highlighted_squares(6, 7, counter);
                            counter++;
                        }
                    }
                }
                fake_board[3][7] = 'o';
                fake_board[2][7] = 'o';
                fake_board[4][7] = 'K';
            }
        }
        wking_x = temp_wking_x;
    } else if (color == 1){

        int temp_bking_x = bking_x;

        if (castling_rights_qs){
            if (board[3][0] == 'o' && board[2][0] == 'o' && board[1][0] == 'o' &&board[3][0] == 'R'){
                if (!search_from_king(color)){
                    fake_board[3][0] = 'K';
                    fake_board[4][0] = 'o';
                    bking_x = 3;
                    if (!search_from_king(color)){
                        fake_board[2][0] = 'K';
                        fake_board[3][0] = 'o';
                        bking_x = 2;
                        if (!search_from_king(color)){
                            highlighted_squares(2, 0, counter);
                            counter++;
                        }
                    }
                }
                fake_board[3][0] = 'o';
                fake_board[2][0] = 'o';
                fake_board[4][0] = 'K';
            }
        }
        if (castling_rights_ks){
            if (board[5][0] == 'o' && board[6][0] == 'o' && board[7][0] == 'r'){
                if (!search_from_king(color)){
                    fake_board[5][0] = 'K';
                    fake_board[4][0] = 'o';
                    bking_x = 5;
                    if (!search_from_king(color)){
                        fake_board[6][0] = 'K';
                        fake_board[5][0] = 'o';
                        bking_x = 6;
                        if (!search_from_king(color)){
                            highlighted_squares(6, 0, counter);
                            counter++;
                        }
                    }
                }
                fake_board[3][0] = 'o';
                fake_board[2][0] = 'o';
                fake_board[4][0] = 'K';
            }
        }
        bking_x = temp_bking_x;
    }
    return counter;
    
}
int legal_moves(int x, int y){
    /*
    Function takes in the coordinates of a piece, the current state of the board, and what color the piece is.
    x can range from (0,7)
    y can range from (0,7)
    Board is a 8x8 2d matrix.
    Color can be -1, 0, 1, 2. Invalid char is -1, black is 1, white is 0, empty is 2.
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
