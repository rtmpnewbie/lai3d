//------------------------------------------------------------------------------
//  idlprotocol.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/idlcompiler/idlprotocol.h"

namespace Tools
{
__ImplementClass(Tools::IDLProtocol, 'ILPR', Core::RefCounted);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
IDLProtocol::IDLProtocol()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
IDLProtocol::Parse(XmlReader* reader)
{
    n_assert(0 != reader);
    n_assert(reader->GetCurrentNodeName() == "Protocol");

    // parse attributes
    this->name = reader->GetString("name");
    this->nameSpace = reader->GetString("namespace");
    
    // parse dependency definitions
    if (reader->SetToFirstChild("Dependency")) do
    {
        Ptr<IDLDependency> dep = IDLDependency::Create();
        if (!dep->Parse(reader))
        {
            this->SetError(dep->GetError());
            return false;
        }
        this->dependencies.Append(dep);
    }
    while (reader->SetToNextChild("Dependency"));
    
    // parse message definitions
    if (reader->SetToFirstChild("Message")) do
    {
        Ptr<IDLMessage> cmd = IDLMessage::Create();
        if (!cmd->Parse(reader))
        {
            this->SetError(cmd->GetError());
            return false;
        }
        this->messages.Append(cmd);
    }
    while (reader->SetToNextChild("Message"));
    return true;
}

} // namespace Tools