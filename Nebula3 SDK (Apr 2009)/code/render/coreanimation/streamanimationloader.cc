//------------------------------------------------------------------------------
//  streamanimationloader.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreanimation/streamanimationloader.h"
#include "coreanimation/animresource.h"
#include "system/byteorder.h"

namespace CoreAnimation
{
__ImplementClass(CoreAnimation::StreamAnimationLoader, 'SANL', Resources::StreamResourceLoader);

using namespace IO;
using namespace Util;
using namespace System;
using namespace Math;

// legacy NAX2 file format structs
#if NEBULA3_LEGACY_SUPPORT
#pragma pack(push, 1)
struct Nax2Header
{
    uint magic;
    SizeT numGroups;
    SizeT numKeys;
};

struct Nax2Group
{
    SizeT numCurves;
    IndexT startKey;
    SizeT numKeys;
    SizeT keyStride;
    float keyTime;
    float fadeInFrames;
    int loopType;
    char metaData[512];
};

struct Nax2Curve
{
    int ipolType;
    int firstKeyIndex;
    int isAnim;
    float keyX;
    float keyY;
    float keyZ;
    float keyW;
};
#pragma pack(pop)
#endif

//------------------------------------------------------------------------------
/**
*/
bool
StreamAnimationLoader::SetupResourceFromStream(const Ptr<Stream>& stream)
{
    n_assert(stream.isvalid());
    n_assert(this->resource.isvalid());
    String fileExt = this->resource->GetResourceId().Value().GetFileExtension();
    #if NEBULA3_LEGACY_SUPPORT
    if (fileExt == "nax2")
    {
        return this->SetupFromNax2(stream);
    }
    else
    #endif
    if (fileExt == "nax3")
    {
        n_error("FIXME!");
        return false;
    }
    else if (fileExt == "nanim3")
    {
        n_error("FIXME!");
        return false;
    }
    else
    {
        n_error("CoreAnimation::StreamAnimationLoader: unrecognized file extension in '%s'\n", this->resource->GetResourceId().Value().AsCharPtr());
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    Setup the animation resource from a legacy Nebula2 binary
    animation file. 
    FIXME: NAX2 files don't contain clip names! Need to feed clip names
    from some external source...
*/
#if NEBULA3_LEGACY_SUPPORT
bool
StreamAnimationLoader::SetupFromNax2(const Ptr<Stream>& stream)
{
    const Ptr<AnimResource>& anim = this->resource.downcast<AnimResource>();
    n_assert(!anim->IsLoaded());
    stream->SetAccessMode(Stream::ReadAccess);
    if (stream->Open())
    {
        uchar* ptr = (uchar*) stream->Map();

        // read header
        ByteOrder byteOrder(ByteOrder::LittleEndian, ByteOrder::Host);
        Nax2Header* naxHeader = (Nax2Header*) ptr;
        byteOrder.ConvertInPlace<uint>(naxHeader->magic);
        byteOrder.ConvertInPlace<SizeT>(naxHeader->numGroups);
        byteOrder.ConvertInPlace<SizeT>(naxHeader->numKeys);
        ptr += sizeof(Nax2Header);

        // check magic value
        if (FourCC(naxHeader->magic) != 'NAX3')
        {
            n_error("nMemoryAnimation::SetupFromNax2(): '%s' has obsolete file format!", stream->GetURI().AsString().AsCharPtr());        
            return false;
        }
        n_assert(0 != naxHeader->numGroups);

        // setup animation clips
        anim->BeginSetupClips(naxHeader->numGroups);
        IndexT clipIndex;
        for (clipIndex = 0; clipIndex < naxHeader->numGroups; clipIndex++)
        {
            Nax2Group* naxGroup = (Nax2Group*) ptr;
            
            // endian conversion
            byteOrder.ConvertInPlace<SizeT>(naxGroup->numCurves);
            byteOrder.ConvertInPlace<IndexT>(naxGroup->startKey);
            byteOrder.ConvertInPlace<SizeT>(naxGroup->numKeys);
            byteOrder.ConvertInPlace<SizeT>(naxGroup->keyStride);
            byteOrder.ConvertInPlace<float>(naxGroup->keyTime);
            byteOrder.ConvertInPlace<float>(naxGroup->fadeInFrames);
            byteOrder.ConvertInPlace<int>(naxGroup->loopType);

            // construct placeholder clip name
            String clipName;
            clipName.Format("clip%d", clipIndex);

            // setup anim clip object
            AnimClip& clip = anim->Clip(clipIndex);
            clip.SetName(clipName);
            clip.SetNumCurves(naxGroup->numCurves);
            clip.SetNumKeys(naxGroup->numKeys);
            clip.SetKeyStride(naxGroup->keyStride);
            clip.SetKeyDuration(Timing::SecondsToTicks(naxGroup->keyTime));
            clip.SetStartKeyIndex(naxGroup->startKey);
            if (0 == naxGroup->loopType)
            {
                // Clamp
                clip.SetPreInfinityType(InfinityType::Constant);
                clip.SetPostInfinityType(InfinityType::Constant);
            }
            else
            {
                clip.SetPreInfinityType(InfinityType::Cycle);
                clip.SetPostInfinityType(InfinityType::Cycle);
            }

            // check if hotspots are used
            String metaDataString(naxGroup->metaData);
            String searchString("Hotspot");
            Array<String> metaDataParts = metaDataString.Tokenize(";");
            IndexT idxMetaDataParts;
            for (idxMetaDataParts = 0; idxMetaDataParts < metaDataParts.Size(); idxMetaDataParts++)
            {
                IndexT startIndex = metaDataParts[idxMetaDataParts].FindStringIndex("Hotspot");
                if(InvalidIndex != startIndex)
                {
                    // setup hotspots
                    String hotspotsString = metaDataParts[idxMetaDataParts].ExtractToEnd(startIndex + searchString.Length() + 1);
                    Array<String> hotspots = hotspotsString.Tokenize(">");

                    IndexT idxHotspot;
                    String name;
                    Timing::Time time;
                    Timing::Time timePerFrame = Timing::TicksToSeconds(clip.GetKeyDuration());

                    // add them
                    clip.BeginEvents(hotspots.Size());
                    for (idxHotspot = 0; idxHotspot < hotspots.Size(); idxHotspot++)
                    {
                        Array<String> hotspot = hotspots[idxHotspot].Tokenize("#");
                        
                        // fallback
                        if (1 == hotspot.Size() && hotspot[0].IsValidFloat())
                        {
                            name = "default";
                            time = hotspot[0].AsFloat() * timePerFrame;
                        }
                        else if (2 == hotspot.Size() && hotspot[1].IsValidFloat())
                        {
                            name = hotspot[0];
                            time = hotspot[1].AsFloat() * timePerFrame;
                        }
                        else
                        {
                            n_error("CoreAnimation::StreamAnimationLoader::SetupFromNax2() Hotspot has wrong format!\n");
                        }

                        // split string into category and name
                        Util::StringAtom category;
                        Util::Array<Util::String> strTokens = name.Tokenize(":");
                        if (strTokens.Size() == 2)
                        {
                            category = strTokens[0];
                            name = strTokens[1];
                            // leave category string in name for better 
                        }

                        // append hotspot
                        AnimEvent newEvent;
                        newEvent.SetName(name);
                        newEvent.SetCategory(category);
                        newEvent.SetTime(Timing::SecondsToTicks(time));
                        clip.AddEvent(newEvent);
                    }
                    // end adding
                    clip.EndEvents();
                }
            }
            ptr += sizeof(Nax2Group);
        }

        // setup clip curves
        for (clipIndex = 0; clipIndex < naxHeader->numGroups; clipIndex++)
        {
            AnimClip& clip = anim->Clip(clipIndex);
            IndexT curveIndex;
            for (curveIndex = 0; curveIndex < clip.GetNumCurves(); curveIndex++)
            {
                Nax2Curve* naxCurve = (Nax2Curve*) ptr;
                
                // endian conversion
                byteOrder.ConvertInPlace<int>(naxCurve->ipolType);
                byteOrder.ConvertInPlace<int>(naxCurve->firstKeyIndex);
                byteOrder.ConvertInPlace<int>(naxCurve->isAnim);
                byteOrder.ConvertInPlace<float>(naxCurve->keyX);
                byteOrder.ConvertInPlace<float>(naxCurve->keyY);
                byteOrder.ConvertInPlace<float>(naxCurve->keyZ);
                byteOrder.ConvertInPlace<float>(naxCurve->keyW);

                AnimCurve& animCurve = clip.CurveByIndex(curveIndex);
                bool isStaticCurve = (-1 == naxCurve->firstKeyIndex);
                if (isStaticCurve)
                {
                    animCurve.SetFirstKeyIndex(0);
                }
                else
                {
                    animCurve.SetFirstKeyIndex(naxCurve->firstKeyIndex);
                }
                animCurve.SetStatic(isStaticCurve);
                animCurve.SetStaticKey(float4(naxCurve->keyX, naxCurve->keyY, naxCurve->keyZ, naxCurve->keyW));
                animCurve.SetActive(0 != naxCurve->isAnim);
                
                // this is a hack, Nebula2 files usually always have translation,
                // rotation and scale (in that order)
                int type = curveIndex % 3;
                switch (type)
                {
                    case 0:
                        animCurve.SetCurveType(CurveType::Translation);
                        break;
                    case 1:
                        n_assert(2 == naxCurve->ipolType);   // nAnimation::IpolType::Quat
                        animCurve.SetCurveType(CurveType::Rotation);
                        break;
                    case 2:
                        animCurve.SetCurveType(CurveType::Scale);
                        break;
                }

                // advance to next NAX2 curve
                ptr += sizeof(Nax2Curve);
            }
        }
        anim->EndSetupClips();

        // finally load keys (endian convert if necessary)
        const Ptr<AnimKeyBuffer> animKeyBuffer = anim->SetupKeyBuffer(naxHeader->numKeys);
        void* keyPtr = animKeyBuffer->Map();
        Memory::Copy(ptr, keyPtr, animKeyBuffer->GetByteSize());

        #pragma warning(push)   
        #pragma warning(disable : 4127) // expression is constant
        if (ByteOrder::Host != ByteOrder::LittleEndian)
        {
            ByteOrder byteOrder(ByteOrder::LittleEndian, ByteOrder::Host);
            float* floatKeyPtr = (float*) keyPtr;
            IndexT keyIndex;
            SizeT numKeys = animKeyBuffer->GetNumKeys();
            for (keyIndex = 0; keyIndex < numKeys; keyIndex++)
            {
                byteOrder.ConvertInPlace<float>(floatKeyPtr[0]);
                byteOrder.ConvertInPlace<float>(floatKeyPtr[1]);
                byteOrder.ConvertInPlace<float>(floatKeyPtr[2]);
                byteOrder.ConvertInPlace<float>(floatKeyPtr[3]);
                floatKeyPtr += 4;
            }
        }
        #pragma warning(pop)
        animKeyBuffer->Unmap();
        stream->Unmap();
        stream->Close();
        return true;
    }
    return false;
}
#endif

} // namespace CoreAnimation
