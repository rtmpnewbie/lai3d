#pragma once
#ifndef DEBUG_DEBUGINTERFACE_H
#define DEBUG_DEBUGINTERFACE_H
//------------------------------------------------------------------------------
/**
    @class Debug::DebugInterface
    
    Interface object of the Debug subsystem. This just creates a DebugHandler
    which runs the DebugServer in its own thread.
    
    (C) 2008 Radon Labs GmbH
*/
#include "interface/interfacebase.h"
#include "core/singleton.h"

//------------------------------------------------------------------------------
namespace Debug
{
class DebugInterface : public Interface::InterfaceBase
{
    __DeclareClass(DebugInterface);
    __DeclareInterfaceSingleton(DebugInterface);
public:
    /// constructor
    DebugInterface();
    /// destructor
    virtual ~DebugInterface();
    /// open the interface object
    virtual void Open();
    
protected:
    /// derive in subclass, create and attach handlers from here
    virtual void OnCreateHandlers();
};

} // namespace Debug
//------------------------------------------------------------------------------
#endif
    