//------------------------------------------------------------------------------
//  zipdirentry.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/zipfs/zipdirentry.h"

namespace IO
{
__ImplementClass(IO::ZipDirEntry, 'ZPDE', Core::RefCounted);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
ZipDirEntry::ZipDirEntry()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Adds a new file entry object to the internal dictionary. NOTE:
    this method will not check whether the entry already exists for 
    performance reasons (doing this would force the dictionary to be
    sorted after every insert).
*/
void
ZipDirEntry::AddFileEntry(const String& name, const Ptr<ZipFileEntry>& fileEntry)
{
    n_assert(fileEntry.isvalid());
    n_assert(name.IsValid());
    this->fileEntries.Add(name, fileEntry);
}

//------------------------------------------------------------------------------
/**
    Adds a new directory entry object to the internal dictionary. NOTE:
    this method will not check whether the entry already exists for 
    performance reasons (doing this would force the dictionary to be
    sorted after every insert).
*/
void
ZipDirEntry::AddDirEntry(const String& name, const Ptr<ZipDirEntry>& dirEntry)
{
    n_assert(dirEntry.isvalid());
    n_assert(name.IsValid());
    this->dirEntries.Add(name, dirEntry);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<ZipFileEntry>&
ZipDirEntry::FindFileEntry(const String& name) const
{
    n_assert(name.IsValid());
    IndexT index = this->fileEntries.FindIndex(name);
    if (InvalidIndex == index)
    {
        // doesn't exist
        static Ptr<ZipFileEntry> invalidPtr;
        return invalidPtr;
    }
    else
    {
        return this->fileEntries.ValueAtIndex(index);
    }
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<ZipDirEntry>&
ZipDirEntry::FindDirEntry(const String& name) const
{
    n_assert(name.IsValid());
    IndexT index = this->dirEntries.FindIndex(name);
    if (InvalidIndex == index)
    {
        // doesn't exist
        static Ptr<ZipDirEntry> invalidPtr;
        return invalidPtr;
    }
    else
    {
        return this->dirEntries.ValueAtIndex(index);
    }
}

//------------------------------------------------------------------------------
/**
*/
const Dictionary<String,Ptr<ZipDirEntry> >&
ZipDirEntry::GetDirEntries() const
{
    return this->dirEntries;
}

//------------------------------------------------------------------------------
/**
*/
const Dictionary<String,Ptr<ZipFileEntry> >&
ZipDirEntry::GetFileEntries() const
{
    return this->fileEntries;
}

} // namespace IO
