#pragma once
#ifndef INTERNALGRAPHICS_SIMPLESTAGEBUILDER_H
#define INTERNALGRAPHICS_SIMPLESTAGEBUILDER_H
//------------------------------------------------------------------------------
/**
    @class InternalGraphics::SimpleStageBuilder
    
    The most simple stage builder just creates a single cell where all
    entities live in. Use this stage builder when you only have one
    or very few graphics entities where no culling is needed.
    
    (C) 2007 Radon Labs GmbH
*/
#include "internalgraphics/stagebuilder.h"

//------------------------------------------------------------------------------
namespace InternalGraphics
{
class SimpleStageBuilder : public StageBuilder
{
    __DeclareClass(SimpleStageBuilder);
public:
    /// constructor
    SimpleStageBuilder();
    /// destructor
    virtual ~SimpleStageBuilder();
    /// setup the stage
    virtual void BuildStage(const Ptr<InternalStage>& stage);
};

} // namespace Graphics
//------------------------------------------------------------------------------
#endif
    