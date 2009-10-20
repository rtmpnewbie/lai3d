//------------------------------------------------------------------------------
//  textreader.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/textreader.h"
#include <string.h>

namespace IO
{
__ImplementClass(IO::TextReader, 'TXTR', IO::StreamReader);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
unsigned char
TextReader::ReadChar()
{
    unsigned char c;
    this->stream->Read(&c, sizeof(c));
    return c;
}

//------------------------------------------------------------------------------
/**
*/
String
TextReader::ReadLine()
{
    String result;
    if (this->stream->Eof())
    {
        return result;
    }
    
    // read file contents in chunks of 128 bytes
    const int bufSize = 64;
    char* buf = (char*) Memory::Alloc(Memory::ScratchHeap, bufSize);

    // read chunk until newline found
    bool newLineFound = false;
    while (!(newLineFound || this->stream->Eof()))
    {
        // fill buffer with contents, properly terminate as string 
        Stream::Size bytesRead = this->stream->Read(buf, bufSize - 1);
        buf[bytesRead] = 0;
     
        // search newline, if found, terminate string at newline
        char* newLinePtr = strchr(buf, '\n');
        if (newLinePtr)
        {
            *newLinePtr = 0;
            newLineFound = true;

            // position stream at the position after the newline
            int newLinePos = (int) (newLinePtr - buf);
            Stream::Offset offset = (newLinePos - bytesRead) + 1;
            this->stream->Seek(offset, Stream::Current);
        }
        result.Append(buf);
    }
    Memory::Free(Memory::ScratchHeap, buf);
    return result;
}

//------------------------------------------------------------------------------
/**
*/
String
TextReader::ReadAll()
{
    this->stream->Seek(0, Stream::Begin);
    Stream::Size size = this->stream->GetSize();
    char* buf = (char*) Memory::Alloc(Memory::ScratchHeap, size + 1);
    Stream::Size bytesRead = this->stream->Read(buf, size);
    buf[bytesRead] = 0;
    String result = buf;
    Memory::Free(Memory::ScratchHeap, buf);
    return result;
}

//------------------------------------------------------------------------------
/**
*/
Array<String>
TextReader::ReadAllLines()
{
    Array<String> result;
    while (!stream->Eof())
    {
        result.Append(this->ReadLine());
    }
    return result;
}

} // namespace IO
