//------------------------------------------------------------------------------
//  frameshader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "frame/frameshader.h"

namespace Frame
{
__ImplementClass(Frame::FrameShader, 'FSHD', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
FrameShader::FrameShader()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
FrameShader::~FrameShader()
{
    // make sure Discard() has been called
    n_assert(!this->mainRenderTarget.isvalid());
    n_assert(this->renderTargets.IsEmpty());
    n_assert(this->shaderVariables.IsEmpty());
    n_assert(this->framePasses.IsEmpty());
    n_assert(this->postEffects.IsEmpty());
}

//------------------------------------------------------------------------------
/**
*/
void
FrameShader::Discard()
{
    IndexT i;
    this->mainRenderTarget = 0;
    for (i = 0; i < this->renderTargets.Size(); i++)
    {
        this->renderTargets.ValueAtIndex(i)->Discard();
    }
    this->renderTargets.Clear();
    this->shaderVariables.Clear();
    for (i = 0; i < this->framePasses.Size(); i++)
    {
        this->framePasses[i]->Discard();
    }
    this->framePasses.Clear();
    for (i = 0; i < this->postEffects.Size(); i++)
    {
        this->postEffects[i]->Discard();
    }
    this->postEffects.Clear();

}

//------------------------------------------------------------------------------
/**
*/
void
FrameShader::Render()
{
    // render passes
    IndexT i;
    for (i = 0; i < this->framePasses.Size(); i++)
    {
        this->framePasses[i]->Render();
    }

    // render post effects
    for (i = 0; i < this->postEffects.Size(); i++)
    {
        this->postEffects[i]->Render();
    }
}

} // namespace Frame