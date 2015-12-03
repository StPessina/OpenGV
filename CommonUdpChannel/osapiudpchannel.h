#ifndef OSAPIUDPSOCKET_H
#define OSAPIUDPSOCKET_H

#include "udpchannel.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAXBUFLEN 4096

class OSAPIUDPChannel : public UDPChannel
{
public:
    OSAPIUDPChannel(QHostAddress sourceAddr, quint16 sourcePort);

    OSAPIUDPChannel(QHostAddress sourceAddr, quint16 sourcePort,
                            AbstractPacketHandlerFactory *packetHandlerFactory);

    /**
     * @brief ~OSAPIUDPSocket deconstructor
     */
    virtual ~OSAPIUDPChannel();

    /**
     * @brief initSocket method create a new socket and register readPendingDatagrams method
     * for receive updates if new datagram is received
     *
     * this method will init SIGNAL/SLOT on udpSocket
     */
    virtual bool initSocket() final;

    /**
     * @brief isSocketOpen method
     * @return true if the socket is open
     */
    virtual bool isSocketOpen() final;

    /**
     * @brief run method for qt thread
     */
    virtual void run();

protected:

    /**
     * @brief writeDatagram method
     * @param datagram to send
     * @param destAddr destination address
     * @param destPort destination port
     */
    virtual int writeDatagram(const QByteArray &datagram, QHostAddress destAddr, quint16 destPort);

    /**
     * @brief hasPendingDatagram
     * @return
     */
    virtual bool hasPendingDatagrams();

private:

    QByteArray datagram;

    int listenerSocket;
    struct addrinfo *pListener, *servinfoListener;

    int sendSocket;
    struct addrinfo *pSend, *servinfoSend;
    QHostAddress lastSendAddress;
    quint16 lastSendPort = 0;


    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    char buf[MAXBUFLEN];
    char s[INET6_ADDRSTRLEN];

    int receive();

    int initSocket(const char* address, const char* port,
                   addrinfo *p, addrinfo *servinfo,
                   bool listen);

    void* getInAddr(struct sockaddr *sa);

    quint16 getInPort(struct sockaddr *sa);

    void closeSocket(int socket, struct addrinfo *p,
                     addrinfo *servinfo);
};

#endif // OSAPIUDPSOCKET_H