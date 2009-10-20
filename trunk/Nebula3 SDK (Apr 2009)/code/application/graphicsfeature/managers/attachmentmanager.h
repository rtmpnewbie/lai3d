#pragma once
//------------------------------------------------------------------------------
/**
    @class GraphicsFeature::AttachmentManager
    
    Main thread side manager (frontend) for managing attachments. 

    (C) 2008 Radon Labs GmbH
*/
#include "game/manager.h"
#include "core/singleton.h"
#include "util/string.h"
#include "math/matrix44.h"
#include "graphics/handle.h"
#include "internalgraphics/attachmentserver.h"

// forward decl
namespace Graphics
{
    class GraphicsEntity;
};

namespace Game
{
    class Entity;
};

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class AttachmentManager : public Game::Manager
{
    __DeclareClass(AttachmentManager);
    __DeclareSingleton(AttachmentManager);

public:
	/// constructor
    AttachmentManager();
    /// destructor
    virtual ~AttachmentManager();

    /// trigger
    virtual void OnFrame();
    /// called when removed from game server
    virtual void OnDeactivate();

    /// add a attachment
    void AddGraphicsAttachment( const Util::StringAtom& jointName,
                                const Util::StringAtom& baseEntityId,
                                const Util::StringAtom& entityToAttachResId,
                                const Math::matrix44& offset,
                                bool keepLocal,
                                InternalGraphics::AttachmentServer::AttachmentRotation rotation);

    /// attach a attachment temporary
    void AddGraphicsAttachmentTemporary( const Util::StringAtom& jointName,
                                         const Util::StringAtom& baseEntityId,
                                         const Util::StringAtom& entityToAttachResId,
                                         const Math::matrix44& offset,
                                         bool keepLocal,
                                         InternalGraphics::AttachmentServer::AttachmentRotation rotation,
                                         Timing::Time duration);

    /// add a attachment
    void AddGraphicsAttachment( const Util::StringAtom& jointName,
                                const Ptr<Game::Entity>& baseEntityPtr,
                                const Util::StringAtom& entityToAttachResId,
                                const Math::matrix44& offset,
                                bool keepLocal,
                                InternalGraphics::AttachmentServer::AttachmentRotation rotation);

    /// attach a attachment temporary
    void AddGraphicsAttachmentTemporary( const Util::StringAtom& jointName,
                                         const Ptr<Game::Entity>& baseEntityPtr,
                                         const Util::StringAtom& entityToAttachResId,
                                         const Math::matrix44& offset,
                                         bool keepLocal,
                                         InternalGraphics::AttachmentServer::AttachmentRotation rotation,
                                         Timing::Time duration);
    
private:
    /// pending attachments
    struct PendingAttachment
    {
        Ptr<Graphics::GraphicsEntity> baseEntity;
        Ptr<Graphics::GraphicsEntity> attachedEntity;
        Util::StringAtom joint;
        Math::matrix44 offset;
        bool keepLocal;
        InternalGraphics::AttachmentServer::AttachmentRotation rotation;
        Timing::Time duration;
        Timing::Time startTime;
    };

    /// called from OnFrame(): check pending attachments
    void UpdatePendingAttachments();

    /// check if we have to delete some attachments
    void CheckTemporaryAttachments();

    /// send final attachment to render thread
    void SendFinalAttachment( const PendingAttachment& attachment );

    /// send final detachment to render thread
    void SendFinalDetachment( const PendingAttachment& detachment );

    Util::Array<PendingAttachment> pendingAttachments;                  // this are not yet loaded
    Util::Array<PendingAttachment> attachedAttachments;                 // they are still attached
}; 

} // namespace GraphicsFeature
//------------------------------------------------------------------------------


    
    