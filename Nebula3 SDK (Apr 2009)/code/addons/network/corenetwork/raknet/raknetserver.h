#pragma once
#ifndef RAKNET_RAKNETSERVER_H
#define RAKNET_RAKNETSERVER_H
//------------------------------------------------------------------------------
/**
    @class RakNet::RaknetServer
  
    Implements a NetServer on top of RakNet.
    It allows sending of NetStreams, which are simple bitstreams with a raknet message id, to a connected client 
    or braodcast to all connected clients.
    Further handling of incoming streams, connections and disconnections  are done in derived class.
    
    (C) 2008 Radon Labs GmbH
*/    
#include "corenetwork/base/netserverbase.h"
#include "extlibs/raknet/source/rakpeer.h"
#include "util/queue.h"
#include "corenetwork/netstream.h"
#include "corenetwork/ipaddress.h"
#include "debug/debugcounter.h"

//------------------------------------------------------------------------------
namespace RakNet
{
class RaknetServer : public Base::NetServerBase
{
    __DeclareClass(RaknetServer);
    __DeclareSingleton(RaknetServer);
public:
    /// constructor
    RaknetServer();
    /// destructor
    virtual ~RaknetServer();

    /// open the device
    bool Open();
    /// close the device
    void Close();
    /// update state of server from received streams
    bool Update();

    /// set time rak network thread sleeps every frame
    void SetThreadSleepTime(int t);
    /// get sleep time
    int GetThreadSleepTime()const;

    /// get number of connections
    IndexT GetConnectionCount() const;
    /// get client ipadrress at index
    Ptr<CoreNetwork::IpAddress> GetClientAddress(IndexT i) const;
    /// is client with ip connected
    bool IsClientConnected(const Ptr<CoreNetwork::IpAddress>& address);
    
    /// send bitstream to connected server
    void SendStream(const Ptr<CoreNetwork::IpAddress>& receiver, bool broadcast, const Ptr<CoreNetwork::NetStream>& stream, PacketPriority prio = HIGH_PRIORITY, PacketReliability reliability = RELIABLE_ORDERED, uint orderingChannel = 0);
    /// get oldest receive stream
    Ptr<CoreNetwork::NetStream> GetStreamAtIndex(const Ptr<CoreNetwork::IpAddress>& client, IndexT index);
    /// remove stream at index
    void RemoveStreamAtIndex(const Ptr<CoreNetwork::IpAddress>& client, IndexT index);
    /// remove stream at index
    void ClearAllStreamsOfClient(const Ptr<CoreNetwork::IpAddress>& client);
    /// get number of received streams
    SizeT GetNumberRecvStreams(const Ptr<CoreNetwork::IpAddress>& client) const;

    /// has new incoming connection
    bool HasNewIncomingConnection() const;
    /// has new lost connections or disconections
    bool HasNewDisconnections() const;
    /// has new incoming nebula3 stream
    bool HasNewStreams() const;
    /// dequeue last connected clients
    Ptr<CoreNetwork::IpAddress> DequeueNewConnectedClient();
    /// dequeue last disconnected clients
    Ptr<CoreNetwork::IpAddress> DequeueNewDisconnectedClient();

    /// print statistics
    Util::String GetStatistics(const Ptr<CoreNetwork::IpAddress>& client) const;

private:
    RakPeerInterface* rakPeer;
    int threadSleepTime; 
    bool newIncomingStream;

    struct ClientEntry
    {
        Util::Array<Ptr<CoreNetwork::NetStream> > streamArray;
        SystemAddress systemAddress;
        uint clientIndex;
    };
    Util::Queue<Ptr<CoreNetwork::IpAddress> > lastConnectedClients;
    Util::Queue<Ptr<CoreNetwork::IpAddress> > lastDisconnectedClients;
    Util::Dictionary<SystemAddress, ClientEntry> clients;

    _declare_counter(PacketsReceived);
};

//------------------------------------------------------------------------------
/**
*/
inline void 
RaknetServer::SetThreadSleepTime(int t)
{
    this->threadSleepTime = t;
}

//------------------------------------------------------------------------------
/**
*/
inline int 
RaknetServer::GetThreadSleepTime()const
{
    return this->threadSleepTime;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
RaknetServer::HasNewIncomingConnection() const
{
    return this->lastConnectedClients.Size() > 0;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
RaknetServer::HasNewDisconnections()const
{
    return this->lastDisconnectedClients.Size() > 0;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
RaknetServer::HasNewStreams() const
{
    return this->newIncomingStream;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
RaknetServer::IsClientConnected(const Ptr<CoreNetwork::IpAddress>& address)
{
    return this->clients.Contains(address->GetSystemAddress());
}
} // namespace RakNet
//------------------------------------------------------------------------------
#endif

