//------------------------------------------------------------------------------
//  proxytest.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "proxytest.h"
#include "util/proxy.h"
#include "util/string.h"

namespace Test
{
__ImplementClass(Test::ProxyTest, 'prxt', Test::TestCase);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
ProxyTest::Run()
{
    String bla("Bla");
    String blub("Blub");

    Proxy<String> proxy0(bla);
    Proxy<String> proxy1(blub);
    Proxy<String> proxy2(bla);
    this->Verify(proxy0.GetObjectRefCount() == 1);
    this->Verify(proxy1.GetObjectRefCount() == 1);
    this->Verify(proxy2.GetObjectRefCount() == 1);

    Proxy<String> proxy3(proxy0);
    Proxy<String> proxy4(proxy1);
    Proxy<String> proxy5(proxy2);
    this->Verify(proxy0.GetObjectRefCount() == 2);
    this->Verify(proxy1.GetObjectRefCount() == 2);
    this->Verify(proxy2.GetObjectRefCount() == 2);
    this->Verify(proxy3.GetObjectRefCount() == 2);
    this->Verify(proxy4.GetObjectRefCount() == 2);
    this->Verify(proxy5.GetObjectRefCount() == 2);

    this->Verify(proxy0 == proxy2);
    this->Verify(proxy0 != proxy1);
    this->Verify(proxy1 != proxy2);
    this->Verify(&proxy0.GetObject() != &proxy2.GetObject());

    this->Verify(proxy3 == proxy0);
    this->Verify(proxy4 == proxy1);
    this->Verify(proxy5 == proxy2);
    this->Verify(proxy5 == proxy0);
    this->Verify(proxy4 != proxy2);
    this->Verify(&proxy0.GetObject() == &proxy3.GetObject());

    proxy0 = proxy1;
    this->Verify(proxy3.GetObjectRefCount() == 1);
    proxy3 = proxy4;    // string in proxy 0 should now have been deleted
    this->Verify(&proxy0.GetObject() == &proxy3.GetObject());
    this->Verify(&proxy0.GetObject() == &proxy4.GetObject());
    this->Verify(proxy0.GetObjectRefCount() == 4);
}

}