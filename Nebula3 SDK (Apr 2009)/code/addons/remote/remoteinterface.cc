//------------------------------------------------------------------------------
//  remoteinterface.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "remote/remoteinterface.h"
#include "remote/remotemessagehandler.h"

namespace Remote
{
__ImplementClass(Remote::RemoteInterface, 'REIF', Interface::InterfaceBase);
__ImplementInterfaceSingleton(Remote::RemoteInterface);

using namespace Interface;

//------------------------------------------------------------------------------
/**
*/
RemoteInterface::RemoteInterface()
{
    __ConstructInterfaceSingleton;
    this->SetThreadCpuCoreId(System::Cpu::MiscThreadCore);
}

//------------------------------------------------------------------------------
/**
*/
RemoteInterface::~RemoteInterface()
{
    __DestructInterfaceSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
RemoteInterface::Open()
{
    this->SetBehaviour(DoNotWait);
    this->SetName("RemoteInterface Thread");
    InterfaceBase::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
RemoteInterface::OnCreateHandlers()
{
    Ptr<RemoteMessageHandler> msgHandler = RemoteMessageHandler::Create();
    this->AttachHandler(msgHandler.upcast<Messaging::Handler>());
}

} // namespace Remote
