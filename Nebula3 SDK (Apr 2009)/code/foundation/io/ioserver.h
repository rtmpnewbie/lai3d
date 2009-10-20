#pragma once
//------------------------------------------------------------------------------
/**
    @class IO::IoServer
  
    The central server object of the IO subsystem offers the following
    services:

    * associate stream classes with URI schemes
    * create the right stream object for a given URI
    * transparant (ZIP) archive support
    * path assign management
    * global filesystem manipulation and query methods
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "util/array.h"
#include "util/string.h"
#include "util/hashtable.h"
#include "io/filetime.h"
#include "io/assign.h"
#include "util/stringatom.h"
#include "io/assignregistry.h"
#include "io/schemeregistry.h"

//------------------------------------------------------------------------------
namespace IO
{
class ArchiveFileSystem;

class Stream;
class URI;

class IoServer : public Core::RefCounted
{
    __DeclareClass(IoServer);
    __DeclareSingleton(IoServer);
public:
    /// constructor
    IoServer();
    /// destructor
    virtual ~IoServer();

    /// mount a file archive (without archive file extension!)
    bool MountArchive(const URI& uri);
    /// unmount a file archive (without archive file extension!)
    void UnmountArchive(const URI& uri);
    /// return true if a archive is mounted (without archive file extension!)
    bool IsArchiveMounted(const URI& uri) const;
    /// enable/disable transparent archive filesystem layering (default is yes)
    void SetArchiveFileSystemEnabled(bool b);
    /// return true if transparent archive filesystem is enabled
    bool IsArchiveFileSystemEnabled() const;
    /// mount standard archives (e.g. home:export.zip and home:export_$(platform).zip)
    void MountStandardArchives();
    /// unmount standard archives
    void UnmountStandardArchives();

    /// create a stream object for the given uri
    Ptr<Stream> CreateStream(const URI& uri) const;
    /// create all missing directories in the path
    bool CreateDirectory(const URI& uri) const;
    /// delete an empty directory
    bool DeleteDirectory(const URI& path) const;
    /// return true if directory exists
    bool DirectoryExists(const URI& path) const;

    /// copy a file
    bool CopyFile(const URI& from, const URI& to) const;
    /// delete a file
    bool DeleteFile(const URI& path) const;
    /// return true if file exists
    bool FileExists(const URI& path) const;
    /// set the readonly status of a file
    void SetReadOnly(const URI& path, bool b) const;
    /// return read only status of a file
    bool IsReadOnly(const URI& path) const;
    /// get the CRC checksum of a file
    unsigned int ComputeFileCrc(const URI& path) const;
    /// set the write-time of a file
    void SetFileWriteTime(const URI& path, FileTime fileTime);
    /// return the last write-time of a file
    FileTime GetFileWriteTime(const URI& path) const;

    /// list all files matching a pattern in a directory
    Util::Array<Util::String> ListFiles(const URI& dir, const Util::String& pattern) const;
    /// list all subdirectories matching a pattern in a directory
    Util::Array<Util::String> ListDirectories(const URI& dir, const Util::String& pattern) const;

private:
    bool archiveFileSystemEnabled;    
    Ptr<ArchiveFileSystem> archiveFileSystem;
    static Threading::CriticalSection ArchiveCriticalSection;
    static bool StandardArchivesMounted;
    
    Ptr<AssignRegistry> assignRegistry;
    Ptr<SchemeRegistry> schemeRegistry;
    static Threading::CriticalSection AssignCriticalSection;
    static Threading::CriticalSection SchemeCriticalSection;
};

//------------------------------------------------------------------------------
/**
*/
inline void
IoServer::SetArchiveFileSystemEnabled(bool b)
{
    this->archiveFileSystemEnabled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
IoServer::IsArchiveFileSystemEnabled() const
{
    return this->archiveFileSystemEnabled;
}

} // namespace IO
//------------------------------------------------------------------------------
