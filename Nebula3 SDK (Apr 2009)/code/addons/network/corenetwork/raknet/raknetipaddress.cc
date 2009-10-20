#include "stdneb.h"
#include "corenetwork/raknet/raknetipaddress.h"

namespace RakNet
{
//------------------------------------------------------------------------------
/**
*/
RakNetIpAddress::RakNetIpAddress()
{
}

//------------------------------------------------------------------------------
/**
*/
RakNetIpAddress::~RakNetIpAddress()
{
}

//------------------------------------------------------------------------------
/**
*/
bool 
RakNetIpAddress::operator==(const RakNetIpAddress& other) const
{
    return this->address == other.address;
}

//------------------------------------------------------------------------------
/**
*/
bool 
RakNetIpAddress::operator!=(const RakNetIpAddress& other) const
{
    return this->address != other.address;
}

//------------------------------------------------------------------------------
/**
*/
bool 
RakNetIpAddress::operator>(const RakNetIpAddress& other) const
{
    return this->address > other.address;
}

//------------------------------------------------------------------------------
/**
*/
bool 
RakNetIpAddress::operator<(const RakNetIpAddress& other) const
{
    return this->address < other.address;
}
}