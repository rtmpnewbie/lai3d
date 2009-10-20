#pragma once
//------------------------------------------------------------------------------
/**
    @class Graphics::Handle
    
    A private Handle typedef to safely pass data between a server and
    a client thread. Don't assume anything about the content of the
    Handle!
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"
#include "core/refcounted.h"

//------------------------------------------------------------------------------
namespace Graphics
{
typedef Ptr<Core::RefCounted> Handle;
}
//------------------------------------------------------------------------------
