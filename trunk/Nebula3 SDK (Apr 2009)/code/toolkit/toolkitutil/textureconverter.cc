//------------------------------------------------------------------------------
//  textureconverter.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "textureconverter.h"
#include "io/ioserver.h"
#include "io/textwriter.h"
#include "io/xmlwriter.h"
#include "toolkitutil/applauncher.h"
#include "toolkitutil/imageconverter.h"
#include "util/guid.h"

namespace ToolkitUtil
{
using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
TextureConverter::TextureConverter() :
    platform(Platform::Win32),
    force(false),
    quiet(false),
    valid(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TextureConverter::~TextureConverter()
{
    if (this->IsValid())
    {
        this->Discard();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
TextureConverter::Setup()
{
    n_assert(!this->IsValid());
    n_assert(this->texAttrTablePath.IsValid());
    n_assert(this->srcDir.IsValid());
    n_assert(this->dstDir.IsValid());

    // setup the texture attributes table
    if (!this->textureAttrTable.Setup(this->texAttrTablePath))
    {
        n_printf("ERROR: failed to open '%s'!", this->texAttrTablePath.AsCharPtr());
        return false;
    }
    this->valid = true;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
TextureConverter::Discard()
{
    n_assert(this->IsValid());
    this->valid = false;
    this->textureAttrTable.Discard();
}

//------------------------------------------------------------------------------
/**
    Perform a file time check to decide whether a texture must be
    converted.
*/
bool
TextureConverter::NeedsConversion(const String& srcPath, const String& dstPath)
{
    // file time check overriden?
    if (this->force)
    {
        return true;
    }

    // otherwise check file times of src and dst file
    IoServer* ioServer = IoServer::Instance();
    if (ioServer->FileExists(dstPath))
    {
        FileTime srcFileTime = ioServer->GetFileWriteTime(srcPath);
        FileTime dstFileTime = ioServer->GetFileWriteTime(dstPath);
        if (dstFileTime > srcFileTime)
        {
            // dst file newer then src file, don't need to convert
            return false;
        }
    }

    // fallthrough: dst file doesn't exist, or it is older then the src file
    return true;
}

//------------------------------------------------------------------------------
/**
    Prepares the actual conversion process, first checks if conversion
    is necessary, removes write protection on target file, and copies
    the file if the source is already in native format. Returns true
    if no further conversion is needed.
*/
bool
TextureConverter::PrepareConversion(const String& srcPath, const String& dstPath)
{
    IoServer* ioServer = IoServer::Instance();

    // check if we can skip conversion based on the file time stamps and force flag
    if (!this->NeedsConversion(srcPath, dstPath))
    {
        return true;
    }

    // remove read-only attr from dst file
    if (ioServer->FileExists(dstPath))
    {
        ioServer->SetReadOnly(dstPath, false);
    }

    // if destination file is already in native format, do a plain copy
    if (srcPath.GetFileExtension() == dstPath.GetFileExtension())
    {
        ioServer->CopyFile(srcPath, dstPath);
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Convert all texture categories in the source directory.
*/
bool
TextureConverter::ConvertAll()
{
    n_assert(this->IsValid());
    bool success = true;
    Array<String> categories = IoServer::Instance()->ListDirectories(this->srcDir, "*");
    IndexT i;
    for (i = 0; i < categories.Size(); i++)
    {
        // ignore revision control system dirs
        if ((categories[i] != "CVS") && (categories[i] != ".svn"))
        {
            success &= this->ConvertCategory(categories[i]);
        }
    }
    return success;
}

//------------------------------------------------------------------------------
/**
    Convert all textures in a given category.
*/
bool
TextureConverter::ConvertCategory(const String& categoryName)
{
    n_assert(this->IsValid());
    n_assert(categoryName.IsValid());
    bool success = true;

    // build absolute category path and get list of textures in category
    String catDir;
    catDir.Format("%s/%s", this->srcDir.AsCharPtr(), categoryName.AsCharPtr());
    Array<String> files = IoServer::Instance()->ListFiles(catDir, "*");

    // for each category texture...
    IndexT i;
    for (i = 0; i < files.Size(); i++)
    {
        if (files[i].CheckFileExtension("tga") ||
            files[i].CheckFileExtension("bmp") ||
            files[i].CheckFileExtension("dds") ||
            files[i].CheckFileExtension("psd"))
        {
            success &= this->ConvertCategoryTexture(categoryName, files[i]);
        }
    }
    return success;
}

//------------------------------------------------------------------------------
/**
    Convert a texture defined by category name and file name.
*/
bool
TextureConverter::ConvertCategoryTexture(const String& categoryName, const String& texName)
{
    n_assert(this->IsValid());
    n_assert(categoryName.IsValid());
    n_assert(texName.IsValid());

    // strip file extension from destination texture name
    String dstTexName = texName;
    dstTexName.StripFileExtension();

    // build an absolute source path and just call the actual texture conversion method
    String srcPath;
    srcPath.Format("%s/%s/%s", this->srcDir.AsCharPtr(), categoryName.AsCharPtr(), texName.AsCharPtr());
    bool success = this->ConvertTexture(srcPath, categoryName, dstTexName);
    return success;
}

//------------------------------------------------------------------------------
/**
    Convert a source texture defined by an absolute path into a destination
    texture defined by category name and texture name. The destination texture
    name may not have a file extension! This method will just branch to
    different platform-specific methods based on the selected target platform.
*/
bool
TextureConverter::ConvertTexture(const String& srcPath, const String& dstCategoryName, const String& dstTexName)
{
    n_assert(this->IsValid());
    n_assert(srcPath.IsValid());
    n_assert(dstCategoryName.IsValid());
    n_assert(dstTexName.IsValid());

    // first make sure the target directory exists
    String dstCategoryDir;
    dstCategoryDir.Format("%s/%s", this->dstDir.AsCharPtr(), dstCategoryName.AsCharPtr());
    IoServer::Instance()->CreateDirectory(dstCategoryDir);

    // select conversion method based on target platform
    switch (this->platform)
    {
        case Platform::Win32:
            return this->ConvertTextureWin32(srcPath, dstCategoryName, dstTexName);
        case Platform::Xbox360:
            return this->ConvertTextureXbox360(srcPath, dstCategoryName, dstTexName);
        case Platform::Wii:
            return this->ConvertTextureWii(srcPath, dstCategoryName, dstTexName);
        default:
            // unsupported platform
            return false;
    }
}

//------------------------------------------------------------------------------
/**
    Win32 texture conversion method. Uses nvdxt (the nVidia texture conversion
    tool) to convert from any format to dds.
*/
bool
TextureConverter::ConvertTextureWin32(const String& srcPath, const String& dstCategoryName, const String& dstTexName)
{
    n_assert(this->IsValid());
    n_assert(this->toolPath.IsValid());
    IoServer* ioServer = IoServer::Instance();

    // build the dst file name
    String dstPath;
    dstPath.Format("%s/%s/%s.dds", this->dstDir.AsCharPtr(), dstCategoryName.AsCharPtr(), dstTexName.AsCharPtr());
    if (this->PrepareConversion(srcPath, dstPath))
    {
        return true;
    }

    // get texture conversion attributes
    String texEntry;
    texEntry.Format("%s/%s.%s", dstCategoryName.AsCharPtr(), dstTexName.AsCharPtr(), srcPath.GetFileExtension().AsCharPtr());
    const TextureAttrs& attrs = this->textureAttrTable.GetEntry(texEntry);

    // build command line args for nvdxt
    String args = "-rescale lo -overwrite ";
    bool isDXT5NormalMap = false;
    if ((attrs.GetRGBPixelFormat() == TextureAttrs::DXT5NM) ||
        (attrs.GetRGBAPixelFormat() == TextureAttrs::DXT5NM))
    {
        args.Append("-dxt5nm ");
        isDXT5NormalMap = true;
    }
    else
    {
        args.Append("-24 ");
        args.Append(TextureAttrs::PixelFormatToString(attrs.GetRGBPixelFormat()));
        args.Append(" -32 ");
        args.Append(TextureAttrs::PixelFormatToString(attrs.GetRGBAPixelFormat()));
        args.Append(" ");
    }
    args.Append("-clampScale ");
    args.AppendInt(attrs.GetMaxWidth());
    args.Append(" ");
    args.AppendInt(attrs.GetMaxHeight());
    if (attrs.GetQuality() == TextureAttrs::Normal)
    {
        args.Append("-quality_normal");
    }
    else if (attrs.GetQuality() == TextureAttrs::High)
    {
        args.Append("-quality_production");
    }
    else if (isDXT5NormalMap)
    {
        // DXT5NM conversion has a bug in quick mode
        args.Append("-quality_normal");
    }
    else
    {
        // quick quality
        args.Append("-quick");
    }
    if (!attrs.GetGenMipMaps())
    {
        args.Append("-nomipmap ");
    }
    args.Append("-");
    args.Append(TextureAttrs::FilterToString(attrs.GetMipMapFilter()));
    args.Append(" ");
    if (isDXT5NormalMap)
    {
        args.Append(" -norm");
    }
    args.Append(" -Rescale");
    args.Append(TextureAttrs::FilterToString(attrs.GetScaleFilter()));
    args.Append(" ");
    args.Append("-file \"");
    args.Append(AssignRegistry::Instance()->ResolveAssignsInString(srcPath));
    args.Append("\" -outdir \"");
    args.Append(AssignRegistry::Instance()->ResolveAssignsInString(dstPath.ExtractDirName()));
    args.Append("\"");

    // launch nvdxt to perform the conversion
    AppLauncher appLauncher;
    appLauncher.SetExecutable(this->toolPath);
    appLauncher.SetWorkingDirectory(this->srcDir);
    appLauncher.SetArguments(args);
    appLauncher.SetNoConsoleWindow(this->quiet);
    if (!appLauncher.LaunchWait())
    {
        n_printf("WARNING: failed to launch converter tool '%s'!\n", this->toolPath.AsCharPtr());
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    Xbox360 texture conversion method.
*/
bool
TextureConverter::ConvertTextureXbox360(const String& srcPath, const String& dstCategoryName, const String& dstTexName)
{
    n_assert(this->IsValid());
    IoServer* ioServer = IoServer::Instance();

    // build the dst file name
    String dstPath;
    dstPath.Format("%s/%s/%s.ntx", this->dstDir.AsCharPtr(), dstCategoryName.AsCharPtr(), dstTexName.AsCharPtr());

    // prepare export, checks if export is necessary, copies file already in native format
    if (this->PrepareConversion(srcPath, dstPath))
    {
        return true;
    }

    // get texture conversion attributes
    String texEntry;
    texEntry.Format("%s/%s.%s", dstCategoryName.AsCharPtr(), dstTexName.AsCharPtr(), srcPath.GetFileExtension().AsCharPtr());
    const TextureAttrs& attrs = this->textureAttrTable.GetEntry(texEntry);

    // first create a temporary TGA file, since the tool only reads non-PSD-formats
    Guid guid;
    guid.Generate();
    String tgaPath;
    tgaPath.Format("temp:%s.tga", guid.AsString().AsCharPtr());
    ImageConverter imgConverter;
    imgConverter.SetSrcFile(srcPath);
    imgConverter.SetDstFile(tgaPath);
    imgConverter.SetMaxWidth(attrs.GetMaxWidth());
    imgConverter.SetMaxHeight(attrs.GetMaxHeight());
    ImageConverter::Result imgResult = imgConverter.Convert();
    if (ImageConverter::Success != imgResult)
    {
        n_printf("ERROR: failed to write temp TGA file '%s' from src '%s'\n", tgaPath.AsCharPtr(), dstPath.AsCharPtr());
        return false;
    }

    // select the right D3D pixel format
    String d3dFormat;
    if (imgConverter.GetDstNumChannels() == 3)
    {
        // RGB source texture
        switch (attrs.GetRGBPixelFormat())
        {
            case TextureAttrs::DXT1A:
            case TextureAttrs::DXT1C:  d3dFormat = "D3DFMT_DXT1"; break;
            case TextureAttrs::DXT3:   d3dFormat = "D3DFMT_DXT3"; break;
            case TextureAttrs::DXT5:   d3dFormat = "D3DFMT_DXT5"; break;
            case TextureAttrs::DXT5NM: d3dFormat = "D3DFMT_DXN"; break;
            case TextureAttrs::U8888:  d3dFormat = "D3DFMT_A8R8G8B8"; break;
            case TextureAttrs::U888:   d3dFormat = "D3DFMT_X8R8G8B8"; break;
            default:                   d3dFormat = "D3DFMT_X8R8G8B8"; break;
        }
    }
    else
    {
        // RGBA source texture
        switch (attrs.GetRGBAPixelFormat())
        {
            case TextureAttrs::DXT1A:
            case TextureAttrs::DXT1C:  d3dFormat = "D3DFMT_DXT1"; break;
            case TextureAttrs::DXT3:   d3dFormat = "D3DFMT_DXT3"; break;
            case TextureAttrs::DXT5:   d3dFormat = "D3DFMT_DXT5"; break;
            case TextureAttrs::DXT5NM: d3dFormat = "D3DFMT_DXN"; break;
            case TextureAttrs::U8888:  d3dFormat = "D3DFMT_A8R8G8B8"; break;
            case TextureAttrs::U888:   d3dFormat = "D3DFMT_X8R8G8B8"; break;
            default:                   d3dFormat = "D3DFMT_X8R8G8B8"; break;
        }
    }

    // create a resource-description-file for the Xbox360 bundler
    String rdfPath;
    rdfPath.Format("temp:%s.rdf", guid.AsString().AsCharPtr());
    Ptr<Stream> stream = ioServer->CreateStream(rdfPath);
    Ptr<XmlWriter> xmlWriter = XmlWriter::Create();
    xmlWriter->SetStream(stream);
    if (!xmlWriter->Open())
    {
        n_printf("ERROR: failed to open rdf file '%s' for writing!\n", rdfPath.AsCharPtr());
        return false;
    }
    xmlWriter->BeginNode("RDF");
    {
        xmlWriter->SetString("Version", "XPR2");
        xmlWriter->BeginNode("Texture");
        {
            String absTgaPath = URI(tgaPath).LocalPath();
            absTgaPath.ReplaceChars("/", '\\');
            xmlWriter->SetString("Name", dstTexName);
            xmlWriter->SetString("Source", absTgaPath);
            xmlWriter->SetString("Format", d3dFormat);
            if (attrs.GetGenMipMaps())
            {
                xmlWriter->SetInt("Levels", 0);
            }
            else
            {
                xmlWriter->SetInt("Levels", 1);
            }
        }
        xmlWriter->EndNode();
    }
    xmlWriter->EndNode();
    xmlWriter->Close();
    xmlWriter = 0;
    stream = 0;

    // setup app launcher
    String args;
    String absRdfPath = URI(rdfPath).LocalPath();
    String absDstPath = URI(dstPath).LocalPath();
    absRdfPath.ReplaceChars("/", '\\');
    absDstPath.ReplaceChars("/", '\\');
    args.Format("%s /o %s", absRdfPath.AsCharPtr(), absDstPath.AsCharPtr());
    bool retval = true;
    AppLauncher appLauncher;
    appLauncher.SetExecutable(this->toolPath);
    appLauncher.SetWorkingDirectory(this->srcDir);
    appLauncher.SetArguments(args);
    appLauncher.SetNoConsoleWindow(this->quiet);
    if (!appLauncher.LaunchWait())
    {
        n_printf("WARNING: failed to launch converter tool '%s'!\n", this->toolPath.AsCharPtr());
        retval = false;
    }

    // delete temporary files
    ioServer->DeleteFile(tgaPath);
    ioServer->DeleteFile(rdfPath);
    return retval;
}

//------------------------------------------------------------------------------
/**
    Xbox360 texture conversion method.
*/
bool
TextureConverter::ConvertTextureWii(const String& srcPath, const String& dstCategoryName, const String& dstTexName)
{
    n_assert(this->IsValid());
    n_assert(this->toolPath.IsValid());
    IoServer* ioServer = IoServer::Instance();

    // ignore bump/spec/emissive maps
    if (String::MatchPattern(srcPath, "*_bump.*") ||
        String::MatchPattern(srcPath, "*_spec.*") ||
        String::MatchPattern(srcPath, "*_emsv.*"))
    {
        n_printf("IGNORE %s\n", srcPath.AsCharPtr());
        return true;
    }

    // build the dst file name
    String dstPath;
    dstPath.Format("%s/%s/%s.tpl", this->dstDir.AsCharPtr(), dstCategoryName.AsCharPtr(), dstTexName.AsCharPtr());

    // prepare export, checks if export is necessary, copies file already in native format
    if (this->PrepareConversion(srcPath, dstPath))
    {
        return true;
    }

    // get texture conversion attributes
    String texEntry;
    texEntry.Format("%s/%s.%s", dstCategoryName.AsCharPtr(), dstTexName.AsCharPtr(), srcPath.GetFileExtension().AsCharPtr());
    const TextureAttrs& attrs = this->textureAttrTable.GetEntry(texEntry);

    // first create a temporary TGA file, since the tool only reads TGA
    Guid guid;
    guid.Generate();
    String tgaPath;
    tgaPath.Format("temp:%s.tga", guid.AsString().AsCharPtr());
    ImageConverter imgConverter;
    imgConverter.SetSrcFile(srcPath);
    imgConverter.SetDstFile(tgaPath);
    imgConverter.SetMaxWidth(attrs.GetMaxWidth());
    imgConverter.SetMaxHeight(attrs.GetMaxHeight());
    ImageConverter::Result imgResult = imgConverter.Convert();
    if (ImageConverter::Success != imgResult)
    {
        n_printf("ERROR: failed to write temp TGA file '%s' from src '%s'\n", tgaPath.AsCharPtr(), dstPath.AsCharPtr());
        return false;
    }

    // create a unique script file for the texture conversion tool (use a GUID as file name
    // because several texture converters could be running in parallel
    String scriptPath;
    scriptPath.Format("temp:%s.tcs", guid.AsString().AsCharPtr());
    Ptr<Stream> stream = ioServer->CreateStream(scriptPath);
    Ptr<TextWriter> textWriter = TextWriter::Create();
    textWriter->SetStream(stream);
    if (!textWriter->Open())
    {
        n_printf("ERROR: failed to open script file '%s' for writing!\n", scriptPath.AsCharPtr());
        return false;
    }
    textWriter->WriteFormatted("file 0 = %s\n\n", URI(tgaPath).LocalPath().AsCharPtr());

    // second line: texture format settings
    // image 0 uses color and alpha from file 0
    if (imgConverter.GetDstNumChannels() == 4)
    {
        // image has alpha channel
        textWriter->WriteString("image\t0 = 0, 0, ");
    }
    else
    {
        // image has no alpha channel
        textWriter->WriteString("image\t0 = 0, x, ");
    }
    
    // select a suitable pixel format
    switch (attrs.GetRGBPixelFormat())
    {
        case TextureAttrs::U8888:
        case TextureAttrs::U888:
            textWriter->WriteString("RGBA8, ");
            break;

        case TextureAttrs::U1555:
        case TextureAttrs::U4444:
        case TextureAttrs::U565:
            textWriter->WriteString("RGB5A3, ");
            break;

        default:
            textWriter->WriteString("CMPR, ");
            break;
    }

    if (attrs.GetGenMipMaps())
    {
        // compute required number of mip channels
        SizeT imgSize = min(imgConverter.GetDstWidth(), imgConverter.GetDstHeight());
        SizeT numMips = 0;
        while ((imgSize >>= 1) > 0)
        {
            numMips++;
        }

        // default: generate 8 mipmaps (0-7) and remap to 0
        textWriter->WriteFormatted("0, %d, 0", numMips);
    }
    textWriter->WriteString("\n\n");

    // third line: texture is at index 0 of tpl file, no color lookup table used
    textWriter->WriteLine("texture\t0 = 0, x \n");
    textWriter->Close();
    textWriter = 0;
    stream = 0;

    // setup app launcher
    String args;
    args.Format("%s %s", URI(scriptPath).LocalPath().AsCharPtr(), URI(dstPath).LocalPath().AsCharPtr());
    bool retval = true;
    AppLauncher appLauncher;
    appLauncher.SetExecutable(this->toolPath);
    appLauncher.SetWorkingDirectory(this->srcDir);
    appLauncher.SetArguments(args);
    appLauncher.SetNoConsoleWindow(this->quiet);
    if (!appLauncher.LaunchWait())
    {
        n_printf("WARNING: failed to launch converter tool '%s'!\n", this->toolPath.AsCharPtr());
        retval = false;
    }

    // delete temporary files
    ioServer->DeleteFile(tgaPath);
    ioServer->DeleteFile(scriptPath);
    return retval;
}

} // namespace ToolkitUtil