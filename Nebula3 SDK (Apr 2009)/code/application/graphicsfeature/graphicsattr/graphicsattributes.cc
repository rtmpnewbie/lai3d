//------------------------------------------------------------------------------
//  attr/attributes.h
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphicsattr/graphicsattributes.h"

namespace Attr
{
    // (simple) graphics property
    DefineString(Graphics, 'GRFX', ReadWrite);        // name of the graphics resource

    // actor graphics property
    DefineString(AnimSet, 'ANMS', ReadWrite);
    DefineString(CharacterSet, 'CHRS', ReadWrite);
};
