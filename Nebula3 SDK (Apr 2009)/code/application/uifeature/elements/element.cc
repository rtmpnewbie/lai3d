//------------------------------------------------------------------------------
//  uifeature/elements/element.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "uifeature/elements/element.h"
#include "uifeature/elements/window.h"
#include "uifeature/uifactory.h"
#include "uifeature/uimanager.h"
#include "uifeature/uievent.h"
#include "coreui/uirenderer.h"
#include "debugrender/debugrender.h"

namespace UI
{
__ImplementClass(Element, 'UIEL', Core::RefCounted);

using namespace Math;
//------------------------------------------------------------------------------
/**
*/
Element::Element() :
    visible(true),
    parentElement(0),
    isValid(false),              
    parentWindow(0),
    enabled(true),
	screenSpaceRect(0,0,0,0),
	defaultColor(0,0,0,1),
	defaultUV(0,0,1,1),
	isConvertedToAbsolutPositions(false),
    elementId(0),
    mouseOver(false),
    ignoreInput(false),
    dirty(false),
    blinkingEnabled(false),
    blinkTime(0.0),
    blinkColor(0,0,0,1),
    fadeInTime(0.0),
    fadeOutTime(0.0)    
{
}

//------------------------------------------------------------------------------
/**
*/
Element::~Element()
{
}

//------------------------------------------------------------------------------
/**
    Called by parent to build the gui hierarchy.
*/
void
Element::AttachElement(const Ptr<Element>& elm)
{
	n_assert(elm.isvalid());
    this->childElements.Append(elm);
	elm->SetParentElement(Ptr<Element>(this));
	elm->SetParentWindow(this->parentWindow);
    elm->OnCreate();
}

//------------------------------------------------------------------------------
/**
    Erase a child element. This will invoke the OnDestroy() method on the
    child. The child will be destroyed
*/
void
Element::EraseElement(const Ptr<Element>& elm)
{
	n_assert(elm.isvalid());
    elm->OnDestroy();
	Util::Array<Ptr<Element>>::Iterator iter = this->childElements.Find(elm);
    n_assert(iter);
    this->childElements.Erase(iter);
}

//------------------------------------------------------------------------------
/**
    Remove a child element from the childElements array. This won't destroy the child.
*/
void
Element::RemoveElement(const Ptr<Element>& elm)
{
	n_assert(elm.isvalid());
    Util::Array<Ptr<Element>>::Iterator iter = this->childElements.Find(elm);
    n_assert(iter);
    this->childElements.Erase(iter);
}

//------------------------------------------------------------------------------
/**
    Provides access to a single child element by an index.
*/
const Ptr<Element>&
Element::GetChildElementAt(int index) const
{
    n_assert(index>=0 && (IndexT)index < this->childElements.Size());    
    return this->childElements[index];
}

//------------------------------------------------------------------------------
/**
    Find a child element by its id. Returns 0 if not found.
*/
Ptr<Element>
Element::FindChildElement(const Util::String& id) const
{
    IndexT i;
    for (i = 0; i < this->childElements.Size(); i++)
    {
        if (id == this->childElements[i]->GetId())
        {
            return this->childElements[i];
        }
    }
    // fallthrough: not found
    return Ptr<Element>(0);
}

//------------------------------------------------------------------------------
/**
    This method is called when the element has been attached to a parent.
*/
void
Element::OnCreate()
{
    n_assert(!this->IsValid());
    this->isValid = true;
    this->elementId = UIManager::Instance()->GetNewElementId();
}

//------------------------------------------------------------------------------
/**
    This method is called when the element has been removed from its parent.
*/
void
Element::OnDestroy()
{
    if (this->IsValid())
    {
        // release focus if necessary
        if (this->IsFocusElement())
        {
            this->UnregisterAsFocusElement();
        }

        // destroy all children
        IndexT i;
        for (i = 0; i < this->childElements.Size(); i++)
        {
            this->childElements[i]->OnDestroy();
        }
        this->childElements.Clear();   

	    UIManager::Instance()->FreeElementId(this->elementId);
        if (this->visible)
        {
            UIRenderer::Instance()->FlushElement(this->elementId);
        }
        
         // clear own parent pointer
        this->parentElement = 0;
        this->parentWindow = 0;
        this->isValid = false;
    }
}

//------------------------------------------------------------------------------
/**
    This method is called once per frame during UIServer::Trigger() after
    the input has been delivered to the gui.
*/
void
Element::OnFrame()
{
    // distribute to children
    if (this->IsVisible())
    {
        IndexT i;
        for (i = 0; i < this->childElements.Size(); i++)
        {
            this->childElements[i]->OnFrame();
        }
    }
}

//------------------------------------------------------------------------------
/**
    This method is called when the element should render itself.
*/
void
Element::OnRender()
{
    int depth = this->CalculateHierarchyDepth() + 1;
    
    Math::float4 color = this->defaultColor;    
    // render clear dirty flag
    UIRenderer::Instance()->DrawElement(this->elementId, this->screenSpaceRect, 
                                        this->defaultUV, depth, color, 
                                        this->GetParentWindow()->GetWindowTexture(), 
                                        this->GetBlinkingEnabled(), 
                                        this->GetBlinkTime(), 
                                        this->GetBlinkColor(), 
                                        this->GetFadeInTime(),
                                        this->GetFadeOutTime(),
                                        dirty);
    this->dirty = false;

    // distribute to children
    IndexT i;
    for (i = 0; i < this->childElements.Size(); i++)
    {
        this->childElements[i]->OnRender();
    }

    // at end of frame, reset the MOUSEOVER state!!!
    this->SetMouseOver(false);
}

//------------------------------------------------------------------------------
/**
    This method is called when the element should render itself in debug mode.
	(Just the shapes)
*/
void
Element::OnRenderDebug()
{
	int depth = this->CalculateHierarchyDepth();

    Math::float4 color(1, 1, 1, 1);//f / ((depth+15)/15) );
    UIRenderer::Instance()->DrawDebugView(this->elementId, this->screenSpaceRect, depth, color);

    // distribute to children
    IndexT i;
    for (i = 0; i < this->childElements.Size(); i++)
    {
        this->childElements[i]->OnRenderDebug();
    }

    // set text 
    Math::float2 dbgTxtPos(this->GetScreenPosition().x() - this->GetScreenSize().x() / 2, 
                           this->GetScreenPosition().y() - this->GetScreenSize().y() / 2);
    _debug_text(this->GetId(), dbgTxtPos, Math::float4(1,1,1,1));

}

//------------------------------------------------------------------------------
/**
    This method returns true if the given mouse position is inside the
    elements screen space area. By default this checks the mouse position
    against the screen space rectangle,
    but you can override the method in a subclass if you need more 
    advanced picking.
*/
bool
Element::Inside(const Math::float2& mousePos)
{
    if (this->IsVisible() && this->IsValid())
    {
        //rough check on screenSpaceRect
		bool inside =  this->screenSpaceRect.inside(mousePos.x(), mousePos.y());

		// DEBUG ============
		//if(inside)
		//{
		//	int d = this->CalculateHierarchyDepth();
		//	Util::String debugTxt;
		//	for(int i = 0; i < d; i++)
		//	{
		//		debugTxt.Append("\n");
		//	}
		//	for(int i = 0; i < d; i++)
		//	{
		//		debugTxt.Append(" ");
		//	}
		//	debugTxt.Append(this->GetId());
		//	debugTxt.Append(" - inside");
		//	_debug_text(debugTxt, Math::float2(0.0f, 0.5f), Math::float4(1,1,1,1));
		//}
		// ===================
        return inside;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    Updates his own screenrect position to absolute positions
*/
void
Element::InitAbsolutePositions()
{
	n_assert(!this->isConvertedToAbsolutPositions);

	// move all childs
	IndexT i;
	for(i = 0; i < this->childElements.Size(); i++)
	{
		// move this child
		Math::float2 moveVec = this->GetScreenPosition();
		this->childElements[i]->Move(moveVec);
		// and redirect to this childs childs
		this->childElements[i]->InitAbsolutePositions();
	}

	this->isConvertedToAbsolutPositions = true;
}

//------------------------------------------------------------------------------
/**    
*/
int
Element::CalculateHierarchyDepth() const
{
	// calculate the depth of the element
	int depth = 0;
	Ptr<Element> p = this->parentElement;
	while(p.isvalid())
	{
		depth++;
		p = p->GetParentElement();
	}
	return depth + this->parentWindow->GetAdditionalZOffset();
}

//------------------------------------------------------------------------------
/**    
*/
void
Element::Move(const Math::float2& vec)
{
	// get position
	Math::float2 pos = this->GetScreenPosition();
	// and set with move
	pos += vec;
	this->SetScreenPosition(pos);

	// now call all childs to move
	for(IndexT i = 0; i < this->childElements.Size(); i++)
	{
		this->childElements[i]->Move(vec);
	}
}

//------------------------------------------------------------------------------
/**
    Checks all childs, appends them, if they are inside the position.
*/
void
Element::GetElementsAt(const Math::float2& mousePos, Util::Array<Ptr<Element>>& elements_out)
{
	// check all childs
	IndexT index;
	for(index = 0; index < this->childElements.Size(); index++)
	{
		if(this->childElements[index]->Inside(mousePos))
		{	
			elements_out.Append(this->childElements[index]);
			this->childElements[index]->GetElementsAt(mousePos, elements_out);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/ 
void
Element::SetVisible(bool b)
{
    if (this->visible && this->isValid)
    {
        UIRenderer::Instance()->FlushElement(this->elementId);
    }
    this->visible = b;
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::IsVisible() const
{
    // recursively check if me or one of my parents is invisible
    if (!this->visible)
    {
        return false;
    }
	else if (this->GetParentElement().isvalid())
    {
        return this->GetParentElement()->IsVisible();
    }    
    else
    {
        return true;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Element::SetParentWindow(const Ptr<Window>& window)
{
	n_assert(window.isvalid());
	n_assert(!this->parentWindow.isvalid());
    
	this->parentWindow = window;    
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Window>&
Element::GetParentWindow() const
{
    return this->parentWindow;
}

//------------------------------------------------------------------------------
/**
    If set to false the element won't call the OnX() methods anymore and doesn't
    distribute the input to it's children.
*/
const bool
Element::IsEnabled() const
{
    // recursively check if me or one of my parents is disabled
    if (!this->enabled)
    {
        return false;
    }
	else if (this->GetParentElement().isvalid())
    {
        return this->GetParentElement()->IsEnabled();
    }    
    else
    {
        return true;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnMouseMove(const Math::float2& mousePos)
{
    // general setting for mouseover
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnLeftButtonDown(const Math::float2& mousePos)
{
    // implement in subclass   
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnLeftButtonPressed(const Math::float2& mousePos)
{
    // implement in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnLeftButtonUp(const Math::float2& mousePos)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnLeftButtonDoubleClicked(const Math::float2& mousePos)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnMiddleButtonDown(const Math::float2& mousePos)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnMiddleButtonPressed(const Math::float2& mousePos)
{
    // implement in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnMiddleButtonUp(const Math::float2& mousePos)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnMiddleButtonDoubleClicked(const Math::float2& mousePos)
{
    // implement in subclass    
}


//------------------------------------------------------------------------------
/**
*/
void
Element::OnRightButtonDown(const Math::float2& mousePos)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnRightButtonPressed(const Math::float2& mousePos)
{
    // implement in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnRightButtonUp(const Math::float2& mousePos)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnRightButtonDoubleClicked(const Math::float2& mousePos)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnChar(const uchar& code)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnKeyDown(Input::Key::Code key)
{
    // implement in subclass    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnKeyUp(Input::Key::Code key)
{
    // implement in subclass    
}


//------------------------------------------------------------------------------
/**
*/
void
Element::OnGamePadButtonDown(Input::GamePad::Button btn)
{
    // implement in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnGamePadButtonUp(Input::GamePad::Button btn)
{
    // implement in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnFocusGain()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnFocusLost()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void 
Element::RegisterAsFocusElement()
{
    UI::UIManager::Instance()->SetFocusElement(Ptr<Element>(this), true);  
}

//------------------------------------------------------------------------------
/**
*/
void 
Element::UnregisterAsFocusElement()
{
    UI::UIManager::Instance()->SetFocusElement(Ptr<Element>(this), false);
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::IsFocusElement() const
{
    return UI::UIManager::Instance()->GetFocusElement() == this;
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnMouseWheelMoved(const bool& up)
{
    // implement in subclass
}

//------------------------------------------------------------------------------
/**
*/
void 
Element::LinkNeighbours()
{
    if (this->parentElement.isvalid())
    {
        const Util::Array<Ptr<Element>>& possibleNeighbours = this->parentElement->GetChildElements();
        const Math::rectangle<float>& ownRect = this->GetScreenSpaceRect();
        IndexT i;
        for (i = 0; i < possibleNeighbours.Size(); ++i)
        {
            if (possibleNeighbours[i].get_unsafe() != this)
            {
                const Math::rectangle<float>& otherRect = possibleNeighbours[i]->GetScreenSpaceRect();

                if (otherRect.left < ownRect.left)
                {
                    if (this->neighbours[Left].isvalid())
                    {
                        const Math::rectangle<float>& preRect = this->neighbours[Left]->GetScreenSpaceRect();
                        float diffOld = n_abs(ownRect.top - otherRect.top);
                        float diffNew = n_abs(ownRect.top - preRect.top);
                        if (diffNew < diffOld)
                        {
                            this->neighbours[Left] = possibleNeighbours[i];
                        }
                    }
                    else
                    {
                        this->neighbours[Left] = possibleNeighbours[i];
                    }
                }
                else if (otherRect.right > ownRect.right)
                {
                    if (this->neighbours[Right].isvalid())
                    {
                        const Math::rectangle<float>& preRect = this->neighbours[Right]->GetScreenSpaceRect();
                        float diffOld = n_abs(ownRect.top - otherRect.top);
                        float diffNew = n_abs(ownRect.top - preRect.top);
                        if (diffNew < diffOld)
                        {
                            this->neighbours[Right] = possibleNeighbours[i];
                        }
                    }
                    else
                    {
                        this->neighbours[Right] = possibleNeighbours[i];
                    }
                }
                
                if (otherRect.top > ownRect.top)
                {
                    if (this->neighbours[Top].isvalid())
                    {
                        const Math::rectangle<float>& preRect = this->neighbours[Top]->GetScreenSpaceRect();
                        float diffOld = n_abs(ownRect.left - otherRect.left);
                        float diffNew = n_abs(ownRect.left - preRect.left);
                        if (diffNew < diffOld)
                        {
                            this->neighbours[Top] = possibleNeighbours[i];
                        }
                    }
                    else
                    {
                        this->neighbours[Top] = possibleNeighbours[i];
                    }
                }
                else if (otherRect.bottom < ownRect.bottom)
                {
                    if (this->neighbours[Bottom].isvalid())
                    {
                        const Math::rectangle<float>& preRect = this->neighbours[Bottom]->GetScreenSpaceRect();
                        float diffOld = n_abs(ownRect.left - otherRect.left);
                        float diffNew = n_abs(ownRect.left - preRect.left);
                        if (diffNew < diffOld)
                        {
                            this->neighbours[Bottom] = possibleNeighbours[i];
                        }
                    }
                    else
                    {
                        this->neighbours[Bottom] = possibleNeighbours[i];
                    }
                }
            }
        }
    }
}
} // namespace UI
