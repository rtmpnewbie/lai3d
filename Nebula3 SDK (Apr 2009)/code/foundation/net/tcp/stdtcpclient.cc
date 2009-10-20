//------------------------------------------------------------------------------
//  stdtcpclient.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "net/tcp/stdtcpclient.h"
#include "io/memorystream.h"

namespace Net
{
__ImplementClass(Net::StdTcpClient, 'STCL', Core::RefCounted);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
StdTcpClient::StdTcpClient() :
    blocking(true)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
StdTcpClient::~StdTcpClient()
{
    if (this->IsConnected())
    {
        this->Disconnect();
    }
}

//------------------------------------------------------------------------------
/**
    Establish a connection with the server. If the client is set to
    non-blocking at the time this method is called, it will return immediately
    with the result Connecting. To check if the connection is standing, just
    call Connect() again in intervals which will eventually return Success.
    On a blocking client, connect returns after a connection has been established,
    or with a time out when no connection could be established.
*/
StdTcpClient::Result
StdTcpClient::Connect()
{
    n_assert(!this->socket.isvalid());
    n_assert(!this->sendStream.isvalid());
    n_assert(!this->recvStream.isvalid());

    // create a new socket and try to connect to server
    this->socket = Socket::Create();
    if (this->socket->Open(Socket::TCP))
    {
        this->socket->SetAddress(this->serverAddr);
        this->socket->SetBlocking(this->blocking);
        this->socket->SetReUseAddr(true);
        this->socket->SetNoDelay(true);

        n_printf("StdTcpClient: connecting to host '%s(%s)' port '%d'...\n", 
            this->serverAddr.GetHostName().AsCharPtr(),
            this->serverAddr.GetHostAddr().AsCharPtr(),
            this->serverAddr.GetPort());

        Socket::Result res = this->socket->Connect();
        if (Socket::Error == res)
        {
            n_printf("StdTcpClient: failed to connect.\n");
            return Error;
        }
        else if (Socket::Success == res)
        {
            n_printf("StdTcpClient: connection established\n");

            this->sendStream = MemoryStream::Create();
            this->recvStream = MemoryStream::Create();
            return Success;
        }
        else
        {
            n_printf("StdTcpClient: connecting...\n");
            return Connecting;
        }
    }
    n_printf("StdTcpClient: failed to open socket!\n");
    this->socket = 0;
    return Error;
}

//------------------------------------------------------------------------------
/**
    Return true if the socket is currently connected. This will actually
    probe the connection using a select().
*/
bool
StdTcpClient::IsConnected() const
{
    if (this->socket.isvalid())
    {
        return this->socket->IsConnected();
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    This disconnects the current connection.
*/
void
StdTcpClient::Disconnect()
{
    n_assert(this->socket.isvalid());
    this->socket->Close();
    this->socket = 0;
    this->sendStream = 0;
    this->recvStream = 0;
}

//------------------------------------------------------------------------------
/**
*/
bool
StdTcpClient::Send()
{
    n_assert(this->sendStream.isvalid());
    if (this->sendStream->GetSize() == 0)
    {
        // nothing to send
        return true;
    }

    this->sendStream->SetAccessMode(Stream::ReadAccess);
    if (this->sendStream->Open())
    {
        // put the socket into blocking mode, so that if the
        // outgoing data doesn't fit into the send buffer the
        // socket will block until the next block of data can be written
        bool wasBlocking = this->socket->GetBlocking();
        this->socket->SetBlocking(true);

        // we may not exceed the maximum message size...
        // so we may have to split the send data into
        // multiple packets
        SizeT maxMsgSize = this->socket->GetMaxMsgSize();
        SizeT sendSize = this->sendStream->GetSize();
        uchar* ptr = (uchar*) this->sendStream->Map();
        SizeT overallBytesSent = 0;
        Socket::Result result = Socket::Success;
        while ((Socket::Success == result) && (overallBytesSent < sendSize))
        {
            SizeT bytesToSend = sendSize - overallBytesSent;
            if (bytesToSend > maxMsgSize)
            {
                bytesToSend = maxMsgSize;
            }
            SizeT bytesSent = 0;
            result = this->socket->Send(ptr, bytesToSend, bytesSent);
            ptr += bytesSent;
            overallBytesSent += bytesSent;
        }
        this->sendStream->Unmap();
        this->socket->SetBlocking(wasBlocking);
        this->sendStream->Close();
        this->sendStream->SetSize(0);
        if ((Socket::Success == result) && (overallBytesSent == sendSize))
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
StdTcpClient::Recv()
{
    n_assert(this->recvStream.isvalid());
    this->recvStream->SetAccessMode(Stream::WriteAccess);
    this->recvStream->SetSize(0);
    if (this->recvStream->Open())
    {
        uchar buf[1024];
        Socket::Result res = Socket::Success;
        do
        {
            // NOTE: if this is a blocking client, the first call
            // to Recv() will block until data is available
            SizeT bytesReceived = 0;
            res = this->socket->Recv(&buf, sizeof(buf), bytesReceived);
            if ((bytesReceived > 0) && (Socket::Success == res))
            {
                this->recvStream->Write(buf, bytesReceived);
            }
        }
        while ((Socket::Success == res) && this->socket->HasRecvData());
        this->recvStream->Close();
    }
    this->recvStream->SetAccessMode(Stream::ReadAccess);
    return (this->recvStream->GetSize() > 0);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Stream>&
StdTcpClient::GetSendStream()
{
    return this->sendStream;
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Stream>&
StdTcpClient::GetRecvStream()
{
    return this->recvStream;
}

} // namespace Net