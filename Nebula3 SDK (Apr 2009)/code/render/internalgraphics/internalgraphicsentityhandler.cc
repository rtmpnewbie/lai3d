//------------------------------------------------------------------------------
//  internalstage.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/internalgraphicsentityhandler.h"
#include "internalgraphics/internalgraphicsprotocol.h"

namespace InternalGraphics
{
__ImplementClass(InternalGraphics::InternalGraphicsEntityHandler, 'IGEH', Core::RefCounted);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
InternalGraphicsEntityHandler::InternalGraphicsEntityHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
InternalGraphicsEntityHandler::~InternalGraphicsEntityHandler()
{
}

//------------------------------------------------------------------------------
/**
*/
bool 
InternalGraphicsEntityHandler::HandleEvent(const Ptr<InternalGraphicsEntityEvent>& event)
{
    // implement in derived class
    return false;
}
} // namespace InternalGraphics