#pragma once
//------------------------------------------------------------------------------
/**
    @class Base::RenderTargetBase

    Base class for render targets. A render targets wraps up to 4 color buffers
    and an optional depth/stencil buffer into a C++ object. The special
    default render target represents the backbuffer and default depth/stencil
    surface.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "coregraphics/antialiasquality.h"
#include "coregraphics/texture.h"
#include "coregraphics/batchtype.h"
#include "math/rectangle.h"

//------------------------------------------------------------------------------
namespace Base
{
class RenderTargetBase : public Core::RefCounted
{
    __DeclareClass(RenderTargetBase);
public:
    /// max number of color buffers
    static const IndexT MaxNumColorBuffers = 4;

    /// constructor
    RenderTargetBase();
    /// destructor
    virtual ~RenderTargetBase();
    
    /// get default render target flag
    bool IsDefaultRenderTarget() const;
    /// return true if color buffer exists
    bool HasColorBuffer(IndexT colorBufferIndex) const;
    /// return true if the render target has a depth/stencil buffer
    bool HasDepthStencilBuffer() const;
    /// return true if valid (has been setup)
    bool IsValid() const;

    /// set render target width
    void SetWidth(SizeT w);
    /// get width of render target in pixels
    SizeT GetWidth() const;
    /// set render target height
    void SetHeight(SizeT h);
    /// get height of render target in pixels
    SizeT GetHeight() const;
    /// set antialias quality
    void SetAntiAliasQuality(CoreGraphics::AntiAliasQuality::Code c);
    /// get anti-alias-quality
    CoreGraphics::AntiAliasQuality::Code GetAntiAliasQuality() const;
    /// add a color buffer
    void AddColorBuffer(CoreGraphics::PixelFormat::Code colorFormat);
    /// get number of color buffers
    SizeT GetNumColorBuffers() const;
    /// get color buffer format at index
    CoreGraphics::PixelFormat::Code GetColorBufferFormat(IndexT colorBufferIndex) const;
    /// add a depth/stencil buffer
    void AddDepthStencilBuffer();
    /// enable mipmap generation for this render target
    void SetMipMapsEnabled(bool b);
    /// get mipmap generation flag
    bool AreMipMapsEnabled() const;
    /// set resolve texture resource id
    void SetResolveTextureResourceId(const Resources::ResourceId& resId);
    /// get resolve texture resource id
    const Resources::ResourceId& GetResolveTextureResourceId() const;
    /// set optional resolve depth texture resource id
    void SetResolveDepthTextureResourceId(const Resources::ResourceId& resId);
    /// get optional resolve depth texture resource id
    const Resources::ResourceId& GetResolveDepthTextureResourceId() const;
    /// set resolve texture width
    void SetResolveTextureWidth(SizeT w);
    /// get resolve texture width
    SizeT GetResolveTextureWidth() const;
    /// set resolve texture height
    void SetResolveTextureHeight(SizeT h);
    /// get resolve texture height
    SizeT GetResolveTextureHeight() const;    
    /// set cpu access flag
    void SetResolveTargetCpuAccess(bool b);
    /// set optional MRT (Multiple Render Target) index, default is 0
    void SetMRTIndex(IndexT i);
    /// get multiple-render-target index
    IndexT GetMRTIndex() const;

    /// set clear color 
    void SetClearColor(const Math::float4& c);
    /// get clear color
    const Math::float4& GetClearColor() const;
    /// set clear depth
    void SetClearDepth(float d);
    /// get clear depth
    float GetClearDepth() const;
    /// set clear stencil value
    void SetClearStencil(uchar s);
    /// get clear stencil value
    uchar GetClearStencil() const;
    /// set the current resolve rectangle (in pixels)
    void SetResolveRect(const Math::rectangle<int>& r);
    /// get resolve rectangle
    const Math::rectangle<int>& GetResolveRect() const;

    /// setup the render target object
    void Setup();
    /// discard the render target object
    void Discard();
    /// begin rendering to the render target
    void BeginPass();
    /// begin a batch
    void BeginBatch(CoreGraphics::BatchType::Code batchType);
    /// end current batch
    void EndBatch();
    /// end current render pass
    void EndPass();
    /// generate mipmap levels
    void GenerateMipLevels();
    /// return true if resolve texture is valid
    bool HasResolveTexture() const;
    /// get the resolve texture as Nebula texture object
    const Ptr<CoreGraphics::Texture>& GetResolveTexture() const;  
    /// return true if resolve texture is valid
    bool HasCPUResolveTexture() const;
    /// get the resolve texture as Nebula texture object
    const Ptr<CoreGraphics::Texture>& GetCPUResolveTexture() const;  
    /// return true if resolve texture is valid
    bool HasDepthResolveTexture() const;
    /// get the resolve texture as Nebula texture object
    const Ptr<CoreGraphics::Texture>& GetDepthResolveTexture() const;
    /// resolve depth buffer
    virtual void ResolveDepthBuffer();

protected:
    friend class RenderDeviceBase;

    /// set to true if default render target
    void SetDefaultRenderTarget(bool b);

    Resources::ResourceId resolveTextureResId;    
    Resources::ResourceId resolveDepthTextureResId;
    CoreGraphics::BatchType::Code batchType;
    SizeT width;
    SizeT height;
    bool resolveTextureDimensionsValid;
    bool resolveRectValid;
    SizeT resolveTextureWidth;
    SizeT resolveTextureHeight;
    IndexT mrtIndex;
    Math::rectangle<int> resolveRect;
    Math::float4 clearColor;
    float clearDepth;
    uchar clearStencil;
    CoreGraphics::AntiAliasQuality::Code antiAliasQuality;
    SizeT numColorBuffers;
    CoreGraphics::PixelFormat::Code colorBufferFormats[MaxNumColorBuffers];
    Ptr<CoreGraphics::Texture> resolveTexture;
    Ptr<CoreGraphics::Texture> resolveCPUTexture;
    Ptr<CoreGraphics::Texture> resolveDepthTexture;
    bool mipMapsEnabled;
    bool isValid;
    bool inBeginPass;
    bool inBeginBatch;
    bool hasDepthStencilBuffer;
    bool isDefaultRenderTarget;
    bool resolveCpuAccess;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
RenderTargetBase::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTargetBase::SetDefaultRenderTarget(bool b)
{
    this->isDefaultRenderTarget = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
RenderTargetBase::IsDefaultRenderTarget() const
{
    return this->isDefaultRenderTarget;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTargetBase::SetWidth(SizeT w)
{
    this->width = w;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
RenderTargetBase::GetWidth() const
{
    return this->width;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTargetBase::SetHeight(SizeT h)
{
    this->height = h;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
RenderTargetBase::GetHeight() const
{
    return this->height;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTargetBase::SetAntiAliasQuality(CoreGraphics::AntiAliasQuality::Code aaq)
{
    this->antiAliasQuality = aaq;
}

//------------------------------------------------------------------------------
/**
*/
inline CoreGraphics::AntiAliasQuality::Code
RenderTargetBase::GetAntiAliasQuality() const
{
    return this->antiAliasQuality;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTargetBase::AddColorBuffer(CoreGraphics::PixelFormat::Code colorFormat)
{
    n_assert(this->numColorBuffers < MaxNumColorBuffers);
    this->colorBufferFormats[this->numColorBuffers++] = colorFormat;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
RenderTargetBase::GetNumColorBuffers() const
{
    return this->numColorBuffers;
}

//------------------------------------------------------------------------------
/**
*/
inline CoreGraphics::PixelFormat::Code
RenderTargetBase::GetColorBufferFormat(IndexT colorBufferIndex) const
{
    n_assert(colorBufferIndex < MaxNumColorBuffers);
    return this->colorBufferFormats[colorBufferIndex];
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTargetBase::AddDepthStencilBuffer()
{
    n_assert(!this->hasDepthStencilBuffer);
    this->hasDepthStencilBuffer = true;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
RenderTargetBase::HasDepthStencilBuffer() const
{
    return this->hasDepthStencilBuffer;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTargetBase::SetClearColor(const Math::float4& c)
{
    this->clearColor = c;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
RenderTargetBase::GetClearColor() const
{
    return this->clearColor;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTargetBase::SetClearDepth(float d)
{
    this->clearDepth = d;
}

//------------------------------------------------------------------------------
/**
*/
inline float
RenderTargetBase::GetClearDepth() const
{
    return this->clearDepth;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTargetBase::SetClearStencil(uchar s)
{
    this->clearStencil = s;
}

//------------------------------------------------------------------------------
/**
*/
inline uchar
RenderTargetBase::GetClearStencil() const
{
    return this->clearStencil;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTargetBase::SetResolveTextureResourceId(const Resources::ResourceId& resId)
{
    this->resolveTextureResId = resId;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
RenderTargetBase::GetResolveTextureResourceId() const
{
    return this->resolveTextureResId;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTargetBase::SetResolveDepthTextureResourceId(const Resources::ResourceId& resId)
{
    this->resolveDepthTextureResId = resId;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
RenderTargetBase::GetResolveDepthTextureResourceId() const
{
    return this->resolveDepthTextureResId;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
RenderTargetBase::HasResolveTexture() const
{
    return this->resolveTexture.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::Texture>&
RenderTargetBase::GetResolveTexture() const
{
    n_assert(this->resolveTexture.isvalid());
    return this->resolveTexture;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
RenderTargetBase::HasCPUResolveTexture() const
{
    return this->resolveCPUTexture.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::Texture>&
RenderTargetBase::GetCPUResolveTexture() const
{
    n_assert(this->resolveCPUTexture.isvalid());
    return this->resolveCPUTexture;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
RenderTargetBase::HasDepthResolveTexture() const
{
    return this->resolveDepthTexture.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::Texture>& 
RenderTargetBase::GetDepthResolveTexture() const
{
    n_assert(this->resolveDepthTexture.isvalid());
    return this->resolveDepthTexture;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTargetBase::SetMipMapsEnabled(bool b)
{
    this->mipMapsEnabled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
RenderTargetBase::AreMipMapsEnabled() const
{
    return this->mipMapsEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTargetBase::SetResolveTextureWidth(SizeT w)
{
    this->resolveTextureWidth = w;
    this->resolveTextureDimensionsValid = true;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
RenderTargetBase::GetResolveTextureWidth() const
{
    return this->resolveTextureWidth;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTargetBase::SetResolveTextureHeight(SizeT h)
{
    this->resolveTextureHeight = h;
    this->resolveTextureDimensionsValid = true;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
RenderTargetBase::GetResolveTextureHeight() const
{
    return this->resolveTextureHeight;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTargetBase::SetResolveRect(const Math::rectangle<int>& r)
{
    this->resolveRectValid = true;
    this->resolveRect = r;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::rectangle<int>&
RenderTargetBase::GetResolveRect() const
{
    return this->resolveRect;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTargetBase::SetResolveTargetCpuAccess(bool b)
{
    this->resolveCpuAccess = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTargetBase::SetMRTIndex(IndexT i)
{
    this->mrtIndex = i;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
RenderTargetBase::GetMRTIndex() const
{
    return this->mrtIndex;
}

} // namespace Base    
//------------------------------------------------------------------------------


