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
#include <sys/time.h>

#include "data.h"
int checkIP(char*);
/**
 * makeConnection takes the details required to make a connection
 * 
 */
int makeConnection(struct servConnection *servCon, struct timeval *curr_time);

int main()
{
    
    while (1)
    {
        //Step 1: Ask user for input of address
        //main function testing input grabbing from console
        char input[1024];
        char inputcopy[1024];
        struct timeval *curr_time = malloc(sizeof(struct timeval));
        
        struct servConnection *servCon = malloc(sizeof(sConnect));
        servCon->ip = NULL;
        servCon->pSentHead = malloc(sizeof(pQueue));
        servCon->pRecvHead = malloc(sizeof(pQueue));
        servCon->pSentTail = NULL;
        servCon->pRecvTail = NULL;
        servCon->type = 1;
        printf("Enter IP address of server to connect to in format of <x.x.x.x>: (quit to exit): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input,"\n")] = 0;
        memcpy(inputcopy, input, sizeof(input));
        servCon->ip = inputcopy;
        if(feof(stdin) || strcmp(input,"quit") == 0)
        {
            printf("EXITING");
            break;
        }
        else{
            if(checkIP(servCon->ip) == -1)
            {
                printf("trying again\n");
            }
            else{
                makeConnection(servCon, curr_time);
            }
        }
    }
    return 0; 
}
int makeConnection(struct servConnection *servCon, struct timeval *curr_time)
{
    //int socketa = socket(domain, type, protocol)
    int cur_ackN = 0;
    int cur_seqN = 0;
    int exp_ackN = 1;
    int exp_seqN = 1;
    servCon->serv_addr->sin_family = AF_INET;
    servCon->serv_addr->sin_addr.s_addr = inet_addr(servCon->ip);
    servCon->serv_addr->sin_port = htons(2);
    servCon->serv_socket = socket(PF_INET, SOCK_STREAM, 0);
    if(connect(servCon->serv_socket, (struct sockaddr*) &servCon->serv_addr,
     sizeof(servCon->serv_addr))<0)
    {
        exit(1);
    }
    //send first heartbeat
    Packet sentBeat = {0, sizeof(int), 0, "0", cur_seqN, cur_ackN};
    Packet recvBeat;
    char *input = (char*)malloc(1024 *sizeof(char));
    int bytes;
    if(send(servCon->serv_socket, &sentBeat, sizeof(Packet), 0) <=0)
    {
        perror("ERROR: HEARTBEAT SENDING FAILURE.");
    }
    else{
        bytes = recv(servCon->serv_socket, &recvBeat, sizeof(Packet), 0);
        if(bytes <= 0)
        {
            perror("ERROR: HEARTBEAT REPLY NOT RECEIVED.");
        }
    }        
    int sessionID = recvBeat.session_id;
    size_t size = 1024;
    printf("Send a message!");
    int isEOF = 0;
    while(isEOF != -1)
    {
        isEOF = getline(&input, &size, stdin);
        if(isEOF != -1)
        {
            printf("Sending message: %s", input);
            Packet *sendBuff = malloc(sizeof(Packet));
            sendBuff->seqn = cur_seqN;
            sendBuff->ackn = exp_ackN;
            sendBuff->length = isEOF;
            sendBuff->session_id = sessionID;
            sendBuff->type = 1;
            strcpy(sendBuff->payload, input);
            if(send(servCon->serv_socket, &sendBuff, sizeof(Packet), 0) <= 0)
            {
                perror("ERROR: PACKET SEND ERROR");
            }
            Packet *recvBuff = malloc(sizeof(Packet));
            if(recv(servCon->serv_socket, &recvBuff, sizeof(Packet), 0) <= 0)
            {
                perror("ERROR: PACKET NOT RECEIVED FROM SERVER.");
            }
            else{
                printf("Packet from server, session ID: %d", recvBuff->session_id);
                printf("Reply message: %s", recvBuff->payload);
            }
        }
    }
    close(servCon->serv_socket);
    return 0;        
}
/**
 * checkIP is a function that scans a string from a pointer for the proper delimiters, and
 * then scans to see if it is valid IP address format.
 */
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
    //printf("WORKING\n");
    while(numbers != NULL)
    {
        //if the ip address segment has a larger size than 3
        //printf("WORKING %d +1\n", count);
        int length = strlen(numbers);
        if(length > 3)
        {
            printf("IP ADDRESS ERROR: SEGMENT %d HAS LENGTH OF %d.\n",count, length);
            return -1;
        }
        else
        {
            for(char* numptr = numbers; *numptr != '\0'; numptr++)
            {
                if(isdigit(*numptr) == 0)
                {
                    printf("IP ADDRESS ERROR: SEGMENT %d IS NOT NUMERIC.\n",count);
                    return -1;
                }
            }
        }
        //printf("WORKING %d +2\n", count);
        numbers = strtok(NULL, delimit);
        count++;
    }
    return 1;
}


