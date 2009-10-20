//------------------------------------------------------------------------------
//  iointerface.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/iointerface.h"
#include "io/iointerfacehandler.h"

namespace IO
{
__ImplementClass(IO::IoInterface, 'IIOF', Interface::InterfaceBase);
__ImplementInterfaceSingleton(IO::IoInterface);

using namespace Messaging;
using namespace Interface;

//------------------------------------------------------------------------------
/**
*/
IoInterface::IoInterface()
{
    __ConstructInterfaceSingleton;
    this->SetThreadCpuCoreId(System::Cpu::IoThreadCore);
}

//------------------------------------------------------------------------------
/**
*/
IoInterface::~IoInterface()
{
    __DestructInterfaceSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
IoInterface::Open()
{
    this->SetName("IoInterface Thread");
    InterfaceBase::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
IoInterface::OnCreateHandlers()
{
    Ptr<IoInterfaceHandler> msgHandler = IoInterfaceHandler::Create();
    this->AttachHandler(msgHandler.upcast<Messaging::Handler>());
}

} // namespace IO
