#pragma once
#ifndef INTERNALGRAPHICS_STAGEBUILDER_H
#define INTERNALGRAPHICS_STAGEBUILDER_H
//------------------------------------------------------------------------------
/**
    @class InternalGraphics::StageBuilder
    
    Stage builders are application-derived classes which construct a stage
    (build a hierarchy of Cells and populate them with graphics entities).
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "attr/attributecontainer.h"

//------------------------------------------------------------------------------
namespace InternalGraphics
{
class InternalStage;

class StageBuilder : public Core::RefCounted
{
    __DeclareClass(StageBuilder);
public:
    /// constructor
    StageBuilder();
    /// destructor
    virtual ~StageBuilder();

    /// set stage builder attributes
    void SetAttributes(const Attr::AttributeContainer& attrs);
    /// get stage builder attributes
    const Attr::AttributeContainer& GetAttributes() const;
    /// setup the stage
    virtual void BuildStage(const Ptr<InternalStage>& stage);

protected:
    Attr::AttributeContainer attrs;
};

//------------------------------------------------------------------------------
/*
*/
inline void
StageBuilder::SetAttributes(const Attr::AttributeContainer& a)
{
    this->attrs = a;
}

//------------------------------------------------------------------------------
/*
*/
inline const Attr::AttributeContainer&
StageBuilder::GetAttributes() const
{
    return this->attrs;
}

} // namespace InternalGraphics
//------------------------------------------------------------------------------
#endif
    