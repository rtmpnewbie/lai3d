#pragma once
#ifndef INTERNALGRAPHICS_INTERNALGRAPHICSENTITYHANDLER_H
#define INTERNALGRAPHICS_INTERNALGRAPHICSENTITYHANDLER_H
//------------------------------------------------------------------------------
/**
    @class InternalGraphics::InternalGraphicsEntityHandler

    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "internalgraphics/internalgraphicsentity.h"

//------------------------------------------------------------------------------
namespace InternalGraphics
{
class InternalGraphicsEntityEvent;

class InternalGraphicsEntityHandler : public Core::RefCounted
{
    __DeclareClass(InternalGraphicsEntityHandler);
public:
    /// constructor
    InternalGraphicsEntityHandler();
    /// destructor
    virtual ~InternalGraphicsEntityHandler();
    /// handle message
    virtual bool HandleEvent(const Ptr<InternalGraphicsEntityEvent>& event);
};

} // namespace InternalGraphics
//------------------------------------------------------------------------------
#endif
