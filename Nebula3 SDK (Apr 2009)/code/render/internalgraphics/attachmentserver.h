#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalGraphics::AttachmentServer
    
    Renderthread side server for managing attachments. Is call once per frame.
    Manages the updates of the positions of the attached entities, the attaching
    and detaching of attachments

    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "graphics/handle.h"
#include "internalgraphics/internalgraphicsentity.h"
#include "util/stringatom.h"
#include "io/uri.h"
#include "resources/resourceid.h"
#include "timing/timer.h"

// forward declarations
namespace Graphics
{
    class BindAttachment;
    class UnbindAttachment;
}
namespace Characters
{
    class CharacterJointInstance;
}
namespace InternalGraphics
{
    class InternalModelEntity;
}
namespace Animation
{
    class AnimEventInfo;
}

//------------------------------------------------------------------------------
namespace InternalGraphics
{
class AttachmentServer : public Core::RefCounted
{
    __DeclareClass(AttachmentServer);
    __DeclareSingleton(AttachmentServer);

public:
    /// how to rotate the attached entity
    enum AttachmentRotation
    {
        Local,
        World,
        Entity,

        NumAttachmentRotations,
        InvalidAttachmentRotation
    };

    /// types for clearing attachments
    enum ClearType
    {
        ClearAll,                               // clear all attachments in world
        ClearAllOnEntity,                       // clear all attachments on entity
        ClearAllOnJoint,                        // clear all attachments on joint
        ClearAllIdenticalOnEntity,              // clear all identical attachments on entity
        ClearAllIdenticalOnJoint,                 // clear all identical attachments on joint

        ClearNone,                              // do not clear anything

        NumClearTypes,
        InvalidClearType
    };

	/// constructor
    AttachmentServer();
    /// destructor
    virtual ~AttachmentServer();
    
    /// open the server
    bool Open();
    /// close the server
    void Close();
    /// return true if open
    bool IsOpen() const;

    /// call this once per frame
    void OnFrame(Timing::Time time);

    /// attach a new entity
    void AttachEntity(const Math::matrix44& offset,
                      const ClearType& clearType,
                      const Util::String& joint,
                      const Ptr<InternalGraphicsEntity>& entityToAttach,
                      const Ptr<InternalModelEntity>& baseEntity,
                      bool keepLocal,
                      AttachmentRotation rotation);

    /// attach a new entity, and detach it after given time
    void AttachEntityTemporary( const Math::matrix44& offset,
                                const ClearType& clearType,
                                const Util::String& joint,
                                const Ptr<InternalGraphicsEntity>& entityToAttach,
                                const Ptr<InternalModelEntity>& baseEntity,
                                bool keepLocal,
                                Timing::Time duration,
                                AttachmentRotation rotation);

    /// detach an entity
    void DetachEntity(const ClearType& clearType,
                      const Util::String& joint,
                      const Ptr<InternalGraphicsEntity>& entityToAttach,
                      const Ptr<InternalModelEntity>& baseEntity);
    /// remove attachment
    void DetachEntity(const Ptr<InternalGraphics::InternalGraphicsEntity>& entity);

private:
    /// calculate the desired rotation

    /// final attachments for rendering
    struct InternalAttachment
    {
        Ptr<InternalGraphics::InternalModelEntity> baseEntity;
        Ptr<InternalGraphics::InternalGraphicsEntity> attachedEntity;
        IndexT jointIndex;
        Math::matrix44 offset;
        Timing::Time startTime;
        Timing::Time duration;
        bool keepLocal;
        bool initPosSet;
        AttachmentRotation rotation;
    };

    /// all attachments in the world
    Util::Array<InternalAttachment> attachments;
    bool isOpen;

    Timing::Time time;
}; 

//------------------------------------------------------------------------------
/**
*/
inline bool
AttachmentServer::IsOpen() const
{
    return this->isOpen;
}

} // namespace InternalGraphics
//------------------------------------------------------------------------------

    
    