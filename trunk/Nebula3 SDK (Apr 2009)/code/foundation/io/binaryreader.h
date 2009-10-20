#pragma once
#ifndef IO_BINARYREADER_H
#define IO_BINARYREADER_H
//------------------------------------------------------------------------------
/**
    @class IO::BinaryReader
    
    A friendly interface to read binary data from a stream. Optionally the
    reader can use memory mapping for optimal read performance. Performs
    automatic byte order conversion if necessary.

    @todo convert endianess!
    
    (C) 2006 Radon Labs GmbH
*/
#include "io/streamreader.h"
#include "math/float4.h"
#include "math/matrix44.h"
#include "util/blob.h"
#include "util/guid.h"
#include "system/byteorder.h"

//------------------------------------------------------------------------------
namespace IO
{
class BinaryReader : public StreamReader
{
    __DeclareClass(BinaryReader);
public:
    /// constructor
    BinaryReader();
    /// destructor
    virtual ~BinaryReader();
    /// call before Open() to enable memory mapping (if stream supports mapping)
    void SetMemoryMappingEnabled(bool b);
    /// return true if memory mapping is enabled
    bool IsMemoryMappingEnabled() const;
    /// set the stream byte order (default is host byte order)
    void SetStreamByteOrder(System::ByteOrder::Type byteOrder);
    /// get the stream byte order
    System::ByteOrder::Type GetStreamByteOrder() const;
    /// begin reading from the stream
    virtual bool Open();
    /// end reading from the stream
    virtual void Close();
    /// read an 8-bit char from the stream
    char ReadChar();
    /// read an 8-bit unsigned character from the stream
    unsigned char ReadUChar();
    /// read a 16-bit short from the stream
    short ReadShort();
    /// read a 16-bit unsigned short from the stream
    unsigned short ReadUShort();
    /// read a 32-bit int from the stream
    int ReadInt();
    /// read a 32-bit unsigned int from the stream
    unsigned int ReadUInt();
    /// read a float value from the stream
    float ReadFloat();
    /// read a double value from the stream
    double ReadDouble();
    /// read a bool value from the stream
    bool ReadBool();
    /// read a string from the stream
    Util::String ReadString();
    /// read a float4 from the stream
    Math::float4 ReadFloat4();
    /// read a matrix44 from the stream
    Math::matrix44 ReadMatrix44();
    /// read a blob of data
    Util::Blob ReadBlob();
    /// read a guid
    Util::Guid ReadGuid();
    /// read raw data
    void ReadRawData(void* ptr, SizeT numBytes);

public:
    bool enableMapping;
    bool isMapped;
    System::ByteOrder byteOrder;
    unsigned char* mapCursor;
    unsigned char* mapEnd;
};

//------------------------------------------------------------------------------
/**
*/
inline void
BinaryReader::SetMemoryMappingEnabled(bool b)
{
    this->enableMapping = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
BinaryReader::IsMemoryMappingEnabled() const
{
    return this->enableMapping;
}

//------------------------------------------------------------------------------
/**
*/
inline void
BinaryReader::SetStreamByteOrder(System::ByteOrder::Type order)
{
    this->byteOrder.SetFromByteOrder(order);
}

//------------------------------------------------------------------------------
/**
*/
inline System::ByteOrder::Type
BinaryReader::GetStreamByteOrder() const
{
    return this->byteOrder.GetFromByteOrder();
}

} // namespace IO
//------------------------------------------------------------------------------
#endif