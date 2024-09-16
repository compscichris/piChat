typedef struct DataPacket
{
    int type;
    int length;
    int session_id;
    char payload[1024];
    int seqn;
    int ackn;
}Packet;
typedef struct PacketQueue
{
    struct Packet *thisPacket;
    struct Packet *nextPacket;
}pQueue;
/**
 * servConnection is a connection structure that represents a connection
 * from a client to a server (the client uses this object to refer to a
 * server).
 * char *ip is a character buffer that stores the 
 */
typedef struct servConnection
{
    char *ip;
    int serv_socket;
    struct sockaddr_in *serv_addr;
    struct PacketQueue *pSentHead;
    struct PacketQueue *pRecvHead;
    //tail of linkedlist structure of sent and received packets
    struct PacketQueue *pSentTail;
    struct PacketQueue *pRecvTail;
}sConnect;
typedef struct cliConnection
{
    char *ip;
    int serv_socket;
    struct sockaddr_in *serv_addr;
    struct PacketQueue *pSentHead;
    struct PacketQueue *pRecvHead;
    //tail of linkedlist structure of sent and received packets
    struct PacketQueue *pSentTail;
    struct PacketQueue *pRecvTail;
}cConnect;