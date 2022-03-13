
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <netdb.h>
#include "commands.h"
#include "sp-game.h"
// for compiler
// when calling gcc
int serverNum = 0;
int server(int portNumber)
{
    fprintf(stderr, "Welcome to Alex's board game!\n  This is the server.\n  When calling client, please use -c [PORT].\n  NOT compatible with: -c HOST [PORT]\n");
    int sockfd, ret;
    struct sockaddr_in serverAddress; // Structure for the server Address

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    char buffer[1024];
    pid_t childpid;

    int move = 0;               // Initalizes a move piped from client
    char fileName[100];         // Initalizes a file name piped from client
    char loadFileName[100];     // Initalizes a load file name piped from cleint
    int winCheck = 0;           // Initalizes the win check value
    int moveValid = 0;          // Initalizes a place to store if a move is valid
    enum commands userCommands; // Initializes an enum for userCommands

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // Creates a new socket
    if (sockfd < 0)
    {
        fprintf(stderr, "Error in connection.\n");
        exit(1);
    }
    fprintf(stderr, "Server Socket is created.\n");

    memset(&serverAddress, '\0', sizeof(serverAddress)); // Completlety zeros out serverAddress struct.
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(portNumber);             // Sets port that the endpoint is listening to
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Sets ip address of the listening server

    ret = bind(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)); // Binds sockfd to given port
    if (ret < 0)
    {
        fprintf(stderr, "Error in binding.\n");
        exit(1);
    }
    printf("Bind to port %d\n", portNumber);

    if (listen(sockfd, 10) == 0)
    {
        fprintf(stderr, "Listening....\n");
    }
    else
    {
        fprintf(stderr, "Error in binding.\n");
    }

    while (1) // Creation of an infinite loop so that multiple servers can be created
    {
        newSocket = accept(sockfd, (struct sockaddr *)&newAddr, &addr_size); // Waits for a connection on sockfd until a connection is made.
        if (newSocket < 0)
        {
            exit(1);
        }
        serverNum++;
        fprintf(stderr, "Client-%d connected from %s:%d\n", serverNum, inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

        if ((childpid = fork()) == 0) // Forks a new process after a new connection is made.
        {
            board *userBoard = initialize(4); // Initializes a board of four as a standard
            close(sockfd);                    // Closes the socket so that other separate connections can be made

            while (1)
            {
                recv(newSocket, &userCommands, sizeof(userCommands), 0); // Accepts a new command from the client.
                fprintf(stderr, "Recieved command from client-%d %d\n", serverNum, userCommands);
                switch (userCommands)
                {
                case cmd_new:
                    freeBoard(userBoard);
                    recv(newSocket, &userBoard->boardDimension, sizeof(int), 0);
                    userBoard = initialize(userBoard->boardDimension);
                    break;
                case cmd_quit:
                    fprintf(stderr, "Client %d disconnected from %s:%d\n", serverNum, inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
                    freeBoard(userBoard);
                    exit(1);

                    break;
                case cmd_getBoard:

                    for (int i = 0; i < userBoard->boardDimension; i++)
                    { // Loops through each row of the double pointer userBoard and writes each row to the pipe.

                        send(newSocket, userBoard->board[i], sizeof(int) * userBoard->boardDimension, 0);
                    }
                    break;
                case cmd_getBoardDim:

                    send(newSocket, &userBoard->boardDimension, sizeof(int), 0); // Retrieves the user board dimension and pipes it to the client for use in the new board function/

                    break;
                case cmd_move:
                    recv(newSocket, &move, sizeof(int), 0);
                    if (is_move_valid(userBoard, move) == 0)
                    {
                        moveValid = 0;
                    }
                    else
                    {
                        moveValid = 1;
                        int userChoice[2];
                        moveBoard(move, userBoard);
                        // Sets the column of where is zero to the location of the column of the user choice that was moved.

                        moveValid = 1;
                    }
                    send(newSocket, &moveValid, sizeof(int), 0);
                    break;
                case cmd_load:
                    recv(newSocket, loadFileName, sizeof(loadFileName), 0);
                    freeBoard(userBoard);
                    loadGame(userBoard, loadFileName);
                    send(newSocket, &userBoard->fileLoadChecker, sizeof(int), 0);
                    break;
                case cmd_save:
                    recv(newSocket, fileName, sizeof(fileName), 0);
                    saveGame(userBoard, fileName);
                    send(newSocket, &userBoard->fileSaveChecker, sizeof(int), 0);
                    break;
                case cmd_winCheck:
                    winCheck = winGameCheck(userBoard);
                    send(newSocket, &winCheck, sizeof(int), 0);
                    break;
                }
            }
        }
    }

    close(newSocket);

    return 0;
}
