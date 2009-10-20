#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "util/string.h"
#include "util/array.h"
#include "threading/threadid.h"
#include "coreui/uielement.h"

//------------------------------------------------------------------------------
namespace CoreUI
{
//------------------------------------------------------------------------------
class RenderUIElements : public Messaging::Message
{
    __DeclareClass(RenderUIElements);
    __DeclareMsgId;
public:
    RenderUIElements() 
    { };
public:
    void SetThreadId(const Threading::ThreadId& val)
    {
        n_assert(!this->handled);
        this->threadid = val;
    };
    const Threading::ThreadId& GetThreadId() const
    {
        return this->threadid;
    };
private:
    Threading::ThreadId threadid;
public:
    void SetUIElements(const Util::Array<CoreUI::UIElement>& val)
    {
        n_assert(!this->handled);
        this->uielements = val;
    };
    const Util::Array<CoreUI::UIElement>& GetUIElements() const
    {
        return this->uielements;
    };
private:
    Util::Array<CoreUI::UIElement> uielements;
};
//------------------------------------------------------------------------------
class LoadUITexture : public Messaging::Message
{
    __DeclareClass(LoadUITexture);
    __DeclareMsgId;
public:
    LoadUITexture() 
    { };
public:
    void SetResourceName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->resourcename = val;
    };
    const Util::String& GetResourceName() const
    {
        return this->resourcename;
    };
private:
    Util::String resourcename;
};
//------------------------------------------------------------------------------
class FlushAllUIElements : public Messaging::Message
{
    __DeclareClass(FlushAllUIElements);
    __DeclareMsgId;
public:
    FlushAllUIElements() 
    { };
public:
    void SetThreadId(const Threading::ThreadId& val)
    {
        n_assert(!this->handled);
        this->threadid = val;
    };
    const Threading::ThreadId& GetThreadId() const
    {
        return this->threadid;
    };
private:
    Threading::ThreadId threadid;
};
//------------------------------------------------------------------------------
class FlushUIElement : public Messaging::Message
{
    __DeclareClass(FlushUIElement);
    __DeclareMsgId;
public:
    FlushUIElement() 
    { };
public:
    void SetThreadId(const Threading::ThreadId& val)
    {
        n_assert(!this->handled);
        this->threadid = val;
    };
    const Threading::ThreadId& GetThreadId() const
    {
        return this->threadid;
    };
private:
    Threading::ThreadId threadid;
public:
    void SetUIElement(const CoreUI::UIElement& val)
    {
        n_assert(!this->handled);
        this->uielement = val;
    };
    const CoreUI::UIElement& GetUIElement() const
    {
        return this->uielement;
    };
private:
    CoreUI::UIElement uielement;
};
//------------------------------------------------------------------------------
class UpdateUIElement : public Messaging::Message
{
    __DeclareClass(UpdateUIElement);
    __DeclareMsgId;
public:
    UpdateUIElement() 
    { };
public:
    void SetThreadId(const Threading::ThreadId& val)
    {
        n_assert(!this->handled);
        this->threadid = val;
    };
    const Threading::ThreadId& GetThreadId() const
    {
        return this->threadid;
    };
private:
    Threading::ThreadId threadid;
public:
    void SetUIElements(const Util::Array<CoreUI::UIElement>& val)
    {
        n_assert(!this->handled);
        this->uielements = val;
    };
    const Util::Array<CoreUI::UIElement>& GetUIElements() const
    {
        return this->uielements;
    };
private:
    Util::Array<CoreUI::UIElement> uielements;
};
} // namespace CoreUI
//------------------------------------------------------------------------------
