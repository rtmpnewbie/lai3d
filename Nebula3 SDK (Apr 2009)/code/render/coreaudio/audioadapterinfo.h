#pragma once
#ifndef COREAUDIO_AUDIOADAPTERINFO_H
#define COREAUDIO_AUDIOADAPTERINFO_H
//------------------------------------------------------------------------------
/**
    @class CoreAudio::AudioAdapterInfo
  
    Contains information about an audio adapter in the system.
    
    (C) 2008 Radon Labs GmbH
*/    
#include "util/string.h"

//------------------------------------------------------------------------------
namespace CoreAudio
{
class AudioAdapterInfo
{
public:
    /// constructor
    AudioAdapterInfo();
    /// set adapter id string
    void SetAdapterId(const Util::String& id);
    /// get adapter id string
    const Util::String& GetAdapterId() const;
    /// set friendly name (human readable)
    void SetDisplayName(const Util::String& name);
    /// get friendly name
    const Util::String& GetDisplayName() const;
    /// set default adapter flag
    void SetDefaultAdapter(bool b);
    /// return true if this is the default adapter
    bool IsDefaultAdapter() const;

private:
    Util::String adapterId;
    Util::String displayName;
    bool isDefaultAdapter;
};

} // namespace CoreAudio
//------------------------------------------------------------------------------
#endif
