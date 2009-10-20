//------------------------------------------------------------------------------
//  renderconsolehandler.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "debugrender/renderconsolehandler.h"
#include "core/coreserver.h"
#include "core/sysfunc.h"
#include "threading/thread.h"
#include "debugrender/debugrender.h"

#if !__WII__
#include "input/keyboard.h"
#include "input/inputserver.h"
#endif

namespace Debug
{
__ImplementClass(Debug::RenderConsoleHandler, 'DREC', IO::ConsoleHandler);

using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
RenderConsoleHandler::RenderConsoleHandler():
bufferSize(100),
linesVisible(12),
curIndex(0)
{    
}

//------------------------------------------------------------------------------
/**
*/
void
RenderConsoleHandler::Print(const String& s)
{
    if (this->linebuffer.Size() == this->bufferSize)
    {
        // remove oldest element
        this->linebuffer.RemoveFront();
    }
    const char* threadName = Threading::Thread::GetMyThreadName();
    String msg;
    if (0 == threadName)
    {
        // add message from the main thread to linebuffer
        this->linebuffer.AddBack(s);
    }
    else
    {
        this->linebuffer.AddBack(s);
    }
    this->curIndex = (int)this->linebuffer.Size() - (int)this->linesVisible;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderConsoleHandler::DebugOut(const String& s)
{
    if (this->linebuffer.Size() == this->bufferSize)
    {
        // remove oldest element
        this->linebuffer.RemoveFront();
    }
    const char* threadName = Threading::Thread::GetMyThreadName();
    String msg;
    if (0 == threadName)
    {
        // a message from the main thread
        this->linebuffer.AddBack(s);
    }
    else
    {
        this->linebuffer.AddBack(s);
    }
    this->curIndex = (int)this->linebuffer.Size() - (int)this->linesVisible;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderConsoleHandler::Error(const String& msg)
{
    if (this->linebuffer.Size() == this->bufferSize)
    {
        // remove oldest element
        this->linebuffer.RemoveFront();
    }

    const char* threadName = Threading::Thread::GetMyThreadName();
    const char* appName = "???";
    if (0 == threadName)
    {
        threadName = "Main";
    }
    if (Core::CoreServer::HasInstance())
    {
        appName = Core::CoreServer::Instance()->GetAppName().Value().AsCharPtr();
    }
    String str;
    str.Format("*** ERROR ***\nApplication: %s\nThread: %s\nError: %s", appName, threadName, msg.AsCharPtr());
    this->linebuffer.AddBack(str);
    Core::SysFunc::Error(str.AsCharPtr());
    this->curIndex = (int)this->linebuffer.Size() - (int)this->linesVisible;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderConsoleHandler::Warning(const String& s)
{
    if (this->linebuffer.Size() == this->bufferSize)
    {
        // remove oldest element
        this->linebuffer.RemoveFront();
    }

    const char* threadName = Threading::Thread::GetMyThreadName();
    String msg;
    if (0 == threadName)
    {
        // a message from the main thread
        this->linebuffer.AddBack(s);
    }
    else
    {
        // a message from another thread, add the thread name to the message
        this->linebuffer.AddBack(s);
    }
    this->curIndex = (int)this->linebuffer.Size() - (int)this->linesVisible;
}

//------------------------------------------------------------------------------
/**
*/
void 
RenderConsoleHandler::Update()
{
    #if !__WII__  
    if (Input::InputServer::HasInstance())
    {
        // check keyboard up and down keys  
        Ptr<Input::InputServer> inputServer = Input::InputServer::Instance();
        Ptr<Input::Keyboard> keyboard = inputServer->GetDefaultKeyboard();
        if(keyboard->KeyUp(Input::Key::Down))
        {
            this->curIndex++;
            this->curIndex = n_min((int)this->linebuffer.Size() - (int)this->linesVisible, this->curIndex);
        }
        else if(keyboard->KeyUp(Input::Key::Up))
        {
            this->curIndex--;
            this->curIndex = n_max(0, this->curIndex);
        }

        // print all lines on screen
        const Math::float4 color(1,1,1,1);
        Math::float2 pos(0,0);    
        List<String>::Iterator listIter = this->linebuffer.Begin();
        IndexT i = 0;
        while (listIter && i < this->curIndex)
        {
            i++;
            listIter++;
        }
        IndexT counter = 0;
        for (listIter; listIter && counter < this->linesVisible; listIter++, counter++)
        {   
            _debug_text(*listIter, pos, color);
            // count linebreaks
            Array<String> numlineBreaks = listIter->Tokenize("\n");
            pos.y() += (numlineBreaks.Size()) * 0.025f;
        }
    }
    #endif
}

} // namespace Debug