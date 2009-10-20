#pragma once
#ifndef COREAUDIO_CUEINFO_H
#define COREAUDIO_CUEINFO_H
//------------------------------------------------------------------------------
/**
    @class CoreAudio::CueInfo
    
    Contains static properties of a cue.
    
    (C) 2008 Radon Labs GmbH
*/
#include "coreaudio/cueid.h"

//------------------------------------------------------------------------------
namespace CoreAudio
{
class CueInfo
{
public:
    /// constructor
    CueInfo();
    /// set the cue id
    void SetCueId(const CueId& cueId);
    /// get the cue id
    const CueId& GetCueId() const;
    /// set interactive flag
    void SetInteractive(bool b);
    /// get interactive flag
    bool IsInteractive() const;
    /// set number of sound variations
    void SetNumVariations(SizeT n);
    /// get number of sound variations
    SizeT GetNumVariations() const;
    /// set max number of instances for this cue
    void SetMaxNumInstances(SizeT n);
    /// get max number of instances 
    SizeT GetMaxNumInstances() const;
    
private:
    CueId cueId;
    bool isInteractive;
    SizeT numVariations;
    SizeT maxNumInstances;
};

//------------------------------------------------------------------------------
/**
*/
inline
CueInfo::CueInfo() :
    isInteractive(false),
    numVariations(0),
    maxNumInstances(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void
CueInfo::SetCueId(const CueId& id)
{
    this->cueId = id;
}

//------------------------------------------------------------------------------
/**
*/
inline const CueId&
CueInfo::GetCueId() const
{
    return this->cueId;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CueInfo::SetInteractive(bool b)
{
    this->isInteractive = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
CueInfo::IsInteractive() const
{
    return this->isInteractive;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CueInfo::SetNumVariations(SizeT n)
{
    this->numVariations = n;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
CueInfo::GetNumVariations() const
{
    return this->numVariations;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CueInfo::SetMaxNumInstances(SizeT n)
{
    this->maxNumInstances = n;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
CueInfo::GetMaxNumInstances() const
{
    return this->maxNumInstances;
}

} // namespace CoreAudio
//------------------------------------------------------------------------------
#endif
    