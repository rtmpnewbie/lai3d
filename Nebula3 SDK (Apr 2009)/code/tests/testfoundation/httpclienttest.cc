//------------------------------------------------------------------------------
//  httpclienttest.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "httpclienttest.h"
#include "http/httpclient.h"
#include "io/memorystream.h"
#include "io/ioserver.h"

namespace Test
{
__ImplementClass(Test::HttpClientTest, 'htct', Test::TestCase);

using namespace Util;
using namespace IO;
using namespace Http;

//------------------------------------------------------------------------------
/**
*/
void
HttpClientTest::Run()
{
    Ptr<IoServer> ioServer = IoServer::Create();
    Ptr<HttpClient> httpClient = HttpClient::Create();

    // build a few URIs we're interested in
    URI serverUri("http://www.radonlabs.de");
    URI imgUri("http://www.radonlabs.de/games/drakensang/screenshots/screenshot0001.jpg");

    bool connected = httpClient->Connect(serverUri);
    this->Verify(connected);
    this->Verify(httpClient->IsConnected());
    if (connected)
    {
        HttpStatus::Code status;
        Ptr<Stream> contentStream = IoServer::Instance()->CreateStream("home:screenshot.jpg");
        status = httpClient->SendRequest(HttpMethod::Get, imgUri, contentStream);
        this->Verify(HttpStatus::OK == status);

        httpClient->Disconnect();
        this->Verify(!httpClient->IsConnected());
    }
}

} // namespace Test