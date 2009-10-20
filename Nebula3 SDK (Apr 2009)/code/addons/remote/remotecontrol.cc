//------------------------------------------------------------------------------
//  remotecontrol.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "remote/remotecontrol.h"
#include "remote/remoterequestreader.h"
#include "io/memorystream.h"
#include "io/xmlreader.h"


namespace Remote
{
__ImplementClass(Remote::RemoteControl, 'RECO', Core::RefCounted);
__ImplementSingleton(Remote::RemoteControl);

using namespace Util;
using namespace Net;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
RemoteControl::RemoteControl() :
    isOpen(false)
{
    __ConstructSingleton;
    this->ipAddress.SetHostName("any");
    this->ipAddress.SetPort(2101);
}

//------------------------------------------------------------------------------
/**
*/
RemoteControl::~RemoteControl()
{
    n_assert(!this->IsOpen());
    __DestructSingleton;        
}

//------------------------------------------------------------------------------
/**
*/
bool
RemoteControl::Open()
{
    n_assert(!this->isOpen);
    this->isOpen = true;
    
    // setup a new TcpServer object
    this->tcpServer = TcpServer::Create();
    this->tcpServer->SetAddress(this->ipAddress);
    bool success = this->tcpServer->Open();

    // create default request handerl
    this->defaultRequestHandler = DefaultRemoteRequestHandler::Create();

    return success;
}

//------------------------------------------------------------------------------
/**
*/
void
RemoteControl::Close()
{
    n_assert(this->isOpen);

    // clear pending requests
    this->pendingRequests.Clear();

    // destroy the default remote request handler
    this->defaultRequestHandler = 0;

    // remove request handlers
    this->requestHandlers.Clear();

    // shutdown TcpServer
    this->tcpServer->Close();
    this->tcpServer = 0;
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
RemoteControl::AttachRequestHandler(const Ptr<RemoteRequestHandler>& requestHandler)
{
    n_assert(requestHandler.isvalid());
    n_assert(this->isOpen);
    n_assert(!this->requestHandlers.Contains(requestHandler->GetControllerName()));
    this->requestHandlers.Add(requestHandler->GetControllerName(), requestHandler);
}

//------------------------------------------------------------------------------
/**
*/
void
RemoteControl::RemoveRequestHandler(const Ptr<RemoteRequestHandler>& requestHandler)
{
    n_assert(requestHandler.isvalid());
    n_assert(this->isOpen);
    this->requestHandlers.Erase(requestHandler->GetControllerName());
}

//------------------------------------------------------------------------------
/**
*/
void
RemoteControl::OnFrame()
{
    n_assert(this->isOpen);

    // handle pending client requests
    Array<Ptr<TcpClientConnection> > recvConns = this->tcpServer->Recv();
    IndexT i;
    for (i = 0; i < recvConns.Size(); i++)
    {
        if (!this->HandleRemoteRequest(recvConns[i]))
        {
            recvConns[i]->Shutdown();
        }
    }

    // handle processed remote requests
    for (i = 0; i < this->pendingRequests.Size();)
    {
        const Ptr<RemoteRequest>& remoteRequest = this->pendingRequests[i].remoteRequest;
        if (remoteRequest->Handled())
        {
            const Ptr<TcpClientConnection>& conn = this->pendingRequests[i].clientConnection;
            if (this->BuildRemoteResponse(conn, remoteRequest))
            {
                conn->Send();
            }
            this->pendingRequests.EraseIndex(i);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
RemoteControl::HandleRemoteRequest(const Ptr<TcpClientConnection>& clientConnection)  
{
    // decode the request
    Ptr<RemoteRequestReader> remoteRequestReader = RemoteRequestReader::Create();
    remoteRequestReader->SetStream(clientConnection->GetRecvStream());
    if (remoteRequestReader->Open())
    {
        remoteRequestReader->ReadRequest();
        remoteRequestReader->Close();
    }
    if (remoteRequestReader->IsValidRemoteRequest())
    {
        // create a content stream for the response
        Ptr<MemoryStream> responseContentStream = MemoryStream::Create();
        
        // build a RemoteRequest object
        Ptr<RemoteRequest> remoteRequest = RemoteRequest::Create();

        remoteRequest->SetXmlReader(remoteRequestReader->GetXmlReader());
        remoteRequest->SetResponseContentStream(responseContentStream.upcast<Stream>());
        remoteRequest->SetStatus(RemoteStatus::NotFound);

        // find a request handler which accepts the request
        Ptr<RemoteRequestHandler> requestHandler;
        Util::String controllerName = remoteRequestReader->GetControllerName();
        if (this->requestHandlers.Contains(controllerName))
        {
            requestHandler = this->requestHandlers[controllerName];
        }
        if (requestHandler.isvalid())
        {
            // asynchronously handle the request
            requestHandler->PutRequest(remoteRequest);
        }
        else
        {
            // no request handler accepts the request, let the default
            // request handler handle the request
            this->defaultRequestHandler->HandleRequest(remoteRequest);
            remoteRequest->SetHandled(true);
        }

        // append request to pending queue
        PendingRequest pendingRequest;
        pendingRequest.clientConnection = clientConnection;
        pendingRequest.remoteRequest = remoteRequest;
        this->pendingRequests.Append(pendingRequest);

        return true;
    }
    else
    {
        // the received data was not a valid Remote request
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
RemoteControl::BuildRemoteResponse(const Ptr<TcpClientConnection>& conn, const Ptr<RemoteRequest>& remoteRequest)
{
    Ptr<RemoteResponseWriter> responseWriter = RemoteResponseWriter::Create();
    responseWriter->SetStream(conn->GetSendStream());
    responseWriter->SetStatusCode(remoteRequest->GetStatus());
    if (RemoteStatus::OK != remoteRequest->GetStatus())
    {
        // an error occured, need to write an error message to the response stream
        Ptr<TextWriter> textWriter = TextWriter::Create();
        textWriter->SetStream(remoteRequest->GetResponseContentStream());
        textWriter->Open();
        textWriter->WriteFormatted("%s %s", RemoteStatus::ToString(remoteRequest->GetStatus()).AsCharPtr(), RemoteStatus::ToHumanReadableString(remoteRequest->GetStatus()).AsCharPtr());
        textWriter->Close();
        remoteRequest->GetResponseContentStream()->SetMediaType(MediaType("text/plain"));
    }
    if (remoteRequest->GetResponseContentStream()->GetSize() > 0)
    {
        remoteRequest->GetResponseContentStream()->GetMediaType().IsValid();
        responseWriter->SetContent(remoteRequest->GetResponseContentStream());
    }
    responseWriter->Open();
    responseWriter->WriteResponse();
    responseWriter->Close();
    return true;
}

} // namespace Remote

