#include "partnerdevice.h"

PartnerDevice::PartnerDevice()
{
    channelOpen=false;
}

PartnerDevice::~PartnerDevice()
{
    foreach (auto channel, streamChannelsOpenMap)
        delete channel.second;
    streamChannelsOpenMap.clear();

    if(isChannelOpen())
        delete controlChannel;
}

bool PartnerDevice::openControlChannel(quint16 port)
{
    controlChannel = new UDPChannelTransmitter(QHostAddress::Any, port);

    controlChannel->initSocket();

    //send open channel message
    controlChannelKey = rand()*32000;

    int CCP = controlChannelKey; //create control
    CCP |= 0x80000000; //Exclusive access

    WriteRegisterCommand writeReg (this,
                                   REG_CONTROL_CHANNEL_PRIVILEGE,
                                   CCP,
                                   ipAddress,
                                   CONTROL_CHANNEL_DEF_PORT);
    controlChannel->sendCommand(writeReg);
    short statusCode = writeReg.getStatusCode();

    if(statusCode==GEV_STATUS_SUCCESS)
        channelOpen=true;
    else {
        channelOpen=false;
        delete controlChannel;
        return false;
    }

    return true;
}

void PartnerDevice::closeControlChannel()
{
    if(isChannelOpen()) { //Send message for channel unlock

        WriteRegisterCommand writeReg (this,
                                       REG_CONTROL_CHANNEL_PRIVILEGE,
                                       0,
                                       ipAddress,
                                       CONTROL_CHANNEL_DEF_PORT);
        controlChannel->sendCommand(writeReg);

        if(writeReg.getStatusCode()==GEV_STATUS_SUCCESS) {
            controlChannelKey = 0;
            channelOpen=false;
            delete controlChannel;
        }
    }
}

bool PartnerDevice::isChannelOpen()
{
    return channelOpen;
}

bool PartnerDevice::setActionControlAccessKey(int key)
{
    if(!isChannelOpen())
        return false;

    WriteRegisterCommand writeReg(this,
                                  REG_ACTION_DEVICE_KEY,
                                  key,
                                  ipAddress,
                                  CONTROL_CHANNEL_DEF_PORT);
    controlChannel->sendCommand(writeReg);
    return writeReg.getStatusCode() == GEV_STATUS_SUCCESS;
}

bool PartnerDevice::setStreamChannelDelay(int channel, quint32 delay)
{
    if(!isChannelOpen())
        return false;

    WriteRegisterCommand writeReg(this,
                                  DeviceRegisterConverter::getStreamChannelRegister(channel,REG_STREAM_CHANNEL_PACKET_DELAY),
                                  delay,
                                  ipAddress,
                                  CONTROL_CHANNEL_DEF_PORT);

    controlChannel->sendCommand(writeReg);

    return writeReg.getStatusCode() == GEV_STATUS_SUCCESS;
}

bool PartnerDevice::setStreamChannelPacketLength(int channel, quint32 size)
{
    if(!isChannelOpen())
        return false;

    WriteRegisterCommand writeReg(this,
                                  DeviceRegisterConverter::getStreamChannelRegister(channel,REG_STREAM_CHANNEL_PACKET_SIZE),
                                  size,
                                  ipAddress,
                                  CONTROL_CHANNEL_DEF_PORT);

    controlChannel->sendCommand(writeReg);

    return writeReg.getStatusCode() == GEV_STATUS_SUCCESS;
}

int PartnerDevice::getStreamingChannelNumber()
{
    if(!isChannelOpen())
        return -1;

    ReadRegisterCommand readReg (this,
                                 REG_NR_STREAM_CHANNELS,
                                 ipAddress,
                                 CONTROL_CHANNEL_DEF_PORT);
    controlChannel->sendCommand(readReg);
    int value = readReg.getRegisterValue();
    return value;
}

int PartnerDevice::openStreamChannel(int channel)
{
    if(!isChannelOpen())
        return GEV_STATUS_ACCESS_DENIED;

    if(getStreamingChannelNumber()<=channel)
        return GEV_STATUS_INVALID_ADDRESS;

    StreamDataReceiver* streamChannel = new StreamDataReceiver(ipAddress, 40000 + channel);
    streamChannelsOpenMap[channel]=streamChannel;

    WriteRegisterCommand writeReg (this,
                                   DeviceRegisterConverter::getStreamChannelRegister(channel,REG_STREAM_CHANNEL_PORT),
                                   40000 + channel,
                                   ipAddress,
                                   CONTROL_CHANNEL_DEF_PORT);
    controlChannel->sendCommand(writeReg);
    short result = writeReg.getStatusCode();

    if(result != GEV_STATUS_SUCCESS) {
        streamChannelsOpenMap[channel]=NULL;
        delete streamChannel;
    }

    return result;
}

StreamDataReceiver *PartnerDevice::getStreamChannel(int channel)
{
    if(!isChannelOpen())
        return NULL;

    if(getStreamingChannelNumber()<=channel)
        return NULL;

    return streamChannelsOpenMap[channel];
}
