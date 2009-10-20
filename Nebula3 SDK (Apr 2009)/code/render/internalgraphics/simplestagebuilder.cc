//------------------------------------------------------------------------------
//  simplestagebuilder.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/simplestagebuilder.h"
#include "internalgraphics/internalstage.h"
#include "internalgraphics/cell.h"

namespace InternalGraphics
{
__ImplementClass(InternalGraphics::SimpleStageBuilder, 'SSBL', InternalGraphics::StageBuilder);

using namespace Math;

//------------------------------------------------------------------------------
/**
*/
SimpleStageBuilder::SimpleStageBuilder()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
SimpleStageBuilder::~SimpleStageBuilder()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
SimpleStageBuilder::BuildStage(const Ptr<InternalStage>& stage)
{
    // the stage just consists of one big root cell
    Ptr<Cell> rootCell = Cell::Create();
    rootCell->SetBoundingBox(bbox(point(0.0f, 0.0f, 0.0f), vector(1000.0f, 1000.0f, 1000.0f)));
    stage->SetRootCell(rootCell);
}

} // namespace Graphics
