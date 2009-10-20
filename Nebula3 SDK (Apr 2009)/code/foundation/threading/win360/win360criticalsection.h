#pragma once
//------------------------------------------------------------------------------
/**
    @class Win360::Win360CriticalSection
  
    Win32/Xbox360-implementation of critical section. Critical section
    objects are used to protect a portion of code from parallel
    execution. Define a static critical section object and
    use its Enter() and Leave() methods to protect critical sections
    of your code.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Win360
{
class Win360CriticalSection
{
public:
    /// constructor
    Win360CriticalSection();
    /// destructor
    ~Win360CriticalSection();
    /// enter the critical section
    void Enter() const;
    /// leave the critical section
    void Leave() const;
private:
    CRITICAL_SECTION criticalSection;
};

//------------------------------------------------------------------------------
/**
*/
inline
Win360CriticalSection::Win360CriticalSection()
{
    InitializeCriticalSectionAndSpinCount(&this->criticalSection, 1024);
}

//------------------------------------------------------------------------------
/**
*/
inline
Win360CriticalSection::~Win360CriticalSection()
{
    DeleteCriticalSection(&this->criticalSection);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win360CriticalSection::Enter() const
{
    EnterCriticalSection(const_cast<LPCRITICAL_SECTION>(&this->criticalSection));
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win360CriticalSection::Leave() const
{
    LeaveCriticalSection(const_cast<LPCRITICAL_SECTION>(&this->criticalSection));
}

};
//------------------------------------------------------------------------------
