//------------------------------------------------------------------------------
//  stagebuilder.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/stagebuilder.h"

namespace InternalGraphics
{
__ImplementClass(InternalGraphics::StageBuilder, 'STGB', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
StageBuilder::StageBuilder()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
StageBuilder::~StageBuilder()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
StageBuilder::BuildStage(const Ptr<InternalStage>& stage)
{
    // empty, overwrite in subclass!
}

} // namespace InternalGraphics
