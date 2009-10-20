//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayerprotocol.h"

namespace Multiplayer
{
    __ImplementClass(Multiplayer::Connect, 'conc', Messaging::Message);
    __ImplementMsgId(Connect);
    __ImplementClass(Multiplayer::Disconnect, 'dcon', Messaging::Message);
    __ImplementMsgId(Disconnect);
    __ImplementClass(Multiplayer::ConnectionSucceeded, 'cosu', Messaging::Message);
    __ImplementMsgId(ConnectionSucceeded);
    __ImplementClass(Multiplayer::ConnectionLost, 'colo', Messaging::Message);
    __ImplementMsgId(ConnectionLost);
    __ImplementClass(Multiplayer::ConnectionFailed, 'cofa', Messaging::Message);
    __ImplementMsgId(ConnectionFailed);
    __ImplementClass(Multiplayer::Chatmsg, 'chms', Messaging::Message);
    __ImplementMsgId(Chatmsg);
    __ImplementClass(Multiplayer::IsClientConnected, 'clco', Messaging::Message);
    __ImplementMsgId(IsClientConnected);
    __ImplementClass(Multiplayer::LoadLevel, 'lolv', Messaging::Message);
    __ImplementMsgId(LoadLevel);
    __ImplementClass(Multiplayer::LevelLoaded, 'lvld', Messaging::Message);
    __ImplementMsgId(LevelLoaded);
    __ImplementClass(Multiplayer::SynchronizationFinished, 'syfi', Messaging::Message);
    __ImplementMsgId(SynchronizationFinished);
    __ImplementClass(Multiplayer::CreateEntity, 'cren', Messaging::Message);
    __ImplementMsgId(CreateEntity);
    __ImplementClass(Multiplayer::DoesEntityExist, 'dene', Messaging::Message);
    __ImplementMsgId(DoesEntityExist);
    __ImplementClass(Multiplayer::EntityExists, 'enex', Messaging::Message);
    __ImplementMsgId(EntityExists);
    __ImplementClass(Multiplayer::GuidNetIdTable, 'guit', Messaging::Message);
    __ImplementMsgId(GuidNetIdTable);
    __ImplementClass(Multiplayer::SetTransformOfEntity, 'stoe', Messaging::Message);
    __ImplementMsgId(SetTransformOfEntity);
    __ImplementClass(Multiplayer::UpdateTransformOfEntity, 'upte', Messaging::Message);
    __ImplementMsgId(UpdateTransformOfEntity);
    __ImplementClass(Multiplayer::CreateClientSideEntity, 'crce', Messaging::Message);
    __ImplementMsgId(CreateClientSideEntity);
    __ImplementClass(Multiplayer::DeleteClientSideEntity, 'dcse', Messaging::Message);
    __ImplementMsgId(DeleteClientSideEntity);
} // Multiplayer

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
