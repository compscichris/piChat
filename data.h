typedef struct DataPacket
{
    int type;
    int length;
    int session_id;
    char payload;
    int seqn;
    int ackn;
}Packet;
typedef struct PacketQueue
{
    struct Packet *thisPacket;
    struct Packet *nextPacket;
}pQueue;