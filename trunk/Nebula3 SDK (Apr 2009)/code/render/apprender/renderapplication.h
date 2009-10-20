#pragma once
//------------------------------------------------------------------------------
/** 
    @class App::RenderApplication
    
    Render application class with multithreaded rendering.
    
    (C) 2008 Radon Labs GmbH
*/
#include "app/application.h"
#include "core/coreserver.h"
#include "debug/debuginterface.h"
#include "io/ioserver.h"
#include "io/iointerface.h"
#include "graphics/graphicsinterface.h"
#include "graphics/display.h"
#include "timing/timer.h"
#include "debug/debuginterface.h"
#include "debug/debugtimer.h"
#include "http/httpinterface.h"
#include "http/httpserverproxy.h"
#include "input/inputserver.h"
#include "timing/mastertime.h"

//------------------------------------------------------------------------------
namespace App
{
class RenderApplication : public Application
{
public:
    /// constructor
    RenderApplication();
    /// destructor
    virtual ~RenderApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();
    /// run the application
    virtual void Run();

private:
    /// update the current time values
    void UpdateTime();

protected:
    /// lookup the toolkit project key in the registry (Win32 only!)
    Util::String LookupProjectDirectory();
    /// set quit requested flag
    void SetQuitRequested(bool b);
    /// return true if quit requested
    bool IsQuitRequested() const;
    /// called to configure display
    virtual void OnConfigureDisplay();
    /// process input (called before rendering)
    virtual void OnProcessInput();
    /// update world 
    virtual void OnUpdateFrame();
    /// get current absolute time
    Timing::Time GetTime() const;
    /// get current frame time
    Timing::Time GetFrameTime() const;

    Ptr<Core::CoreServer> coreServer;
    Ptr<Debug::DebugInterface> debugInterface;
    Ptr<IO::IoServer> ioServer;
    Ptr<IO::IoInterface> ioInterface;
    Ptr<Graphics::GraphicsInterface> graphicsInterface;
    Ptr<Graphics::Display> display;    
    Ptr<Http::HttpInterface> httpInterface;
    Ptr<Http::HttpServerProxy> httpServerProxy;
    Ptr<Input::InputServer> inputServer;   
    Ptr<Timing::MasterTime> masterTimer;            

    Timing::Time time;
    Timing::Time frameTime;
    bool quitRequested;

    _declare_timer(MainThreadFrameTimeAll);
    _declare_timer(MainThreadWaitForGraphicsFrame);
};

//------------------------------------------------------------------------------
/**
*/
inline void
RenderApplication::SetQuitRequested(bool b)
{
    this->quitRequested = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
RenderApplication::IsQuitRequested() const
{
    return this->quitRequested;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
RenderApplication::GetTime() const
{
    return this->time;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
RenderApplication::GetFrameTime() const
{
    return this->frameTime;
}

} // namespace App
//------------------------------------------------------------------------------

