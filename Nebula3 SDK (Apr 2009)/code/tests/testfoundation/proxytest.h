#pragma once
#ifndef TEST_PROXYTEST_H
#define TEST_PROXYTEST_H
//------------------------------------------------------------------------------
/**
    @class Test::ProxyTest
    
    Test Util::Proxy functionality.
    
    (C) 2007 Radon Labs GmbH
*/
#include "testbase/testcase.h"

//------------------------------------------------------------------------------
namespace Test
{
class ProxyTest : public TestCase
{
    __DeclareClass(ProxyTest);
public:
    /// run the test
    virtual void Run();
};

}; // namespace Test
//------------------------------------------------------------------------------
#endif        
    