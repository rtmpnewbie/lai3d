#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::RCSWrapper
    
    A transparent wrapper around CVS or SVN, only supports updating.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/string.h"
#include "toolkitutil/logger.h"

//------------------------------------------------------------------------------
namespace ToolkitUtil
{
class RCSWrapper
{
public:
    /// constructor
    RCSWrapper();
    /// destructor
    ~RCSWrapper();

    /// set location of SVN tool
    void SetSVNToolLocation(const Util::String& path);
    /// get location of SVN tool
    const Util::String& GetSVNToolLocation() const;
    /// set directories for "commit timestamp" mode
    void SetCommitTimeStampDirectories(const Util::Array<Util::String>& paths);
    /// get "commit timestamp" directories
    const Util::Array<Util::String>& GetCommitTimeStampDirectories() const;
    /// set directories for "update timestamp" update mode
    void SetUpdateTimeStampDirectories(const Util::Array<Util::String>& paths);
    /// get "update timestamp" directories
    const Util::Array<Util::String>& GetUpdateTimeStampDirectories() const;
    /// perform update operation
    bool Update(Logger& logger);

private:
    /// write a temporary config file for SVN
    void WriteTempSVNConfigFiles();
    /// helper method to copy content of a directory
    void CopyDirectory(const Util::String& fromDir, const Util::String& toDir);
    /// update one directory
    bool UpdateDirectory(Logger& logger, const Util::String& directory, bool commitTimeStampMode);

    Util::String svnToolLocation;
    Util::Array<Util::String> updateTimeStampDirs;
    Util::Array<Util::String> commitTimeStampDirs;
};

//------------------------------------------------------------------------------
/**
*/
inline void
RCSWrapper::SetSVNToolLocation(const Util::String& path)
{
    this->svnToolLocation = path;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
RCSWrapper::GetSVNToolLocation() const
{
    return this->svnToolLocation;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RCSWrapper::SetCommitTimeStampDirectories(const Util::Array<Util::String>& dirs)
{
    this->commitTimeStampDirs = dirs;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Util::String>&
RCSWrapper::GetCommitTimeStampDirectories() const
{
    return this->commitTimeStampDirs;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RCSWrapper::SetUpdateTimeStampDirectories(const Util::Array<Util::String>& dirs)
{
    this->updateTimeStampDirs = dirs;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Util::String>&
RCSWrapper::GetUpdateTimeStampDirectories() const
{
    return this->updateTimeStampDirs;
}

} // namespace ToolkitUtil
//------------------------------------------------------------------------------
    