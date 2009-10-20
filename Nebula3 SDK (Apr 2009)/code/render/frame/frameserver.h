#pragma once
//------------------------------------------------------------------------------
/**
    @class Frame::FrameServer
    
    Server object of the frame subsystem. Factory for FrameShaders.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "resources/resourceid.h"
#include "frame/frameshader.h"

//------------------------------------------------------------------------------
namespace Frame
{
class FrameServer : public Core::RefCounted
{
    __DeclareClass(FrameServer);
    __DeclareSingleton(FrameServer);
public:
    /// constructor
    FrameServer();
    /// destructor
    virtual ~FrameServer();
    /// open the frame server (loads all frame shaders)
    bool Open();
    /// close the frame server
    void Close();
    /// return true if open
    bool IsOpen() const;
    /// return true if a frame shader exists
    bool HasFrameShader(const Resources::ResourceId& name) const;
    /// get frame shader by name
    const Ptr<FrameShader>& GetFrameShaderByName(const Resources::ResourceId& name) const;
    
private:
    Util::Dictionary<Resources::ResourceId, Ptr<FrameShader> > frameShaders;
    bool isOpen;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
FrameServer::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
FrameServer::HasFrameShader(const Resources::ResourceId& resId) const
{
    return this->frameShaders.Contains(resId);
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<FrameShader>&
FrameServer::GetFrameShaderByName(const Resources::ResourceId& resId) const
{
    return this->frameShaders[resId];
}

} // namespace Frame
//------------------------------------------------------------------------------

    