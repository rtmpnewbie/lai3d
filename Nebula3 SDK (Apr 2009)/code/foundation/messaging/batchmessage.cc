//------------------------------------------------------------------------------
//  batchmessage.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "messaging/batchmessage.h"

namespace Messaging
{
__ImplementClass(Messaging::BatchMessage, 'MSGB', Messaging::Message);
__ImplementMsgId(BatchMessage);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
BatchMessage::BatchMessage() :
    messages(256, 256)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
BatchMessage::AddMessage(const Ptr<Message>& msg)
{
    this->messages.Append(msg);
}

//------------------------------------------------------------------------------
/**
*/
const Array<Ptr<Message> >&
BatchMessage::GetMessages() const
{
    return this->messages;
}

} // namespace Messaging
