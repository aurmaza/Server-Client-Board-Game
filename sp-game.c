#include "commands.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
void freeBoard(board *b);

board *initialize(int new)
{
    board *ret = malloc(sizeof(board));      // Generates a pointer for board ret that points to a memory address of the size need to hold a board.
    ret->board = calloc(new, sizeof(int *)); // Allocates memory for a board with the size of the given dimension by the user.
    ret->boardDimension = new;               // Sets the user's input to the value of BoardDimension
    srand(time(NULL));                       // Initializes the random number generator for later randomization of board.
    int counter = ((new *new) - 1);          // Represents the numbers added to our board array
    for (int i = 0; i < new; i++)
    {
        ret->board[i] = calloc(new, sizeof(int)); // Allocates memory for each row in our board.
        for (int j = 0; j < new; j++)
        {

            ret->board[i][j] = counter;
            counter--;
        }
    }
    // Randomization of rows
    for (int i = 0; i < new; i++)
    {
        int random = (rand() % ((new) - 1)); // Creates a variable that acts as a randomizer for our board. Uses given board dimension for the range of numbers
        for (int j = 0; j < new; j++)
        {
            int temp = ret->board[i][j];              // Creates a temporary value storing the value of the board at row i column j
            ret->board[i][j] = ret->board[random][j]; // Moves the value at row "random" to where temp was
            ret->board[random][j] = temp;             // Sets the value at row "random" to the temporary value previously stored.
        }
    }
    // Randomization of columns
    for (int i = 0; i < new; i++)
    {
        int random = (rand() % ((new) - 1)); // Creates a variable that acts as a randomizer for our board. Uses given board dimension for the range of numbers
        for (int j = 0; j < new; j++)
        {
            int temp = ret->board[i][j];              // Creates a temporary value storing the value of the board at row i column j
            ret->board[i][j] = ret->board[i][random]; // Moves the value at column "random" to where temp was
            ret->board[i][random] = temp;             // Sets the value at column "random" to the temporary value previously stored.
            if (ret->board[i][random] == 0)
            { // Finds the row and column of the value zero

                ret->whereIsZero[0] = i;      // Sets the row of zero to the first value whereIsZero
                ret->whereIsZero[1] = random; // Sets the column value of zero to the second value of whereIsZero
            }
        }
    }
    return ret; // Returns the initalized board
}
int is_move_valid(board *b, int playerMove)
{
    int userChoice[2];

    // Iterates through the given board and finds the row and column of playerMove.
    for (int i = 0; i < b->boardDimension; i++)
    {
        for (int j = 0; j < b->boardDimension; j++)
        {
            if (b->board[i][j] == playerMove)
            {                      // Finds the location of the player's move.
                userChoice[0] = i; // Sets the first value in userChoice to the row of the payer's move.
                userChoice[1] = j; // Sets the second value in userCHoice to the column of the player's move.
            }
        }
    }
    if ((abs(b->whereIsZero[0] - userChoice[0]) == 1 ^ (abs(b->whereIsZero[1] - userChoice[1]) == 1)) // If User choice is one column away from zero XOR(exclusive or) User choice is one row away from zero
        && !(abs(b->whereIsZero[0] - userChoice[0]) > 1)                                              // AND if the distance between rows is not greater than one
        && !(abs(b->whereIsZero[1] - userChoice[1]) > 1))
    { // AND the distance between columns is not greater than one, the move is valid so return 1.

        return 1; // Move is valid
    }

    return 0; // Move is not valid
}
board *loadGame(board *b, char *fileName)
{
    b->fileLoadChecker = 1;
    FILE *loadFile = fopen(fileName, "r"); // Opens the file the user inputted.
    if (loadFile != NULL)
    { // If the file exists/had no error opening, proceed to rewriting the board

        int k;

        k = fgetc(loadFile); // Sets k equal to the boardDimension which was the first char in the file loaded.
        b = initialize(k);   // Initialize a random board with dimensions of k.

        for (int i = 0; i < b->boardDimension; i++)
        {
            for (int j = 0; j < b->boardDimension; j++)
            {

                k = fgetc(loadFile); // Retrieves the next char

                if (k == 0)
                {

                    b->whereIsZero[0] = i;
                    b->whereIsZero[1] = j;
                }
                b->board[i][j] = k; // Sets char value to the given location in board b
            }
        }

        if (fclose(loadFile))
        { // Returns zero when closed succesfully, so statement will not print unless closing file fails.

            b->fileLoadChecker = 0;
        }
    }

    return b; // Returns board b
}
void swap(int *p1, int *p2)
{

    int temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}
void moveBoard(int move, board *userBoard)
{
    int userChoice[2];
    for (int i = 0; i < userBoard->boardDimension; i++)
    {
        for (int j = 0; j < userBoard->boardDimension; j++)
        {
            if (userBoard->board[i][j] == move)
            { // Finds the location of the user's choice.

                userChoice[0] = i; // Sets the first value of userChoice to the row of the given move
                userChoice[1] = j; // Sets the second value of userChoice to the column of the given move
            }
        }
    }

    swap(&userBoard->board[userChoice[0]][userChoice[1]], &userBoard->board[userBoard->whereIsZero[0]][userBoard->whereIsZero[1]]); // Swaps the valid move with the zero value and vice versa.
    userBoard->whereIsZero[0] = userChoice[0];                                                                                      // Sets the row of where is zero to the location of the row of the  user choice that was moved
    userBoard->whereIsZero[1] = userChoice[1];
}
void freeBoard(board *b)
{

    for (int i = 0; i < b->boardDimension; i++)
    { // Loops thru each row of the board b

        free(b->board[i]); // Frees each row of board matrix
    }
    free(b->board); // Frees the board matrix
    free(b);        // Frees the board
}
void saveGame(board *b, char *fileName)
{
    b->fileSaveChecker = 1;
    FILE *saveFile = fopen(fileName, "w"); // Opens a file under the given filename
    fputc(b->boardDimension, saveFile);    // Puts the dimension of the game as the first char in the file
    for (int i = 0; i < b->boardDimension; i++)
    {
        for (int j = 0; j < b->boardDimension; j++)
        {

            fputc(b->board[i][j], saveFile); // Writes the char at given board location to stream
        }
    }

    if (fclose(saveFile))
    { // Returns zero when closed succesfully, so statement will not print unless closing file fails.

        b->fileSaveChecker = 0;
    }
}
int winGameCheck(board *b)
{

    for (int j = 0; j < b->boardDimension; j++)
    {
        for (int k = 1; k < b->boardDimension; k++)
        {
            if (b->board[j][k - 1] != b->board[j][k] - 1)
            { // Checks if every value in the row is onwe less then the other (if the values in each row are in order).

                return 0; // Game not yet won
            }
        }
    }
    for (int j = 1; j < b->boardDimension; j++)
    {

        if (b->board[j - 1][0] - b->board[j][0] != -1 * b->boardDimension)
        { // Checks if every column is in order.

            return 0; // Game not yet won
        }
    }
    return 1; // Game won!
}
// implementation is here
//.h is for declaring
// if I want to use this i
// create a .h file for sp-game.c for all of the functions that are to be used.