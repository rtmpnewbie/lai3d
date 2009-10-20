#pragma once
//------------------------------------------------------------------------------
/**
    @class Base::UIRendererBase
  
    Base class for internal UI rendering.
    
    (C) 2008 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "core/singleton.h"
#include "math/rectangle.h"
#include "coreui/uielement.h"
#include "threading/threadid.h"
#include "resources/managedtexture.h"
#include "resources/resourcemanager.h"
#include "coregraphics/shaderinstance.h"
#include "util/stringatom.h"

//------------------------------------------------------------------------------
namespace Base
{
class CoreUIRendererBase : public Core::RefCounted
{
    __DeclareClass(CoreUIRendererBase);
    __DeclareSingleton(CoreUIRendererBase);
public:
    /// constructor
    CoreUIRendererBase();
    /// destructor
    virtual ~CoreUIRendererBase();
    
    /// open the the ui renderer
    virtual void Open();
    /// close the ui renderer
    virtual void Close();
    /// check if ui renderer open
    virtual bool IsOpen() const;
    /// update elements, removes faded out elements
    void UpdateElements();

    /// draw the accumulated ui
    virtual void DrawUI(const Ptr<CoreGraphics::ShaderInstance>& shdInst);

    /// delete added ui by thread id
    virtual void DeleteUIElementsByThreadId(Threading::ThreadId threadId);

    /// has ui element
    bool HasUIELement(const CoreUI::UIElement& uiElement) const;
    /// add text element for rendering
    void AddUIElement(CoreUI::UIElement& uiElement);
    /// add multiple text elements for rendering
    void AddUIElements(const Util::Array<CoreUI::UIElement>& uiElements);
    /// remove element
    void RemoveUIElement(const CoreUI::UIElement& uiElement, bool considerFadeOut = true);
    /// remove element
    void RemoveUIElements(const Util::Array<CoreUI::UIElement>& uiElements, bool considerFadeOut = true);
    /// update ui elements
    void UpdateUIElements(const Util::Array<CoreUI::UIElement>& uiElements);
    /// update ui element
    void UpdateUIElements(const CoreUI::UIElement& uiElement);

    /// add texture
    virtual void LoadUITexture(const Util::String &resource);

    /// return count of all elements
    int GetElementCount() const;

protected:
    struct ElementByTexture
    {
        Ptr<Resources::ManagedTexture> texturePtr;
        CoreUI::UIElement element;
    };

    /// all elements by depth
    Util::Dictionary<int, ElementByTexture> elementzByDepth;
    /// all textures
    Util::Dictionary<Util::StringAtom, Ptr<Resources::ManagedTexture> > texturePool;
    /// elements in fadeout
    Util::Dictionary<int, ElementByTexture> fadeOutElements;

    bool isOpen;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
CoreUIRendererBase::IsOpen() const
{
    return this->isOpen;
}

} // namespace Base
//------------------------------------------------------------------------------
