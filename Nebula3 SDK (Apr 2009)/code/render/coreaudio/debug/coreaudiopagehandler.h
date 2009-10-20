#pragma once
#ifndef DEBUG_COREAUDIOPAGEHANDLER_H
#define DEBUG_COREAUDIOPAGEHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Debug::CoreAudioPageHandler
  
    Provide a HTML debug page for the CoreAudio subsystem.
    
    Usage:
    http://host/coreaudio
    
    (C) 2008 Radon Labs GmbH
*/    
#include "http/httprequesthandler.h"
#include "resources/resourceid.h"
#include "http/html/htmlpagewriter.h"
#include "http/httpstatus.h"

//------------------------------------------------------------------------------
namespace Debug
{
class CoreAudioPageHandler : public Http::HttpRequestHandler
{
    __DeclareClass(CoreAudioPageHandler);
public:
    /// constructor
    CoreAudioPageHandler();
    /// handle a http request, the handler is expected to fill the content stream with response data
    virtual void HandleRequest(const Ptr<Http::HttpRequest>& request);

private:
    /// handle a soundbankinfo request
    Http::HttpStatus::Code HandleSoundBankInfoRequest(const Resources::ResourceId& resId, const Ptr<IO::Stream>& responseStream);
};

} // namespace Debug
//------------------------------------------------------------------------------
#endif

