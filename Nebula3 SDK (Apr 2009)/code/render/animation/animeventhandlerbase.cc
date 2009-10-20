//------------------------------------------------------------------------------
//  animeventhandlerbase.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "animation/animeventhandlerbase.h"

namespace Animation
{
__ImplementClass(Animation::AnimEventHandlerBase, 'AEHB', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
AnimEventHandlerBase::AnimEventHandlerBase():
isOpen(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AnimEventHandlerBase::~AnimEventHandlerBase()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
AnimEventHandlerBase::Open()
{
    n_assert(!this->isOpen);
    n_assert(this->categoryName.IsValid());

    // TODO overwrite in subclass

    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
AnimEventHandlerBase::Close()
{
    n_assert(this->isOpen);
    n_assert(this->categoryName.IsValid());

    // TODO overwrite in subclass

    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
bool
AnimEventHandlerBase::HandleEvent(const Animation::AnimEventInfo& event)
{
    n_assert(this->isOpen);

    // fallthrough: message was handled
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
AnimEventHandlerBase::OnFrame(Timing::Time time)
{
    // overwrite in subclass
}
} // namespace Animation
