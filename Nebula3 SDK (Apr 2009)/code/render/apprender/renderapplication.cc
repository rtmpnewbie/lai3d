//------------------------------------------------------------------------------
//  renderapplication.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "render_classregistry.h"
#include "apprender/renderapplication.h"
#include "io/logfileconsolehandler.h"
#include "memory/debug/memorypagehandler.h"
#include "core/debug/corepagehandler.h"
#include "io/debug/iopagehandler.h"
#include "http/debug/svgtestpagehandler.h"
#include "http/debug/helloworldrequesthandler.h"
#include "threading/debug/threadpagehandler.h"
#include "io/debug/consolepagehandler.h"
#include "timing/mastertime.h"
#include "timing/timeprotocol.h"

#if __WIN32__
#include "system/win32/win32registry.h"
#endif

namespace App
{
using namespace Core;
using namespace Debug;
using namespace IO;
using namespace Graphics;
using namespace Input;
using namespace CoreGraphics;
using namespace Timing;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
RenderApplication::RenderApplication() :
    //time(0.0),
    //frameTime(0.0),
    quitRequested(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
RenderApplication::~RenderApplication()
{
    n_assert(!this->IsOpen());
}

//------------------------------------------------------------------------------
/**
*/
String
RenderApplication::LookupProjectDirectory()
{
    String projectDir = "home:";
    #if __WIN32__
        if (System::Win32Registry::Exists(System::Win32Registry::CurrentUser, "Software\\RadonLabs\\Toolkit", "project"))
        {
            projectDir = System::Win32Registry::ReadString(System::Win32Registry::CurrentUser, "Software\\RadonLabs\\Toolkit", "project");
        }
    #endif
    return projectDir;
}

//------------------------------------------------------------------------------
/**
*/
bool
RenderApplication::Open()
{
    n_assert(!this->IsOpen());
    if (Application::Open())
    {
        // check if a toolkit project key is set, if yes, use this as root directory
        String projDir = this->LookupProjectDirectory();
            
        // setup core subsystem
        this->coreServer = CoreServer::Create();
        this->coreServer->SetCompanyName(this->companyName);
        this->coreServer->SetAppName(this->appName);
        this->coreServer->SetRootDirectory(projDir);
        this->coreServer->Open();

        // create mastertimer
        this->masterTimer = Timing::MasterTime::Create();
        this->masterTimer->Start();

        // setup io subsystem
        this->ioServer = IoServer::Create();
        this->ioServer->MountStandardArchives();
        this->ioInterface = IoInterface::Create();
        this->ioInterface->Open();

        // attach a log file console handler
        #if __WIN32__
        Ptr<LogFileConsoleHandler> logFileHandler = LogFileConsoleHandler::Create();
        Console::Instance()->AttachHandler(logFileHandler.upcast<ConsoleHandler>());
        #endif

        // setup http subsystem
        this->httpInterface = Http::HttpInterface::Create();
        this->httpInterface->Open();
        this->httpServerProxy = Http::HttpServerProxy::Create();
        this->httpServerProxy->Open();
        this->httpServerProxy->AttachRequestHandler(Debug::CorePageHandler::Create());
        this->httpServerProxy->AttachRequestHandler(Debug::ThreadPageHandler::Create());
        this->httpServerProxy->AttachRequestHandler(Debug::MemoryPageHandler::Create());
        this->httpServerProxy->AttachRequestHandler(Debug::ConsolePageHandler::Create());
        this->httpServerProxy->AttachRequestHandler(Debug::IoPageHandler::Create());
        this->httpServerProxy->AttachRequestHandler(Debug::SvgTestPageHandler::Create());
        this->httpServerProxy->AttachRequestHandler(Debug::HelloWorldRequestHandler::Create());

        // setup debug subsystem
        this->debugInterface = DebugInterface::Create();
        this->debugInterface->Open();

        // setup asynchronous graphics subsystem
        this->graphicsInterface = GraphicsInterface::Create();
        this->graphicsInterface->Open();
        this->display = Display::Create();
        this->OnConfigureDisplay();
        this->display->Open();

        // setup input subsystem
        this->inputServer = InputServer::Create();
        this->inputServer->Open();

        // setup debug timers and counters
        _setup_timer(MainThreadFrameTimeAll);
        _setup_timer(MainThreadWaitForGraphicsFrame);

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::OnConfigureDisplay()
{
    // display adapter
    Adapter::Code adapter = Adapter::Primary;
    if (this->args.HasArg("-adapter"))
    {
        adapter = Adapter::FromString(this->args.GetString("-adapter"));
        if (this->display->AdapterExists(adapter))
        {
            this->display->SetAdapter(adapter);
        }
    }

    // display mode
    DisplayMode displayMode;
    if (this->args.HasArg("-x"))
    {
        displayMode.SetXPos(this->args.GetInt("-x"));
    }
    if (this->args.HasArg("-y"))
    {
        displayMode.SetYPos(this->args.GetInt("-y"));
    }
    if (this->args.HasArg("-w"))
    {
        displayMode.SetWidth(this->args.GetInt("-w"));
    }
    if (this->args.HasArg("-h"))
    {
        displayMode.SetHeight(this->args.GetInt("-h"));
    }
    displayMode.SetAspectRatio(float(displayMode.GetWidth()) / float(displayMode.GetHeight()));
    this->display->SetDisplayMode(displayMode);
    this->display->SetFullscreen(this->args.GetBoolFlag("-fullscreen"));
    this->display->SetAlwaysOnTop(this->args.GetBoolFlag("-alwaysontop"));
    this->display->SetVerticalSyncEnabled(this->args.GetBoolFlag("-vsync"));
    if (this->args.HasArg("-aa"))
    {
        this->display->SetAntiAliasQuality(AntiAliasQuality::FromString(this->args.GetString("-aa")));
    }
    #if __XBOX360__
        this->display->SetAntiAliasQuality(AntiAliasQuality::Medium);
    #endif
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::Close()
{
    n_assert(this->IsOpen());
    
    _discard_timer(MainThreadFrameTimeAll);
    _discard_timer(MainThreadWaitForGraphicsFrame);

    this->inputServer->Close();
    this->inputServer = 0;

    this->display->Close();
    this->display = 0;

    this->graphicsInterface->Close();
    this->graphicsInterface = 0;

    this->debugInterface->Close();
    this->debugInterface = 0;

    this->httpServerProxy->Close();
    this->httpServerProxy = 0;
    this->httpInterface->Close();
    this->httpInterface = 0;

    this->ioInterface->Close();
    this->ioInterface = 0;
    this->ioServer = 0;

    this->masterTimer->Stop();
    this->masterTimer = 0;

    this->coreServer->Close();
    this->coreServer = 0;

    Application::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::Run()
{
    n_assert(this->isOpen);
    while (!(this->inputServer->IsQuitRequested() || this->IsQuitRequested()))
    {
        _start_timer(MainThreadFrameTimeAll);

        // synchronize with the graphics frame, to prevent the game thread
        // to run ahead of the graphics thread
        _start_timer(MainThreadWaitForGraphicsFrame);
        GraphicsInterface::Instance()->WaitForFrameEvent();
        _stop_timer(MainThreadWaitForGraphicsFrame);

        // handle any http requests from the HttpServer thread
        this->httpServerProxy->HandlePendingRequests();

        // process input
        this->inputServer->BeginFrame();
        this->inputServer->OnFrame();
        this->OnProcessInput();

        // update time
        this->UpdateTime();

        // run "game logic"
        this->OnUpdateFrame();

        // send batched graphics messages
        GraphicsInterface::Instance()->FlushBatchedMessages();

        this->inputServer->EndFrame();

        _stop_timer(MainThreadFrameTimeAll);

        if (!this->display->IsFullscreen())
        {
            Core::SysFunc::Sleep(0.0);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::OnProcessInput()
{
    // empty, override this method in a subclass
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::OnUpdateFrame()
{
    // empty, override this method in a subclass
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::UpdateTime()
{    
    MasterTime* masterTime = MasterTime::Instance();
    masterTime->Update();
    
    Time curTime = masterTime->GetTime();
    this->frameTime = curTime - this->time;
    this->time = curTime;

    // at first send time sync to render thread
    Ptr<Timing::SyncTime> sync = Timing::SyncTime::Create();    
    Util::Dictionary<Timing::TimingType::Code, Timing::Time> times;
    times.BeginBulkAdd();
    times.Add(Timing::TimingType::MasterTime, curTime);
    times.Add(Timing::TimingType::ScaledTime, curTime);
    times.EndBulkAdd();
    sync->SetTimes(times);
    Graphics::GraphicsInterface::Instance()->SendBatched(sync.cast<Messaging::Message>());

}

} // namespace App
