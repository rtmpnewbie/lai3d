#pragma once
#ifndef REMOTE_REMOTEINTERFACE_H
#define REMOTE_REMOTEINTERFACE_H
//------------------------------------------------------------------------------
/**
    @class Remote::RemoteInterface
    
    The RemoteInterface launches the RemoteControl thread and is the communication
    interface with the RemoteControl thread.
    
    (C) 2008 Radon Labs GmbH
*/
#include "interface/interfacebase.h"
#include "core/singleton.h"

//------------------------------------------------------------------------------
namespace Remote
{
class RemoteInterface : public Interface::InterfaceBase
{
    __DeclareClass(RemoteInterface);
    __DeclareInterfaceSingleton(RemoteInterface);
public:
    /// constructor
    RemoteInterface();
    /// destructor
    virtual ~RemoteInterface();
    /// open the interface object
    virtual void Open();
    
protected:
    /// derive in subclass, create and attach handlers from here
    virtual void OnCreateHandlers();
};
        
} // namespace Remote
//------------------------------------------------------------------------------
#endif