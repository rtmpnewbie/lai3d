//------------------------------------------------------------------------------
//  frameposteffect.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "frame/frameposteffect.h"
#include "coregraphics/memoryvertexbufferloader.h"
#include "coregraphics/indexbuffer.h"
#include "coregraphics/renderdevice.h"

namespace Frame
{
__ImplementClass(Frame::FramePostEffect, 'FPEF', Core::RefCounted);

using namespace Util;
using namespace Resources;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
FramePostEffect::FramePostEffect()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
FramePostEffect::~FramePostEffect()
{
    // make sure Discard() has been called
    n_assert(!this->shader.isvalid());
    n_assert(!this->renderTarget.isvalid());
    n_assert(this->shaderVariables.IsEmpty());
    n_assert(this->batches.IsEmpty());
}

//------------------------------------------------------------------------------
/**
    This setsup the quad mesh for rendering the fullscreen quad.
*/
void
FramePostEffect::Setup()
{
    n_assert(!this->vertexBuffer.isvalid());
    n_assert(this->renderTarget.isvalid());
    
    // setup a vertex declaration and vertex buffer for a fullscreen quad
    Array<VertexComponent> vertexComponents;
    vertexComponents.Append(VertexComponent(VertexComponent::Position, 0, VertexComponent::Float3));
    vertexComponents.Append(VertexComponent(VertexComponent::TexCoord, 0, VertexComponent::Float2));

    // compute a half pixel offset
    SizeT w = this->renderTarget->GetWidth();
    SizeT h = this->renderTarget->GetHeight();
    Math::float4 pixelSize(2.0f / float(w), 2.0f / float(h), 0.0f, 0.0f);
    Math::float4 halfPixelSize = pixelSize * 0.5f;

    // add a PixelSize shader variable if our shader wants it
    ShaderVariable::Semantic semPixelSize("PixelSize");
    if (this->shader->HasVariableBySemantic(semPixelSize))
    {
        Ptr<ShaderVariable> var = this->shader->GetVariableBySemantic(semPixelSize);
        Ptr<ShaderVariableInstance> varInst = var->CreateInstance();
        varInst->SetFloat4(pixelSize);
        this->shaderVariables.Append(varInst);
    }
    
    // compute screen rectangle coordinates
    float left   = -1.0f - halfPixelSize.x();
    float right  = +1.0f - halfPixelSize.x();
    float top    = +1.0f + halfPixelSize.y();
    float bottom = -1.0f + halfPixelSize.y();

    // compute uv coordinates with proper half pixel offset
    float u0 = 0.0f;
    float u1 = 1.0f;
    float v0 = 0.0f;
    float v1 = 1.0f;

    // setup a vertex buffer with 2 triangles
    float v[6][5];

    // first triangle
    v[0][0] = left;  v[0][1] = top;    v[0][2] = 0.0f; v[0][3] = u0; v[0][4] = v0;
    v[1][0] = left;  v[1][1] = bottom; v[1][2] = 0.0f; v[1][3] = u0; v[1][4] = v1;
    v[2][0] = right; v[2][1] = top;    v[2][2] = 0.0f; v[2][3] = u1; v[2][4] = v0;

    // second triangle
    v[3][0] = left;  v[3][1] = bottom; v[3][2] = 0.0f; v[3][3] = u0; v[3][4] = v1;
    v[4][0] = right; v[4][1] = bottom; v[4][2] = 0.0f; v[4][3] = u1; v[4][4] = v1;
    v[5][0] = right; v[5][1] = top;    v[5][2] = 0.0f; v[5][3] = u1; v[5][4] = v0;

    // setup vertex buffer with memory-vertexbuffer-loader
    this->vertexBuffer = VertexBuffer::Create();
    Ptr<MemoryVertexBufferLoader> vbLoader = MemoryVertexBufferLoader::Create();
    vbLoader->Setup(vertexComponents, 6, v, 6 * 5 * sizeof(float), VertexBuffer::UsageImmutable, VertexBuffer::AccessNone);
    this->vertexBuffer->SetLoader(vbLoader.upcast<ResourceLoader>());
    this->vertexBuffer->SetAsyncEnabled(false);
    this->vertexBuffer->Load();
    n_assert(this->vertexBuffer->IsLoaded());
    this->vertexBuffer->SetLoader(0);

    // setup a primitive group object
    this->primGroup.SetBaseVertex(0);
    this->primGroup.SetNumVertices(6);
    this->primGroup.SetBaseIndex(0);
    this->primGroup.SetNumIndices(0);
    this->primGroup.SetPrimitiveTopology(PrimitiveTopology::TriangleList);
}

//------------------------------------------------------------------------------
/**
*/
void
FramePostEffect::Discard()
{
    if (this->shader.isvalid())
    {
        this->shader->Discard();
        this->shader = 0;
    }
    if (this->renderTarget.isvalid())
    {
        if (this->renderTarget->IsValid())
        {
            this->renderTarget->Discard();
        }
        this->renderTarget = 0;
    }
    if (this->vertexBuffer.isvalid())
    {
        this->vertexBuffer->Unload();
        this->vertexBuffer = 0;
    }
    IndexT i;
    for (i = 0; i < this->batches.Size(); i++)
    {
        this->batches[i]->Discard();
    }
    this->batches.Clear();
    this->shaderVariables.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
FramePostEffect::Render()
{
    n_assert(this->renderTarget.isvalid());
    RenderDevice* renderDevice = RenderDevice::Instance();

    // update render target
    // @todo: hmm... should be able to define whether a clear is necessary or not!    
    if (this->renderTarget.isvalid())
    {
        // update render targets
        this->renderTarget->SetClearColor(Math::float4(0.0f, 0.0f, 0.0f, 0.0f));
        this->renderTarget->SetClearDepth(ClearDepthValue);
        this->renderTarget->SetClearStencil(0);  
    }

    // apply shader variables
    IndexT varIndex;
    for (varIndex = 0; varIndex < this->shaderVariables.Size(); varIndex++)
    {
        this->shaderVariables[varIndex]->Apply();
    }

    // first render fullscreen quad, then any render batches 
    if (this->renderTarget.isvalid())
    {
        renderDevice->BeginPass(this->renderTarget, this->shader);
    }
    else
    {
        renderDevice->BeginPass(this->multipleRenderTarget, this->shader);
    }     
    renderDevice->SetStreamSource(0, this->vertexBuffer, 0);
    renderDevice->SetVertexLayout(this->vertexBuffer->GetVertexLayout());
    renderDevice->SetPrimitiveGroup(this->primGroup);
    renderDevice->Draw();
    IndexT batchIndex;
    for (batchIndex = 0; batchIndex < this->batches.Size(); batchIndex++)
    {
        this->batches[batchIndex]->Render();
    }
    renderDevice->EndPass();
}

} // namespace Frame
