//------------------------------------------------------------------------------
//  uixmlparser.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "uifeature/uixmlparser.h"
#include "uifeature/uifactory.h"
#include "uifeature/elements/canvas.h"
#include "uifeature/elements/label.h"
#include "io/xmlreader.h"
#include "io/filestream.h"
#include "io/ioserver.h"

namespace UI
{
__ImplementClass(UIXmlParser, 'UIPA', Core::RefCounted);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
UIXmlParser::UIXmlParser() : fileName(""),
							 windowCount(-1)
{
    // get factory
	this->factory = UIFactory::Instance();
}

//------------------------------------------------------------------------------
/**
*/
UIXmlParser::~UIXmlParser()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
const Util::Array<Ptr<Window>>&
UIXmlParser::ParseFile()
{
    // cleat windows array
    this->windows.Clear();

	// open xml 
	Ptr<IO::XmlReader> xml = IO::XmlReader::Create();
    xml->SetStream(IO::IoServer::Instance()->CreateStream("export:ui/" + this->fileName));
	xml->Open();
	n_assert(xml->IsOpen());

	// check root node
	String nodeName = xml->GetCurrentNodeName();
	n_assert("Nebula3" == nodeName);

	// get next node.. has to be a window node
	if(xml->SetToFirstChild("Window"))
	{
		do
		{
			this->ParseWindowNode(xml);
		}while(xml->SetToNextChild());
	}

	return this->windows;
}

//------------------------------------------------------------------------------
/**
*/
void
UIXmlParser::ParseWindowNode(Ptr<IO::XmlReader>& xml)
{
	n_assert(xml->GetCurrentNodeName() == "Window");

	// parse node
	Ptr<Window> newWindow = Window::Create();
	String xalign = xml->GetString("xAlign");
	String yalign = xml->GetString("yAlign");
	newWindow->SetScreenAlignmentX(Window::StringToAlignmentX(xalign));
	newWindow->SetScreenAlignmentY(Window::StringToAlignmentY(yalign));
	newWindow->SetWindowTitle(xml->GetString("title"));
    newWindow->SetWindowTexture(xml->GetString("texture"));
	this->windows.Append(newWindow);
	this->windowCount++;

    float fadeInTime = 0.0f;
    float fadeOutTime = 0.0f;
    if (xml->HasAttr("fadeIn"))fadeInTime = xml->GetFloat("fadeIn");
    if (xml->HasAttr("fadeOut"))fadeOutTime = xml->GetFloat("fadeOut");
	
	// now parse the elements in the window node
	// first has to be a canvas
	xml->SetToFirstChild();
    this->ParseCanvasNode(xml, newWindow);
    // set fade times when canvas is loaded
    newWindow->SetFadeInTime(fadeInTime);
    newWindow->SetFadeOutTime(fadeOutTime);
	newWindow->InitAbsolutePosition();
	xml->SetToParent();
}

//------------------------------------------------------------------------------
/**
*/
void
UIXmlParser::ParseCanvasNode(Ptr<IO::XmlReader>& xml, Ptr<Window>& parent)
{
	n_assert(xml->GetCurrentNodeName() == "Canvas");

	// parse node
	Ptr<Canvas> newCanvas = factory->CreateElement("Canvas").cast<Canvas>();
	newCanvas->SetId(this->ElementGetId(xml));
	newCanvas->SetDefaultColor(this->ElementGetDefaultColor(xml));	
	newCanvas->SetDefaultUV(this->ElementGetDefaultUV(xml));
	newCanvas->SetVisible(this->ElementGetVisible(xml));
	newCanvas->SetEnabled(this->ElementGetEnabled(xml));
	newCanvas->SetPixelPosition(this->ElementGetPosition(xml));	
	newCanvas->SetPixelSize(this->ElementGetSize(xml));
    newCanvas->SetBlinkingEnabled(this->ElementGetBlinking(xml));
    newCanvas->SetBlinkColor(this->ElementGetBlinkColor(xml));
    newCanvas->SetBlinkTime(this->ElementGetBlinkTime(xml));
	newCanvas->SetParentWindow(parent);
	
	// set window canvas
	parent->SetCanvas(newCanvas);
	
	// call directly OnCreate() cause this is called in AttachElement, but
	// a canvas will never be attached like other elements
	newCanvas->OnCreate();

	Ptr<Element> el = newCanvas.cast<Element>();

	// parse childs recursivly, if there is one
	if(xml->SetToFirstChild())
	{		
		do
		{
			this->ParseChildElements(xml, el);
		}while(xml->SetToNextChild());
	}

    // link neighbors of canvas and its children
    this->LinkElementNeighbors(el);
}

//------------------------------------------------------------------------------
/**
*/
void
UIXmlParser::ParseChildElements(Ptr<IO::XmlReader>& xml, Ptr<Element>& parent)
{
	// set to 
	String nodeType = xml->GetCurrentNodeName();

	if		("Button" == nodeType)			this->ParseButtonNode(xml, parent);
	else if	("Label" == nodeType)			this->ParseLabelNode(xml, parent);
}

//------------------------------------------------------------------------------
/**
*/
void
UIXmlParser::ParseButtonNode(Ptr<IO::XmlReader>& xml, Ptr<Element>& parent)
{
	n_assert(xml->GetCurrentNodeName() == "Button");

	// parse node
	Ptr<Button> newButton = factory->CreateElement("Button").cast<Button>();
	newButton->SetId(this->ElementGetId(xml));
	newButton->SetDefaultColor(this->ElementGetDefaultColor(xml));	
	newButton->SetDefaultUV(this->ElementGetDefaultUV(xml));
	newButton->SetVisible(this->ElementGetVisible(xml));
	newButton->SetEnabled(this->ElementGetEnabled(xml));
	newButton->SetPixelPosition(this->ElementGetPosition(xml));	
	newButton->SetPixelSize(this->ElementGetSize(xml));
	newButton->SetIgnoreInput(this->ElementIgnoreInput(xml));
    newButton->SetBlinkingEnabled(this->ElementGetBlinking(xml));
    newButton->SetBlinkColor(this->ElementGetBlinkColor(xml));
    newButton->SetBlinkTime(this->ElementGetBlinkTime(xml));

	IndexT stateIndex;
	for(stateIndex = 0; stateIndex < Button::NumVisualStates; stateIndex++)
	{
		newButton->SetStateUV((Button::VisualState)stateIndex, this->ElementGetVisualStateUV(xml, (Button::VisualState)stateIndex));
	}

	// and append
	Ptr<Element> el = newButton.cast<Element>();
	parent->AttachElement(el);
	
	// check for further childs
	if(xml->SetToFirstChild())
	{
		do
		{
			this->ParseChildElements(xml, el);
		}while(xml->SetToNextChild());
	}
}

//------------------------------------------------------------------------------
/**
*/
void
UIXmlParser::ParseLabelNode(Ptr<IO::XmlReader>& xml, Ptr<Element>& parent)
{
	n_assert(xml->GetCurrentNodeName() == "Label");

	// parse node
	Ptr<Label> newLabel = factory->CreateElement("Label").cast<Label>();
	newLabel->SetId(this->ElementGetId(xml));
	newLabel->SetDefaultColor(this->ElementGetDefaultColor(xml));	
	newLabel->SetDefaultUV(this->ElementGetDefaultUV(xml));
	newLabel->SetVisible(this->ElementGetVisible(xml));
	newLabel->SetEnabled(this->ElementGetEnabled(xml));
	newLabel->SetPixelPosition(this->ElementGetPosition(xml));	
	newLabel->SetPixelSize(this->ElementGetSize(xml));
    newLabel->SetIgnoreInput(this->ElementIgnoreInput(xml));
    newLabel->SetBlinkingEnabled(this->ElementGetBlinking(xml));
    newLabel->SetBlinkColor(this->ElementGetBlinkColor(xml));
    newLabel->SetBlinkTime(this->ElementGetBlinkTime(xml));

	// and append
	Ptr<Element> el = newLabel.cast<Element>();
	parent->AttachElement(el);

	// check for further childs
	if(xml->SetToFirstChild())
	{
		do
		{
			this->ParseChildElements(xml, el);
		}while(xml->SetToNextChild());
	}
}

//------------------------------------------------------------------------------
/**
*/
bool
UIXmlParser::ElementGetVisible(Ptr<IO::XmlReader>& xml) const
{
	// optional
	if(xml->HasAttr("visible"))		return xml->GetBool("visible");
	return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
UIXmlParser::ElementGetEnabled(Ptr<IO::XmlReader>& xml) const
{
	// optional
	if(xml->HasAttr("enabled"))		return xml->GetBool("enabled");
	return true;
}
//------------------------------------------------------------------------------
/**
*/
Math::float2 
UIXmlParser::ElementGetPosition(Ptr<IO::XmlReader>& xml) const
{
	return xml->GetFloat2("position");
}

//------------------------------------------------------------------------------
/**
*/
Math::float2 
UIXmlParser::ElementGetSize(Ptr<IO::XmlReader>& xml) const
{
	return xml->GetFloat2("size");
}

//------------------------------------------------------------------------------
/**
*/
Util::String 
UIXmlParser::ElementGetId(Ptr<IO::XmlReader>& xml) const
{
	return xml->GetString("id");
}

//------------------------------------------------------------------------------
/**
*/
Math::float4 
UIXmlParser::ElementGetDefaultColor(Ptr<IO::XmlReader>& xml) const
{
	if(xml->HasAttr("defaultColor"))		return xml->GetFloat4("defaultColor");
	return Math::float4(1,1,1,1);
}

//------------------------------------------------------------------------------
/**
*/
Math::float4 
UIXmlParser::ElementGetDefaultUV(Ptr<IO::XmlReader>& xml) const
{	
	return xml->GetFloat4("defaultUV");
}

//------------------------------------------------------------------------------
/**
*/
bool 
UIXmlParser::ElementIgnoreInput(Ptr<IO::XmlReader>& xml) const
{
    if(xml->HasAttr("ignoreInput"))		return xml->GetBool("ignoreInput");
    return false;
}


//------------------------------------------------------------------------------
/**
*/
Math::float4 
UIXmlParser::ElementGetVisualStateUV(Ptr<IO::XmlReader>& xml, Button::VisualState state) const
{	
	switch(state)
	{
	case Button::Normal:
		return xml->GetFloat4("defaultUV");
		break;
	case Button::Pressed:
		return xml->GetFloat4("pressedUV");
		break;
	case Button::MouseOver:
		return xml->GetFloat4("mouseOverUV");
		break;
	case Button::Disabled:
		return xml->GetFloat4("disabledUV");
		break;
	default:
		n_error("Wrong visual state requested in UIXmlParser::ElementGetUV");
	}
	// dummy return, to prevent compiler waring
	return Math::float4(0,0,0,0);
}

//------------------------------------------------------------------------------
/**
*/
float 
UIXmlParser::ElementGetBlinkTime(Ptr<IO::XmlReader>& xml) const
{
    if(xml->HasAttr("blinkTime"))		return xml->GetFloat("blinkTime");
    return 0.0;
}

//------------------------------------------------------------------------------
/**
*/
Math::float4 
UIXmlParser::ElementGetBlinkColor(Ptr<IO::XmlReader>& xml) const
{
    if(xml->HasAttr("blinkColor"))		return xml->GetFloat4("blinkColor");
    return Math::float4(0,0,0,1);
}

//------------------------------------------------------------------------------
/**
*/
bool 
UIXmlParser::ElementGetBlinking(Ptr<IO::XmlReader>& xml) const
{
    if(xml->HasAttr("blinking"))		return xml->GetBool("blinking");
    return false;
}

//------------------------------------------------------------------------------
/**
    Recursively link all neighbor elements of all children elements
*/
void 
UIXmlParser::LinkElementNeighbors(const Ptr<Element>& element)
{
    element->LinkNeighbours();

    // go thru children
    const Util::Array<Ptr<Element>>& children = element->GetChildElements();
    IndexT i;
    for (i = 0; i < children.Size(); ++i)
    {
    	this->LinkElementNeighbors(children[i]);
    }
}
} // namespace UI
