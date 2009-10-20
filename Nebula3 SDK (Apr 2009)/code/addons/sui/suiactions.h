#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "util/stringatom.h"
#include "math/float2.h"

//------------------------------------------------------------------------------
namespace SUI
{
//------------------------------------------------------------------------------
class SUIAction : public Messaging::Message
{
    __DeclareClass(SUIAction);
    __DeclareMsgId;
public:
    SUIAction() 
    { };
public:
    void SetLayoutId(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->layoutid = val;
    };
    const Util::StringAtom& GetLayoutId() const
    {
        return this->layoutid;
    };
private:
    Util::StringAtom layoutid;
public:
    void SetElementId(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->elementid = val;
    };
    const Util::StringAtom& GetElementId() const
    {
        return this->elementid;
    };
private:
    Util::StringAtom elementid;
};
//------------------------------------------------------------------------------
class SUIActionSetVisible : public SUIAction
{
    __DeclareClass(SUIActionSetVisible);
    __DeclareMsgId;
public:
    SUIActionSetVisible() 
    { };
public:
    void SetVisible(bool val)
    {
        n_assert(!this->handled);
        this->visible = val;
    };
    bool GetVisible() const
    {
        return this->visible;
    };
private:
    bool visible;
};
//------------------------------------------------------------------------------
class SUIActionSetSize : public SUIAction
{
    __DeclareClass(SUIActionSetSize);
    __DeclareMsgId;
public:
    SUIActionSetSize() 
    { };
public:
    void SetSize(const Math::float2& val)
    {
        n_assert(!this->handled);
        this->size = val;
    };
    const Math::float2& GetSize() const
    {
        return this->size;
    };
private:
    Math::float2 size;
};
//------------------------------------------------------------------------------
class SUIActionPlayLayoutAnim : public SUIAction
{
    __DeclareClass(SUIActionPlayLayoutAnim);
    __DeclareMsgId;
public:
    SUIActionPlayLayoutAnim() :
        resettime(true)
    { };
public:
    void SetAnim(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->anim = val;
    };
    const Util::StringAtom& GetAnim() const
    {
        return this->anim;
    };
private:
    Util::StringAtom anim;
public:
    void SetResetTime(bool val)
    {
        n_assert(!this->handled);
        this->resettime = val;
    };
    bool GetResetTime() const
    {
        return this->resettime;
    };
private:
    bool resettime;
};
//------------------------------------------------------------------------------
class SUIActionPlayElementAnim : public SUIAction
{
    __DeclareClass(SUIActionPlayElementAnim);
    __DeclareMsgId;
public:
    SUIActionPlayElementAnim() :
        resettime(true)
    { };
public:
    void SetAnim(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->anim = val;
    };
    const Util::StringAtom& GetAnim() const
    {
        return this->anim;
    };
private:
    Util::StringAtom anim;
public:
    void SetResetTime(bool val)
    {
        n_assert(!this->handled);
        this->resettime = val;
    };
    bool GetResetTime() const
    {
        return this->resettime;
    };
private:
    bool resettime;
};
} // namespace SUI
//------------------------------------------------------------------------------
