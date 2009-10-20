#pragma once
//------------------------------------------------------------------------------
/**
    @class Animation::AnimEventServer

    This is the server, which is triggered if a animation event is emitted.

    Attach here some handlers to handle special animevents. Handler can be
    registered via RegisterAnimEventHandler and unregistered through
    UnregisterAnimEventHandler messages from the GraphicsProtocol!
    
    Animeventhandler are specified by there category name, they handle!

    (C) 2009 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "animation/animeventhandlerbase.h"

//------------------------------------------------------------------------------
namespace Animation
{
class AnimEventServer : public Core::RefCounted
{
    __DeclareClass(AnimEventServer);
    __DeclareSingleton(AnimEventServer);
public:

    /// constructor
    AnimEventServer();
    /// destructor
    virtual ~AnimEventServer();    

    /// open the server
    void Open();
    /// close the server
    void Close();
    /// return true if open
    bool IsOpen() const;

    /// delegate to attached handler
    void OnFrame(Timing::Time time);

    /// attach an animeventhandler
    void RegisterAnimEventHandler(const Ptr<AnimEventHandlerBase>& newHandler);
    /// detach an animeventhandler
    void UnregisterAnimEventHandler(const Util::StringAtom& categoryName);

    /// delegate this event to a attached handler
    bool HandleEventz(const Util::Array<Animation::AnimEventInfo>& eventz);

private:
    bool isOpen;
    Util::Dictionary<Util::StringAtom, Ptr<AnimEventHandlerBase> > animEventHandler;

};

//------------------------------------------------------------------------------
/**
*/
inline bool
AnimEventServer::IsOpen() const
{
    return this->isOpen;
}
} // namespace Audio
//------------------------------------------------------------------------------
    