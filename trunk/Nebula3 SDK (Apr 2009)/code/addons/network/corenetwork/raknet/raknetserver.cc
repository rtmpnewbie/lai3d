//------------------------------------------------------------------------------
//  RaknetServer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "corenetwork/raknet/raknetserver.h"
#include "corenetwork/netconfig.h"
#include "corenetwork/packetid.h"
#include "extlibs/raknet/source/raknetstatistics.h"

namespace RakNet
{
__ImplementClass(RakNet::RaknetServer, 'RKSR', Base::NetServerBase);
__ImplementSingleton(RakNet::RaknetServer);

using namespace CoreNetwork;

//------------------------------------------------------------------------------
/**
*/
RaknetServer::RaknetServer():
    rakPeer(0),
    threadSleepTime(0)
{
    __ConstructSingleton;
    _setup_counter(PacketsReceived);
}

//------------------------------------------------------------------------------
/**
*/
RaknetServer::~RaknetServer()
{
    __DestructSingleton;
    _discard_counter(PacketsReceived);
}

//------------------------------------------------------------------------------
/**
*/
bool
RaknetServer::Open()
{
    n_assert(!this->IsOpen());
    bool success = false;
    success = NetServerBase::Open();
    
    this->rakPeer = n_new(RakPeer);
    this->rakPeer->SetMaximumIncomingConnections(this->maxConnections);
    SocketDescriptor socketDesc(this->GetPortNumber(), 0);
    // TODO: allow multiple socket descriptors
    // 0 threadsleeptime for fast response
    success = this->rakPeer->Startup(this->GetMaxConnections(), 
                           this->GetThreadSleepTime(), 
                           &socketDesc,
                           1);    

    n_printf("Server started with ip %s and port %i !\n", this->rakPeer->GetLocalIP(0), this->GetPortNumber());
    return success;
}

//------------------------------------------------------------------------------
/**
*/
void
RaknetServer::Close()
{
    n_assert(this->IsOpen());
    n_assert(0 != this->rakPeer);
    this->rakPeer->Shutdown(100);
    
    n_delete(this->rakPeer);

    NetServerBase::Close();
}

//------------------------------------------------------------------------------
/**
*/
bool 
RaknetServer::Update()
{
    this->newIncomingStream = false;

    bool success = true;

    _begin_counter(PacketsReceived);
    Util::String reason;
    Packet *p = rakPeer->Receive();
    while (p)
    {
        switch (p->data[0])
        {
        case ID_NEW_INCOMING_CONNECTION:
            {
                // sort in new connection
                ClientEntry newClient;
                newClient.systemAddress = p->systemAddress;
                newClient.clientIndex = this->rakPeer->GetIndexFromSystemAddress(p->systemAddress);
                this->clients.Add(p->systemAddress, newClient);
                n_printf("New Connection from %s! \n Connections = %i\n", p->systemAddress.ToString(true), this->GetConnectionCount());  
                Ptr<IpAddress> newIpAddress = IpAddress::Create();
                newIpAddress->SetSystemAddress(p->systemAddress);
                this->lastConnectedClients.Enqueue(newIpAddress);
                break;
            }
        case ID_CONNECTION_LOST:
            {
                reason = "lost conenction";
            }
        case ID_DISCONNECTION_NOTIFICATION:
            {
                // sort in new connection    
                if (this->clients.Contains(p->systemAddress))
                {
                    this->clients.Erase(p->systemAddress);
                }
                
                Ptr<IpAddress> newIpAddress = IpAddress::Create();
                newIpAddress->SetSystemAddress(p->systemAddress);
                this->lastDisconnectedClients.Enqueue(newIpAddress);

                reason = "disconnected";
                n_printf("Client at %s! has %s\n Connections = %i\n", p->systemAddress.ToString(true), reason.AsCharPtr(), this->GetConnectionCount());  
            }
            break;

        case PacketId::ID_NEBULA3_MESSAGE:
        case PacketId::ID_NEBULA3_ATTRLIST:
        case PacketId::ID_NEBULA3_STRING:
            {
                // push new netstream for message or attrlist
                n_assert(this->clients.Contains(p->systemAddress));
                Ptr<NetStream> newStream = NetStream::Create();
                newStream->Open();
                newStream->SetMessageId((MessageID)p->data [0]);
                newStream->Write((void*)(p->data + PACKET_DATA_BYTEOFFSET), p->bitSize/8);                
                newStream->Close();
                this->clients[p->systemAddress].streamArray.Append(newStream);

                _incr_counter(PacketsReceived, 1);
                this->newIncomingStream = true;
                break;
            }
        }
        rakPeer->DeallocatePacket(p);
        p = rakPeer->Receive();
    }

    _end_counter(PacketsReceived);

    // check if any clients was killed and doesn't send any disconnection message
    IndexT i;
    for (i = 0; i < this->clients.Size(); i++)
    {
        if (!this->rakPeer->IsConnected(this->clients.KeyAtIndex(i)))
        {
            this->clients.EraseAtIndex(i);
        }
    }

    return success;
}

//------------------------------------------------------------------------------
/**
*/
IndexT 
RaknetServer::GetConnectionCount() const
{    
    return this->clients.Size();
    //
    //uint i,count;
    //for (i = 0, count = 0; i < this->clients.Size; i++)
    //{
    //    if (this->rakPeer->GetSystemAddressFromIndex(i) != UNASSIGNED_SYSTEM_ADDRESS)
    //    {
    //        count++;
    //    }
    //}
    ////count = this->rakPeer->NumberOfConnections();
    ////use of: this->rakPeer->NumberOfConnections (void) const ????
    //return count;
}

//------------------------------------------------------------------------------
/**
*/
void 
RaknetServer::SendStream(const Ptr<CoreNetwork::IpAddress>& receiver, bool broadcast, const Ptr<CoreNetwork::NetStream>& stream, PacketPriority prio /*= HIGH_PRIORITY*/, PacketReliability reliability /*= RELIABLE_ORDERED*/, uint orderingChannel /*= 0*/)
{
    n_assert(broadcast || this->rakPeer->IsConnected(receiver->GetSystemAddress(), false));
    
    SystemAddress recAddress = UNASSIGNED_SYSTEM_ADDRESS;
    if (receiver.isvalid())
    {
        recAddress = receiver->GetSystemAddress();
    }
    this->rakPeer->Send(stream->GetBitstream(),       // bitstream
                        prio,                         // priority: LOW,MEDIUM,HIGH  
                        reliability,                  // reliability  UNRELIABLE, UNRELIABLE_SEQUENCED, RELIABLE, RELIABLE_ORDERED, RELIABLE_SEQUENCED
                        orderingChannel,              // if needed, sort your packet into an ordering channel  
                        recAddress,                   // for not broadcasting: send to receiver address , for broadcast address of whom not send to
                        broadcast);                   // broadcast to all connected systems? 
}

//------------------------------------------------------------------------------
/**
*/
Ptr<CoreNetwork::NetStream> 
RaknetServer::GetStreamAtIndex(const Ptr<CoreNetwork::IpAddress>& client, IndexT index)
{
    n_assert(this->clients.Contains(client->GetSystemAddress()));
    n_assert(index < this->clients[client->GetSystemAddress()].streamArray.Size());
    return this->clients[client->GetSystemAddress()].streamArray[index];
}

//------------------------------------------------------------------------------
/**
*/
void 
RaknetServer::RemoveStreamAtIndex(const Ptr<CoreNetwork::IpAddress>& client, IndexT index)
{
    n_assert(this->clients.Contains(client->GetSystemAddress()));
    n_assert(index < this->clients[client->GetSystemAddress()].streamArray.Size());
    this->clients[client->GetSystemAddress()].streamArray.EraseIndex(index);
}

//------------------------------------------------------------------------------
/**
*/
void 
RaknetServer::ClearAllStreamsOfClient(const Ptr<CoreNetwork::IpAddress>& client)
{
    n_assert(this->clients.Contains(client->GetSystemAddress()));    
    this->clients[client->GetSystemAddress()].streamArray.Clear();
}

//------------------------------------------------------------------------------
/**
*/
SizeT 
RaknetServer::GetNumberRecvStreams(const Ptr<CoreNetwork::IpAddress>& client) const
{
    n_assert(this->clients.Contains(client->GetSystemAddress()));
    return this->clients[client->GetSystemAddress()].streamArray.Size();
}

//------------------------------------------------------------------------------
/**
*/
Ptr<CoreNetwork::IpAddress>
RaknetServer::GetClientAddress(IndexT i) const
{
    n_assert(i < NUM_CLIENTS);
    SystemAddress address = this->rakPeer->GetSystemAddressFromIndex(i);
    Ptr<CoreNetwork::IpAddress> ipAddress;
    if (address != UNASSIGNED_SYSTEM_ADDRESS)
    {
        ipAddress = CoreNetwork::IpAddress::Create();
        ipAddress->SetSystemAddress(address);
    }

    return ipAddress;
}

//------------------------------------------------------------------------------
/**
*/
Ptr<CoreNetwork::IpAddress>
RaknetServer::DequeueNewConnectedClient()
{
    return this->lastConnectedClients.Dequeue();
}

//------------------------------------------------------------------------------
/**
*/
Ptr<CoreNetwork::IpAddress>
RaknetServer::DequeueNewDisconnectedClient()
{
    return this->lastDisconnectedClients.Dequeue();
}

//------------------------------------------------------------------------------
/**
*/
Util::String 
RaknetServer::GetStatistics(const Ptr<CoreNetwork::IpAddress>& client) const
{
    char buffer[8192];
    StatisticsToString(this->rakPeer->GetStatistics(client->GetSystemAddress()), (char*)&buffer, 1);
    Util::String str(buffer);
    return str;
}
} // namespace CoreNetwork
