
typedef struct {
    int board_start_x;
} X_Start_Coords;

typedef struct {
    SDL_Texture* texture;
    int width;
    int height;
} ChessPiece;

int* gFrameBuffer;
SDL_Window* gSDLWindow;
SDL_Renderer* gSDLRenderer;
SDL_Texture* gSDLTexture;
static int gDone;

int WINDOW_WIDTH = 1920 / 2;
int WINDOW_HEIGHT = 1080 / 2;

int SQUARE_SIZE;
int BOARD_SIZE;

float board_start_x;

int selected_x = 100;
int selected_y = 100;

int castling_rights_QS = 1;
int castling_rights_KS = 1;
int castling_rights_qs = 1;
int castling_rights_ks = 1;

// Create a board array with 120 squares (to include invalid areas)
char board[8][8] = {
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},  // Black pieces (top row)
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},  // Black pawns
    {'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o'},  // Empty row
    {'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o'},  // Empty row
    {'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o'},  // Empty row
    {'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o'},  // Empty row
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},  // White pawns
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}   // White pieces (bottom row)
};

