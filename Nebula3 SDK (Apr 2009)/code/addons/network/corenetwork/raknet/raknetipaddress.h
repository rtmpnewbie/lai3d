#pragma once
#ifndef RAKNET_RAKNETIPADDRESS_H
#define RAKNET_RAKNETIPADDRESS_H
#include "core/refcounted.h"
#include "extlibs/raknet/source/raknettypes.h"
#include "util/string.h"

namespace RakNet
{
    class RakNetIpAddress : public Core::RefCounted
{
public:
    /// constructor
    RakNetIpAddress();
    /// destructor
    ~RakNetIpAddress();
    /// get ip as string
    Util::String AsString() const;
    /// from string
    void FromString(const Util::String& str);
    /// get raknet system address
    const SystemAddress& GetSystemAddress() const;
    /// set system address
    void SetSystemAddress(const SystemAddress& systemAddress);
    /// equal operator
    bool operator== (const RakNetIpAddress& other) const;
    /// unequal operator
    bool operator!= (const RakNetIpAddress& other) const;
    /// greater operator
    bool operator> (const RakNetIpAddress& other) const;
    /// less operator
    bool operator< (const RakNetIpAddress& other) const;
    /// as uint
    uint AsUint() const;

private:
    SystemAddress address;
};
//------------------------------------------------------------------------------
/**
*/
inline
Util::String 
RakNetIpAddress::AsString() const
{
    Util::String str(this->address.ToString(true));

    return str;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
RakNetIpAddress::FromString(const Util::String& str)
{
    Util::Array<Util::String> tokens = str.Tokenize(":");    
    n_assert(tokens.Size() == 2);
    this->address.SetBinaryAddress(tokens[0].AsCharPtr());
    this->address.port = tokens[1].AsInt();
}

//------------------------------------------------------------------------------
/**
*/
inline const SystemAddress& 
RakNetIpAddress::GetSystemAddress() const
{
    return this->address;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
RakNetIpAddress::SetSystemAddress(const SystemAddress& systemAddress)
{
    this->address = systemAddress;
}

//------------------------------------------------------------------------------
/**
*/
inline uint 
RakNetIpAddress::AsUint() const
{
    return this->address.binaryAddress;
}
}
#endif // RAKNET_RAKNETIPADDRESS_H