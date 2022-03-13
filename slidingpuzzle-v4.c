int client();
int server();
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{

    if (argc > 1 && argc < 4) // Condition for detecting a correct input length.
    {
        if (strcmp(argv[1], "-s") == 0) // Condition for server
        {
            if (argc > 2) // If the user did give a port number, serve that number to the server function
            {
                server(atoi(argv[2]));
            }
            else
            {
                server(3490); // If no port was given, the default is 3490.
            }
        }

        else if (strcmp(argv[1], "-c") == 0) // Condition for creation of a client
        {
            if (argc > 2) // If a port is given, use the user's given port numbers
            {
                client(atoi(argv[2]));
            }
            else // Else, the default port number is 3490
            {
                client(3490);
            }
        }
        else
        {
            fprintf(stderr, "Error: Invalid Input");
        }
    }
    else
    {

        fprintf(stderr, "Error: Invalid Input");
    }

    return 0;
}