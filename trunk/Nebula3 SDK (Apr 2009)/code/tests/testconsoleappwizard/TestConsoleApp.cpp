//------------------------------------------------------------------------------
//  TestConsoleApp.cpp
//  (C) 2009 CgPanda
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "TestConsoleApp.h"

namespace Test
{
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
TestConsoleApp::TestConsoleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
TestConsoleApp::~TestConsoleApp()
{
	if (this->IsOpen())
	{
		this->Close();
	}
}

//------------------------------------------------------------------------------
/**
*/
bool
TestConsoleApp::Open()
{
	n_assert(!this->IsOpen());
	if (ConsoleApplication::Open())
	{
		// TODO: add your setup code here
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void
TestConsoleApp::Close()
{
	n_assert(this->IsOpen());
	// TODO: add your discard code here
	ConsoleApplication::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
TestConsoleApp::Run()
{
	bool running = true;
	while (running)
	{
		n_printf("Hello World!\n");
	}
}

} // namespace Test
