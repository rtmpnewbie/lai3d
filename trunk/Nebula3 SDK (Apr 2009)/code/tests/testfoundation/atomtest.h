#pragma once
#ifndef TEST_ATOMTEST_H
#define TEST_ATOMTEST_H
//------------------------------------------------------------------------------
/**
    @class Test::AtomTest
    
    Test Util::Atom functionality.
    
    (C) 2007 Radon Labs GmbH
*/
#include "testbase/testcase.h"

//------------------------------------------------------------------------------
namespace Test
{
class AtomTest : public TestCase
{
    __DeclareClass(AtomTest);
public:
    /// run the test
    virtual void Run();
};

}; // namespace Test
//------------------------------------------------------------------------------
#endif        
        