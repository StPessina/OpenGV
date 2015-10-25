#ifndef DEVICECONTROLCHANNEL_H
#define DEVICECONTROLCHANNEL_H

#include "CommonControlChannel/controlchannel.h"

class ControlChannelSlave : public ControlChannel
{
public:
    ControlChannelSlave(QHostAddress sourceAddr,
                         quint16 sourcePort,
                         AbstractMessageHandlerFactory *messageHandlerFactory);

    virtual ~ControlChannelSlave();

    void processTheDatagram(QByteArray datagram, QHostAddress sender, quint16 senderPort);

private:
    AbstractMessageHandlerFactory* messageHandlerFactory;
};

#endif // DEVICECONTROLCHANNEL_H