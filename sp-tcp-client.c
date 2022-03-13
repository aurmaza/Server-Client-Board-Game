#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "commands.h" //header file for commands in sp-game.c

int clientSocket;

void newBoard()
{
    int intCheck;
    int temp;
    char boardDimension[10]; // Creates a char so that detection for errors like "test" will equal zero when put through atoi

    printf("Enter a board dimension (2-10): \n");

    scanf(" %s", boardDimension);
    temp = atoi(boardDimension); // Turns the input into only numbers.
    if (temp == 0)
    { // If no numbers were inputted, temp will represent zero and invalid input will be printed
        printf("Invalid input, please try again!\n");
    }
    else
    {
        enum commands newB = cmd_new;
        send(clientSocket, &newB, sizeof(cmd_new), 0);
        send(clientSocket, &temp, sizeof(int), 0);
    }
}
void printBoard() // move to sp-game.c.  logic of game is called from server
{

    int dimension = 0;
    enum commands getDim = cmd_getBoardDim; // Creates command for getting board dimension

    send(clientSocket, &getDim, sizeof(cmd_getBoardDim), 0); // Writes the board dimension command

    recv(clientSocket, &dimension, sizeof(int), 0); // Recieves the result of the board dimension

    enum commands getBoard = cmd_getBoard;                  // Creates command for getting a board
    send(clientSocket, &getBoard, sizeof(cmd_getBoard), 0); // Sends the get board command to server

    int *row = malloc(sizeof(int) * dimension); // Allocates memory for printing a row
    for (int i = 0; i < dimension; i++)
    {

        recv(clientSocket, row, sizeof(int) * dimension, 0); // Recieves the given board
        for (int j = 0; j < dimension; j++)
        {

            printf(" %d\t", row[j]); // Prints the board
        }
        printf("\n");
    }

    free(row);
}
void moveUsrBoard()
{
    int move;

    printf("\nEnter a number that you would like to  move: ");
    scanf(" %d", &move);
    enum commands makeMove = cmd_move;
    send(clientSocket, &makeMove, sizeof(cmd_move), 0); // Writes over the pipe the make move command
    send(clientSocket, &move, sizeof(int), 0);          // Writes the given move to the user
}
void loadBoard()
{
    int fileLoadCheck = 1;
    enum commands loadBoard = cmd_load;
    send(clientSocket, &loadBoard, sizeof(cmd_load), 0);
    char loadFileName[100]; // Creates a space to store the name of the file being loaded
    printf("Please input a file name to load: \n");
    scanf(" %s", loadFileName);
    send(clientSocket, loadFileName, sizeof(loadFileName), 0);
    recv(clientSocket, &fileLoadCheck, sizeof(int), 0); // Recieves if a file has loaded succesfully or unsuccesfully represented by a one or zero respectfully.
    if (fileLoadCheck == 1)
    {

        printf("File loaded sucessfuly\n");
    }
    else if (fileLoadCheck == 0)
    {
        printf("File loaded incorrectly\n");
    }
}
void saveBoard()
{
    int fileSaveCheck = 1; // Represents the boolean to see if a file saved correctly or incorrectly
    enum commands saveBoard = cmd_save;
    send(clientSocket, &saveBoard, sizeof(cmd_save), 0);
    char fileName[100]; // Creates a space to store file name
    printf("Please input a file name to save: \n");
    scanf(" %s", fileName); // Scans for the user's inputted file name
    send(clientSocket, fileName, sizeof(fileName), 0);
    recv(clientSocket, &fileSaveCheck, sizeof(int), 0); // Recieves a 1 or 0 to see if a file saved correctly or incorrectly
    if (fileSaveCheck == 1)
    {

        printf("File saved sucessfuly\n");
    }
    else if (fileSaveCheck == 0)
    {
        printf("File saved incorrectly\n");
    }
}
int client(int portNumber)
{

    int ret;                       // Represents the return value of connect();
    struct sockaddr_in serverAddr; // Creates the structure for the serverAddress
    char *userInput = malloc(sizeof(char));
    int winCheck = 0;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0); // Creates a socket
    if (clientSocket < 0)
    {
        printf("Error in connection.\n");
        exit(1);
    }
    printf("Client Socket is created.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portNumber);             // Sets port that endppint is listening
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Sets ip address of server.

    ret = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)); // Opens a connection on the client socket to the server address
    if (ret < 0)
    {
        printf("Error in connection to port [%d].\n", portNumber);
        exit(1);
    }

    printf("Connected to Server.\n");

    while (1)
    {

        printf("Option: [p]rint, [m]ove, [n]ew, [l]oad, [s]ave, [q]uit: \n");
        scanf(" %s", userInput);

        if (userInput[0] == 'n')
        {
            newBoard();
        }
        if (userInput[0] == 'p')
        {
            printBoard();
        }
        if (userInput[0] == 'm')
        {
            int moveValid = 0;
            moveUsrBoard();
            recv(clientSocket, &moveValid, sizeof(int), 0); // Recieves the boolean for if a move is valid or not
            if (moveValid == 1)
            {

                printf("Piece has moved.\n");
            }
            else if (moveValid == 0)
            {
                printf("\nInvalid move, please try again.\n");
            }
        }
        if (userInput[0] == 'l')
        {
            loadBoard();
        }
        if (userInput[0] == 's')
        {
            saveBoard();
        }
        if (userInput[0] == 'q')
        {
            enum commands quit = cmd_quit;
            send(clientSocket, &quit, sizeof(cmd_quit), 0);
            sleep(1);
            close(clientSocket);
            printf("Quit from server.\n");
            exit(1);
        }
        enum commands winner = cmd_winCheck;
        send(clientSocket, &winner, sizeof(cmd_winCheck), 0);
        recv(clientSocket, &winCheck, sizeof(int), 0);
        if (winCheck == 1)
        {
            printf("You won the game!\n");
        }
    }
    free(userInput);
    return 0;
}