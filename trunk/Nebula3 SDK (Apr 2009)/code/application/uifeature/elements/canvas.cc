//------------------------------------------------------------------------------
//  uifeature/elements/canvas.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "uifeature/elements/canvas.h"
#include "uifeature/elements/window.h"
#include "uifeature/uimanager.h"

namespace UI
{
__ImplementClass(Canvas, 'UICV', Element);

//------------------------------------------------------------------------------
/**
*/
Canvas::Canvas()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Canvas::~Canvas()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This creates a graphics entity representing the visual of the canvas
    and converts the resource name into a Nebula2 node pointer.
*/
void
Canvas::OnCreate()
{
    // canvases don't have parents
	n_assert(this->parentWindow.isvalid());
	n_assert(!this->parentElement.isvalid());
	Element::OnCreate();

    // create graphics entity, first check if it actually exists...
    //n_assert(this->resourceName.IsValid());
    //Util::String resPath;
    //resPath.Format("gfxlib:%s.n3", this->resourceName.Get());
    //if (nFileServer2::Instance()->FileExists(resPath))
    //{
	   // this->graphicsEntity = Graphics::Entity::Create();
    //    this->graphicsEntity->SetVisibilityBoxCheckEnabled(false);
    //    this->graphicsEntity->SetTimeSource(Game::GuiTimeSource::Instance());
    //    this->graphicsEntity->SetResourceName(this->resourceName);
    //    this->graphicsEntity->SetRenderFlag(nRenderContext::DoOcclusionQuery, false);
    //    this->graphicsEntity->OnActivate();

    //    // find the Nebula node which represents the canvas
    //    nTransformNode* rootNode   = this->graphicsEntity->GetResource()->GetNode();
    //    nTransformNode* canvasNode = this->FindCanvasNodeInHierarchy(rootNode);
    //    n_assert(canvasNode);
    //    this->SetGfxNode(canvasNode);

    //    // scale the canvas to fit the aspect ratio
    //    this->FitAspectRatio();

    //    // create child elements
    //    Element::OnCreate(parent);
    //}
    //else
    //{
    //    n_error("UI::Canvas::OnCreate(): graphics resource doesn't exist '%s'!", resPath.AsCharPtr());
    //}
}

//------------------------------------------------------------------------------
/**
*/
void
Canvas::OnDestroy()
{
    Element::OnDestroy();
}

//------------------------------------------------------------------------------
/**
*/
void
Canvas::OnRender()
{
	Element::OnRender();
}

//------------------------------------------------------------------------------
/**
*/
void
Canvas::OnFrame()
{
    Element::OnFrame();    
}

}; // namespace UI
