//------------------------------------------------------------------------------
//  internalgraphicsserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "internalgraphics/internalstage.h"
#include "internalgraphics/internalview.h"
#include "internalgraphics/stagebuilder.h"
#include "resources/resourcemanager.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/displaydevice.h"
#include "coregraphics/shaperenderer.h"
#include "lighting/internalgloballightentity.h"
#include "characters/skinnedmeshrenderer.h"

#if !__WII__
#include "particles/particlerenderer.h"
#include "coreui/coreuirenderer.h"
#include "video/videoserver.h"
#endif

namespace InternalGraphics
{
__ImplementClass(InternalGraphics::InternalGraphicsServer, 'IGFS', Core::RefCounted);
__ImplementSingleton(InternalGraphics::InternalGraphicsServer);

using namespace Core;
using namespace Util;
using namespace Math;
using namespace Resources;
using namespace CoreGraphics;
using namespace Characters;

//------------------------------------------------------------------------------
/**
*/
InternalGraphicsServer::InternalGraphicsServer() :
    isOpen(false),
    frameCount(0),
    renderDebug(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
InternalGraphicsServer::~InternalGraphicsServer()
{
    n_assert(!this->isOpen);
    n_assert(this->stages.IsEmpty());
    n_assert(this->stageIndexMap.IsEmpty());
    n_assert(this->views.IsEmpty());
    n_assert(this->viewIndexMap.IsEmpty());
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsServer::Open()
{
    n_assert(!this->isOpen);
    this->isOpen = true;

    // get time shader variable from shader server (only if platform supports it)
    ShaderServer* shdServer = ShaderServer::Instance();
    ShaderVariable::Semantic timeSem = ShaderVariable::Semantic("Time");    
    if (shdServer->HasSharedVariableBySemantic(timeSem))
    {        
        this->timeShaderVar = shdServer->GetSharedVariableBySemantic(timeSem);
        n_assert(this->timeShaderVar.isvalid());
    }

    // setup the render-thread-plugin registry
    this->rtPluginRegistry = RenderModules::RTPluginRegistry::Create();
    this->rtPluginRegistry->Setup();

    // setup profiling timers
    _setup_timer(InternalGfxServerEndFrameTimer);
    _setup_timer(InternalGfxServerRenderView);
    _setup_timer(InternalGfxServerUpdateLightLinks);
    _setup_timer(InternalGfxServerUpdateVisibilityLinks);
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsServer::Close()
{
    n_assert(this->isOpen);
    n_assert(this->entities.IsEmpty());
    this->timeShaderVar = 0;
    this->DiscardAllViews();
    this->DiscardAllStages();
    this->rtPluginRegistry->Discard();
    this->rtPluginRegistry = 0;
    this->isOpen = false;

    _discard_timer(InternalGfxServerEndFrameTimer);
    _discard_timer(InternalGfxServerRenderView);
    _discard_timer(InternalGfxServerUpdateLightLinks);
    _discard_timer(InternalGfxServerUpdateVisibilityLinks);
}

//------------------------------------------------------------------------------
/**
*/
Ptr<InternalStage>
InternalGraphicsServer::CreateStage(const StringAtom& stageName, const Ptr<StageBuilder>& stageBuilder)
{
    n_assert(this->isOpen);
    n_assert(!this->stageIndexMap.Contains(stageName));

    Ptr<InternalStage> newStage = InternalStage::Create();
    newStage->SetName(stageName);
    newStage->SetStageBuilder(stageBuilder);
    newStage->OnAttachToServer();

    this->stages.Append(newStage);
    this->stageIndexMap.Add(stageName, this->stages.Size() - 1);

    // notify render-thread plugins
    this->rtPluginRegistry->OnStageCreated(newStage);

    return newStage;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsServer::DiscardStage(const Ptr<InternalStage>& stage)
{
    n_assert(this->stageIndexMap.Contains(stage->GetName()));

    // notify render-thread plugins
    this->rtPluginRegistry->OnDiscardStage(stage);

    this->stages.EraseIndex(this->stageIndexMap[stage->GetName()]);

    // need to rebuild stageIndexMap
    this->stageIndexMap.Clear();
    IndexT i;
    for (i = 0; i < this->stages.Size(); i++)
    {
        this->stageIndexMap.Add(this->stages[i]->GetName(), i);
    }
    stage->OnRemoveFromServer();
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsServer::DiscardAllStages()
{
    IndexT i;
    SizeT num = this->stages.Size();
    for (i = 0; i < num; i++)
    {
        this->rtPluginRegistry->OnDiscardStage(this->stages[i]);
        this->stages[i]->OnRemoveFromServer();
    }
    this->stages.Clear();
    this->stageIndexMap.Clear();
}

//------------------------------------------------------------------------------
/**
*/
bool
InternalGraphicsServer::HasStage(const StringAtom& stageName) const
{
    return this->stageIndexMap.Contains(stageName);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<InternalStage>&
InternalGraphicsServer::GetStageByName(const StringAtom& stageName) const
{
    return this->stages[this->stageIndexMap[stageName]];
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<InternalStage> >&
InternalGraphicsServer::GetStages() const
{
    return this->stages;
}

//------------------------------------------------------------------------------
/**
*/
Ptr<InternalView>
InternalGraphicsServer::CreateView(const Core::Rtti& viewClass, const StringAtom& viewName, bool isDefaultView)
{
    n_assert(!this->viewIndexMap.Contains(viewName));
    n_assert(viewClass.IsDerivedFrom(InternalView::RTTI));

    Ptr<InternalView> newView = (InternalView*) viewClass.Create();
    newView->SetName(viewName);
    newView->OnAttachToServer();

    this->views.Append(newView);
    this->viewIndexMap.Add(viewName, this->views.Size() - 1);
    if (isDefaultView)
    {
        this->SetDefaultView(newView);
    }

    // notify render-thread plugins
    this->rtPluginRegistry->OnViewCreated(newView);

    return newView;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsServer::DiscardView(const Ptr<InternalView>& view)
{
    n_assert(this->viewIndexMap.Contains(view->GetName()));
    this->rtPluginRegistry->OnDiscardView(view);
    this->views.EraseIndex(this->viewIndexMap[view->GetName()]);

    // need to rebuild viewIndexMap
    this->viewIndexMap.Clear();
    IndexT i;
    for (i = 0; i < this->views.Size(); i++)
    {
        this->viewIndexMap.Add(this->views[i]->GetName(), i);
    }

    // check if this was the default view
    if (view == this->defaultView)
    {
        this->defaultView = 0;
    }
    view->OnRemoveFromServer();
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsServer::DiscardAllViews()
{
    IndexT i;
    SizeT num = this->views.Size();
    for (i = 0; i < num; i++)
    {
        this->rtPluginRegistry->OnDiscardView(this->views[i]);
        this->views[i]->OnRemoveFromServer();
    }
    this->views.Clear();
    this->viewIndexMap.Clear();
    this->defaultView = 0;
}

//------------------------------------------------------------------------------
/**
*/
bool
InternalGraphicsServer::HasView(const StringAtom& viewName) const
{
    return this->viewIndexMap.Contains(viewName);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<InternalView>&
InternalGraphicsServer::GetViewByName(const StringAtom& viewName) const
{
    return this->views[this->viewIndexMap[viewName]];
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<InternalView> >&
InternalGraphicsServer::GetViews() const
{
    return this->views;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsServer::SetDefaultView(const Ptr<InternalView>& defView)
{
    this->defaultView = defView;
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<InternalView>&
InternalGraphicsServer::GetDefaultView() const
{
    return this->defaultView;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsServer::RegisterEntity(const Ptr<InternalGraphicsEntity>& entity)
{
    this->entities.Append(entity);
    this->entityIndexMap.Add(entity->GetId(), this->entities.Size() - 1);
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsServer::UnregisterEntity(const Ptr<InternalGraphicsEntity>& entity)
{
    IndexT mapIndex = this->entityIndexMap.FindIndex(entity->GetId());
    n_assert(InvalidIndex != mapIndex);

    IndexT entityIndex = this->entityIndexMap.ValueAtIndex(mapIndex);
    this->entities.EraseIndexSwap(entityIndex);
    this->entityIndexMap.EraseAtIndex(mapIndex);

    // need to fixup the lookup index of the entity which has been
    // swapped into place of the erased entity, index doesn't
    // need to fixed if the erased element was the last in the array of course
    if (entityIndex < this->entityIndexMap.Size())
    {
        this->entityIndexMap[this->entities[entityIndex]->GetId()] = entityIndex;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
InternalGraphicsServer::HasEntity(InternalGraphicsEntity::Id id) const
{
    return this->entityIndexMap.Contains(id);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<InternalGraphicsEntity>&
InternalGraphicsServer::GetEntityById(InternalGraphicsEntity::Id id) const
{
    const Ptr<InternalGraphicsEntity>& entity = this->entities[this->entityIndexMap[id]];
    // if this assertion triggers, this means, that UnregisterEntity() failed
    // to fix indices in the entityIndexMap!!!
    n_assert(entity->GetId() == id);
    return entity;
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<InternalGraphicsEntity> >&
InternalGraphicsServer::GetEntities() const
{
    return this->entities;
}

//------------------------------------------------------------------------------
/**
    FIXME: iterating stuff is terrible!
*/
Ptr<Lighting::InternalGlobalLightEntity>
InternalGraphicsServer::GetCurrentGlobalLightEntity() const
{   
    Ptr<Lighting::InternalGlobalLightEntity> globLight = 0;
    const Ptr<InternalView>& view = this->GetDefaultView();
    if(view.isvalid())
    {
        const Ptr<InternalStage>& stage = view->GetStage();
        if(stage.isvalid())
        {
            const Util::Array<Ptr<InternalGraphics::InternalGraphicsEntity> >& lights = stage->GetEntitiesByType(InternalGraphicsEntityType::Light);
            IndexT index;
            for(index = 0; index < lights.Size(); index++)
            {
                if(lights[index]->IsA(Lighting::InternalGlobalLightEntity::RTTI))
                {
                    globLight = lights[index].downcast<Lighting::InternalGlobalLightEntity>();
                    break;
                }
            }
        }
    }    
    return globLight;
}

//------------------------------------------------------------------------------
/**
*/
void
InternalGraphicsServer::OnFrame(Timing::Time curTime)
{
    RenderDevice* renderDevice = RenderDevice::Instance();
    DisplayDevice* displayDevice = DisplayDevice::Instance();

    // call pre-render update on resource manager
    ResourceManager::Instance()->Prepare();

    // process window messages
    displayDevice->ProcessWindowMessages();

    // render the default view
    if (this->defaultView.isvalid() && this->defaultView->GetCameraEntity().isvalid())
    {
        // start rendering
        if (renderDevice->BeginFrame())
        {
            const Ptr<InternalStage>& defaultStage = this->defaultView->GetStage();
            
            // notify skinned mesh renderer
            SkinnedMeshRenderer::Instance()->OnBeginFrame();

            // update transformdevice with camera transforms for this frame
            this->defaultView->ApplyCameraSettings();

            // update the view's stage, this will happen only once
            // per frame, regardless of how many views are attached to the stage
            // FIXME: move ParticleRenderer-Stuff into Event Handlers!
            this->rtPluginRegistry->OnUpdateBefore(this->frameCount, curTime);
            #if !__WII__
            Particles::ParticleRenderer::Instance()->BeginAttach();
            #endif
            defaultStage->UpdateEntities(curTime, this->frameCount);
            #if !__WII__
            Particles::ParticleRenderer::Instance()->EndAttach();
            #endif
            this->rtPluginRegistry->OnUpdateAfter(this->frameCount, curTime);

            _start_timer(InternalGfxServerUpdateVisibilityLinks);
            // update visibility from the default view's camera
            this->defaultView->UpdateVisibilityLinks();
            _stop_timer(InternalGfxServerUpdateVisibilityLinks);

            _start_timer(InternalGfxServerUpdateLightLinks);
            // update light linking for visible lights
            defaultStage->UpdateLightLinks();
            _stop_timer(InternalGfxServerUpdateLightLinks);

            // update time
            if (this->timeShaderVar.isvalid())
            {                
                this->timeShaderVar->SetFloat((float)curTime);
            }

            _start_timer(InternalGfxServerRenderView);
            // finally render the view
            this->rtPluginRegistry->OnRenderBefore(this->frameCount, curTime);
            this->defaultView->Render();
            this->rtPluginRegistry->OnRenderAfter(this->frameCount, curTime);
            _stop_timer(InternalGfxServerRenderView);

            // render bounding boxes if debug rendering is enabled
            if (this->renderDebug)
            {
                this->defaultView->RenderDebug();
            }

            // render video
            #if !__WII__
            Video::VideoServer::Instance()->OnFrame(curTime);
            #endif

            _start_timer(InternalGfxServerEndFrameTimer)
            renderDevice->EndFrame();
            renderDevice->Present();
            _stop_timer(InternalGfxServerEndFrameTimer)

            // notify skinned mesh renderer
            SkinnedMeshRenderer::Instance()->OnEndFrame();
        }
    }
    #if !__WII__
    else if (CoreUI::CoreUIRenderer::HasInstance() 
             && CoreUI::CoreUIRenderer::Instance()->GetElementCount() > 0
             && CoreUI::CoreUIRenderer::Instance()->AllTexturesLoaded())
    {
        if (renderDevice->BeginFrame())
        {
            const Ptr<CoreGraphics::ShaderInstance>& shdInst = CoreUI::CoreUIRenderer::Instance()->GetShader();
            renderDevice->BeginPass(renderDevice->GetDefaultRenderTarget(), shdInst);
            CoreUI::CoreUIRenderer::Instance()->DrawUI(shdInst);
            renderDevice->EndPass();
            renderDevice->EndFrame();
            renderDevice->Present();
        }
    }
    #endif
    
    // call post-render update on Resource Manager
    ResourceManager::Instance()->Update();

    // if we're running in windowed mode, give up time-slice
    if (!displayDevice->IsFullscreen())
    {
        Timing::Sleep(0.0);
    }

    // update the frame count
    this->frameCount++;
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalGraphicsServer::AttachEntityEventHandler(InternalGraphicsEntityType::Code type, const Ptr<InternalGraphicsEntityHandler>& handler)
{
    n_assert(!this->entityHandler[type].isvalid());
    this->entityHandler[type] = handler;
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalGraphicsServer::RemoveEntityEventHandler(const Ptr<InternalGraphicsEntityHandler>& handler)
{
    IndexT i;
    for (i = 0; i < InternalGraphicsEntityType::NumTypes; i++)
    {
        if (this->entityHandler[i] == handler)
        {
            this->entityHandler[i] = 0;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
InternalGraphicsServer::PutGraphicsEntityEvent(InternalGraphicsEntityType::Code type, const Ptr<InternalGraphicsEntityEvent>& event)
{
    if (this->entityHandler[type].isvalid())
    {
        if (this->entityHandler[type]->HandleEvent(event))
        {
            event->SetHandled(true);
        }
    }
}

} // namespace InternalGraphics
