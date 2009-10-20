//------------------------------------------------------------------------------
//  zipfilestream.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/zipfs/zipfilestream.h"
#include "io/zipfs/zipfilesystem.h"
#include "io/zipfs/ziparchive.h"
#include "io/zipfs/zipfileentry.h"
#include "io/archfs/archive.h"

namespace IO
{
__ImplementClass(IO::ZipFileStream, 'ZFST', IO::Stream);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
ZipFileStream::ZipFileStream() :
    size(0),
    position(0),
    buffer(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ZipFileStream::~ZipFileStream()
{
    if (this->IsOpen())
    {
        this->Close();
    }
    n_assert(0 == this->buffer);
}

//------------------------------------------------------------------------------
/**
*/
bool
ZipFileStream::CanRead() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
ZipFileStream::CanWrite() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
ZipFileStream::CanSeek() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
ZipFileStream::CanBeMapped() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
Stream::Size
ZipFileStream::GetSize() const
{
    return this->size;
}

//------------------------------------------------------------------------------
/**
*/
Stream::Position
ZipFileStream::GetPosition() const
{
    return this->position;
}

//------------------------------------------------------------------------------
/**
    Open the stream for reading. This will decompress the entire file
    from the zip archive into memory.
*/
bool
ZipFileStream::Open()
{
    n_assert(!this->IsOpen());
    n_assert(0 == this->buffer);
    n_assert(ReadAccess == this->accessMode);

    if (Stream::Open())
    {
        // get zip archive which contains the file
        Ptr<ZipArchive> zipArchive = ZipFileSystem::Instance()->FindArchive(this->uri).cast<ZipArchive>();
        if (zipArchive.isvalid())
        {
            Dictionary<String,String> params = this->uri.ParseQuery();
            if (params.Contains("file"))
            {
                const String& pathInZip = params["file"];
                String pwd;
                if (params.Contains("pwd"))
                {
                    pwd = params["pwd"];
                }

                // find the zip file in the archive, the path into
                // the archive is encoded in the query part of our URI
                const Ptr<ZipFileEntry>& entry = zipArchive->FindFileEntry(pathInZip);
                if (entry.isvalid())
                {
                    // read content of zip file entry into private buffer
                    this->size = entry->GetFileSize();
                    this->position = 0;
                    this->buffer = (unsigned char*) Memory::Alloc(Memory::StreamDataHeap, this->size);
                    n_assert(0 != this->buffer);
                    bool success = entry->Read(this->buffer, this->size, pwd);
                    if (success)
                    {
                        return true;
                    }
                }
            }
        }
        // fallthrough: failure
        this->Close();
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
ZipFileStream::Close()
{
    n_assert(this->IsOpen());
    Stream::Close();
    if (0 != this->buffer)
    {
        Memory::Free(Memory::StreamDataHeap, this->buffer);
        this->buffer = 0;
    }
    this->position = 0;
    this->size = 0;
}

//------------------------------------------------------------------------------
/**
*/
Stream::Size
ZipFileStream::Read(void* ptr, Size numBytes)
{
    n_assert(this->IsOpen());
    n_assert(!this->IsMapped()); 
    n_assert(ReadAccess == this->accessMode)
    n_assert((this->position >= 0) && (this->position <= this->size));

    // check if end-of-stream is near
    Size readBytes = Math::n_min(numBytes, this->size - this->position);
    n_assert((this->position + readBytes) <= this->size);
    if (readBytes > 0)
    {
        Memory::Copy(this->buffer + this->position, ptr, readBytes);
        this->position += readBytes;
    }
    return readBytes;
}

//------------------------------------------------------------------------------
/**
*/
void
ZipFileStream::Seek(Offset offset, SeekOrigin origin)
{
    n_assert(this->IsOpen());
    n_assert(!this->IsMapped()); 
    n_assert((this->position >= 0) && (this->position <= this->size));
    switch (origin)
    {
        case Begin:
            this->position = offset;
            break;
        case Current:
            this->position += offset;
            break;
        case End:
            this->position = this->size + offset;
            break;
    }

    // make sure read/write position doesn't become invalid
    this->position = Math::n_iclamp(this->position, 0, this->size);
}

//------------------------------------------------------------------------------
/**
*/
bool
ZipFileStream::Eof() const
{
    n_assert(this->IsOpen());
    n_assert(!this->IsMapped());
    n_assert((this->position >= 0) && (this->position <= this->size));
    return (this->position == this->size);
}

//------------------------------------------------------------------------------
/**
*/
void*
ZipFileStream::Map()
{
    n_assert(this->IsOpen());
    n_assert(ReadAccess == this->accessMode);
    Stream::Map();
    n_assert(this->GetSize() > 0);
    return this->buffer;
}

//------------------------------------------------------------------------------
/**
*/
void
ZipFileStream::Unmap()
{
    n_assert(this->IsOpen());
    Stream::Unmap();
}

} // namespace IO
