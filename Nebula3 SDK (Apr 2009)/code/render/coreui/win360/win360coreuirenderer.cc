//------------------------------------------------------------------------------
//  win32coreuirenderer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreui/win360/win360coreuirenderer.h"
#include "coregraphics/memoryvertexbufferloader.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/transformdevice.h"
#include "timing/slavetime.h"

namespace Win32
{
__ImplementClass(Win360CoreUIRenderer, 'WUIR', Base::CoreUIRendererBase);
__ImplementSingleton(Win360CoreUIRenderer);

using namespace CoreGraphics;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
Win360CoreUIRenderer::Win360CoreUIRenderer():
curNumVertices(0)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
Win360CoreUIRenderer::~Win360CoreUIRenderer()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
Win360CoreUIRenderer::Open()
{
    // create vertex buffer
    this->vertexBuffer = VertexBuffer::Create();

    // MemoryVertexBufferLoader::Setup mit 0-Datapointern
    Util::Array<VertexComponent> vertexComponents;
    vertexComponents.Append(VertexComponent(VertexComponent::Position, 0, VertexComponent::Float3));
    vertexComponents.Append(VertexComponent(VertexComponent::Color, 0, VertexComponent::Float4));
    vertexComponents.Append(VertexComponent(VertexComponent::TexCoord, 0, VertexComponent::Float2));

    Ptr<MemoryVertexBufferLoader> vbLoader = MemoryVertexBufferLoader::Create();
    vbLoader->Setup(vertexComponents, 
                    4096, 
                    0, 
                    4096 * 9 * sizeof(float),
                    Base::ResourceBase::UsageDynamic,
                    Base::ResourceBase::AccessWrite);
    this->vertexBuffer->SetLoader(vbLoader.upcast<ResourceLoader>());
    this->vertexBuffer->SetAsyncEnabled(false);
    this->vertexBuffer->Load();
    n_assert(this->vertexBuffer->IsLoaded());
    this->vertexBuffer->SetLoader(0);

    // set primitive grout
    this->primGroup.SetBaseVertex(0);
    this->primGroup.SetNumVertices(4096);
    this->primGroup.SetBaseIndex(0);
    this->primGroup.SetNumIndices(0);

#if __XBOX360__
    this->primGroup.SetPrimitiveTopology(PrimitiveTopology::QuadList);
#else
    this->primGroup.SetPrimitiveTopology(PrimitiveTopology::TriangleList);
#endif

    // get the shader semantic
    this->guiTextureSemantic = ShaderVariable::Semantic("GuiTexture");

    // get gui shader for loading screen rendering
    ResourceId shdResId = ResourceId("shd:gui");
    this->guiShaderInst = ShaderServer::Instance()->CreateShaderInstance(shdResId);    

    Base::CoreUIRendererBase::Open();
}

//------------------------------------------------------------------------------
/**
*/
void 
Win360CoreUIRenderer::SetupVertexBuffer()
{
    // get elements
    Util::Array<ElementByTexture> elements = this->elementzByDepth.ValuesAsArray();
    // append fadeout elements
    elements.AppendArray(this->fadeOutElements.ValuesAsArray());
    float depthDivisor = elements[elements.Size() - 1].element.GetDepth() + TINY;

    // reset vertex buffer
    float* vertexPtr = (float*)this->vertexBuffer->Map(VertexBuffer::MapWriteDiscard);
    this->curNumVertices = 0;

    // now build the buffer
    // now do for all elements, but do it backwards, cause they are sorted from near to far, 
    // and we have to render them in other order
    IndexT index;
    for(index = elements.Size() - 1; index >= 0; index--)
    {
        // setup new element
        VertexBufferEntry newOne;
        newOne.texture = elements[index].texturePtr;
        newOne.startIndex = this->curNumVertices;

        // skip elements which texture isn't loaded yet
        if (elements[index].texturePtr->GetState() != Resources::Resource::Loaded)
        {
            continue;
        }   
        // do as long, as the elements use the same texture and its not the last
        while(index >= 0)
        {
            // get element data to render
            const CoreUI::UIElement& curElement = elements[index].element;
            Math::rectangle<float> r = curElement.GetGraphicsDimension();
            const Math::float4& uv = curElement.GetUV();
            Math::float4 color = curElement.GetColor();   
            // compute blink color
            //if (curElement.GetBlinking())
            //{
            //    //float lerpFactor = Math::n_fmod((float)(curTime - curElement.GetCreationTime()), (float)curElement.GetBlinkTime());
            //    //lerpFactor -= floor(lerpFactor);
            //    //lerpFactor = Math::n_abs((lerpFactor - 0.5f) * 2.0f);
            //    //color = Math::float4::lerp(color, Math::float4::multiply(color, curElement.GetBlinkColor()), lerpFactor);
            //}

            // compute fade color
            if (curElement.IsFadingIn())
            {                
                // fadein
                float blendValue = (float)(curElement.GetCurrentFadeInTime() / curElement.GetFadeInTime());
                color.w() *= blendValue;
            }
            else if (curElement.IsFadingOut())
            {
                // fadeout
                float blendValue = (float)(curElement.GetCurrentFadeOutTime() / curElement.GetFadeOutTime());
                color.w() *= blendValue;
            }
            float depth = (float)curElement.GetDepth() / depthDivisor;

            // first triangle, top-left
            *vertexPtr++ = r.left;    *vertexPtr++ = r.top;     *vertexPtr++ = depth;
            *vertexPtr++ = color.x(); *vertexPtr++ = color.y(); *vertexPtr++ = color.z(); *vertexPtr++ = color.w();
            *vertexPtr++ = uv.x();    *vertexPtr++ = uv.y();

            // first triangle, bottom-left
            *vertexPtr++ = r.left;    *vertexPtr++ = r.bottom;  *vertexPtr++ = depth;
            *vertexPtr++ = color.x(); *vertexPtr++ = color.y(); *vertexPtr++ = color.z(); *vertexPtr++ = color.w();
            *vertexPtr++ = uv.x();    *vertexPtr++ = uv.w();
#if !__XBOX360__
            // first triangle, top-right
            *vertexPtr++ = r.right;   *vertexPtr++ = r.top;     *vertexPtr++ = depth;
            *vertexPtr++ = color.x(); *vertexPtr++ = color.y(); *vertexPtr++ = color.z(); *vertexPtr++ = color.w();
            *vertexPtr++ = uv.z();    *vertexPtr++ = uv.y();

            // second triangle, bottom-left
            *vertexPtr++ = r.left;    *vertexPtr++ = r.bottom;  *vertexPtr++ = depth;
            *vertexPtr++ = color.x(); *vertexPtr++ = color.y(); *vertexPtr++ = color.z(); *vertexPtr++ = color.w();
            *vertexPtr++ = uv.x();    *vertexPtr++ = uv.w();
#endif
            // second triangle, bottom-right
            *vertexPtr++ = r.right;   *vertexPtr++ = r.bottom;  *vertexPtr++ = depth;
            *vertexPtr++ = color.x(); *vertexPtr++ = color.y(); *vertexPtr++ = color.z(); *vertexPtr++ = color.w();
            *vertexPtr++ = uv.z();    *vertexPtr++ = uv.w();

            // second triangle, top-right
            *vertexPtr++ = r.right;   *vertexPtr++ = r.top;     *vertexPtr++ = depth;
            *vertexPtr++ = color.x(); *vertexPtr++ = color.y(); *vertexPtr++ = color.z(); *vertexPtr++ = color.w();
            *vertexPtr++ = uv.z();    *vertexPtr++ = uv.y();
            
#if __XBOX360__
            this->curNumVertices += 4;
#else
            this->curNumVertices += 6;
#endif

            // check if the next element has a different texture
            if (index > 0 && elements[index - 1].texturePtr != newOne.texture)
            {
                break;
            }

            // finally increment counter
            index--;
        }

        // set end index
        newOne.numIndices = this->curNumVertices - newOne.startIndex;
        this->vertexBufferEntries.Append(newOne);       
    }

    // close vertex buffer
    this->vertexBuffer->Unmap();
}

//------------------------------------------------------------------------------
/**
*/
void 
Win360CoreUIRenderer::DrawUI(const Ptr<CoreGraphics::ShaderInstance>& shdInst)
{
    //in case of this assertion: increase the number of vertices (4096) in open-function
    n_assert((this->GetElementCount() * 6) < this->vertexBuffer->GetNumVertices());  
    
    // update elements, removes elements which are faded out
    this->UpdateElements();

    // just do if one element min
    if (this->GetElementCount() <= 0)
    {
        return;
    }

    // setup the vertex buffer
    this->SetupVertexBuffer();

    // now simply draw the vertex buffer
    IndexT index;
    for (index = 0; index < this->vertexBufferEntries.Size(); index++)
    {
        // current entry
        VertexBufferEntry& entry = this->vertexBufferEntries[index];

        //// apply shader variables
        shdInst->GetVariableBySemantic(this->guiTextureSemantic)->SetTexture(entry.texture->GetTexture());
        shdInst->Commit();

        //// render the vertex buffer
        RenderDevice* renderDevice = RenderDevice::Instance();        
        this->primGroup.SetBaseVertex(entry.startIndex);
        this->primGroup.SetNumVertices(entry.numIndices);
        renderDevice->SetStreamSource(0, this->vertexBuffer, 0);
        renderDevice->SetVertexLayout(this->vertexBuffer->GetVertexLayout());
        renderDevice->SetPrimitiveGroup(this->primGroup);
        renderDevice->Draw();
    }

    this->vertexBufferEntries.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
Win360CoreUIRenderer::Close()
{   
    if (this->vertexBuffer.isvalid())
    {
        this->vertexBuffer->Unload();
        this->vertexBuffer = 0;
    }
    if (this->guiShaderInst.isvalid())
    {
        this->guiShaderInst->Discard();
        this->guiShaderInst = 0;
    }
    Base::CoreUIRendererBase::Close();
}

//------------------------------------------------------------------------------
/**
*/
bool 
Win360CoreUIRenderer::AllTexturesLoaded() const
{
    const Util::Array<ElementByTexture>& elements = this->elementzByDepth.ValuesAsArray();
    // do not render any ui until all textures are loaded
    IndexT index;
    for(index = 0; index < elements.Size(); ++index)
    {        
        if (elements[index].texturePtr->GetState() != Resources::Resource::Loaded)
        {
            return false;
        }        
    }
    return true;
}
} // namespace Win32
