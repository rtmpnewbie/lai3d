#pragma once
//------------------------------------------------------------------------------
/**
    @class IO::ZipDirEntry

    A directory entry in a zip arcive. The ZipDirEntry class is thread-safe,
    all public methods can be invoked from on the same object from different
    threads.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "io/zipfs/zipfileentry.h"

//------------------------------------------------------------------------------
namespace IO    
{
class ZipDirEntry : public Core::RefCounted
{
    __DeclareClass(ZipDirEntry);
public:
    /// constructor
    ZipDirEntry();
    
    /// find a direct child file entry, return 0 if not exists
    const Ptr<ZipFileEntry>& FindFileEntry(const Util::String& name) const;
    /// find a direct child directory entry, return 0 if not exists
    const Ptr<ZipDirEntry>& FindDirEntry(const Util::String& name) const;
    /// access to dir entries
    const Util::Dictionary<Util::String,Ptr<ZipDirEntry> >& GetDirEntries() const;
    /// access to file entries
    const Util::Dictionary<Util::String,Ptr<ZipFileEntry> >& GetFileEntries() const;
    
private:
    friend class ZipArchive;

    /// add a file child entry
    void AddFileEntry(const Util::String& name, const Ptr<ZipFileEntry>& fileEntry);
    /// add a directory child entry
    void AddDirEntry(const Util::String& name, const Ptr<ZipDirEntry>& dirEntry);

    Util::Dictionary<Util::String, Ptr<ZipFileEntry> > fileEntries;
    Util::Dictionary<Util::String, Ptr<ZipDirEntry> > dirEntries;
};

} // namespace IO
//------------------------------------------------------------------------------

    