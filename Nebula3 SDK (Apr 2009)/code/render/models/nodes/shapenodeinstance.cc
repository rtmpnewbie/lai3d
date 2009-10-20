//------------------------------------------------------------------------------
//  shapenodeinstance.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/nodes/shapenodeinstance.h"
#include "coregraphics/renderdevice.h"
#include "models/modelnode.h"

namespace Models
{
__ImplementClass(Models::ShapeNodeInstance, 'SPNI', Models::StateNodeInstance);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
ShapeNodeInstance::ShapeNodeInstance()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ShapeNodeInstance::~ShapeNodeInstance()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeNodeInstance::Render()
{
    StateNodeInstance::Render();
    RenderDevice::Instance()->Draw();
}    

//------------------------------------------------------------------------------
/**
*/
void
ShapeNodeInstance::OnNotifyVisible(IndexT frameIndex)
{
    // just tell our model node that we are a visible instance
    this->modelNode->AddVisibleNodeInstance(frameIndex, this);
}

} // namespace Models
