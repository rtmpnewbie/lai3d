//------------------------------------------------------------------------------
//  testviewerapplication.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testviewer/testviewerapplication.h"
#include "debugrender/debugrender.h"
#include "debugrender/debugshaperenderer.h"
#include "math/quaternion.h"
#include "input/keyboard.h"

namespace Tools
{
using namespace CoreGraphics;
using namespace Graphics;
using namespace Math;
using namespace Util;
using namespace Resources;
using namespace Timing;
using namespace Debug;
using namespace Input;

//------------------------------------------------------------------------------
/**
*/
TestViewerApplication::TestViewerApplication() : renderDebug(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TestViewerApplication::~TestViewerApplication()
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
TestViewerApplication::Open()
{
    n_assert(!this->IsOpen());
    if (ViewerApplication::Open())
    {
        // setup lights
        matrix44 lightTransform = matrix44::rotationx(n_deg2rad(-45.0f));
        this->globalLight = GlobalLightEntity::Create();
        this->globalLight->SetTransform(lightTransform);
        this->globalLight->SetColor(float4(1.0, 0.0f, 0.0f, 1.0f));
        this->globalLight->SetBackLightColor(float4(0.0f, 0.0f, 1.0f, 1.0f));
        this->globalLight->SetAmbientLightColor(float4(0.2f, 0.2f, 0.0f, 1.0f));
        this->globalLight->SetCastShadows(false);
        this->stage->AttachEntity(this->globalLight.cast<GraphicsEntity>());

        /*
        lightTransform = matrix44::multiply(matrix44::scaling(75.0f, 75.0f, 100.0f), matrix44::lookatrh(point(20.0f, 20.0f, 20.0f), point::origin(), vector::upvec()));
        this->localLight0 = SpotLightEntity::Create();
        this->localLight0->SetTransform(lightTransform);
        this->localLight0->SetColor(float4(1.0f, 1.0f, 1.0f, 0.0f));        
        this->localLight0->SetCastShadows(false);
        this->stage->AttachEntity(this->localLight0.cast<GraphicsEntity>());
        */

        // setup models
        
        this->ground = ModelEntity::Create();
        this->ground->SetResourceId(ResourceId("mdl:examples/ground.n3"));
        this->ground->SetTransform(matrix44::translation(0.0f, 0.0f, 0.0f));
        this->stage->AttachEntity(ground.cast<GraphicsEntity>());
        
        IndexT j;
        for (j = 0; j < 6; ++j)
        {
            IndexT i;
            for (i = 0; i < 6; ++i)
            {
                Ptr<ModelEntity> model = ModelEntity::Create();
                model->SetTransform(matrix44::translation(1.0f * i, 0.0, 1.0f * j));
                model->SetResourceId(ResourceId("mdl:characters/mensch_m.n3"));
                this->stage->AttachEntity(model.cast<GraphicsEntity>());
                this->models.Append(model);  

                // apply skin
                Ptr<Graphics::ApplySkinList> skinList = Graphics::ApplySkinList::Create();
                skinList->SetSkinList(StringAtom("mann_nackt"));
                model->SendMsg(skinList.cast<GraphicsEntityMessage>());

                // play animation
                Ptr<Graphics::AnimPlayLoop> playLoop = Graphics::AnimPlayLoop::Create();
                playLoop->SetBlendPriority(0);
                playLoop->SetBlendWeight(1.0f);
                playLoop->SetTimeFactor(1.0f);
                playLoop->SetStartTime(0);
                playLoop->SetTimeOffset(0);
                playLoop->SetFadeInTime(200);
                playLoop->SetFadeOutTime(200);
                playLoop->SetClipName(StringAtom("gehen_01"));
                model->SendMsg(playLoop.cast<GraphicsEntityMessage>());
            } 
        }

        // wait for resources to be loaded
        GraphicsInterface::Instance()->WaitForPendingResources();

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
TestViewerApplication::Close()
{
    this->stage->RemoveEntity(this->globalLight.cast<GraphicsEntity>());
    // this->stage->RemoveEntity(this->localLight0.cast<GraphicsEntity>());
    this->stage->RemoveEntity(this->ground.cast<GraphicsEntity>());    
    this->globalLight = 0;
    this->localLight0 = 0;
    this->ground = 0;

    IndexT i;
    for (i = 0; i < this->models.Size(); ++i)
    {
        this->stage->RemoveEntity(this->models[i].cast<GraphicsEntity>());    	
    }
    this->models.Clear();

    ViewerApplication::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
TestViewerApplication::OnProcessInput()
{
    const Ptr<Keyboard>& kbd = InputServer::Instance()->GetDefaultKeyboard();
    
    // enable/disable debug view
    if (kbd->KeyDown(Key::F4))
    {
        // turn on debug rendering        
        Ptr<Debug::RenderDebugView> renderDebugMsg = Debug::RenderDebugView::Create();
        renderDebugMsg->SetThreadId(Threading::Thread::GetMyThreadId());
        renderDebugMsg->SetEnableDebugRendering(!this->renderDebug);
        Graphics::GraphicsInterface::Instance()->SendBatched(renderDebugMsg.cast<Messaging::Message>());
        this->renderDebug = !this->renderDebug;
    }
    ViewerApplication::OnProcessInput();
}

//------------------------------------------------------------------------------
/**
*/
void
TestViewerApplication::OnUpdateFrame()
{
/*
    point lookatPos = this->model->GetTransform().getrow3();

    // animate lights
    point pos(20.0f * n_sin(curTime), 
              20.0f + 15.0f * n_sin(curTime * 0.5f), 
              20.0f + 7.5f * n_sin(curTime * 0.33f));
    matrix44 lightTransform = matrix44::multiply(matrix44::scaling(75.0f, 75.0f, 100.0f), matrix44::lookatrh(pos, lookatPos, vector::upvec()));
    this->localLight0->SetTransform(lightTransform);
*/
    // test text rendering
    float frameTime = (float)this->GetFrameTime();
    Util::String fpsTxt;
    fpsTxt.Format("Game FPS: %.2f", 1/frameTime);
    _debug_text(fpsTxt, Math::float2(0.0,0.0), Math::float4(1,1,1,1))

    // render a few debug shapes
    /*
    float curTime = (float) this->GetTime();
    IndexT x;
    for (x = 0; x < 10; x++)
    {
        IndexT y;
        for (y = 0; y < 10; y++)
        {
            quaternion rot = quaternion::rotationyawpitchroll(0.0f, curTime, 0.0f);
            matrix44 m = matrix44::affinetransformation(1.0f, vector::nullvec(), rot, point(x * 2.0f, 1.0f, y * 2.0f));
            DebugShapeRenderer::Instance()->DrawBox(m, float4(1.0f, 0.0f, 0.0f, 0.5f));
        }
    }
    */
    ViewerApplication::OnUpdateFrame();
}

} // namespace Tools
