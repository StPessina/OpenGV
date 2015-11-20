#include "streamimagedatapayloadhandler.h"

StreamImageDataPayloadHandler::StreamImageDataPayloadHandler(GVComponent *target, QByteArray datagram,
                                                             QHostAddress senderAddress, quint16 senderPort)
    : AbstractStreamDataHandler(target, PacketFormat::DATA_PAYLOAD_GENIRIC_FORMAT, datagram, senderAddress, senderPort)
{
}

int StreamImageDataPayloadHandler::execute()
{
    int resultStatus;
    if(!checkHeader())
        resultStatus = GEV_STATUS_INVALID_HEADER;
    else {

        StreamDataReceiver* receiver = (StreamDataReceiver*) target;

        quint32 pixelFormat = receiver->getPixelFormat();

        QByteArray datagramWithoutHeader = datagram.mid(20);

        switch (pixelFormat) {
        case GVSP_PIX_MONO16: {
            int size = datagramWithoutHeader.size() / 2;
            for (int i = 0; i < size; ++i) {
                quint16 data = ConversionUtils::getShortFromQByteArray(datagramWithoutHeader,i*2);
                receiver->addStreamData(getRequestBlockId(),
                                        new Mono16(data));
            }
            break;
        }
        default:
            break;
        }

    }

    return resultStatus;
}

quint16 StreamImageDataPayloadHandler::getAckDatagramLengthWithoutHeader()
{
    return 0;
}

char *StreamImageDataPayloadHandler::getAckDatagramWithoutHeader()
{
    return NULL;
}

