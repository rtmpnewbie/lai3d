//------------------------------------------------------------------------------
//  viewerapplication.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "apprender/viewerapplication.h"
#include "apprender/platformconfig.h"
#include "graphics/stagebuilders.h"
#include "graphics/view.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/gamepad.h"
#include "internalgraphics/internalview.h"

namespace App
{
using namespace Math;
using namespace Graphics;
using namespace Util;
using namespace Resources;
using namespace Input;
using namespace Debug;

//------------------------------------------------------------------------------
/**
*/
ViewerApplication::ViewerApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ViewerApplication::~ViewerApplication()
{
    n_assert(!this->stage.isvalid());
}

//------------------------------------------------------------------------------
/**
*/
bool
ViewerApplication::Open()
{
    if (RenderApplication::Open())
    {
        StringAtom defaultStageName("DefaultStage");
        StringAtom defaultViewName("DefaultView");

        // setup the camera util object
        this->mayaCameraUtil.Setup(point(0.0f, 0.0f, 0.0f), point(0.0f, 0.0f, 10.0f), vector(0.0f, 1.0f, 0.0f));

        // create a GraphicServer, Stage and View
        this->graphicsServer = GraphicsServer::Create();
        this->graphicsServer->Open();
        this->debugShapeRenderer = DebugShapeRenderer::Create();
        this->debugTextRenderer = DebugTextRenderer::Create();

        // create a default stage
        Attr::AttributeContainer stageBuilderAttrs;
        this->stage = this->graphicsServer->CreateStage(defaultStageName, 
                                                        Graphics::SimpleStageBuilder::RTTI, 
                                                        stageBuilderAttrs);

        // create a default view
        // FIXME: sucks that I have to use InternalGraphics here!
        this->view = this->graphicsServer->CreateView(InternalGraphics::InternalView::RTTI,
                                                      defaultViewName,
                                                      defaultStageName,
                                                      ResourceId(DEFAULT_FRAMESHADER_NAME),
                                                      true);

        // create a camera entity
        this->camera = CameraEntity::Create();
        this->camera->SetTransform(this->mayaCameraUtil.GetCameraTransform());
        this->stage->AttachEntity(this->camera.cast<GraphicsEntity>());
        this->view->SetCameraEntity(this->camera);

        // load any scene objects from cmd line parameters
        this->SetupSceneFromCmdLineArgs();

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
ViewerApplication::Close()
{
    // clear scene entities
    IndexT i;
    for (i = 0; i < this->sceneModelEntities.Size(); ++i)
    {
        this->stage->RemoveEntity(this->sceneModelEntities[i].cast<GraphicsEntity>());
    }
    this->sceneModelEntities.Clear();    

    this->stage->RemoveEntity(this->camera.cast<GraphicsEntity>());
    this->camera = 0;

    this->graphicsServer->DiscardView(this->view);
    this->view = 0;
    
    this->graphicsServer->DiscardStage(this->stage);
    this->stage = 0;

    this->debugTextRenderer = 0;
    this->debugShapeRenderer = 0;
    this->graphicsServer->Close();
    this->graphicsServer = 0;

    // call parent class
    RenderApplication::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
ViewerApplication::OnProcessInput()
{
    #if !__WII__
    
    // update the camera from input
    InputServer* inputServer = InputServer::Instance();
    const Ptr<Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
    const Ptr<Mouse>& mouse = inputServer->GetDefaultMouse();
    const Ptr<GamePad>& gamePad = inputServer->GetDefaultGamePad(0);

    // standard input handling: manipulate camera
    this->mayaCameraUtil.SetOrbitButton(mouse->ButtonPressed(MouseButton::LeftButton));
    this->mayaCameraUtil.SetPanButton(mouse->ButtonPressed(MouseButton::MiddleButton));
    this->mayaCameraUtil.SetZoomButton(mouse->ButtonPressed(MouseButton::RightButton));
    this->mayaCameraUtil.SetZoomInButton(mouse->WheelForward());
    this->mayaCameraUtil.SetZoomOutButton(mouse->WheelBackward());
    this->mayaCameraUtil.SetMouseMovement(mouse->GetMovement());
    
    // process gamepad input
    float zoomIn = 0.0f;
    float zoomOut = 0.0f;
    float2 panning(0.0f, 0.0f);
    float2 orbiting(0.0f, 0.0f);
    if (gamePad->IsConnected())
    {
        const float gamePadZoomSpeed = 50.0f;
        const float gamePadOrbitSpeed = 10.0f;
        const float gamePadPanSpeed = 10.0f;
        if (gamePad->ButtonDown(GamePad::AButton))
        {
            this->mayaCameraUtil.Reset();
        }
        if (gamePad->ButtonDown(GamePad::StartButton) ||
            gamePad->ButtonDown(GamePad::BackButton))
        {
            this->SetQuitRequested(true);
        }
        float frameTime = (float) this->GetFrameTime();
        zoomIn       += gamePad->GetAxisValue(GamePad::RightTriggerAxis) * frameTime * gamePadZoomSpeed;
        zoomOut      += gamePad->GetAxisValue(GamePad::LeftTriggerAxis) * frameTime * gamePadZoomSpeed;
        panning.x()  += gamePad->GetAxisValue(GamePad::RightThumbXAxis) * frameTime * gamePadPanSpeed;
        panning.y()  += gamePad->GetAxisValue(GamePad::RightThumbYAxis) * frameTime * gamePadPanSpeed;
        orbiting.x() += gamePad->GetAxisValue(GamePad::LeftThumbXAxis) * frameTime * gamePadOrbitSpeed;
        orbiting.y() += gamePad->GetAxisValue(GamePad::LeftThumbYAxis) * frameTime * gamePadOrbitSpeed;
    }

    // process keyboard input
    if (keyboard->KeyDown(Key::Escape))
    {
        this->SetQuitRequested(true);
    }
    if (keyboard->KeyDown(Key::Space))
    {
        this->mayaCameraUtil.Reset();
    }
    if (keyboard->KeyPressed(Key::Left))
    {
        panning.x() -= 0.1f;
    }
    if (keyboard->KeyPressed(Key::Right))
    {
        panning.x() += 0.1f;
    }
    if (keyboard->KeyPressed(Key::Up))
    {
        panning.y() -= 0.1f;
    }
    if (keyboard->KeyPressed(Key::Down))
    {
        panning.y() += 0.1f;
    }

    this->mayaCameraUtil.SetPanning(panning);
    this->mayaCameraUtil.SetOrbiting(orbiting);
    this->mayaCameraUtil.SetZoomIn(zoomIn);
    this->mayaCameraUtil.SetZoomOut(zoomOut);
    this->mayaCameraUtil.Update();
    this->camera->SetTransform(this->mayaCameraUtil.GetCameraTransform());
    
    #endif // __WII__
}

//------------------------------------------------------------------------------
/**
*/
void
ViewerApplication::OnUpdateFrame()
{
    this->graphicsServer->OnFrame();
    this->debugShapeRenderer->OnFrame();
    this->debugTextRenderer->OnFrame();
}

//------------------------------------------------------------------------------
/**
*/
void 
ViewerApplication::SetupSceneFromCmdLineArgs()
{
    const Util::CommandLineArgs& args = this->GetCmdLineArgs();

    if (args.HasArg("-view"))
    {
        Util::String sceneFilepath = args.GetString("-view");
        sceneFilepath.StripFileExtension();
        sceneFilepath.Append(".n3");
        // load modelentity
        Ptr<Graphics::ModelEntity> modelEntity = Graphics::ModelEntity::Create();
        modelEntity->SetResourceId(Resources::ResourceId(sceneFilepath));
        modelEntity->SetTransform(matrix44::identity());
        this->stage->AttachEntity(modelEntity.cast<Graphics::GraphicsEntity>());
        this->sceneModelEntities.Append(modelEntity);
    }
}
} // namespace App

    
