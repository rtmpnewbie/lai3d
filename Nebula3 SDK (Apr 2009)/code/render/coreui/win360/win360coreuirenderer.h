#pragma once
#ifndef WIN32COREUIRENDERER_H
#define WIN32COREUIRENDERER_H
//------------------------------------------------------------------------------
/**
    @class Win32::Win32CoreUIRenderer
  
    Win32 class for internal UI rendering.
    
    (C) 2008 Radon Labs GmbH
*/
#include "render/coreui/base/coreuirendererbase.h"
#include "coregraphics/vertexbuffer.h"
#include "coregraphics/primitivegroup.h"
#include "coregraphics/texture.h"
#include "coregraphics/shadervariable.h"

namespace Win32
{
class Win360CoreUIRenderer : public Base::CoreUIRendererBase
{
    __DeclareClass(Win360CoreUIRenderer);
    __DeclareSingleton(Win360CoreUIRenderer);
public:
    /// Constructor
    Win360CoreUIRenderer();
    /// Destructor
    virtual ~Win360CoreUIRenderer();

    /// setup the ui
    virtual void Open();
    /// draw the accumulated ui
    virtual void DrawUI(const Ptr<CoreGraphics::ShaderInstance>& shdInst);
    /// Discard all Member
    virtual void Close();
    /// get gui shader instance
    const Ptr<CoreGraphics::ShaderInstance>& GetShader() const;
    /// check texture resources
    bool AllTexturesLoaded() const;

private:
    /// prepare the vertex buffer stuff
    void SetupVertexBuffer();

    struct VertexBufferEntry
    {
        int startIndex;
        int numIndices;
        Ptr<Resources::ManagedTexture> texture;
    };
    Util::Array<VertexBufferEntry> vertexBufferEntries;

    Ptr<CoreGraphics::VertexBuffer> vertexBuffer;
    CoreGraphics::ShaderVariable::Semantic guiTextureSemantic;
    CoreGraphics::PrimitiveGroup primGroup;
    SizeT curNumVertices;
    Ptr<CoreGraphics::ShaderInstance> guiShaderInst;
}; //class

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderInstance>& 
Win360CoreUIRenderer::GetShader() const
{
    return this->guiShaderInst;
}

}; // namespace Win32

#endif