#ifndef BOT_H
#define BOT_H
extern void match_bot_board();
extern void play_best_move(int color, int *castling);
extern int bot_pawn_logic(int x, int y, int color, int mode, int *castling, MoveList* move_list);
extern int bot_knight_logic(int x, int y, int color, int mode, int *castling, MoveList* move_list);
extern int bot_rook_logic(int x, int y, int color, int mode,int *castling, MoveList* move_list);
extern int bot_bishop_logic(int x, int y, int color, int mode, int *castling, MoveList* move_list);
extern int bot_queen_logic(int x, int y, int color, int mode, int *castling, MoveList* move_list);
extern int bot_king_logic(int x, int y, int color, int mode, int *castling, MoveList* move_list);
extern int bot_king_search_help(int x, int y, int color, char piece_to_check1, char piece_to_check2);
extern int bot_search_from_king(int color);
extern int bot_check_for_castle(int color, int *castling, MoveList* move_list);
extern int bot_is_checkmate(int *castling, int color, MoveList* move_list);
extern int bot_check_piece_color(int x, int y);
extern Move alphaBetaRoot(int depth, int color, int *castling);
extern Move alphaBetaMax(int alpha, int beta, int depthleft, int color, int *castling);
extern Move alphaBetaMin(int alpha, int beta, int depthleft, int color, int *castling);


#endif