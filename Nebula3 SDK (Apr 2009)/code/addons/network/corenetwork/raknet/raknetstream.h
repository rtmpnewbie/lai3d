#ifndef RAKNET_RAKNETSTREAM_H
#define RAKNET_RAKNETSTREAM_H
//------------------------------------------------------------------------------
/**
    A NetStream is a bitstream with a associated MessageId.
*/
#include "foundation/io/stream.h"
#include "extlibs/raknet/source/bitstream.h"

namespace RakNet
{

class RaknetStream : public IO::Stream
{
    __DeclareClass(RaknetStream);
public:
    /// constructor
    RaknetStream();
    ///destructor
    virtual ~RaknetStream();

    /// open the stream and create bitstream
    virtual bool Open();
    /// close the stream and destroy bitstream
    virtual void Close();
    /// can read
    virtual bool CanRead() const;
    /// can write
    virtual bool CanWrite() const;
    /// write message id
    void Write(MessageID id);
    /// directly write to the stream
    virtual void Write(const void* ptr, Size numBytes);
    /// directly read from the stream includes message id
    virtual Size Read(void* ptr, Size numBytes);
    /// get bitstream
    BitStream* GetBitstream() const;
    /// get message id
    MessageID GetMessageId() const;
    /// set message id
    void SetMessageId(MessageID id); 
    /// get size of whole stream in bytes
    Stream::Size GetSize() const;
    
private:

    MessageID messageId;
    BitStream* bitStream;
};
    
//------------------------------------------------------------------------------
/**
*/
inline BitStream* 
RaknetStream::GetBitstream() const
{
    n_assert(0 != this->bitStream);
    return this->bitStream;
}
}
#endif // RAKNET_RAKNETSTREAM_H