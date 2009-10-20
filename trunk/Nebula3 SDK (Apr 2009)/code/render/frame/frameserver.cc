//------------------------------------------------------------------------------
//  frameserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "frame/frameserver.h"
#include "io/ioserver.h"
#include "io/uri.h"
#include "frame/frameshaderloader.h"

namespace Frame
{
__ImplementClass(Frame::FrameServer, 'FSRV', Core::RefCounted);
__ImplementSingleton(Frame::FrameServer);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
FrameServer::FrameServer() :
    isOpen(false)
{
    __ConstructSingleton;
}        

//------------------------------------------------------------------------------
/**
*/
FrameServer::~FrameServer()
{
    if (this->IsOpen())
    {
        this->Close();
    }
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
FrameServer::Open()
{
    n_assert(!this->IsOpen());
    this->isOpen = true;

    // load frame shaders
    Array<String> files = IoServer::Instance()->ListFiles("frame:", "*.xml");
    IndexT fileIndex;
    for (fileIndex = 0; fileIndex < files.Size(); fileIndex++)
    {
        URI uri("frame:" + files[fileIndex]);
        Array<Ptr<FrameShader> > frameShaders = FrameShaderLoader::LoadFrameShaders(uri);
        IndexT frameShaderIndex;
        for (frameShaderIndex = 0; frameShaderIndex < frameShaders.Size(); frameShaderIndex++)
        {
            const Ptr<FrameShader>& frameShader = frameShaders[frameShaderIndex];
            this->frameShaders.Add(frameShader->GetName(), frameShader);
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
FrameServer::Close()
{
    n_assert(this->IsOpen());

    // discard frame shaders
    IndexT i;
    for (i = 0; i < this->frameShaders.Size(); i++)
    {
        this->frameShaders.ValueAtIndex(i)->Discard();
    }
    this->frameShaders.Clear();
    this->isOpen = false;
}

} // namespace Frame
