#pragma once
#ifndef ANIMATION_ANIMMAPPER_H
#define ANIMATION_ANIMMAPPER_H
//------------------------------------------------------------------------------
/**
    @class Animation::AnimMapper

    The task of AnimMapper objects is to find a clip index given a 
    set of input arguments by resolving a mapping condition tree
    the condition tree is defined in an XML input file.
    
    (C) 2008 Radon Labs GmbH
*/
#include "resources/resource.h"

//------------------------------------------------------------------------------
namespace Animation
{
class AnimMapper : public Core::RefCounted
{
    __DeclareClass(AnimMapper);
public:
    /// constructor
    AnimMapper();
    /// destructor
    virtual AnimMapper();
    
    /// unload the resource, or cancel the pending load
    virtual void Unload();

    /// return true if an attribute is used by the mapper
    bool HasAttr(const Attr::AttrId& attrId) const;
    /// generic attribute setter
    void SetAttr(const Attr::Attribute& attr);
    /// get generic attribute
    const Attr::Attribute& GetAttr(const Attr::AttrId& attrId) const;
    /// set a string input parameter
    void SetString(const Attr::StringAttrId& attrId, const Util::String& s);
    /// get a string parameter
    const Util::String& GetString(const Attr::StringAttrId& attrId) const;
    /// set a float input parameter
    void SetFloat(const Attr::FloatAttrId& attrId, float f);
    /// get a float input parameter
    float GetFloat(const Attr::FloatAttrId& attrId) const;
    /// set an int input parameter
    void SetInt(const Attr::IntAttrId& attrId, int f);
    /// get an int input parameter
    int GetInt(const Attr::IntAttrId& attrId) const;
    /// set a bool input parameter
    void SetBool(const Attr::BoolAttrId& attrId, bool b);
    /// get a bool input parameter
    bool GetBool(const Attr::BoolAttrId& attrId) const;

private:
    // FIXME: need to encode condition tree here
};

} // namespace Animation
//------------------------------------------------------------------------------
#endif

    