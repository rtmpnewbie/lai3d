#pragma once
#ifndef RAKNET_RAKNETCLIENT_H
#define RAKNET_RAKNETCLIENT_H
//------------------------------------------------------------------------------
/**
    @class RakNet::RaknetClient

    The client uses a raknet peer to send and receive network packets to a connected server.
    It allows sending of NetStreams, which are simple bitstreams with a raknet message id.
    Further handling of incoming streams are done in derived class.
      
    (C) 2008 Radon Labs GmbH
*/    
#include "corenetwork/base/netclientbase.h"
#include "extlibs/raknet/source/rakpeer.h"
#include "corenetwork/netstream.h"
#include "util/stack.h"
#include "io/uri.h"

//------------------------------------------------------------------------------
namespace RakNet
{
class RaknetClient : public Base::NetClientBase
{
    __DeclareClass(RaknetClient);
    __DeclareSingleton(RaknetClient);
public:
    /// constructor
    RaknetClient();
    /// destructor
    virtual ~RaknetClient();

    /// open client thread
    virtual bool Open();
    /// close client thread
    virtual void Close();
    /// connect, returns true if client goes into connecting attempt
    bool Connect();
    /// disconnect from server
    void Disconnect();
    /// return true if connected
    bool IsConnected() const;
    /// update state of client from received streams
    bool Update();
    /// send bitstream to connected server
    void SendStream(const Ptr<CoreNetwork::NetStream>& stream, PacketPriority prio = HIGH_PRIORITY, PacketReliability reliability = RELIABLE_ORDERED, uint orderingChannel = 0);
    /// get stream at index
    Ptr<CoreNetwork::NetStream> GetStreamAtIndex(IndexT index);
    /// remove stream at index 
    void RemoveStreamAtIndex(IndexT index);
    /// get number of received streams
    SizeT GetNumberRecvStreams() const;
    /// set ip address and port from IO::Uri
    void SetClientAddress(const IO::URI& uri);

    /// print statistics
    Util::String GetStatistics() const;

private:
    RakPeerInterface* rakPeer;
    Util::Array<Ptr<CoreNetwork::NetStream> > streamArray;    
    SocketDescriptor socketDescriptor;
};
//------------------------------------------------------------------------------
inline void 
RaknetClient::SetClientAddress(const IO::URI& uri)
{
    n_assert(uri.Host().Length() <= 32);
    strcpy(this->socketDescriptor.hostAddress, uri.Host().AsCharPtr());
    n_assert(uri.Port().AsInt() < 65536);
    this->socketDescriptor.port = (ushort) uri.Port().AsInt();
}
} // namespace RakNet
//------------------------------------------------------------------------------
#endif

