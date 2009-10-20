//------------------------------------------------------------------------------
//  defaultremoterequesthandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/coreserver.h"
#include "remote/defaultremoterequesthandler.h"
#include "timing/calendartime.h"
#include "remote/remotecontrol.h"

namespace Remote
{
__ImplementClass(Remote::DefaultRemoteRequestHandler, 'DRRH', Remote::RemoteRequestHandler);

using namespace Core;
using namespace Util;
using namespace Timing;

//------------------------------------------------------------------------------
/**
*/
void
DefaultRemoteRequestHandler::HandleRequest(const Ptr<RemoteRequest>& request)
{
    // don't accept anything then Get
    //if (RemoteMethod::Get != request->GetMethod())
    //{
    //    request->SetStatus(RemoteStatus::NotImplemented);
    //    return;
    //}

    //
    // implement request handle here!!!
    //
}

} // namespace Remote