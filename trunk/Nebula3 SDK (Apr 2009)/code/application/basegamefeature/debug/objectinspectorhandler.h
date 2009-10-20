#pragma once
//------------------------------------------------------------------------------
/**
    @class Debug::ObjectInspectorHandler
    
    Provide information about all registered game entities to 
    the debug http server.
    
    (C) 2007 Radon Labs GmbH
*/
#include "http/httprequesthandler.h"

namespace Http
{
    class HtmlPageWriter;
}
//------------------------------------------------------------------------------
namespace Debug
{
class ObjectInspectorHandler : public Http::HttpRequestHandler
{
    __DeclareClass(ObjectInspectorHandler);
public:
    /// constructor
    ObjectInspectorHandler();
    /// handle a http request, the handler is expected to fill the content stream with response data
    virtual void HandleRequest(const Ptr<Http::HttpRequest>& request);

private:
    void WriteEntityInfo( const Util::String &category, const Ptr<Http::HtmlPageWriter>& htmlWriter );

};

} // namespace Debug
//------------------------------------------------------------------------------

    