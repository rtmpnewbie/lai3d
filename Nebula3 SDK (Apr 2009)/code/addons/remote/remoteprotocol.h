#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "remote/remoterequesthandler.h"

//------------------------------------------------------------------------------
namespace Remote
{
//------------------------------------------------------------------------------
class AttachRequestHandler : public Messaging::Message
{
    __DeclareClass(AttachRequestHandler);
    __DeclareMsgId;
public:
    AttachRequestHandler() 
    { };
public:
    void SetRequestHandler(const Ptr<Remote::RemoteRequestHandler>& val)
    {
        n_assert(!this->handled);
        this->requesthandler = val;
    };
    const Ptr<Remote::RemoteRequestHandler>& GetRequestHandler() const
    {
        return this->requesthandler;
    };
private:
    Ptr<Remote::RemoteRequestHandler> requesthandler;
};
//------------------------------------------------------------------------------
class RemoveRequestHandler : public Messaging::Message
{
    __DeclareClass(RemoveRequestHandler);
    __DeclareMsgId;
public:
    RemoveRequestHandler() 
    { };
public:
    void SetRequestHandler(const Ptr<Remote::RemoteRequestHandler>& val)
    {
        n_assert(!this->handled);
        this->requesthandler = val;
    };
    const Ptr<Remote::RemoteRequestHandler>& GetRequestHandler() const
    {
        return this->requesthandler;
    };
private:
    Ptr<Remote::RemoteRequestHandler> requesthandler;
};
} // namespace Remote
//------------------------------------------------------------------------------
