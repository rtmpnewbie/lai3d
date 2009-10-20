//------------------------------------------------------------------------------
//  raknetclient.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "corenetwork/raknet/raknetclient.h"
#include "corenetwork/packetid.h"
#include "extlibs/raknet/source/raknettypes.h"

namespace RakNet
{
__ImplementClass(RakNet::RaknetClient, 'RKCL', Base::NetClientBase);
__ImplementSingleton(RakNet::RaknetClient);

using namespace CoreNetwork;
//------------------------------------------------------------------------------
/**
*/
RaknetClient::RaknetClient():
rakPeer(0)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
RaknetClient::~RaknetClient()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/ 
bool 
RaknetClient::Open()
{
    n_assert(0 == this->rakPeer);
    bool success = NetClientBase::Open();
    this->rakPeer = n_new(RakPeer);
    success = this->rakPeer->Startup(1, 0, &this->socketDescriptor, 1);

    n_printf("Client started!\n");

    return success;
}
//------------------------------------------------------------------------------
/**
*/
void 
RaknetClient::Close()
{
    n_assert(0 != this->rakPeer);
    this->rakPeer->Shutdown(100);

    //RakNetworkFactory::DestroyRakPeerInterface(this->rakPeer);
    n_delete(this->rakPeer);
}

//------------------------------------------------------------------------------
/**
*/
bool 
RaknetClient::Connect()
{
    n_assert(0 != this->rakPeer);
    n_assert(this->rakPeer->IsActive());

    // TODO allow password access
    bool result = this->rakPeer->Connect(this->serverAdress.AsCharPtr(), this->hostPort, 0,0, 0);
    this->connectionFailed = false;
    // on true result raknet goes into attempting connection state, doesn't mean a immediate connection
    if (result)
    {
        n_printf("Trying to connect at ip: %s with port: %i\n", this->serverAdress.AsCharPtr(), this->hostPort);
    }
    else
    {
        n_printf("Cannot connect at ip: %s with port: %i\n", this->serverAdress.AsCharPtr(), this->hostPort);
    }

    return result;
}

//------------------------------------------------------------------------------
/**
*/
void 
RaknetClient::Disconnect()
{
    n_assert(0 != this->rakPeer);
    n_assert(this->rakPeer->IsActive());
    
    // client must have only one connection at a time
    this->rakPeer->CloseConnection(this->rakPeer->GetSystemAddressFromIndex(0),true,0);
}

//------------------------------------------------------------------------------
/**
*/
bool 
RaknetClient::IsConnected() const
{
    n_assert(0 != this->rakPeer);
    return this->isConnected;
    //return this->rakPeer->IsConnected(this->rakPeer->GetSystemAddressFromIndex(0), inProgress);
}

//------------------------------------------------------------------------------
/**
*/
void 
RaknetClient::SendStream(const Ptr<CoreNetwork::NetStream>& stream, PacketPriority prio, PacketReliability reliability, uint orderingChannel)
{
    n_assert(this->IsConnected());

    // @TODO: handle receiver management for clients,
    // currently client sends to all connected systems (for client-server structure its only the server, so its ok)
    this->rakPeer->Send(stream->GetBitstream(),    // bitstream
                        prio,                      // priority: LOW,MEDIUM,HIGH  
                        reliability,               // reliability  UNRELIABLE, UNRELIABLE_SEQUENCED, RELIABLE, RELIABLE_ORDERED, RELIABLE_SEQUENCED
                        orderingChannel,           // if needed, sort your packet into an orderingchannel  
                        UNASSIGNED_SYSTEM_ADDRESS, // for not braodcasting: send to receiver address , for broadcast address of whom not send to
                        true);                     // broadcast to all connected systems? 
}

//------------------------------------------------------------------------------
/**
*/
bool 
RaknetClient::Update()
{
    bool success = true;
    // validate state and sort out user msg streams
    Packet *p = rakPeer->Receive();    
    while (p)
    {
        switch (p->data[0])
        {
        case ID_CONNECTION_REQUEST_ACCEPTED:
            this->isConnected = true;
            this->connectionLost = false;
            this->connectionSucceeded = true;
            n_printf("ID_CONNECTION_ATTEMPT_ACCEPTED\n");
            break;
            // print out errors
        case ID_CONNECTION_ATTEMPT_FAILED:
            n_printf("Client Error: ID_CONNECTION_ATTEMPT_FAILED\n");
            this->connectionFailed = true;
            this->isConnected = false;
            this->connectionLost = false;
            this->connectionSucceeded = false;
            success = false;
            break;
        case ID_ALREADY_CONNECTED:
            n_printf("Client Error: ID_ALREADY_CONNECTED\n");
            break;
        case ID_NO_FREE_INCOMING_CONNECTIONS:
            n_printf("Client Error: ID_NO_FREE_INCOMING_CONNECTIONS\n");
            this->isConnected = false;
            this->connectionFailed = true;
            this->connectionSucceeded = false;
            success = false;
            break;
        case ID_DISCONNECTION_NOTIFICATION:
            n_printf("ID_DISCONNECTION_NOTIFICATION\n");
            this->isConnected = false;
            this->connectionLost = true;
            this->connectionSucceeded = false;
            break;
        case ID_CONNECTION_LOST:
            n_printf("Client Error: ID_CONNECTION_LOST\n");
            this->isConnected = false;
            this->connectionLost = true;
            this->connectionSucceeded = false;
            success = false;
            break;
        case ID_MODIFIED_PACKET:
            n_printf("Client Error: ID_MODIFIED_PACKET\n");
            success = false;
            break;
            // @TODO: handle rest of possible errors
        case PacketId::ID_NEBULA3_MESSAGE:
            // copy packet for later use
            Ptr<NetStream> newStream = NetStream::Create();
            newStream->Open();
            newStream->Write((void*)(p->data + PACKET_DATA_BYTEOFFSET), p->bitSize/8);
            newStream->Close();
            this->streamArray.Append(newStream);
            break;
        }
        this->rakPeer->DeallocatePacket(p);
        p = this->rakPeer->Receive();
    }

    return success;
}

//------------------------------------------------------------------------------
/**
*/
Ptr<CoreNetwork::NetStream> 
RaknetClient::GetStreamAtIndex(IndexT index)
{
    n_assert(index < this->streamArray.Size());
    return this->streamArray[index];    
}

//------------------------------------------------------------------------------
/**
*/
void 
RaknetClient::RemoveStreamAtIndex(IndexT index)
{
    n_assert(index < this->streamArray.Size());
    return this->streamArray.EraseIndex(index);    
}

//------------------------------------------------------------------------------
/**
*/
SizeT 
RaknetClient::GetNumberRecvStreams() const
{
    return this->streamArray.Size();
}

//------------------------------------------------------------------------------
/**
*/
Util::String 
RaknetClient::GetStatistics() const
{
    char buffer[8192];
    Util::String str;
    if (this->IsConnected())
    {
        StatisticsToString(this->rakPeer->GetStatistics(this->rakPeer->GetSystemAddressFromIndex(0)), (char*)&buffer, 1);
        str = buffer;
    }    
    return str;
}

} // namespace CoreGraphics
