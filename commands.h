#ifndef COMMANDS_H
#define COMMANDS_H
enum commands
{
    cmd_new = 200,
    cmd_move,
    cmd_print,
    cmd_save,
    cmd_load,
    cmd_quit,
    cmd_getBoard,
    cmd_getBoardDim,
    cmd_winCheck
};

typedef struct
{                       // Board Struct
    int **board;        // Points to board matrix.
    int boardDimension; // Represents the board Dimension for usage in multiple functions
    int whereIsZero[2];
    int fileLoadChecker;
    int fileSaveChecker; // Represents the location of zero for checking if a move is valid.
} board;

#endif
// game logic need alll to be in sp-game
// main server has a counter everytime someone connects increment
// then forked process will have that counter value
// before forking, as connection is succesful, increment that number
// increment should be right before the fork.