void freeBoard(board *b);
board *initialize(int new);
void swap(int *p1, int *p2);
int is_move_valid(board *b, int playerMove);
void moveBoard(int move, board *userBoard);
board *loadGame(board *b, char *fileName);
void saveGame(board *b, char *fileName);
int winGameCheck(board *b);