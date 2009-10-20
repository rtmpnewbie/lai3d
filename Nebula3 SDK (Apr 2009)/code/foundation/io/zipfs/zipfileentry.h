#pragma once
//------------------------------------------------------------------------------
/**
    @class IO::ZipFileEntry
  
    A file entry in a zip archive. The ZipFileEntry class is thread-safe,
    all public methods can be invoked from on the same object from different
    threads.
    
    (C) 2006 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "io/stream.h"
#include "zlib/unzip.h"

//------------------------------------------------------------------------------
namespace IO
{
class ZipFileEntry : public Core::RefCounted
{
    __DeclareClass(ZipFileEntry);
public:
    /// constructor
    ZipFileEntry();
    /// destructor
    ~ZipFileEntry();
    /// get the uncompressed file size in bytes
    IO::Stream::Size GetFileSize() const;
    /// read the *entire* content into the provided memory buffer
    bool Read(void* buf, IO::Stream::Size bufSize, const Util::String& password="") const;

private:
    friend class ZipArchive;
    
    /// setup the file entry object
    void Setup(unzFile zipFileHandle, Threading::CriticalSection* critSect);

    Threading::CriticalSection* archiveCritSect;
    unzFile zipFileHandle;    // handle on zip file
    unz_file_pos filePosInfo; // info about position in zip file
    uint uncompressedSize;    // uncompressed size of the file
};

} // namespace IO
//------------------------------------------------------------------------------

