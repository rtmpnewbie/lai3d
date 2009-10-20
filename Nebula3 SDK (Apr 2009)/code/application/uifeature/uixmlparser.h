#pragma once
#ifndef IO_UIXMLREADER_H
#define IO_UIXMLREADER_H
//------------------------------------------------------------------------------
/**
    @class IO::UIExcelXmlReader
    
    This reader parses the XML - ui file and creates the UI tree.
	    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "util/string.h"
#include "util/array.h"
#include "uifeature/elements/window.h"
#include "uifeature/elements/button.h"
namespace IO
{
	class XmlReader;
};

//------------------------------------------------------------------------------
namespace UI
{
class UIFactory;
class Window;

class UIXmlParser : public Core::RefCounted
{
    __DeclareClass(UIXmlParser);
public:
    /// constructor
    UIXmlParser();
    /// destructor
    virtual ~UIXmlParser();

	/// sets the filename of the ui.xml file
	void SetFileName(const Util::String& name);
	/// gets the filename
	const Util::String& GetFileName() const;

	/// opens the file and creates the gui structure
	const Util::Array<Ptr<Window>>& ParseFile();

private:
	/// parse the window node
	void ParseWindowNode(Ptr<IO::XmlReader>& xml);
	/// parse the button node
	void ParseCanvasNode(Ptr<IO::XmlReader>& xml, Ptr<Window>& parent);

	/// parses a single element
	void ParseChildElements(Ptr<IO::XmlReader>& xml, Ptr<Element>& parent);

	/// parse the button node
	void ParseButtonNode(Ptr<IO::XmlReader>& xml, Ptr<Element>& parent);
	/// parse the label node
	void ParseLabelNode(Ptr<IO::XmlReader>& xml, Ptr<Element>& parent);
    /// recursively link neighbours of element
    void LinkElementNeighbors(const Ptr<Element>& element);

	/// helper functions
	bool ElementGetVisible(Ptr<IO::XmlReader>& xml) const;
	bool ElementGetEnabled(Ptr<IO::XmlReader>& xml) const;
	Math::float2 ElementGetPosition(Ptr<IO::XmlReader>& xml) const;
	Math::float2 ElementGetSize(Ptr<IO::XmlReader>& xml) const;
	Util::String ElementGetId(Ptr<IO::XmlReader>& xml) const;	
	Math::float4 ElementGetDefaultUV(Ptr<IO::XmlReader>& xml) const;
	Math::float4 ElementGetVisualStateUV(Ptr<IO::XmlReader>& xml, Button::VisualState state) const;
    bool ElementGetBlinking(Ptr<IO::XmlReader>& xml) const;
    float ElementGetBlinkTime(Ptr<IO::XmlReader>& xml) const;
    Math::float4 ElementGetBlinkColor(Ptr<IO::XmlReader>& xml) const;

    /// optional
    Math::float4 ElementGetDefaultColor(Ptr<IO::XmlReader>& xml) const;
    bool ElementIgnoreInput(Ptr<IO::XmlReader>& xml) const;

	Util::String fileName;
	Util::Array<Ptr<Window>> windows;
	Ptr<UIFactory> factory;
	int windowCount;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
UIXmlParser::SetFileName(const Util::String& name)
{
	this->fileName = name;
}


//------------------------------------------------------------------------------
/**
*/
inline
const Util::String& 
UIXmlParser::GetFileName() const
{
	return this->fileName;
}

} // namespace UI
//------------------------------------------------------------------------------
#endif
