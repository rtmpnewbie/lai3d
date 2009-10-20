//------------------------------------------------------------------------------
//  interfacehandlerbase.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "interface/interfacehandlerbase.h"

namespace Interface
{
__ImplementClass(Interface::InterfaceHandlerBase, 'IFHB', Messaging::Handler);

//------------------------------------------------------------------------------
/**
*/
InterfaceHandlerBase::InterfaceHandlerBase()
{
    // empty
}     

} // namespace Interface