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
#include "data.h"
#include <ctype.h>
/**
 * server.c
 * Phase 1 of this personal project will be to allow one single client to connect to the server.
 * The server program will continually listen for a socket connection from the client.
 * Once the client has connected, the server will listen to input sent from client, and will 
 * reply to the client with what the client sent.
 * Author: compscichris
 * Date: 9/1/24
 */
struct User
{
    char name[32];
    int sessionID;
};
int serverConnection(struct servConnection *servSock, struct cliConnection *clients);
//idea: Use the server to create a chat server that will store every user's messages.

//HOW A CONNECTION FROM A CLIENT IS INITIATED.
//CONNECTION IS INITIALIZED FIRST, AND HEARTBEATS ARE MADE FROM CLIENT FIRST, AND SERVER
//SENDS BACK A HEARTBEAT. THE SERVER SHOULD HAVE 
int main()
{
    printf("SERVER INITIALIZING...\n");
    
    //EXTRA: FUTURE FEATURE USED IN ARRAY OF CLICONNECTIONS
    struct cliConnection clients[8];
    
    //int serv_socket, cli1_socket, cli2_socket;
    //struct sockaddr_in serv_addr, cli1_addr, cli2_addr;
    //struct timeval *curr_time = malloc(sizeof(struct timeval));
    struct servConnection *servSock = malloc(sizeof(sConnect));
    servSock->serv_socket = socket(PF_INET, SOCK_STREAM, 0);
    servSock->ip = NULL;
    servSock->pSentHead = NULL;
    servSock->pRecvHead = NULL;
    servSock->pSentTail = NULL;
    servSock->pRecvTail = NULL;
    servSock->type = 1;
    //servSock->serv_addr = malloc(sizeof(struct sockaddr_in));
    int cur_ackN = 0;
    int cur_seqN = 0;
    int exp_ackN = 1;
    int exp_seqN = 1;
    
    //server socket initialization
    memset(servSock->serv_addr, 0, sizeof(struct sockaddr_in));
    servSock->serv_addr->sin_family = PF_INET;
    servSock->serv_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    servSock->serv_addr->sin_port = htons(2);
    
    serverConnection(servSock, &clients[0]);
    
}
int serverConnection(struct servConnection *servSock, struct cliConnection *client)
{
    int s_sessionID = rand();
    //struct cliConnection *client = malloc(sizeof(struct cliConnection));
    //int serv_socket, cli1_socket, cli2_socket;
    //struct sockaddr_in serv_addr, cli1_addr, cli2_addr;
    struct timeval *curr_time = malloc(sizeof(struct timeval));
    int cur_ackN = 0;
    int cur_seqN = 0;
    int exp_ackN = 1;
    int exp_seqN = 1;
    //server socket initialization
    //head of linkedlist structure of sent and received packets
    //servSock->pSentHead = malloc(sizeof(pQueue));
    //servSock->pRecvHead = malloc(sizeof(pQueue));
    //tail of linkedlist structure of sent and received packets
    //servSock->pSentTail = NULL;
    //servSock->pRecvTail = NULL;
    //memset(servSock->serv_addr, 0, sizeof(struct sockaddr_in));
    //servSock->serv_addr->sin_family = PF_INET;
    //servSock->serv_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    //servSock->serv_addr->sin_port = htons(2);
    //servSock->serv_socket = socket(PF_INET, SOCK_STREAM, 0);
    if(servSock->serv_socket<0)
    {
        perror("SERVER INITIALIZATION ERROR: SOCKET CREATION FAILURE.\n");
        exit(1);
    }    
    
    //binds the server address to the server socket 
    if(bind(servSock->serv_socket, (struct sockaddr *) &servSock->serv_addr, sizeof(struct sockaddr))<0)
    {
        perror("ERROR: SERVER BINDING FAILURE.");
        exit(1);
    }
    //ACCEPTING CONNECTIONS
    while(1)
    {
        //LISTENING TO NEW SOCKET CONNECTIONS
        if(listen(servSock->serv_socket, 5) <0)
        {
            perror("ERROR: SERVER LISTENING");
            exit(1);
        }
        socklen_t addrlen;
        addrlen = sizeof(struct sockaddr_in);
        //connect to the client1 socket
        client->cli_socket = accept(servSock->serv_socket, (struct sockaddr *) client->cli_addr, &addrlen);
        if(client->cli_socket < 0)
        {
            perror("ERROR: CLIENT NOT ACCEPTED.");
            exit(1);
        }
        client->pSentHead = malloc(sizeof(pQueue));
        client->pRecvHead = malloc(sizeof(pQueue));
        //tail of linkedlist structure of sent and received packets
        client->pSentTail = NULL;
        client->pRecvTail = NULL;
        //Create a timer
        struct timeval connect_time;
        connect_time.tv_sec = 1;
        connect_time.tv_usec = 0;
        curr_time = &connect_time;
        gettimeofday(&connect_time, NULL);
        
        fd_set readfds;
        int nfds = client->cli_socket + 1;
        int heartbeat_rv;
        FD_ZERO(&readfds);
        FD_SET(client->cli_socket, &readfds);
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
            if(FD_ISSET(client->cli_socket, &readfds))
            {
                Packet recvBuff;
                int bytes = recv(client->cli_socket, &recvBuff, sizeof(Packet), 0);
                if(bytes <= 0)
                {
                    perror("Error receiving heartbeat from client");
                }
                else{
                    if(recvBuff.seqn == exp_seqN)
                    {
                        struct PacketQueue *curRecv;
                        //first packet not in linkedlist of received packets
                        if(client->pRecvTail == NULL)
                        {
                            curRecv = client->pRecvHead;
                            curRecv->thisPacket = &recvBuff;
                            curRecv->nextPacket = NULL;
                            client->pRecvTail = client->pRecvHead;
                        }
                        else{
                            curRecv = malloc(sizeof(Packet));
                            curRecv->thisPacket = &recvBuff;
                            curRecv->nextPacket = NULL;
                            client->pRecvTail->nextPacket = curRecv;
                            client->pRecvTail = client->pRecvHead;
                        }
                        struct PacketQueue *reply;
                        //first packet not in linkedlist of sent packets
                        Packet sendBuff = {0, sizeof(int), s_sessionID, {0}, cur_seqN, cur_ackN};
                        if(client->pSentTail == NULL)
                        {
                            reply = client->pSentHead;
                            reply->thisPacket = &sendBuff;
                            reply->nextPacket = NULL;
                            client->pSentTail = client->pSentHead;
                        }
                        else{
                            reply = malloc(sizeof(Packet));
                            reply->thisPacket = &sendBuff;
                            reply->nextPacket = NULL;
                            client->pSentTail->nextPacket = reply;
                            client->pSentTail = client->pSentHead;
                        }
                        if(send(client->cli_socket, &sendBuff, sizeof(Packet), 0) <= 0)
                        {
                            perror("ERROR: HEARTBEAT NOT RECEIVED.");
                        }
                    }
                    
                }
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