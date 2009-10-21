//------------------------------------------------------------------------------
//  d3d9streamtextureloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/d3d9/d3d9streamtextureloader.h"
#include "coregraphics/texture.h"
#include "coregraphics/renderdevice.h"
#include "io/ioserver.h"

#include "memory/memory.h"
#include "math/color.h"

namespace Direct3D9
{
__ImplementClass(Direct3D9::D3D9StreamTextureLoader, 'D9TL', Resources::StreamResourceLoader);

using namespace CoreGraphics;
using namespace Resources;
using namespace IO;
using namespace Util;
using namespace System;

//------------------------------------------------------------------------------
/**
    This method actually setups the Texture object from the data in the stream.
*/
bool
D3D9StreamTextureLoader::SetupResourceFromStream(const Ptr<Stream>& stream)
{
    n_assert(stream.isvalid());
    n_assert(stream->CanBeMapped());
    HRESULT hr;
    IDirect3DDevice9* d3d9Device = RenderDevice::Instance()->GetDirect3DDevice();
    n_assert(0 != d3d9Device);
    n_assert(this->resource->IsA(Texture::RTTI));
    const Ptr<Texture>& res = this->resource.downcast<Texture>();
    n_assert(!res->IsLoaded());

    stream->SetAccessMode(Stream::ReadAccess);
    if (stream->Open())
    {
        void* srcData = stream->Map();
        UINT srcDataSize = stream->GetSize();

        // first need to check image info whether to determine texture type
        D3DXIMAGE_INFO imageInfo = { 0 };
        hr = D3DXGetImageInfoFromFileInMemory(srcData, srcDataSize, &imageInfo);
        if (FAILED(hr))
        {
            n_error("D3D9StreamTextureLoader: failed to obtain image info from file '%s'!", this->resource->GetResourceId().Value().AsCharPtr());
            return false;
        }

        // load texture based on texture type
        if (D3DRTYPE_TEXTURE == imageInfo.ResourceType)
        {
            // mipmap usage test enabled? -> DEBUG ONLY!
            Ptr<Stream> mipMapStream;
            bool visualizeMipMaps = RenderDevice::Instance()->GetVisualizeMipMaps();
            if (visualizeMipMaps)
            {
                if ((imageInfo.Width == imageInfo.Height) &&
                    (imageInfo.Width >= 128) && (imageInfo.Width <= 2048) &&
                    ((imageInfo.Format == D3DFMT_DXT1) || (imageInfo.Format == D3DFMT_DXT3)) &&
                    (!Util::String::MatchPattern(stream->GetURI().AsString(), "*dx9*")) &&
                    (!Util::String::MatchPattern(stream->GetURI().AsString(), "*lightmap*")))
                {
                    // overwrite texture data with mipmap test texture
                    Util::String mipTestFilename;
                    mipTestFilename.Format("tex:system/miptest_%d.dds", (imageInfo.Width > imageInfo.Height) ? imageInfo.Width : imageInfo.Height);
                    // create stream
                    mipMapStream = IoServer::Instance()->CreateStream(IO::URI(mipTestFilename));
                    mipMapStream->SetAccessMode(Stream::ReadAccess);
                    mipMapStream->Open();
                    // overwrite data ptr
                    srcData = mipMapStream->Map();
                }
            }
            // load 2D texture
            IDirect3DTexture9* d3d9Texture = 0;
            hr = D3DXCreateTextureFromFileInMemory(d3d9Device, srcData, srcDataSize, &d3d9Texture);
            if (FAILED(hr))
            {
                n_error("D3D9StreamTextureLoader: D3DXCreateTextureFromFileInMemory() failed for file '%s'!", this->resource->GetResourceId().Value().AsCharPtr());
                return false;
            }
            res->SetupFromD3D9Texture(d3d9Texture);

            if (mipMapStream.isvalid() && visualizeMipMaps)
            {
                mipMapStream->Unmap();
                mipMapStream->Close();
            }
        }
        else if (D3DRTYPE_VOLUMETEXTURE == imageInfo.ResourceType)
        {
            // load 3D texture
            IDirect3DVolumeTexture9* d3d9VolumeTexture = 0;
            hr = D3DXCreateVolumeTextureFromFileInMemory(d3d9Device, srcData, srcDataSize, &d3d9VolumeTexture);
            if (FAILED(hr))
            {
                n_error("D3D9StreamTextureLoader: D3DXCreateVolumeTextureFromFileInMemory() failed for file '%s'!", this->resource->GetResourceId().Value().AsCharPtr());
                return false;
            }
            res->SetupFromD3D9VolumeTexture(d3d9VolumeTexture);
        }
        else if (D3DRTYPE_CUBETEXTURE == imageInfo.ResourceType)
        {
            // load cube texture
            IDirect3DCubeTexture9* d3d9CubeTexture = 0;
            hr = D3DXCreateCubeTextureFromFileInMemory(d3d9Device, srcData, srcDataSize, &d3d9CubeTexture);
            if (FAILED(hr))
            {
                n_error("D3D9StreamTextureLoader: D3DXCreateCubeTextureFromFileInMemory() failed for file '%s'!", this->resource->GetResourceId().Value().AsCharPtr());
                return false;
            }
            res->SetupFromD3D9CubeTexture(d3d9CubeTexture);
        }
        stream->Unmap();
        stream->Close();
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
bool
D3D9StreamTextureLoader::IsBLPTexture( const Ptr<Stream>& stream ) const
{
	Util::FourCC fourcc;
	stream->Read(&fourcc, sizeof(fourcc));
	// restore stream pointer
	stream->Seek(-int(sizeof(fourcc)), Stream::Current);

	return fourcc == String("BLP2");
}

//------------------------------------------------------------------------------
bool
D3D9StreamTextureLoader::LoadBLPTexture( const Ptr<Stream>& stream )
{
	enum
	{
		MaxMipmapNum = 16,
		PaletteColorNum = 256
	};
	struct BLP2Header
	{
		Util::FourCC id;				///< Always 'BLP2'
		uint type;						///< 0: JPEG compression, 1: Uncompressed or DirectX compression
		byte encoding;					///< 1: Uncompressed, 2: DirectX compression
		byte alphaDepth;				///< 0: No alpha channel, 1: 1 bit alpha, 8: 8 bit alpha
		byte alphaEncoding;				///< 0: DXT1 alpha (0 or 1 bit alpha), 1: DXT2/3 alpha (4 bit alpha), 7: DXT4/5 alpha (interpolated alpha)
		byte hasMips;					///< 0: No mip levels, 1: Mip levels present (the number of levels is determined by the image size)
		uint width;						///< Dimensions of the image in pixels (always a power of two)
		uint height;					///< Dimensions of the image in pixels (always a power of two)
		uint offsets[MaxMipmapNum];		///< Offset from the start of the file to the image data
		uint lengths[MaxMipmapNum];		///< Length in bytes of the image data
		uint palette[PaletteColorNum];	///< 4-byte BGRA color values for paletted textures (this field is present regardless of whether the texture actually uses palettes)
	};
	const Ptr<Texture>& res = this->resource.downcast<Texture>();
	IDirect3DDevice9* d3d9Device = RenderDevice::Instance()->GetDirect3DDevice();
	IDirect3DTexture9* d3d9Texture = NULL;
	D3DFORMAT d3d9Format;

	// read blp header from the stream
	BLP2Header blp2Header;
	stream->Read(&blp2Header, sizeof(BLP2Header));
	n_assert(blp2Header.id == String("BLP2"));

	// compute mipmap num
	SizeT numMipmaps = 1;
	uint minWidth = (blp2Header.width < blp2Header.height) ? blp2Header.width : blp2Header.height;
	if (blp2Header.hasMips)
	{
		for (; numMipmaps < MaxMipmapNum; numMipmaps++)
		{
			if (0 == blp2Header.offsets[numMipmaps] ||
				0 == blp2Header.lengths[numMipmaps] ||
				0 == (minWidth>>=1))
				break;
		}
	}

	// data format
	bool isCompressed = true;
	if (2 == blp2Header.encoding)
	{
		// compressed
		d3d9Format = D3DFMT_DXT1;
		if (8 == blp2Header.alphaDepth)
		{
			if ( 7== blp2Header.alphaEncoding)
				d3d9Format = D3DFMT_DXT5;
			else
				d3d9Format = D3DFMT_DXT3;
		}
	}
	else
	{
		// uncompressed
		d3d9Format = D3DFMT_A8R8G8B8;
		isCompressed = false;
	}
	// create d3d9 texture
	HRESULT hr;
	hr = D3DXCreateTexture(d3d9Device, blp2Header.width, blp2Header.height,
		numMipmaps, 0, d3d9Format, D3DPOOL_MANAGED, &d3d9Texture);
	if (FAILED(hr))
	{
		return false;
	}
	// read mipmaps data
	byte* buffer = (byte*)Memory::Alloc(Memory::DefaultHeap, blp2Header.lengths[0]);
	uint width = blp2Header.width;
	uint height = blp2Header.height;
	for (IndexT i = 0; i < numMipmaps; i++)
	{
		if (0 == width) width = 1;
		if (0 == height) height = 1;
		if (0 == blp2Header.offsets[i] || 0 == blp2Header.lengths[i])
			break;
		// read into buffer
		stream->Seek(blp2Header.offsets[i], Stream::Begin);
		stream->Read(buffer, blp2Header.lengths[i]);
		// copy to d3d9 texture
		D3DLOCKED_RECT d3dLockRect;
		hr = d3d9Texture->LockRect(i, &d3dLockRect, NULL, 0);
		if (SUCCEEDED(hr))
		{
			if (isCompressed)
			{
				Memory::Copy(buffer, d3dLockRect.pBits, blp2Header.lengths[i]);
			}
			else
			{
				uint* colors = (uint*)d3dLockRect.pBits;
				IndexT index = 0;
				for(uint h = 0; h < height; h++)
				{
					for (uint w = 0; w < height; w++)
					{
						uint colorRGBA = blp2Header.palette[buffer[index]];
						colors[index] = Math::color::from_rgba(colorRGBA).to_uint();
						++index;
					}
				}
			}
			d3d9Texture->UnlockRect(i);
		}
		// next mip level
		width >>= 1;
		height >>= 1;
	}
	Memory::Free(Memory::DefaultHeap, buffer);

	res->SetupFromD3D9Texture(d3d9Texture);
	stream->Close();
	return true;
}

} // namespace Direct3D9