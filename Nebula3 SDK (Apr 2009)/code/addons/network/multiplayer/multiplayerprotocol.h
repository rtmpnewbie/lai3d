#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "io/uri.h"
#include "util/string.h"
#include "util/guid.h"
#include "math/matrix44.h"

//------------------------------------------------------------------------------
namespace Multiplayer
{
//------------------------------------------------------------------------------
class Connect : public Messaging::Message
{
    __DeclareClass(Connect);
    __DeclareMsgId;
public:
    Connect() 
    { };
public:
    void SetServerAddress(const IO::URI& val)
    {
        n_assert(!this->handled);
        this->serveraddress = val;
    };
    const IO::URI& GetServerAddress() const
    {
        return this->serveraddress;
    };
private:
    IO::URI serveraddress;
};
//------------------------------------------------------------------------------
class Disconnect : public Messaging::Message
{
    __DeclareClass(Disconnect);
    __DeclareMsgId;
public:
    Disconnect() 
    { };
};
//------------------------------------------------------------------------------
class ConnectionSucceeded : public Messaging::Message
{
    __DeclareClass(ConnectionSucceeded);
    __DeclareMsgId;
public:
    ConnectionSucceeded() 
    { };
};
//------------------------------------------------------------------------------
class ConnectionLost : public Messaging::Message
{
    __DeclareClass(ConnectionLost);
    __DeclareMsgId;
public:
    ConnectionLost() 
    { };
};
//------------------------------------------------------------------------------
class ConnectionFailed : public Messaging::Message
{
    __DeclareClass(ConnectionFailed);
    __DeclareMsgId;
public:
    ConnectionFailed() 
    { };
};
//------------------------------------------------------------------------------
class Chatmsg : public Messaging::Message
{
    __DeclareClass(Chatmsg);
    __DeclareMsgId;
public:
    Chatmsg() 
    { };
public:
    void SetText(const Util::String& val)
    {
        n_assert(!this->handled);
        this->text = val;
    };
    const Util::String& GetText() const
    {
        return this->text;
    };
private:
    Util::String text;
public:
    void Encode(const Ptr<IO::BinaryWriter>& writer)
    {
        writer->WriteString(this->GetText());
    };
public:
    void Decode(const Ptr<IO::BinaryReader>& reader)
    {
        this->SetText(reader->ReadString());
    };
};
//------------------------------------------------------------------------------
class IsClientConnected : public Messaging::Message
{
    __DeclareClass(IsClientConnected);
    __DeclareMsgId;
public:
    IsClientConnected() 
    { };
public:
    void SetIsConnected(bool val)
    {
        n_assert(!this->handled);
        this->isconnected = val;
    };
    bool GetIsConnected() const
    {
        n_assert(this->handled);
        return this->isconnected;
    };
private:
    bool isconnected;
};
//------------------------------------------------------------------------------
class LoadLevel : public Messaging::Message
{
    __DeclareClass(LoadLevel);
    __DeclareMsgId;
public:
    LoadLevel() 
    { };
public:
    void SetLevelname(const Util::String& val)
    {
        n_assert(!this->handled);
        this->levelname = val;
    };
    const Util::String& GetLevelname() const
    {
        return this->levelname;
    };
private:
    Util::String levelname;
public:
    void Encode(const Ptr<IO::BinaryWriter>& writer)
    {
        writer->WriteString(this->GetLevelname());
    };
public:
    void Decode(const Ptr<IO::BinaryReader>& reader)
    {
        this->SetLevelname(reader->ReadString());
    };
};
//------------------------------------------------------------------------------
class LevelLoaded : public Messaging::Message
{
    __DeclareClass(LevelLoaded);
    __DeclareMsgId;
public:
    LevelLoaded() 
    { };
};
//------------------------------------------------------------------------------
class SynchronizationFinished : public Messaging::Message
{
    __DeclareClass(SynchronizationFinished);
    __DeclareMsgId;
public:
    SynchronizationFinished() 
    { };
};
//------------------------------------------------------------------------------
class CreateEntity : public Messaging::Message
{
    __DeclareClass(CreateEntity);
    __DeclareMsgId;
public:
    CreateEntity() 
    { };
public:
    void SetCategory(const Util::String& val)
    {
        n_assert(!this->handled);
        this->category = val;
    };
    const Util::String& GetCategory() const
    {
        return this->category;
    };
private:
    Util::String category;
public:
    void SetEntityId(const Util::String& val)
    {
        n_assert(!this->handled);
        this->entityid = val;
    };
    const Util::String& GetEntityId() const
    {
        return this->entityid;
    };
private:
    Util::String entityid;
public:
    void SetGuid(const Util::Guid& val)
    {
        n_assert(!this->handled);
        this->guid = val;
    };
    const Util::Guid& GetGuid() const
    {
        return this->guid;
    };
private:
    Util::Guid guid;
public:
    void Encode(const Ptr<IO::BinaryWriter>& writer)
    {
        writer->WriteString(this->GetCategory());
        writer->WriteString(this->GetEntityId());
        writer->WriteGuid(this->GetGuid());
    };
public:
    void Decode(const Ptr<IO::BinaryReader>& reader)
    {
        this->SetCategory(reader->ReadString());
        this->SetEntityId(reader->ReadString());
        this->SetGuid(reader->ReadGuid());
    };
};
//------------------------------------------------------------------------------
class DoesEntityExist : public Messaging::Message
{
    __DeclareClass(DoesEntityExist);
    __DeclareMsgId;
public:
    DoesEntityExist() 
    { };
public:
    void SetGuid(const Util::Guid& val)
    {
        n_assert(!this->handled);
        this->guid = val;
    };
    const Util::Guid& GetGuid() const
    {
        return this->guid;
    };
private:
    Util::Guid guid;
public:
    void Encode(const Ptr<IO::BinaryWriter>& writer)
    {
        writer->WriteGuid(this->GetGuid());
    };
public:
    void Decode(const Ptr<IO::BinaryReader>& reader)
    {
        this->SetGuid(reader->ReadGuid());
    };
};
//------------------------------------------------------------------------------
class EntityExists : public Messaging::Message
{
    __DeclareClass(EntityExists);
    __DeclareMsgId;
public:
    EntityExists() 
    { };
public:
    void SetGuid(const Util::Guid& val)
    {
        n_assert(!this->handled);
        this->guid = val;
    };
    const Util::Guid& GetGuid() const
    {
        return this->guid;
    };
private:
    Util::Guid guid;
public:
    void SetEntityExists(bool val)
    {
        n_assert(!this->handled);
        this->entityexists = val;
    };
    bool GetEntityExists() const
    {
        return this->entityexists;
    };
private:
    bool entityexists;
public:
    void Encode(const Ptr<IO::BinaryWriter>& writer)
    {
        writer->WriteGuid(this->GetGuid());
        writer->WriteBool(this->GetEntityExists());
    };
public:
    void Decode(const Ptr<IO::BinaryReader>& reader)
    {
        this->SetGuid(reader->ReadGuid());
        this->SetEntityExists(reader->ReadBool());
    };
};
//------------------------------------------------------------------------------
class GuidNetIdTable : public Messaging::Message
{
    __DeclareClass(GuidNetIdTable);
    __DeclareMsgId;
public:
    GuidNetIdTable() 
    { };
public:
    void SetIdTable(const Util::Blob& val)
    {
        n_assert(!this->handled);
        this->idtable = val;
    };
    const Util::Blob& GetIdTable() const
    {
        return this->idtable;
    };
private:
    Util::Blob idtable;
public:
    void Encode(const Ptr<IO::BinaryWriter>& writer)
    {
        writer->WriteBlob(this->GetIdTable());
    };
public:
    void Decode(const Ptr<IO::BinaryReader>& reader)
    {
        this->SetIdTable(reader->ReadBlob());
    };
};
//------------------------------------------------------------------------------
class SetTransformOfEntity : public Messaging::Message
{
    __DeclareClass(SetTransformOfEntity);
    __DeclareMsgId;
public:
    SetTransformOfEntity() 
    { };
public:
    void SetMatrix(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->matrix = val;
    };
    const Math::matrix44& GetMatrix() const
    {
        return this->matrix;
    };
private:
    Math::matrix44 matrix;
public:
    void SetGuid(const Util::Guid& val)
    {
        n_assert(!this->handled);
        this->guid = val;
    };
    const Util::Guid& GetGuid() const
    {
        return this->guid;
    };
private:
    Util::Guid guid;
public:
    void Encode(const Ptr<IO::BinaryWriter>& writer)
    {
        writer->WriteMatrix44(this->GetMatrix());
        writer->WriteGuid(this->GetGuid());
    };
public:
    void Decode(const Ptr<IO::BinaryReader>& reader)
    {
        this->SetMatrix(reader->ReadMatrix44());
        this->SetGuid(reader->ReadGuid());
    };
};
//------------------------------------------------------------------------------
class UpdateTransformOfEntity : public Messaging::Message
{
    __DeclareClass(UpdateTransformOfEntity);
    __DeclareMsgId;
public:
    UpdateTransformOfEntity() 
    { };
public:
    void SetMatrix(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->matrix = val;
    };
    const Math::matrix44& GetMatrix() const
    {
        return this->matrix;
    };
private:
    Math::matrix44 matrix;
public:
    void SetGuid(const Util::Guid& val)
    {
        n_assert(!this->handled);
        this->guid = val;
    };
    const Util::Guid& GetGuid() const
    {
        return this->guid;
    };
private:
    Util::Guid guid;
public:
    void Encode(const Ptr<IO::BinaryWriter>& writer)
    {
        writer->WriteMatrix44(this->GetMatrix());
        writer->WriteGuid(this->GetGuid());
    };
public:
    void Decode(const Ptr<IO::BinaryReader>& reader)
    {
        this->SetMatrix(reader->ReadMatrix44());
        this->SetGuid(reader->ReadGuid());
    };
};
//------------------------------------------------------------------------------
class CreateClientSideEntity : public Messaging::Message
{
    __DeclareClass(CreateClientSideEntity);
    __DeclareMsgId;
public:
    CreateClientSideEntity() 
    { };
public:
    void SetCategory(const Util::String& val)
    {
        n_assert(!this->handled);
        this->category = val;
    };
    const Util::String& GetCategory() const
    {
        return this->category;
    };
private:
    Util::String category;
public:
    void SetTemplateName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->templatename = val;
    };
    const Util::String& GetTemplateName() const
    {
        return this->templatename;
    };
private:
    Util::String templatename;
public:
    void SetGuid(const Util::Guid& val)
    {
        n_assert(!this->handled);
        this->guid = val;
    };
    const Util::Guid& GetGuid() const
    {
        return this->guid;
    };
private:
    Util::Guid guid;
public:
    void SetMatrix(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->matrix = val;
    };
    const Math::matrix44& GetMatrix() const
    {
        return this->matrix;
    };
private:
    Math::matrix44 matrix;
public:
    void Encode(const Ptr<IO::BinaryWriter>& writer)
    {
        writer->WriteString(this->GetCategory());
        writer->WriteString(this->GetTemplateName());
        writer->WriteGuid(this->GetGuid());
        writer->WriteMatrix44(this->GetMatrix());
    };
public:
    void Decode(const Ptr<IO::BinaryReader>& reader)
    {
        this->SetCategory(reader->ReadString());
        this->SetTemplateName(reader->ReadString());
        this->SetGuid(reader->ReadGuid());
        this->SetMatrix(reader->ReadMatrix44());
    };
};
//------------------------------------------------------------------------------
class DeleteClientSideEntity : public Messaging::Message
{
    __DeclareClass(DeleteClientSideEntity);
    __DeclareMsgId;
public:
    DeleteClientSideEntity() 
    { };
public:
    void SetGuid(const Util::Guid& val)
    {
        n_assert(!this->handled);
        this->guid = val;
    };
    const Util::Guid& GetGuid() const
    {
        return this->guid;
    };
private:
    Util::Guid guid;
public:
    void Encode(const Ptr<IO::BinaryWriter>& writer)
    {
        writer->WriteGuid(this->GetGuid());
    };
public:
    void Decode(const Ptr<IO::BinaryReader>& reader)
    {
        this->SetGuid(reader->ReadGuid());
    };
};
} // namespace Multiplayer
//------------------------------------------------------------------------------
