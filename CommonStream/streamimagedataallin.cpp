#include "streamimagedataallin.h"

StreamImageDataAllIn::StreamImageDataAllIn(QHostAddress destAddress,
                                             quint16 destPort, quint64 blockId64,
                                             quint32 pixelFormat,
                                             quint32 sizex, quint32 sizey,
                                             quint32 offsetx, quint32 offsety,
                                             quint16 paddingx, quint16 paddingy,
                                             QByteArray data)
    : AbstractStreamData(destAddress, destPort, PacketFormat::DATA_ALLIN_FORMAT, blockId64, 0)
{
    this->pixelFormat = pixelFormat;
    this->sizex = sizex;
    this->sizey = sizey;
    this->offsetx = offsetx;
    this->offsety = offsety;
    this->paddingx = paddingx;
    this->paddingy = paddingy;

    this->data = data;
}

StreamImageDataAllIn::~StreamImageDataAllIn()
{

}

int StreamImageDataAllIn::executeAnswer(QByteArray answer)
{
    return 0;
}

quint16 StreamImageDataAllIn::getLengthWithoutHeader()
{
    return 36 + 20 + data.size();
}

QByteArray StreamImageDataAllIn::getPacketDatagramWithoutHeader()
{
    QByteArray body;
    body.reserve(getLengthWithoutHeader());

    StreamImageDataLeader leaderBody(getDestinationAddress(),
                                     getDestionationPort(),
                                     getBlockId64(),
                                     getPacketId32(),
                                     pixelFormat,sizex,sizey,offsetx,offsety,paddingx,paddingy);
    body.append(leaderBody.getPacketDatagramWithoutHeader());

    StreamImageDataTrailer trailerBody(getDestinationAddress(),
                                       getDestionationPort(),
                                       getBlockId64(),
                                       getPacketId32(),
                                       sizey);
    body.append(trailerBody.getPacketDatagramWithoutHeader());

    body.append(data);

    return body;
}
