//------------------------------------------------------------------------------
//  raknetstream.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "corenetwork/raknet/raknetstream.h"
#include "corenetwork/packetid.h"

namespace RakNet
{
__ImplementClass(RakNet::RaknetStream, 'RKST', IO::Stream);

using namespace IO;
using namespace CoreNetwork;
//------------------------------------------------------------------------------
/**
*/
RaknetStream::RaknetStream():
bitStream(0),
messageId(PacketId::ID_NEBULA3_INVALID_ID)
{
    this->bitStream = n_new(RakNet::BitStream);
}

//------------------------------------------------------------------------------
/**
*/
RaknetStream::~RaknetStream()
{  
    n_delete(this->bitStream);
}

//------------------------------------------------------------------------------
/**
*/
bool 
RaknetStream::Open()
{
    n_assert(this->bitStream != 0);
    bool success = Stream::Open();

    return success;
}

//------------------------------------------------------------------------------
/**
*/
void 
RaknetStream::Close()
{
    n_assert(this->bitStream != 0);  
    Stream::Close();
}

//------------------------------------------------------------------------------
/**
*/
bool 
RaknetStream::CanRead() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
RaknetStream::CanWrite() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
RaknetStream::Write(const void* ptr, Size numBytes)
{
    n_assert(this->IsOpen());

    this->bitStream->Write((char*)ptr, numBytes);
}

//------------------------------------------------------------------------------
/**
*/
void
RaknetStream::Write(MessageID id)
{
    n_assert(this->IsOpen());
    this->bitStream->Write(id);
}

//------------------------------------------------------------------------------
/**
*/
Stream::Size
RaknetStream::Read(void* ptr, Size numBytes)
{
    n_assert(this->IsOpen());
    bool allBytesRead = this->bitStream->Read((char*)ptr, numBytes);
    if (allBytesRead)
    {
        return numBytes;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
MessageID 
RaknetStream::GetMessageId() const
{
    return this->messageId;
}

//------------------------------------------------------------------------------
/**
*/
void 
RaknetStream::SetMessageId(MessageID id)
{
    this->messageId = id;
}

//------------------------------------------------------------------------------
/**
*/
Stream::Size 
RaknetStream::GetSize() const
{
    return this->bitStream->GetNumberOfBytesUsed();
}

}