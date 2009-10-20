//------------------------------------------------------------------------------
//  uirendererbase.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreui/base/coreuirendererbase.h"
#include "coregraphics/shaperenderer.h"
#include "coregraphics/transformdevice.h"
#include "math/point.h"
#include "apprender/platformconfig.h"
#include "timing/slavetime.h"

namespace Base
{
__ImplementClass(CoreUIRendererBase, 'UIRB', Core::RefCounted);
__ImplementSingleton(CoreUIRendererBase);

using namespace Math;
using namespace Resources;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
CoreUIRendererBase::CoreUIRendererBase() : isOpen(false) 
{
	__ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
CoreUIRendererBase::~CoreUIRendererBase()
{
	n_assert(!this->IsOpen());
	__DestructSingleton
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRendererBase::Open()
{
	n_assert(!this->IsOpen());
    this->isOpen = true;

    // initialize dummy texture for "no-textured" objects
    ResourceId fullTexResId = Util::String(Util::String("textures:system/transparent_15blue") + TEXTURE_EXTENSION);
    Ptr<ManagedTexture> managedTexture = ResourceManager::Instance()->CreateManagedResource(Texture::RTTI, fullTexResId).downcast<ManagedTexture>();      
    this->texturePool.Add(Util::StringAtom(""), managedTexture);
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRendererBase::Close()
{
	n_assert(this->IsOpen());
    this->isOpen = false;
    
    this->elementzByDepth.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRendererBase::LoadUITexture(const Util::String &resource)
{
    // soft check
    if(!this->texturePool.Contains(resource))
    {
        // append texture extension depending on platform
        ResourceId fullTexResId = Util::String(Util::String("textures:") + resource + TEXTURE_EXTENSION);
        Ptr<ManagedTexture> managedTexture = ResourceManager::Instance()->CreateManagedResource(Texture::RTTI, fullTexResId).downcast<ManagedTexture>();      

        // create new entry
        this->texturePool.Add(resource, managedTexture);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRendererBase::AddUIElement(CoreUI::UIElement& uiElement)
{
	n_assert(this->IsOpen());

    // append    
    const Util::String& tex = uiElement.GetTexturePath();
    n_assert(texturePool.Contains(tex));
    
    ElementByTexture newOne;
    newOne.element = uiElement;
    newOne.texturePtr = texturePool[tex];
	this->elementzByDepth.Add(uiElement.GetDepth(), newOne);
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRendererBase::RemoveUIElement(const CoreUI::UIElement& uiElement, bool considerFadeOut)
{
	n_assert(this->IsOpen());

    // find index of element to delete
    IndexT desiredIndex = InvalidIndex;
    Util::Array<ElementByTexture> elements = this->elementzByDepth.ValuesAsArray();
    for(IndexT index = 0; index < elements.Size(); index++)
    {
        if(elements[index].element == uiElement)
        {
            desiredIndex = index;
            break;
        }
    }

    // assert and delete
    n_assert(desiredIndex != InvalidIndex);

    if (considerFadeOut && this->elementzByDepth.ValueAtIndex(desiredIndex).element.GetFadeOutTime() > 0)
    {
        // fadeout
        this->elementzByDepth.ValueAtIndex(desiredIndex).element.SetFadingOut(true);
        this->fadeOutElements.Add(this->elementzByDepth.KeyAtIndex(desiredIndex), this->elementzByDepth.ValueAtIndex(desiredIndex));
    }
    // delete
    this->elementzByDepth.EraseAtIndex(desiredIndex);       
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRendererBase::AddUIElements(const Util::Array<CoreUI::UIElement>& uiElements)
{
	n_assert(this->IsOpen());
	
    // append them all
    IndexT index;
    for(index = 0; index < uiElements.Size(); index++)
    {
        if (!this->HasUIELement(uiElements[index]))
        {
            this->AddUIElement(uiElements[index]);
        }        
    }
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRendererBase::DeleteUIElementsByThreadId(Threading::ThreadId threadId)
{
    // search em all
    IndexT index;
    for(index = 0; index < this->elementzByDepth.ValuesAsArray().Size(); index++)
    {
        // get elements at this depth
        const CoreUI::UIElement& el = this->elementzByDepth.ValueAtIndex(index).element;

        Threading::ThreadId textThreadId = el.GetThreadId();
        n_assert(textThreadId != Threading::InvalidThreadId);
        if (textThreadId == threadId)
        {
            if (this->elementzByDepth.ValueAtIndex(index).element.GetFadeOutTime() > 0)
            {
                // fadeout
                this->fadeOutElements.Add(this->elementzByDepth.KeyAtIndex(index), this->elementzByDepth.ValueAtIndex(index));
                this->fadeOutElements[this->elementzByDepth.KeyAtIndex(index)].element.SetFadingOut(true);
            }
            this->elementzByDepth.EraseAtIndex(index);
            index--;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
int
CoreUIRendererBase::GetElementCount() const
{
    return this->elementzByDepth.Size() + this->fadeOutElements.Size();
}

//------------------------------------------------------------------------------
/**
*/
void 
CoreUIRendererBase::UpdateElements()
{
    Timing::Time frameTime = Timing::SlaveTime::Instance()->GetFrameTime(Timing::TimingType::MasterTime);    
    
    // at first update fade in elements
    IndexT index;
    for (index = 0; index < this->elementzByDepth.Size(); index++)
    {
        // increment fade in value
        CoreUI::UIElement& ele = this->elementzByDepth.ValueAtIndex(index).element;
        if (ele.IsFadingIn())
        {
            // increment fade in value
            ele.SetCurrentFadeInTime(ele.GetCurrentFadeInTime() + frameTime);
            if (ele.GetCurrentFadeInTime() >= ele.GetFadeInTime())
            {
                ele.SetFadingIn(false);
                ele.SetCurrentFadeInTime(ele.GetFadeInTime());
            }
        }
    }

    // now update all fade out elements
    for(index = 0; index < this->fadeOutElements.Size(); index++)
    {
        // decrement fade time
        CoreUI::UIElement& ele = this->fadeOutElements.ValueAtIndex(index).element;        
        ele.SetCurrentFadeOutTime(ele.GetCurrentFadeOutTime() - frameTime);
        if(ele.GetCurrentFadeOutTime() <= 0)
        {
            this->fadeOutElements.Erase(ele.GetDepth());
            --index;           
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
CoreUIRendererBase::DrawUI(const Ptr<CoreGraphics::ShaderInstance>& shdInst)
{
    // IMPLEMENT IN SUBCLASS
}

//------------------------------------------------------------------------------
/**
*/
bool 
CoreUIRendererBase::HasUIELement(const CoreUI::UIElement& uiElement) const
{
    IndexT desiredIndex = InvalidIndex;
    const Util::Array<ElementByTexture>& elements = this->elementzByDepth.ValuesAsArray();
    for(IndexT index = 0; index < elements.Size(); index++)
    {
        if(elements[index].element == uiElement)
        {
            desiredIndex = index;
            break;
        }
    }
    return (desiredIndex != InvalidIndex);
}

//------------------------------------------------------------------------------
/**
*/
void 
CoreUIRendererBase::RemoveUIElements(const Util::Array<CoreUI::UIElement>& uiElements, bool considerFadeOut /*= true*/)
{
    IndexT index;
    for(index = 0; index < uiElements.Size(); index++)
    {        
        this->RemoveUIElement(uiElements[index], considerFadeOut);                
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
CoreUIRendererBase::UpdateUIElements(const Util::Array<CoreUI::UIElement>& uiElements)
{
    IndexT index;
    for(index = 0; index < uiElements.Size(); index++)
    {        
        this->UpdateUIElements(uiElements[index]);                
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
CoreUIRendererBase::UpdateUIElements(const CoreUI::UIElement& uiElement)
{
    IndexT desiredIndex = InvalidIndex;
    Util::Array<ElementByTexture> elements = this->elementzByDepth.ValuesAsArray();
    for(IndexT index = 0; index < elements.Size(); index++)
    {
        if (elements[index].element.GetElementId() == uiElement.GetElementId())
        {
            desiredIndex = index;
            break;
        }
    }

    // assert and delete
    n_assert(desiredIndex != InvalidIndex);
    ElementByTexture elementByTexture = this->elementzByDepth.ValueAtIndex(desiredIndex);
    elementByTexture.element = uiElement;
    this->elementzByDepth.EraseAtIndex(desiredIndex);
    this->elementzByDepth.Add(uiElement.GetDepth(), elementByTexture);
}
} // namespace Base
