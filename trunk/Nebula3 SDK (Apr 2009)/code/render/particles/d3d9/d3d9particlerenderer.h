#pragma once
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9ParticleRenderer
    
    Particle system renderer for D3D9/Xbox360. The renderer makes use of
    hardware instancing to prevent writing redundant data to 
    dynamic vertex buffers.
    
    See here for details: 
    http://zeuxcg.blogspot.com/2007/09/particle-rendering-revisited.html
    
    (C) 2008 Radon Labs GmbH
*/
#include "particles/base/particlerendererbase.h"
#include "coregraphics/vertexbuffer.h"
#include "coregraphics/indexbuffer.h"
#include "coregraphics/vertexlayout.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9ParticleRenderer : public Base::ParticleRendererBase
{
    __DeclareClass(D3D9ParticleRenderer);
    __DeclareSingleton(D3D9ParticleRenderer);
public:
    /// constructor
    D3D9ParticleRenderer();
    /// destructor
    virtual ~D3D9ParticleRenderer();
    
    /// setup the particle renderer
    void Setup();
    /// discard the particle renderer
    void Discard();

    /// begin attaching visible particle systems
    void BeginAttach();
    /// attach a visible particle system instance
    Particles::ParticleRenderInfo AddVisibleParticleSystem(const Ptr<Particles::ParticleSystemInstance>& particleSystem);
    /// finish attaching visible particle systems
    void EndAttach();

    /// render particles of previously attached particle system
    void RenderParticleSystem(const Particles::ParticleRenderInfo& renderInfo);

private:
    static const SizeT MaxNumRenderedParticles = 8192;

    Ptr<CoreGraphics::VertexBuffer> particleVertexBuffer;
    Ptr<CoreGraphics::VertexBuffer> cornerVertexBuffer;
    Ptr<CoreGraphics::IndexBuffer> cornerIndexBuffer;
    CoreGraphics::PrimitiveGroup primGroup;
    Ptr<CoreGraphics::VertexLayout> vertexLayout;
    IndexT curParticleVertexIndex;
    void* mappedVertices;
    void* curVertexPtr;
};

} // namespace Direct3D9
//------------------------------------------------------------------------------
    
    