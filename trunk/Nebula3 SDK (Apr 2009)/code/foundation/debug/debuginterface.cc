//------------------------------------------------------------------------------
//  debuginterface.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "debug/debuginterface.h"
#include "debug/debughandler.h"

namespace Debug
{
__ImplementClass(Debug::DebugInterface, 'DBIF', Interface::InterfaceBase);
__ImplementInterfaceSingleton(Debug::DebugInterface);

using namespace Interface;

//------------------------------------------------------------------------------
/**
*/
DebugInterface::DebugInterface()
{
    __ConstructInterfaceSingleton;
    this->SetThreadCpuCoreId(System::Cpu::MiscThreadCore);
}

//------------------------------------------------------------------------------
/**
*/
DebugInterface::~DebugInterface()
{
    __DestructInterfaceSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
DebugInterface::Open()
{
    this->SetBehaviour(DoNotWait);
    this->SetName("DebugInterface Thread");
    InterfaceBase::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
DebugInterface::OnCreateHandlers()
{
    Ptr<DebugHandler> debugHandler = DebugHandler::Create();
    this->AttachHandler(debugHandler.upcast<Messaging::Handler>());
}

} // namespace Debug
