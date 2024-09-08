#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
//libraries
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
/**
 * server.c
 * Author: compscichris
 * Date: 9/1/24
 */
struct User
{
    char name[32];
    int sessionID;
};
//idea: Use the server to create a chat server that will store every user's messages.
int main()
{

    int serv_socket, cli1_socket, cli2_socket;
    struct sockaddr_in serv_addr, cli1_addr, cli2_addr;
    struct timeval *curr_time = malloc(sizeof(struct timeval));
    
    //server socket initialization


    if(serv_socket<0)
    {
        perror("ERROR creating socket");
        exit(1);
    }    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(2);
    serv_socket = socket(PF_INET, SOCK_STREAM, 0);
    //binds the server address to the server socket 
    if(bind(serv_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
    {
        perror("ERROR: SERVER BINDING");
        exit(1);
    }
    //ACCEPTING CONNECTIONS
    while(1)
    {
        //LISTENING TO NEW SOCKET CONNECTIONS
        if(listen(serv_socket, 5) <0)
        {
            perror("ERROR: SERVER LISTENING");
            exit(1);
        }
        socklen_t addrlen;
        addrlen = sizeof(struct sockaddr_in);
        //connect to the client1 socket
        cli1_socket = accept(serv_socket, (struct sockaddr*)&serv_addr, &addrlen);
        if(cli1_socket < 0)
        {
            perror("ERROR: CLIENT NOT ACCEPTED.");
            exit(1);
        }
        char payload[1024];
        while(1)
        {
            uint32_t length;
            int n = recv(cli1_socket,(char *) &length, 4, 0);
            size_t size = ntohl(length);
            int lin = recv(cli1_socket, payload, size, 0);
        }
        //Create a timer
        struct timeval connect_time;
        connect_time.tv_sec = 1;
        connect_time.tv_usec = 0;
        curr_time = &connect_time;
        gettimeofday(&connect_time, NULL);
        
        int connection_lost=0;
        fd_set readfds;
        int nfds = cli1_socket + 1;
        int heartbeat_rv;
        FD_ZERO(&readfds);
        FD_SET(cli1_socket, &readfds);
        //accept first heartbeat
        heartbeat_rv = select(nfds, &readfds, NULL, NULL, curr_time);
        if(heartbeat_rv == -1)
        {
            perror("ERROR: Heartbeat detection failure");
        }
        else if(heartbeat_rv == 0)
        {
            perror("ERROR: TIMEOUT");
        }
        else
        {
            packet buffer;
            recv(cli1_socket, buffer, length, flags);
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