#include <stdio.h>

enum {
    EMPTY, wp, wn, wb, wq, wk, wr, bp, bn, bb, bq, bk, br
};

enum {
    A1 = 21, B1, C1, D1, E1, F1, G1, H1,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7,
    A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ
};

// Create a board array with 100 squares (to include invalid areas)
int board[120];

void setup_board() {
    // Initialize empty board
    for (int i = 0; i < 120; i++) {
        board[i] = EMPTY;
    }

    // Place White Pieces
    board[A1] = wr; board[B1] = wn; board[C1] = wb; board[D1] = wq;
    board[E1] = wk; board[F1] = wb; board[G1] = wn; board[H1] = wr;
    
    for (int i = A2; i <= H2; i++) {
        board[i] = wp;
    }

    // Place Black Pieces
    board[A8] = br; board[B8] = bn; board[C8] = bb; board[D8] = bq;
    board[E8] = bk; board[F8] = bb; board[G8] = bn; board[H8] = br;
    
    for (int i = A7; i <= H7; i++) {
        board[i] = bp;
    }
}

void print_board() {
    for (int rank = 9; rank >= 2; rank--) {
        for (int file = 1; file <= 8; file++) {
            int square = rank * 10 + file;
            int piece = board[square];

            switch (piece) {
                case EMPTY: printf(". "); break;
                case wp: printf("P "); break;
                case wn: printf("N "); break;
                case wb: printf("B "); break;
                case wq: printf("Q "); break;
                case wk: printf("K "); break;
                case wr: printf("R "); break;
                case bp: printf("p "); break;
                case bn: printf("n "); break;
                case bb: printf("b "); break;
                case bq: printf("q "); break;
                case br: printf("r "); break;
                case bk: printf("k "); break;
                default: printf("? ");
            }
        }
        printf("\n");
    }
}

int main() {
    setup_board();
    print_board();
    return 0;
}
