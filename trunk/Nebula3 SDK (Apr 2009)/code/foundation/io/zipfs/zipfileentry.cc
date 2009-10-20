//------------------------------------------------------------------------------
//  zipfileentry.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/zipfs/zipfileentry.h"

namespace IO
{
__ImplementClass(IO::ZipFileEntry, 'ZPFE', Core::RefCounted);

using namespace Util;
using namespace Threading;

//------------------------------------------------------------------------------
/**
*/
ZipFileEntry::ZipFileEntry() :
    archiveCritSect(0),
    zipFileHandle(0),
    uncompressedSize(0)
{
    Memory::Clear(&this->filePosInfo, sizeof(this->filePosInfo));
}

//------------------------------------------------------------------------------
/**
*/
ZipFileEntry::~ZipFileEntry()
{
    this->zipFileHandle = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
ZipFileEntry::Setup(unzFile h, CriticalSection* critSect)
{
    n_assert(0 != h);
    n_assert(0 == this->zipFileHandle);
    n_assert(0 != critSect);
    this->zipFileHandle = h;

    // store pointer to archive's critical section
    this->archiveCritSect = critSect;

    // store file position
    int res = unzGetFilePos(this->zipFileHandle, &this->filePosInfo);
    n_assert(UNZ_OK == res);

    // get other data about the file
    unz_file_info fileInfo;
    res = unzGetCurrentFileInfo(this->zipFileHandle, &fileInfo, 0, 0, 0, 0, 0, 0);
    n_assert(UNZ_OK == res);
    this->uncompressedSize = fileInfo.uncompressed_size;
}

//------------------------------------------------------------------------------
/**
*/
Stream::Size
ZipFileEntry::GetFileSize() const
{
    return this->uncompressedSize;
}

//------------------------------------------------------------------------------
/**
    NOTE: this method only supports reading the entire file at once, thus
    numBytes must be equal to the result of GetFileSize()!
    NOTE: zlib only allows one thread at a time to work on a given zip archive,
    thus we need to serialize access to the archive using a per-archive
    critical section (which we got handed in the Setup method)
*/
bool
ZipFileEntry::Read(void* buf, Stream::Size numBytes, const String& password) const
{
    n_assert(0 != this->zipFileHandle);
    n_assert(0 != buf);
    n_assert(0 != this->archiveCritSect);
    n_assert(numBytes == (Stream::Size) this->uncompressedSize);

    // take the archive's critical section
    this->archiveCritSect->Enter();

    // set current file to this file
    int res = unzGoToFilePos(this->zipFileHandle, const_cast<unz_file_pos*>(&this->filePosInfo));
    if (UNZ_OK != res)
    {
        this->archiveCritSect->Leave();
        return false;
    }

    // open the current file with optional password
    if (password.IsValid())
    {
        res = unzOpenCurrentFilePassword(this->zipFileHandle, password.AsCharPtr());
    }
    else
    {
        res = unzOpenCurrentFile(this->zipFileHandle);
    }
    if (UNZ_OK != res)
    {
        this->archiveCritSect->Leave();
        return false;
    }

    // read uncompressed data 
    int readResult = unzReadCurrentFile(this->zipFileHandle, buf, numBytes);
    
    // close the file
    res = unzCloseCurrentFile(this->zipFileHandle);
    this->archiveCritSect->Leave();

    if (UNZ_OK != res)
    {
        return false;
    }

    if (numBytes == readResult)
    {
        return true;
    }
    else
    {
        return false;
    }
}

} // namespace ZipFileEntry
