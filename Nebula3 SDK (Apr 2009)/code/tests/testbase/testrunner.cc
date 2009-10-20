//------------------------------------------------------------------------------
//  testrunner.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "testrunner.h"

namespace Test
{
__ImplementClass(Test::TestRunner, 'TSTR', Core::RefCounted);

using namespace Core;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
TestRunner::AttachTestCase(TestCase* testCase)
{
    n_assert(testCase);
    this->testCases.Append(testCase);
}

//------------------------------------------------------------------------------
/**
*/
void
TestRunner::Run()
{
    IndexT i;
    int numSucceeded = 0;
    int numFailed = 0;
    for (i = 0; i < this->testCases.Size(); i++)
    {
        TestCase* curTestCase = this->testCases[i];
        n_printf("-> Running test: %s\n", curTestCase->GetClassName().AsCharPtr());
        curTestCase->Run();
        if (curTestCase->GetNumFailed() == 0)
        {
            n_printf("-> SUCCESS: %s runs %d tests ok!\n", curTestCase->GetClassName().AsCharPtr(), curTestCase->GetNumSucceeded());
        }
        else
        {
            n_printf("-> FAILURE: %d of %d tests failed in %s!\n", 
                curTestCase->GetNumFailed(),
                curTestCase->GetNumVerified(),
                curTestCase->GetClassName().AsCharPtr());
        }
        numFailed += curTestCase->GetNumFailed();
        numSucceeded += curTestCase->GetNumSucceeded();
        n_printf("\n");
    }
    n_printf("* TEST RESULT: %d succeeded, %d failed!\n", numSucceeded, numFailed);
}

}; // namespace Test
