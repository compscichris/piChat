#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//libraries
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
int checkIP(char*);
int main()
{
    //main function testing input grabbing from console
    char input[1024];
    char inputcopy[1024];
    char *ip = NULL;
    while (1)
    {
        
        //Step 1: Ask user for input of address
        printf("Enter IP address of server to connect to in format of <x.x.x.x>: (quit to exit): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input,"\n")] = 0;
        memcpy(inputcopy, input, sizeof(input));
        ip = inputcopy;
        if(feof(stdin) || input == "quit")
        {
            printf("EXITING");
            break;
        }
        else{
            if(checkIP(ip) == -1)
            {
                printf("trying again\n");
            }
            else{
                //int socketa = socket(domain, type, protocol)
                int cli_socket;
                struct sockaddr_in serv_addr;
                serv_addr.sin_family = AF_INET;
                serv_addr.sin_addr.s_addr = inet_addr(*ip);
                serv_addr.sin_port = htons(2);
                int sockfd = socket(PF_INET, SOCK_STREAM, 0);
                if(connect(cli_socket, (struct sockaddr*) &serv_addr, sizeof(serv_addr))<0)
                {
                    exit(1);
                }
                char *input = (char*)malloc(1024 *sizeof(char));
                size_t size = 1024;
                printf("Send a message!");
                int isEOF = 0;
                while(isEOF != -1)
                {
                    isEOF = getline(&input, &size, stdin);
                    if(isEOF != -1)
                    {
                        printf("%s", input);
                    }
                }
                close(cli_socket);
            }
        }
    }
    
    
}
/**
 * checkIP is a function that scans a string from a pointer for the proper delimiters, and
 * then scans to see if it is valid IP address format.
 */
int checkIP(char *ip)
{
    //if is valid IP
    char* delimit = ".";
    char* numbers = strtok(ip, delimit);
    int count = 1;
    while(numbers != NULL)
    {
        //if the ip address segment has a larger size than 3
        if(strlen(numbers) > 3)
        {
            printf("IP ADDRESS ERROR: SEGMENT %d HAS LENGTH OF %d.\n",count, strlen(numbers));
            return -1;
        }
        else
        {
            for(char* numptr = numbers; *numptr != '\0'; numptr++)
            {
                if(isdigit(*numptr) == 0)
                {
                    printf("IP ADDRESS ERROR: SEGMENT %d IS NOT NUMERIC.\n",count, strlen(numbers));
                    return -1;
                }
            }
        }
        numbers = strtok(NULL, delimit);
        count++;
    }
    return 1;
}


