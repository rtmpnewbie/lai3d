#ifndef UI_LABEL_H
#define UI_LABEL_H
//------------------------------------------------------------------------------
/**
    A label represents a simple backgroundplane that contains a graphic and
    can have a tooltip set.
    
    (C) 2005 Radon Labs GmbH
*/
#include "uifeature/elements/control.h"
#include "timing/time.h"

//------------------------------------------------------------------------------
namespace UI
{
class Label : public Element
{
	__DeclareClass(Label);
public:
    /// constructor
    Label();
    /// destructor
    virtual ~Label();
    /// called when gui hierarchy is created
    virtual void OnCreate();
};
}; // namespace UI
//------------------------------------------------------------------------------
#endif