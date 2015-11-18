#ifndef READREGISTERMESSAGEHANDLER_H
#define READREGISTERMESSAGEHANDLER_H

#include "Device/deviceregisters.h"
#include "CommonBootstrapRegister/bootstrapregister.h"

#include "CommonCommand/abstractcommandhandler.h"
#include "Device/gvdevice.h"

#include "CommonPacket/conversionutils.h"

#include "DeviceCommandHandler/deviceackcode.h"

#include "opengv_global.h"

/**
 * @brief The ReadRegisterMessageHandler class implements read register handler
 * required (R-157cd)
 */
class ReadRegisterCommandHandler : public AbstractCommandHandler
{
public:
    ReadRegisterCommandHandler(GVDevice* target,
                               QByteArray datagram,
                               QHostAddress senderAddress,
                               quint16 senderPort);

    int execute();

    quint16 getAckDatagramLengthWithoutHeader();

    /**
     * @brief getAckDatagramWithoutHeader
     * @return datagram (R-164c)
     */
    char* getAckDatagramWithoutHeader();

private:
    int numberOfRegisters;
};

#endif // READREGISTERMESSAGEHANDLER_H