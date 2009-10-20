#pragma once
//------------------------------------------------------------------------------
/**
    @class Frame::FramePostEffect
    
    A frame post-effect implements draws a fullscreen quad through a
    shader which implements the post effect. Additionally it is
    possible to add render batches to a post effect, these will
    be called after the fullscreen quad is rendered, allowing the
    application to render stuff on top of the final frame.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "resources/resourceid.h"
#include "coregraphics/rendertarget.h"
#include "coregraphics/multiplerendertarget.h"
#include "coregraphics/shaderinstance.h"
#include "coregraphics/shadervariable.h"
#include "coregraphics/shadervariableinstance.h"
#include "coregraphics/vertexbuffer.h"
#include "coregraphics/primitivegroup.h"
#include "frame/framebatch.h"

//------------------------------------------------------------------------------
namespace Frame
{
class FramePostEffect : public Core::RefCounted
{
    __DeclareClass(FramePostEffect);
public:
    /// constructor
    FramePostEffect();
    /// destructor
    virtual ~FramePostEffect();
    /// setup the post effect
    void Setup();
    /// discard the post effect
    void Discard();
    /// render the post effect
    void Render();

    /// set the name of the frame pass
    void SetName(const Resources::ResourceId& resId);
    /// get the name of the frame pass
    const Resources::ResourceId& GetName() const;
    /// set pass shader
    void SetShader(const Ptr<CoreGraphics::ShaderInstance>& shd);
    /// get pass shader
    const Ptr<CoreGraphics::ShaderInstance>& GetShader() const;
    /// set render target
    void SetRenderTarget(const Ptr<CoreGraphics::RenderTarget>& rt);
    /// get render target
    const Ptr<CoreGraphics::RenderTarget>& GetRenderTarget() const;
    /// set multiple render target
    void SetMultipleRenderTarget(const Ptr<CoreGraphics::MultipleRenderTarget>& rt);
    /// get multiple render target
    const Ptr<CoreGraphics::MultipleRenderTarget>& GetMultipleRenderTarget() const;

    /// add a shader variable instance to the frame pass
    void AddVariable(const Ptr<CoreGraphics::ShaderVariableInstance>& var);
    /// get number of shader variables
    SizeT GetNumVariables() const;
    /// get shader variable by index
    const Ptr<CoreGraphics::ShaderVariableInstance>& GetVariableByIndex(IndexT i) const;
    /// get shader variabel by semantic
    const Ptr<CoreGraphics::ShaderVariableInstance>& GetVariableBySemantic(const CoreGraphics::ShaderVariable::Semantic& sem) const;

    /// add a frame batch to the frame pass
    void AddBatch(const Ptr<FrameBatch>& batch);
    /// get number of frame batches
    SizeT GetNumBatches() const;
    /// get batch by index
    const Ptr<FrameBatch>& GetBatchByIndex(IndexT i) const;

private:
    Resources::ResourceId name;
    Ptr<CoreGraphics::VertexBuffer> vertexBuffer;
    Ptr<CoreGraphics::ShaderInstance> shader;
    Ptr<CoreGraphics::RenderTarget> renderTarget;
    Ptr<CoreGraphics::MultipleRenderTarget> multipleRenderTarget;
    Util::Array<Ptr<CoreGraphics::ShaderVariableInstance> > shaderVariables;
    CoreGraphics::PrimitiveGroup primGroup;
    Ptr<CoreGraphics::ShaderVariable> shdPixelSize;
    Util::Array<Ptr<FrameBatch> > batches;
};

//------------------------------------------------------------------------------
/**
*/
inline void
FramePostEffect::SetName(const Resources::ResourceId& resId)
{
    this->name = resId;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
FramePostEffect::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FramePostEffect::SetShader(const Ptr<CoreGraphics::ShaderInstance>& shd)
{
    this->shader = shd;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderInstance>&
FramePostEffect::GetShader() const
{
    return this->shader;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FramePostEffect::SetRenderTarget(const Ptr<CoreGraphics::RenderTarget>& rt)
{
    this->renderTarget = rt;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::RenderTarget>&
FramePostEffect::GetRenderTarget() const
{
    return this->renderTarget;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FramePostEffect::SetMultipleRenderTarget(const Ptr<CoreGraphics::MultipleRenderTarget>& rt)
{
    this->multipleRenderTarget = rt;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::MultipleRenderTarget>&
FramePostEffect::GetMultipleRenderTarget() const
{
    return this->multipleRenderTarget;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FramePostEffect::AddVariable(const Ptr<CoreGraphics::ShaderVariableInstance>& var)
{
    this->shaderVariables.Append(var);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
FramePostEffect::GetNumVariables() const
{
    return this->shaderVariables.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderVariableInstance>& 
FramePostEffect::GetVariableByIndex(IndexT i) const
{
    return this->shaderVariables[i];
}

//------------------------------------------------------------------------------
/**
*/
inline
const Ptr<CoreGraphics::ShaderVariableInstance>& 
FramePostEffect::GetVariableBySemantic(const CoreGraphics::ShaderVariable::Semantic& sem) const
{
    IndexT i;
    for (i = 0; i < this->shaderVariables.Size(); i++)
    {
        const Ptr<CoreGraphics::ShaderVariable>& shdVar = this->shaderVariables[i]->GetShaderVariable();
        if (shdVar->GetSemantic() == sem)
        {
            return this->shaderVariables[i];
        }
    }
    n_error("ShaderVariable %s doesn't exits in frameposteffect %s", sem.Value().AsCharPtr(), this->GetName().Value().AsCharPtr());
    return this->shaderVariables[0];
}

//------------------------------------------------------------------------------
/**
*/
inline void
FramePostEffect::AddBatch(const Ptr<FrameBatch>& batch)
{
    this->batches.Append(batch);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
FramePostEffect::GetNumBatches() const
{
    return this->batches.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<FrameBatch>&
FramePostEffect::GetBatchByIndex(IndexT i) const
{
    return this->batches[i];
}

} // namespace Frame
//------------------------------------------------------------------------------

     