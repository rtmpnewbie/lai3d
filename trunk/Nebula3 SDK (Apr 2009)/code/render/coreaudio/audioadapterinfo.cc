//------------------------------------------------------------------------------
//  audioadapterinfo.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreaudio/audioadapterinfo.h"

namespace CoreAudio
{
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
AudioAdapterInfo::AudioAdapterInfo() :
    isDefaultAdapter(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
AudioAdapterInfo::SetAdapterId(const String& id)
{
    this->adapterId = id;
}

//------------------------------------------------------------------------------
/**
*/
const String&
AudioAdapterInfo::GetAdapterId() const
{
    return this->adapterId;
}

//------------------------------------------------------------------------------
/**
*/
void
AudioAdapterInfo::SetDisplayName(const String& name)
{
    this->displayName = name;
}

//------------------------------------------------------------------------------
/**
*/
const String&
AudioAdapterInfo::GetDisplayName() const
{
    return this->displayName;
}

//------------------------------------------------------------------------------
/**
*/
void
AudioAdapterInfo::SetDefaultAdapter(bool b)
{
    this->isDefaultAdapter = b;
}

//------------------------------------------------------------------------------
/**
*/
bool
AudioAdapterInfo::IsDefaultAdapter() const
{
    return this->isDefaultAdapter;
}

} // namespace CoreAudio
