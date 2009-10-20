//------------------------------------------------------------------------------
//  resourcedictionarybuilder.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "resourcedictionarybuilder.h"
#include "io/ioserver.h"
#include "io/stream.h"
#include "io/binarywriter.h"
#include "system/byteorder.h"

namespace ToolkitUtil
{
using namespace Util;
using namespace IO;
using namespace System;

//------------------------------------------------------------------------------
/**
*/
ResourceDictionaryBuilder::ResourceDictionaryBuilder() :
    platform(Platform::Win32)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ResourceDictionaryBuilder::BuildDictionary()
{
    n_assert(this->texDir.IsValid());
    n_assert(this->dict.IsEmpty());

    if (!this->AddTexturesToDictionary())
    {
        return false;
    }
    if (!this->SaveDictionary())
    {
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
ResourceDictionaryBuilder::AddTexturesToDictionary()
{
    // for each category...
    Array<String> categories = IoServer::Instance()->ListDirectories(this->texDir, "*");
    IndexT catIndex;
    for (catIndex = 0; catIndex < categories.Size(); catIndex++)
    {
        // ignore revision control system dirs
        if ((categories[catIndex] != "CVS") && (categories[catIndex] != ".svn"))
        {
            // for each texture in category...
            String catDir;
            catDir.Format("%s/%s", this->texDir.AsCharPtr(), categories[catIndex].AsCharPtr());
            String pattern;
            switch (this->platform)
            {
                case Platform::Xbox360: pattern = "*.ntx"; break;
                case Platform::Wii:     pattern = "*.tpl"; break;
                default:                pattern = "*.dds"; break;
            }
            Array<String> files = IoServer::Instance()->ListFiles(catDir, pattern);
            IndexT fileIndex;
            for (fileIndex = 0; fileIndex < files.Size(); fileIndex++)
            {
                // build abs path and resource id string
                String absPath;
                absPath.Format("%s/%s/%s", this->texDir.AsCharPtr(), categories[catIndex].AsCharPtr(), files[fileIndex].AsCharPtr());
                String resId;
                resId.Format("tex:%s/%s", categories[catIndex].AsCharPtr(), files[fileIndex].AsCharPtr());

                // get the resource file size, we don't care about header overhead
                SizeT fileSize = 0;
                Ptr<Stream> stream = IoServer::Instance()->CreateStream(absPath);
                stream->SetAccessMode(Stream::ReadAccess);
                if (stream->Open())
                {
                    fileSize = stream->GetSize();
                    stream->Close();
                }
                stream = 0;

                // check for errors
                bool skip = false;
                if (0 == fileSize)
                {
                    skip = true;
                    n_printf("ERROR, SKIPPED RESOURCE %s because could not get file size or file size is 0, RESOURCE IGNORED!\n", absPath.AsCharPtr());
                }
                if (resId.Length() >= MaxResIdLength)
                {
                    skip = true;
                    n_printf("ERROR, SKIPPED RESOURCE %s because is res id is longer then %d bytes!\n", resId.AsCharPtr(), MaxResIdLength);                    
                }
                if (!skip)
                {
                    n_printf("Adding '%s' (%d bytes) to dictionary.\n", resId.AsCharPtr(), fileSize);
                    this->dict.Add(resId, fileSize);
                }
            }
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
ResourceDictionaryBuilder::SaveDictionary()
{
    String dstPath;
    dstPath.Format("dst:resources.dic");

    Ptr<Stream> stream = IoServer::Instance()->CreateStream(dstPath);
    Ptr<BinaryWriter> writer = BinaryWriter::Create();
    writer->SetStream(stream);
    if (Platform::Win32 == this->platform)
    {
        writer->SetStreamByteOrder(ByteOrder::LittleEndian);
    }
    else
    {
        writer->SetStreamByteOrder(ByteOrder::BigEndian);
    }
    if (writer->Open())
    {
        // write header
        FourCC magic('RDIC');
        writer->WriteUInt(magic.AsUInt());      // magic number 'RDIC'
        writer->WriteUInt(1);                   // version (0001)
        writer->WriteUInt(this->dict.Size());   // number of entries in the dictionary

        // for each dictionary entry...
        IndexT i;
        for (i = 0; i < this->dict.Size(); i++)
        {
            const StringAtom& resId = this->dict.KeyAtIndex(i);
            SizeT fileSize = this->dict.ValueAtIndex(i);
            n_assert(resId.Value().Length() < MaxResIdLength);
            n_assert(fileSize > 0);

            // copy resource id content to a memory buffer
            uchar buf[MaxResIdLength] = { 0 };
            Memory::Copy(resId.Value().AsCharPtr(), buf, resId.Value().Length());

            // write current entry to file
            writer->WriteUInt(fileSize);
            writer->WriteRawData(buf, sizeof(buf));
        }
        writer->Close();
        return true;
    }
    else
    {
        n_printf("ERROR: couldn't open '%s' for writing!\n");
        return false;
    }
}

} // namespace ToolkitUtil
