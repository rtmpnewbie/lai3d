#pragma once
#ifndef MODELS_SHAPENODEINSTANCE_H
#define MODELS_SHAPENODEINSTANCE_H
//------------------------------------------------------------------------------
/**
    @class Models::ShapeNodeInstance
  
    The ShapeNodeInstance actually renders a static shape, and holds
    all the necessary per-instance state to do so.
    
    (C) 2007 Radon Labs GmbH
*/
#include "models/nodes/statenodeinstance.h"

//------------------------------------------------------------------------------
namespace Models
{
class ShapeNodeInstance : public StateNodeInstance
{
    __DeclareClass(ShapeNodeInstance);
public:
    /// constructor
    ShapeNodeInstance();
    /// destructor
    virtual ~ShapeNodeInstance();
    
    /// perform rendering
    virtual void Render();

protected:
    /// notify that we are visible
    virtual void OnNotifyVisible(IndexT frameIndex);
};

} // namespace Models
//------------------------------------------------------------------------------
#endif

