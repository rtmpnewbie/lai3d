//------------------------------------------------------------------------------
//  idlmessage.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/idlcompiler/idlmessage.h"

namespace Tools
{
__ImplementClass(Tools::IDLMessage, 'ILMS', Core::RefCounted);

using namespace IO;

//------------------------------------------------------------------------------
/**
*/
IDLMessage::IDLMessage() :
    parentClass("Messaging::Message")
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
IDLMessage::Parse(XmlReader* reader)
{
    n_assert(0 != reader);
    n_assert(reader->GetCurrentNodeName() == "Message");

    // parse attributes
    this->name = reader->GetString("name");
    this->fourcc = reader->GetString("fourcc");
    if (reader->HasAttr("derivedFrom"))
    {
        this->parentClass = reader->GetString("derivedFrom");
    }

    // parse input args
    if (reader->SetToFirstChild("InArg")) do
    {
        Ptr<IDLArg> arg = IDLArg::Create();
        if (!arg->Parse(reader))
        {
            this->SetError(arg->GetError());
            return false;
        }
        this->inArgs.Append(arg);
    }
    while (reader->SetToNextChild("InArg"));

    // parse output args
    if (reader->SetToFirstChild("OutArg")) do
    {
        Ptr<IDLArg> arg = IDLArg::Create();
        if (!arg->Parse(reader))
        {
            this->SetError(arg->GetError());
            return false;
        }
        this->outArgs.Append(arg);
    }
    while (reader->SetToNextChild("OutArg"));
    return true;
}

} // namespace Tools