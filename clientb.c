//Project: Postman
//Author: Chris
//client.c is a program that is used to connect to a server program hosting an online email
//system. This program will repeatedly ask for user credentials unless locked out.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//libraries
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
int main()
{
    //main function testing input grabbing from console
    char input[1024];
    while (1)
    {
        printf("enter a command (quit to exit): ");
        fgets(input, sizeof(input), stdin);
        if(feof(stdin) || input == "quit")
        {
            printf("EXITING");
            break;
        }
        else{
            printf("trying again\n");
        }
    }
    //int socketa = socket(domain, type, protocol)
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    int 
}
