#pragma once
#ifndef RESOURCES_RESOURCEID_H
#define RESOURCES_RESOURCEID_H
//------------------------------------------------------------------------------
/**
    @class Resources::ResourceId
    
    ResourceId's are unique identifier of resources used for sharing and
    locating the resource data on disc.
    
    (C) 2007 Radon Labs GmbH
*/
#include "util/string.h"
#include "util/atom.h"

//------------------------------------------------------------------------------
namespace Resources
{
typedef Util::Atom<Util::String> ResourceId;
}  // namespace Resources
//------------------------------------------------------------------------------
#endif
    