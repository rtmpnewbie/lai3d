//------------------------------------------------------------------------------
//  n2converter.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "n2converter.h"
#include "toolkitutil/binarymodelwriter.h"
#include "toolkitutil/xmlmodelwriter.h"
#include "io/ioserver.h"
#include "math/point.h"
#include "math/vector.h"
#include "io/xmlreader.h"

namespace ToolkitUtil
{
using namespace Util;
using namespace IO;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
N2Converter::N2Converter() :
    platform(Platform::Win32),
    force(false),
    binary(false),
    nodeType(InvalidNodeType)
{
    // empty
    this->animInfo.animation.Clear();
    this->animInfo.key.Clear();
    this->animInfo.loopType.Clear();
    this->animInfo.paramName.Clear();
    this->animInfo.animatorNode = InvalidAnimatorNodeType;
    this->animInfo.animationGroup = -1;
    this->animInfo.vectorName.Clear();
    this->animInfo.posKey.Clear();
    this->animInfo.eulerKey.Clear();
    this->animInfo.scaleKey.Clear();
    this->animInfo.layer.Clear();
}

//------------------------------------------------------------------------------
/**
*/
N2Converter::~N2Converter()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
N2Converter::ConvertAll()
{
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
*/
bool
N2Converter::ConvertCategory(const String& category)
{
    n_assert(category.IsValid());
    bool success = true;

    // build absolute category path and get list of textures in category
    String catDir;
    catDir.Format("%s/%s", this->srcDir.AsCharPtr(), category.AsCharPtr());
    Array<String> files = IoServer::Instance()->ListFiles(catDir, "*.n2");

    // for each file...
    IndexT i;
    for (i = 0; i < files.Size(); i++)
    {
        success &= this->ConvertFile(category, files[i]);
    }
    return success;
}

//------------------------------------------------------------------------------
/**
*/
bool
N2Converter::ConvertFile(const String& category, const String& fileName)
{
    this->srcPath.Format("%s/%s/%s", this->srcDir.AsCharPtr(), category.AsCharPtr(), fileName.AsCharPtr());
    String dstFilename = fileName;
    dstFilename.StripFileExtension();
    if (this->binary)
    {
        dstFilename.Append(".n3");
    }
    else
    {
        dstFilename.Append(".xml");
    }
    this->dstPath.Format("%s/%s/%s", this->dstDir.AsCharPtr(), category.AsCharPtr(), dstFilename.AsCharPtr());
    
    // check if conversion is necessary
    if (!this->NeedsConversion(srcPath, dstPath))
    {
        return true;
    }

    // make sure the dst directory exists
    IoServer* ioServer = IoServer::Instance();
    String dstCatDir;
    dstCatDir.Format("%s/%s", this->dstDir.AsCharPtr(), category.AsCharPtr());
    ioServer->CreateDirectory(dstCatDir);

    // setup IO streams and stream readers/writers
    Ptr<Stream> srcStream = ioServer->CreateStream(srcPath);
    Ptr<BinaryReader> reader = BinaryReader::Create();
    reader->SetStream(srcStream);
    if (!reader->Open())
    {
        n_printf("Could not open '%s' for reading!\n", srcPath.AsCharPtr());
        return false;
    }
    Ptr<Stream> dstStream = ioServer->CreateStream(dstPath);
    Ptr<ModelWriter> modelWriter;
    if (this->binary)
    {
        modelWriter = BinaryModelWriter::Create();
    }
    else
    {
        modelWriter = XmlModelWriter::Create();
    }
    modelWriter->SetStream(dstStream);
    modelWriter->SetPlatform(this->platform);
    if (!modelWriter->Open())
    {
        n_printf("Could not open '%s' for writing!\n", dstPath.AsCharPtr());
        return false;
    }

    this->lastAnimatorName.Clear();
    this->animatedNodesToAnimator.Clear();
    this->animatorInformation.Clear();

    // perform actual conversion
    String modelName = category;
    modelName.Append("/");
    modelName.Append(fileName);
    modelName.StripFileExtension();
    if (!this->PerformConversion(modelName, reader, modelWriter))
    {
        n_printf("Conversion failed: '%s'!\n", srcPath.AsCharPtr());
        return false;
    }

    // done
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
N2Converter::PerformConversion(const String& curModelName, const Ptr<BinaryReader>& reader, const Ptr<ModelWriter>& writer)
{
    // first make sure the source file is actually a N2 file, and skip the header
    FourCC magic(reader->ReadUInt());
    if (magic != FourCC('NOB0'))
    {
        n_printf("Source file '%s' is not a Nebula2 binary file!\n", this->srcPath.AsCharPtr());
        return false;
    }
    
    // skip source header data
    String headerData = reader->ReadString();

    // recursively read nodes
    writer->BeginModel("Models::Model", FourCC('MODL'), curModelName);
    if (!this->ParseNodes(reader, writer))
    {
        n_printf("Conversion error in RecurseReadNodes() ('%s')!\n", this->srcPath.AsCharPtr());
        return false;
    }
    writer->EndModel();
    n_assert(this->nodeTypeStack.IsEmpty());
    
    // all done
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
N2Converter::ParseNodes(const Ptr<BinaryReader>& reader, const Ptr<ModelWriter>& writer)
{
    const FourCC newFourCC('_new');
    const FourCC selFourCC('_sel');
    FourCC curFourCC;
    while (!reader->Eof())
    {
        curFourCC = reader->ReadUInt();   
        if (curFourCC == newFourCC)
        {
            // save nodeinformation from last node
            this->FillAnimatorInformation();

            // new node encountered
            String objClass = reader->ReadString();
            String objName = reader->ReadString();
            this->SetAnimatorPath(objName, objClass);
            this->BeginNode(objClass, objName, writer);
        }
        else if (curFourCC == selFourCC)
        {
            // skip relative path '..'
            String selPath = reader->ReadString();
            // erase last path segment
            if (path.Size() > 0)
            {
                this->path.EraseIndex(path.Size()-1);     
            }            
            this->EndNode(writer);

            
        }
        else
        {
            // read current data tag
            this->ReadDataTag(curFourCC, reader, writer);
        }
    }
    if (/*!this->animatorInformation.IsEmpty()
        && */!this->animatedNodesToAnimator.IsEmpty())
    {
        // set the last animator node
        this->FillAnimatorInformation();

        // write optional animator node
        this->WriteAnimatorNode(writer);

        writer->EndModelNode();
    }

    // done
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void 
N2Converter::WriteAnimatorNode(const Ptr<ModelWriter>& writer)
{
    IndexT i;    
    writer->BeginModelNode("Model::AnimatorNode", FourCC('MANI'), "animator");
    for (i = 0; i < this->animatorInformation.Size(); i++)
    {
        AnimatorInfo& animInfo = this->animatorInformation.ValueAtIndex(i);

        // catch errors
        if (((animInfo.animatorNode == IntAnimator) ||
            (animInfo.animatorNode == FloatAnimator) ||
            (animInfo.animatorNode == Float4Animator)) &&
            (animInfo.paramName.IsEmpty()))
        {
            continue;
        }

        if (this->animatedNodesToAnimator.Contains(this->animatorInformation.KeyAtIndex(i)))
        {
            writer->BeginTag("BeginAnimSection", FourCC('BASE'));
            writer->WriteInt((int)animInfo.animatorNode);
            
            const Array<String>& pathToNodes = this->animatedNodesToAnimator[this->animatorInformation.KeyAtIndex(i)];
            IndexT pathIdx;
            for (pathIdx = 0; pathIdx < pathToNodes.Size(); pathIdx++)
            {
                writer->BeginTag("AnimatedNodes", FourCC('ANNO'));
                writer->WriteString(pathToNodes[pathIdx]);
                writer->EndTag();
            }

            if (!animInfo.animation.IsEmpty())
            {
                writer->BeginTag("AnimationName", FourCC('SANI'));
                writer->WriteString(animInfo.animation);
                writer->EndTag();
            }

            if (!animInfo.loopType.IsEmpty())
            {
                writer->BeginTag("LoopType", FourCC('SLPT'));
                writer->WriteString(animInfo.loopType);
                writer->EndTag();
            }

            if (!animInfo.paramName.IsEmpty())
            {
                writer->BeginTag("ParamName", FourCC('SPNM'));
                writer->WriteString(animInfo.paramName);
                writer->EndTag();
            }

            if (!animInfo.key.IsEmpty())
            {
                writer->BeginTag("Key", FourCC('ADDK'));
                //format
                switch(animInfo.key[0].value.GetType())
                {
                case Variant::Float:
                    writer->WriteString("Float");
                    break;
                case Variant::Float4:
                    writer->WriteString("Float4");
                    break;
                case Variant::Int:
                    writer->WriteString("Int");
                    break;
                }
                //size
                writer->WriteInt((int)animInfo.key.Size());
                for(IndexT j = 0; j < animInfo.key.Size(); j++)
                {
                    writer->WriteFloat(animInfo.key[j].time);
                    switch (animInfo.key[j].value.GetType())
                    {
                    case Variant::Float:
                        writer->WriteFloat(animInfo.key[j].value.GetFloat());
                        break;
                    case Variant::Float4:
                        writer->WriteFloat4(animInfo.key[j].value.GetFloat4());
                        break;
                    case Variant::Int:
                        writer->WriteInt(animInfo.key[j].value.GetInt());
                        break;
                    }
                }

                writer->EndTag();
            }

            if (!animInfo.vectorName.IsEmpty())
            {
                writer->BeginTag("VectorName", FourCC('SVCN'));
                writer->WriteString(animInfo.vectorName);
                writer->EndTag();
            }

            if (animInfo.animationGroup > -1)
            {
                writer->BeginTag("AnimationGroup", FourCC('SAGR'));
                writer->WriteInt(animInfo.animationGroup);
                writer->EndTag();
            }

            if (!animInfo.posKey.IsEmpty())
            {
                writer->BeginTag("PositionKey", FourCC('ADPK'));
                if (animInfo.animatorNode == 5)  //uvAnimator
                {
                    writer->WriteInt((int)animInfo.posKey.Size());
                    for (IndexT j = 0; j < animInfo.posKey.Size(); j++)
                    {
                        writer->WriteInt(animInfo.layer[j]);
                        writer->WriteFloat(animInfo.posKey[j].time);
                        writer->WriteFloat4(animInfo.posKey[j].value.GetFloat4());
                    }
                }
                else    //transformanimator
                {
                    writer->WriteInt((int)animInfo.posKey.Size());
                    for(IndexT j = 0; j < animInfo.posKey.Size(); j++)
                    {
                        writer->WriteFloat(animInfo.posKey[j].time);
                        writer->WriteFloat4(animInfo.posKey[j].value.GetFloat4());
                    }
                }
                writer->EndTag();
            }

            if (!animInfo.eulerKey.IsEmpty())
            {
                writer->BeginTag("EulerKey", FourCC('ADEK'));
                if (animInfo.animatorNode == 5)
                {
                    writer->WriteInt((int)animInfo.eulerKey.Size());
                    for (IndexT j = 0; j < animInfo.eulerKey.Size(); j++)
                    {
                        writer->WriteInt(animInfo.layer[j]);
                        writer->WriteFloat(animInfo.eulerKey[j].time);
                        writer->WriteFloat4(animInfo.eulerKey[j].value.GetFloat4());
                    }
                }
                else 
                {
                    writer->WriteInt((int)animInfo.eulerKey.Size());
                    for(IndexT j = 0; j < animInfo.eulerKey.Size(); j++)
                    {
                        writer->WriteFloat(animInfo.eulerKey[j].time);
                        writer->WriteFloat4(animInfo.eulerKey[j].value.GetFloat4());
                    }
                }
                writer->EndTag();
            }

            if (!animInfo.scaleKey.IsEmpty())
            {
                writer->BeginTag("ScaleKey", FourCC('ADSK'));
                if (animInfo.animatorNode == 5)
                {
                    writer->WriteInt((int)animInfo.scaleKey.Size());
                    for (IndexT j = 0; j < animInfo.scaleKey.Size(); j++)
                    {
                        writer->WriteInt(animInfo.layer[j]);
                        writer->WriteFloat(animInfo.scaleKey[j].time);
                        writer->WriteFloat4(animInfo.scaleKey[j].value.GetFloat4());
                    }
                }
                else 
                {
                    writer->WriteInt((int)animInfo.scaleKey.Size());
                    for(IndexT j = 0; j < animInfo.scaleKey.Size(); j++)
                    {
                        writer->WriteFloat(animInfo.scaleKey[j].time);
                        writer->WriteFloat4(animInfo.scaleKey[j].value.GetFloat4());
                    }
                }
                writer->EndTag();
            }              
            writer->EndTag();   
        }
    }
    writer->EndModelNode();
}

//------------------------------------------------------------------------------
/**
*/
void
N2Converter::BeginNode(const String& objClass, const String& objName, const Ptr<ModelWriter>& writer)
{
    // check for known classes
    if (objClass == "ntransformnode")
    {
        this->nodeType = TransformNode;
        this->animNodeType = InvalidAnimatorNodeType;
        writer->BeginModelNode("Models::TransformNode", FourCC('TRFN'), objName);
    }
    else if (objClass == "nshapenode")
    {
        this->nodeType = ShapeNode;
        this->animNodeType = InvalidAnimatorNodeType;
        writer->BeginModelNode("Models::ShapeNode", FourCC('SPND'), objName);
    }    
    else if (objClass == "nmultilayerednode")
    {
        this->nodeType = ShapeNode;
        this->animNodeType = InvalidAnimatorNodeType;
        writer->BeginModelNode("Models::ShapeNode", FourCC('SPND'), objName);
    }
    else if (objClass == "ncharacter3node")
    {
        this->nodeType = Character3Node;
        this->animNodeType = InvalidAnimatorNodeType;
        writer->BeginModelNode("Characters::CharacterNode", FourCC('CHRN'), objName);
        this->ConvertSkinLists(writer);
    }
    else if (objClass == "ncharacter3skinshapenode")
    {
        this->nodeType = Character3SkinShapeNode;
        this->animNodeType = InvalidAnimatorNodeType;
        writer->BeginModelNode("Characters::CharacterSkinNode", FourCC('CHSN'), objName);
    }
    else if (objClass == "nintanimator")
    {
        this->nodeType = EmbeddedNodeType;
        this->animNodeType = IntAnimator;
        this->lastAnimatorName = this->ConvertRelPathToAbsolute(objName, true);
    }
    else if (objClass == "nfloatanimator")
    {
        this->nodeType = EmbeddedNodeType;
        this->animNodeType = FloatAnimator;
        this->lastAnimatorName = this->ConvertRelPathToAbsolute(objName, true);
    }
    else if (objClass == "nvectoranimator")
    {
        this->nodeType = EmbeddedNodeType;
        this->animNodeType = Float4Animator;
        this->lastAnimatorName = this->ConvertRelPathToAbsolute(objName, true);
    }
    else if (objClass == "ntransformanimator")
    {
        this->nodeType = EmbeddedNodeType;
        this->animNodeType = TransformAnimator;
        this->lastAnimatorName = this->ConvertRelPathToAbsolute(objName, true);
    }
    else if (objClass == "ntransformcurveanimator")
    {
        this->nodeType = EmbeddedNodeType;
        this->animNodeType = TransformCurveAnimator;
        this->lastAnimatorName = this->ConvertRelPathToAbsolute(objName, true);
    }
    else if (objClass == "nuvanimator")
    {
        this->nodeType = EmbeddedNodeType;
        this->animNodeType = UvAnimator;
        this->lastAnimatorName = this->ConvertRelPathToAbsolute(objName, true);
    }
    else if (objClass == "ncharacter3skinanimator")
    {
        this->nodeType = EmbeddedNodeType;
        this->animNodeType = InvalidAnimatorNodeType;
    }
    else if (objClass == "nparticleshapenode2")
    {
        this->nodeType = ParticleSystemNode;
        writer->BeginModelNode("Particles::ParticleSystemNode", FourCC('PSND'), objName);
    }
    else if (objClass == "nlodnode")
    {
        // use normal transformnode for nlodnode
        this->nodeType = TransformNode;
        this->animNodeType = InvalidAnimatorNodeType;
        writer->BeginModelNode("Models::TransformNode", FourCC('TRFN'), objName);
    }
    else
    {
        this->nodeType = InvalidNodeType;
        this->animNodeType = InvalidAnimatorNodeType;
    }
    this->nodeTypeStack.Push(this->nodeType);
}

//------------------------------------------------------------------------------
/**
*/
void
N2Converter::EndNode(const Ptr<ModelWriter>& writer)
{
    NodeType nodeType = this->nodeTypeStack.Pop();
    if ((nodeType != InvalidNodeType) && (nodeType != EmbeddedNodeType))
    {
        if (!this->nodeTypeStack.IsEmpty() || this->animatedNodesToAnimator.IsEmpty())
        {
            writer->EndModelNode();
        }
    }
    this->nodeType = InvalidNodeType;
}

//------------------------------------------------------------------------------
/**
*/
void
N2Converter::ReadDataTag(FourCC fourCC, const Ptr<BinaryReader>& reader, const Ptr<ModelWriter>& writer)
{
    ushort length = reader->ReadUShort();
    if (InvalidNodeType == this->nodeType)
    {
        // if current node type is unknown, skip the tag's data block 
        reader->GetStream()->Seek(length, Stream::Current);
    }
    else
    {
        if ((FourCC('SLCB') == fourCC) && (this->nodeType != EmbeddedNodeType))
        {
            // setlocalbox
            float x, y, z;
            writer->BeginTag("LocalBox", FourCC('LBOX'));
            x = reader->ReadFloat();
            y = reader->ReadFloat();
            z = reader->ReadFloat();
            writer->WriteFloat4(point(x, y, z));
            x = reader->ReadFloat();
            y = reader->ReadFloat();
            z = reader->ReadFloat();
            writer->WriteFloat4(vector(x, y, z));
            writer->EndTag();
        }
        else if (FourCC('SPOS') == fourCC)
        {
            // setposition
            writer->BeginTag("Position", FourCC('POSI'));
            float x = reader->ReadFloat();
            float y = reader->ReadFloat();
            float z = reader->ReadFloat();
            writer->WriteFloat4(point(x, y, z));
            writer->EndTag();
        }
        else if (FourCC('SQUT') == fourCC)
        {
            // setquat
            writer->BeginTag("Rotation", FourCC('ROTN'));
            writer->WriteFloat4(reader->ReadFloat4());
            writer->EndTag();
        }
        else if (FourCC('SSCL') == fourCC)
        {
            // setscale
            writer->BeginTag("Scale", FourCC('SCAL'));
            float x = reader->ReadFloat();
            float y = reader->ReadFloat();
            float z = reader->ReadFloat();
            writer->WriteFloat4(vector(x, y, z));
            writer->EndTag();
        }
        else if (FourCC('SRTP') == fourCC)
        {
            // setrotatepivot
            writer->BeginTag("RotatePivot", FourCC('RPIV'));
            float x = reader->ReadFloat();
            float y = reader->ReadFloat();
            float z = reader->ReadFloat();
            writer->WriteFloat4(point(x, y, z));
            writer->EndTag();
        }
        else if (FourCC('SSCP') == fourCC)
        {
            // setscalepivot
            writer->BeginTag("ScalePivot", FourCC('SPIV'));
            float x = reader->ReadFloat();
            float y = reader->ReadFloat();
            float z = reader->ReadFloat();
            writer->WriteFloat4(point(x, y, z));
            writer->EndTag();
        }
        else if (FourCC('SVSP') == fourCC)
        {
            // setviewspace flag
            writer->BeginTag("RelativeToViewSpace", FourCC('SVSP')); 
            reader->ReadBool();
            writer->EndTag();
        }
        else if (FourCC('SLKV') == fourCC)
        {
            // setlockviewer flag
            writer->BeginTag("LockViewer", FourCC('SLKV')); 
            writer->WriteBool(reader->ReadBool());
            writer->EndTag();
        }
        else if (FourCC('SMID') == fourCC)
        {
            // setmindistance flag
            writer->BeginTag("MinDistance", FourCC('SMID')); 
            writer->WriteFloat(reader->ReadFloat());
            writer->EndTag();
        }
        else if (FourCC('SMAD') == fourCC)
        {
            // setmaxdistance flag
            writer->BeginTag("MaxDistance", FourCC('SMAD')); 
            writer->WriteFloat(reader->ReadFloat());
            writer->EndTag();
        }
        else if (FourCC('STXT') == fourCC)
        {
            // settexture
            writer->BeginTag("ShaderTexture", FourCC('STXT'));
            writer->WriteString(reader->ReadString());

            String textureName = reader->ReadString();
            textureName.StripFileExtension();
            writer->WriteString(textureName);
            writer->EndTag();
        }
        else if (FourCC('SINT') == fourCC)
        {
            // setint
            writer->BeginTag("ShaderInt", FourCC('SINT'));
            writer->WriteString(reader->ReadString());
            writer->WriteInt(reader->ReadInt());
            writer->EndTag();
        }
        else if (FourCC('SFLT') == fourCC)
        {
            // setfloat
            writer->BeginTag("ShaderFloat", FourCC('SFLT'));
            writer->WriteString(reader->ReadString());
            writer->WriteFloat(reader->ReadFloat());
            writer->EndTag();
        }
        else if (FourCC('SVEC') == fourCC)
        {
            // setvector
            writer->BeginTag("ShaderVector", FourCC('SVEC'));
            writer->WriteString(reader->ReadString());
            writer->WriteFloat4(reader->ReadFloat4());
            writer->EndTag();
        }
        else if (FourCC('SSHD') == fourCC)
        {
            // setshader
            String shaderName = reader->ReadString();
            writer->BeginTag("Shader", FourCC('SHDR'));
            writer->WriteString(this->ConvertShaderName(shaderName));
            writer->EndTag();

            // map shader name to ModelNodeType
            writer->BeginTag("ModelNodeType", FourCC('MNTP'));
            writer->WriteString(this->MapShaderNameToModelNodeType(shaderName));
            writer->EndTag();
        }
        else if (FourCC('SMSH') == fourCC)
        {
            // setmesh
            writer->BeginTag("Mesh", FourCC('MESH'));
            writer->WriteString(reader->ReadString());
            writer->EndTag();
        }
        else if (FourCC('SGRI') == fourCC)
        {
            // setgroupindex
            writer->BeginTag("PrimitiveGroupIndex", FourCC('PGRI'));
            writer->WriteInt(reader->ReadInt());
            writer->EndTag();
        }
        else if (FourCC('SANM') == fourCC)
        {
            // setanim
            writer->BeginTag("Animation", FourCC('ANIM'));
            writer->WriteString(reader->ReadString());
            writer->EndTag();
        }
        else if (FourCC('BJNT') == fourCC)
        {
            // beginjoints
            writer->BeginTag("NumJoints", 'NJNT');
            writer->WriteInt(reader->ReadInt());
            writer->EndTag();
        }
        else if (FourCC('SJNT') == fourCC)
        {
            // setjoint
            writer->BeginTag("Joint", 'JONT');
            int jointIndex = reader->ReadInt();
            int parentJointIndex = reader->ReadInt();
            point poseTranslation;
            poseTranslation.x() = reader->ReadFloat();
            poseTranslation.y() = reader->ReadFloat();
            poseTranslation.z() = reader->ReadFloat();
            float4 poseRotation = reader->ReadFloat4();
            vector poseScale;
            poseScale.x() = reader->ReadFloat();
            poseScale.y() = reader->ReadFloat();
            poseScale.z() = reader->ReadFloat();
            String name = reader->ReadString();
            writer->WriteInt(jointIndex);
            writer->WriteInt(parentJointIndex);
            writer->WriteFloat4(poseTranslation);
            writer->WriteFloat4(poseRotation);
            writer->WriteFloat4(poseScale);
            writer->WriteString(name);
            writer->EndTag();
        }
        else if (FourCC('BGCL') == fourCC)
        {
            // beginclips
            SizeT numClips = reader->ReadInt();
            this->characterClips.SetSize(numClips);
        }
        else if (FourCC('STCL') == fourCC)
        {
            // setclip
            IndexT i = reader->ReadInt();
            IndexT clipIndex = reader->ReadInt();
            String clipName = reader->ReadString();
            this->characterClips[clipIndex] = clipName;
        }
        else if (FourCC('EDCL') == fourCC)
        {
            // endclips
            writer->BeginTag("Clips", 'CLIP');
            writer->WriteInt(this->characterClips.Size());
            IndexT i;
            for (i = 0; i < this->characterClips.Size(); i++)
            {
                n_assert(this->characterClips[i].IsValid());
                writer->WriteString(this->characterClips[i]);
            }
            writer->EndTag();
        }            
        else if (FourCC('SVRT') == fourCC)
        {
            // setvariations
            writer->BeginTag("Variations", 'VART');
            writer->WriteString(reader->ReadString());
            writer->EndTag();
        }
        else if (FourCC('BGFR') == fourCC)
        {
            // setnumfragments
            int numFragments = reader->ReadInt();
            writer->BeginTag("NumSkinFragments", 'NSKF');
            writer->WriteInt(numFragments);
            writer->EndTag();
            this->skinFragmentPrimGroupIndices.SetSize(numFragments);
            this->skinFragmentJointPalettes.SetSize(numFragments);
        }
        else if (FourCC('SFGI') == fourCC)
        {
            IndexT fragIndex = reader->ReadInt();
            this->skinFragmentPrimGroupIndices[fragIndex] = reader->ReadInt();
        }
        else if (FourCC('BGJP') == fourCC)
        {
            // beginjointpalette            
            int fragIndex = reader->ReadInt();
            int skinFragmentNumJoints = reader->ReadInt();            
            this->skinFragmentJointPalettes[fragIndex].SetSize(skinFragmentNumJoints);
        }
        else if (FourCC('SJID') == fourCC)
        {
            // setjointindices
            // skip unnecessary data
            int fragIndex = reader->ReadInt();
            int paletteStartIndex = reader->ReadInt();

            // add up to 8 joint indices
            IndexT i;
            IndexT curIndex = paletteStartIndex;
            for (i = 0; i < 8; i++)
            {
                int jointIndex = reader->ReadInt();
                if (curIndex < this->skinFragmentJointPalettes[fragIndex].Size())
                {
                    this->skinFragmentJointPalettes[fragIndex][curIndex++] = jointIndex;
                }
            }
        }
        else if (FourCC('EDFR') == fourCC)
        {
            // endfragment
            IndexT i;
            for (i = 0; i < this->skinFragmentPrimGroupIndices.Size(); i++)
            {
                writer->BeginTag("SkinFragment", 'SFRG');
                writer->WriteInt(this->skinFragmentPrimGroupIndices[i]);
                writer->WriteIntArray(this->skinFragmentJointPalettes[i].AsArray());
                writer->EndTag();
            }
        }        
        else if (FourCC('SCVA') == fourCC)
        {
            this->WriteParticleEnvelopeCurve("ParticleEmissionFrequency", 'EFRQ', reader, writer);
        }
        else if (FourCC('SCVB') == fourCC)
        {
            this->WriteParticleEnvelopeCurve("ParticleLifeTime", 'PLFT', reader, writer);
        }
        else if (FourCC('SCVD') == fourCC)
        {
            this->WriteParticleEnvelopeCurve("ParticleSpreadMin", 'PSMN', reader, writer);
        }
        else if (FourCC('SCVE') == fourCC)
        {
            this->WriteParticleEnvelopeCurve("ParticleSpreadMax", 'PSMX', reader, writer);
        }
        else if (FourCC('SCVF') == fourCC)
        {
            this->WriteParticleEnvelopeCurve("ParticleStartVelocity", 'PSVL', reader, writer);
        }
        else if (FourCC('SCVH') == fourCC)
        {
            this->WriteParticleEnvelopeCurve("ParticleRotationVelocity", 'PRVL', reader, writer);
        }
        else if (FourCC('SCVJ') == fourCC)
        {
            this->WriteParticleEnvelopeCurve("ParticleSize", 'PSZE', reader, writer);
        }
        else if (FourCC('SCVL') == fourCC)
        {
            this->WriteParticleEnvelopeCurve("ParticleMass", 'PMSS', reader, writer);
        }
        else if (FourCC('STMM') == fourCC)
        {
            this->WriteParticleEnvelopeCurve("ParticleTimeManipulator", 'PTMN', reader, writer);
        }
        else if (FourCC('SCVN') == fourCC)
        {
            this->WriteParticleEnvelopeCurve("ParticleVelocityFactor", 'PVLF', reader, writer);
        }
        else if (FourCC('SCVQ') == fourCC)
        {
            this->WriteParticleEnvelopeCurve("ParticleAirResistance", 'PAIR', reader, writer);
        }
        else if (FourCC('SCVC') == fourCC)
        {
            this->WriteParticleRGBEnvelopeCurve("ParticleRed", 'PRED', "ParticleGreen", 'PGRN', "ParticleBlue", 'PBLU', reader, writer);
        }
        else if (FourCC('SCVM') == fourCC)
        {
            this->WriteParticleEnvelopeCurve("ParticleAlpha", 'PALP', reader, writer);
        }
        else if (FourCC('SEMD') == fourCC)
        {
            writer->BeginTag("ParticleEmissionDuration", 'PEDU');
            writer->WriteFloat(reader->ReadFloat());
            writer->EndTag();
        }
        else if (FourCC('SLOP') == fourCC)
        {
            writer->BeginTag("ParticleLoopEmission", 'PLPE');
            writer->WriteInt(reader->ReadUChar());
            writer->EndTag();
        }
        else if (FourCC('SACD') == fourCC)
        {
            writer->BeginTag("ParticleActivityDistance", 'PACD');
            writer->WriteFloat(reader->ReadFloat());
            writer->EndTag();
        }
        else if (FourCC('SROF') == fourCC)
        {
            writer->BeginTag("ParticleRenderOldestFirst", 'PROF');
            writer->WriteInt(reader->ReadUChar());
            writer->EndTag();
        }
        else if (FourCC('SBBO') == fourCC)
        {
            writer->BeginTag("ParticleBillboardOrientation", 'PBBO');
            writer->WriteInt(reader->ReadUChar());
            writer->EndTag();
        }
        else if (FourCC('SRMN') == fourCC)
        {
            writer->BeginTag("ParticleRotationMin", 'PRMN');
            writer->WriteFloat(reader->ReadFloat());
            writer->EndTag();
        }
        else if (FourCC('SRMX') == fourCC)
        {
            writer->BeginTag("ParticleRotationMax", 'PRMX');
            writer->WriteFloat(reader->ReadFloat());
            writer->EndTag();
        }
        else if (FourCC('SGRV') == fourCC)
        {
            writer->BeginTag("ParticleGravity", 'PGRV');
            writer->WriteFloat(reader->ReadFloat());
            writer->EndTag();
        }
        else if (FourCC('SPST') == fourCC)
        {
            writer->BeginTag("ParticleStretch", 'PSTC');
            writer->WriteFloat(reader->ReadFloat());
            writer->EndTag();
        }
        else if (FourCC('STTX') == fourCC)
        {
            writer->BeginTag("ParticleTextureTile", 'PTTX');
            writer->WriteFloat((float)reader->ReadInt());
            writer->EndTag();
        }
        else if (FourCC('SSTS') == fourCC)
        {
            writer->BeginTag("ParticleStretchToStart", 'PSTS');
            writer->WriteInt(reader->ReadUChar());
            writer->EndTag();
        }
        else if (FourCC('SCVR') == fourCC)
        {
            writer->BeginTag("ParticleVelocityRandomize", 'PVRM');
            writer->WriteFloat(reader->ReadFloat());
            writer->EndTag();
        }
        else if (FourCC('SCVS') == fourCC)
        {
            writer->BeginTag("ParticleRotationRandomize", 'PRRM');
            writer->WriteFloat(reader->ReadFloat());
            writer->EndTag();
        }
        else if (FourCC('SCVT') == fourCC)
        {
            writer->BeginTag("ParticleSizeRandomize", 'PSRM');
            writer->WriteFloat(reader->ReadFloat());
            writer->EndTag();
        }
        else if (FourCC('SPCT') == fourCC)
        {
            writer->BeginTag("ParticlePrecalcTime", 'PPCT');
            writer->WriteFloat(reader->ReadFloat());
            writer->EndTag();
        }
        else if (FourCC('SCVU') == fourCC)
        {
            writer->BeginTag("ParticleRandomRotDir", 'PRRD');
            writer->WriteInt(reader->ReadUChar());
            writer->EndTag();
        }
        else if (FourCC('SSDT') == fourCC)
        {
            writer->BeginTag("ParticleStretchDetail", 'PSDL');
            writer->WriteInt(reader->ReadInt());
            writer->EndTag();
        }
        else if (FourCC('SVAF') == fourCC)
        {
            writer->BeginTag("ParticleViewAngleFade", 'PVAF');
            writer->WriteInt(reader->ReadUChar());
            writer->EndTag();
        }
        else if (FourCC('STDL') == fourCC)
        {
            writer->BeginTag("ParticleStartDelay", 'PDEL');
            writer->WriteFloat(reader->ReadFloat());
            writer->EndTag();
        }
        else if (FourCC('STUS') == fourCC)
        {
            Array<Math::float4> multilayerStretch;
            Array<Math::float4> multilayerSpec;
            // FIXME: layer size is fixed to 8 layers
            IndexT i;
            const SizeT numLayer = 8;
            float curStretch[numLayer];
            float curSpec[numLayer];
            for (i = 0; i < numLayer; i++)
            {
                n_assert(reader->ReadInt() == i);
                curStretch[i] = reader->ReadFloat();

                FourCC curFourCC = reader->ReadUInt(); 
                n_assert2(FourCC('SSPI') == curFourCC, "Reading multilayer parameter: Wrong fourCC!");
                ushort length = reader->ReadUShort();

                n_assert(reader->ReadInt() == i);
                curSpec[i] = reader->ReadFloat();
                
                if (i < numLayer - 1)
                {
                    curFourCC = reader->ReadUInt(); 
                    n_assert(FourCC('STUS') == curFourCC);
                    length = reader->ReadUShort();
                }
            }  

            // group single stretch and spec values to float4 for better use as shadervariables
            const int float4Size = 4;
            SizeT numVars = numLayer / float4Size;
            for (i = 0; i < numVars; i++)
            {
                // stretch
                writer->BeginTag("MultilayerUVStretch", 'STUS');
                writer->WriteInt(i);
                writer->WriteFloat4(Math::float4(curStretch[i * float4Size],
                                    curStretch[i * float4Size + 1],
                                    curStretch[i * float4Size + 2],
                                    curStretch[i * float4Size + 3]));
                writer->EndTag();

                // specular
                writer->BeginTag("MultilayerSpecIntensity", 'SSPI');
                writer->WriteInt(i);
                writer->WriteFloat4(Math::float4(curSpec[i * float4Size],
                                    curSpec[i * float4Size + 1],
                                    curSpec[i * float4Size + 2],
                                    curSpec[i * float4Size + 3]));
                writer->EndTag();  
            }
        }

        else if (FourCC('ADDA') == fourCC)
        {
            String absPath = this->ConvertRelPathToAbsolute(reader->ReadString());
            IndexT index = this->animatedNodesToAnimator.FindIndex(absPath);

            if (index == -1)
            {
                // add new Key if Absolute Path not exists
                Array<String> nodePath;
                nodePath.Append(this->ConvertPathFromArrayToString(this->path));
                this->animatedNodesToAnimator.Add(absPath, nodePath);
            }
            else
            {
                // append nodepath to key if key already exists
                this->animatedNodesToAnimator[absPath].Append(this->ConvertPathFromArrayToString(this->path));
            }
        }
        else if (FourCC('SLPT') == fourCC)
        {
            // nebula3 uses no skinanimators, so this fourcc is only used by the other animators
            if (this->animNodeType >= IntAnimator && this->animNodeType <= UvAnimator)
            {
                this->animInfo.loopType = reader->ReadString();
            }
            else
            {                
                reader->GetStream()->Seek(length, Stream::Current);
            }
        }
        else if (FourCC('ADDK') == fourCC)
        {
            if (this->animNodeType == Float4Animator)
            {
                KeyValues val;
                val.time = reader->ReadFloat();
                val.value = reader->ReadFloat4();
                this->animInfo.key.Append(val);
            }
            else if (this->animNodeType == FloatAnimator)
            {
                KeyValues val;
                val.time = reader->ReadFloat();
                val.value = reader->ReadFloat();
                this->animInfo.key.Append(val);
            }
            else if (this->animNodeType == IntAnimator)
            {
                KeyValues val;
                val.time = reader->ReadFloat();
                val.value = reader->ReadInt();
                this->animInfo.key.Append(val);
            }
            else
            {                
                reader->GetStream()->Seek(length, Stream::Current);
            }
        }
        else if (FourCC('SPNM') == fourCC)
        {
            //ShaderParamName
            this->animInfo.paramName = reader->ReadString();
        }
        else if (FourCC('SVCN') == fourCC)
        {
            //VectorName
            this->animInfo.vectorName = reader->ReadString();
        }
        else if (FourCC('ADPK') == fourCC)
        {
            //PositionKey
            KeyValues val;
            if (this->animNodeType == UvAnimator)
            {
                this->animInfo.layer.Append(reader->ReadInt());
                val.time = (float)reader->ReadFloat();
                float u = reader->ReadFloat();
                float v = reader->ReadFloat();
                val.value.SetFloat4(Math::float4(u, v, 0, 1));
            }
            else //transformanimator
            {
                val.time = (float)reader->ReadFloat();
                float x = reader->ReadFloat();
                float y = reader->ReadFloat();
                float z = reader->ReadFloat();
                val.value.SetFloat4(Math::float4(x, y, z, 1));
            }
            
            this->animInfo.posKey.Append(val);
        }
        else if (FourCC('ADEK') == fourCC)
        {
            //EulerKey
            KeyValues val;
            if (this->animNodeType == UvAnimator)
            {
                this->animInfo.layer.Append(reader->ReadInt());
                val.time = (float)reader->ReadFloat();
                float u = n_deg2rad(reader->ReadFloat());
                float v = n_deg2rad(reader->ReadFloat());
                val.value.SetFloat4(Math::float4(u, v, 0, 1));
            }
            else //transformanimator
            {
                val.time = (float)reader->ReadFloat();
                float x = reader->ReadFloat();
                float y = reader->ReadFloat();
                float z = reader->ReadFloat();
                val.value.SetFloat4(Math::float4(x, y, z, 1));
            }
            this->animInfo.eulerKey.Append(val);
        }
        else if (FourCC('ADSK') == fourCC)
        {
            //ScaleKey
            KeyValues val;
            if (this->animNodeType == UvAnimator)
            {
                this->animInfo.layer.Append(reader->ReadInt());
                val.time = (float)reader->ReadFloat();
                float u = reader->ReadFloat();
                float v = reader->ReadFloat();
                val.value.SetFloat4(Math::float4(u, v, 1, 1));
            }
            else //transformanimator
            {
                val.time = (float)reader->ReadFloat();
                float x = reader->ReadFloat();
                float y = reader->ReadFloat();
                float z = reader->ReadFloat();
                val.value.SetFloat4(Math::float4(x, y, z, 1));
            }
            this->animInfo.scaleKey.Append(val);
        }
        else if (FourCC('SANI') == fourCC)
        {
            //animation
            this->animInfo.animation = reader->ReadString();
        }
        else if (FourCC('SAGR') == fourCC)
        {
            //animationgroup
            this->animInfo.animationGroup = reader->ReadInt();
        }
        else if (FourCC('SSTA') == fourCC)
        {
            writer->BeginTag("StringAttr", 'SSTA');
            writer->WriteString(reader->ReadString());
            writer->WriteString(reader->ReadString());
            writer->EndTag();
        }
        else
        {
            // skip unknown data tags
            reader->GetStream()->Seek(length, Stream::Current);
        }
    }
}

//------------------------------------------------------------------------------
/**
    Reads a particle system envelope curve data entry from the reader, 
    and writes it as a new data tag to the model writer.
*/
void
N2Converter::WriteParticleEnvelopeCurve(const String& tagName, FourCC fourCC, const Ptr<BinaryReader>& reader, const Ptr<ModelWriter>& writer)
{
    writer->BeginTag(tagName, fourCC);
    writer->WriteFloat(reader->ReadFloat());    // val0
    writer->WriteFloat(reader->ReadFloat());    // val1
    writer->WriteFloat(reader->ReadFloat());    // val2
    writer->WriteFloat(reader->ReadFloat());    // val3
    writer->WriteFloat(reader->ReadFloat());    // keyPos0
    writer->WriteFloat(reader->ReadFloat());    // keyPos1
    writer->WriteFloat(reader->ReadFloat());    // frequency
    writer->WriteFloat(reader->ReadFloat());    // amplitude
    writer->WriteInt(reader->ReadInt());        // modfunc
    writer->EndTag();
}

//------------------------------------------------------------------------------
/**
    Special version of WriteParticleEnvelopeCurve which reads an
    RGB envelope curve and writes it as 3 independent envelope curves.
*/
void
N2Converter::WriteParticleRGBEnvelopeCurve(const String& redTagName, FourCC redFourCC,
                                           const String& greenTagName, FourCC greenFourCC,
                                           const String& blueTagName, FourCC blueFourCC,
                                           const Ptr<BinaryReader>& reader,
                                           const Ptr<ModelWriter>& writer)
{
    // read original data
    float red[4], green[4], blue[4];
    float keyPos0, keyPos1;
    float frequency, amplitude;
    int modFunc;
    IndexT i;
    for (i = 0; i < 4; i++)
    {
        red[i]   = reader->ReadFloat();
        green[i] = reader->ReadFloat();
        blue[i]  = reader->ReadFloat();
    }
    keyPos0 = reader->ReadFloat();
    keyPos1 = reader->ReadFloat();

    // NOTE: Nebula2-RGB-EnvelopeCurves don't have the sine/cosine modulation!
    frequency = 1.0f;
    amplitude = 0.0f;
    modFunc = 0;

    // write red data tag
    writer->BeginTag(redTagName, redFourCC);
    writer->WriteFloat(red[0]);
    writer->WriteFloat(red[1]);
    writer->WriteFloat(red[2]);
    writer->WriteFloat(red[3]);
    writer->WriteFloat(keyPos0);
    writer->WriteFloat(keyPos1);
    writer->WriteFloat(frequency);
    writer->WriteFloat(amplitude);
    writer->WriteInt(modFunc);
    writer->EndTag();

    // write green data tag
    writer->BeginTag(greenTagName, greenFourCC);
    writer->WriteFloat(green[0]);
    writer->WriteFloat(green[1]);
    writer->WriteFloat(green[2]);
    writer->WriteFloat(green[3]);
    writer->WriteFloat(keyPos0);
    writer->WriteFloat(keyPos1);
    writer->WriteFloat(frequency);
    writer->WriteFloat(amplitude);
    writer->WriteInt(modFunc);
    writer->EndTag();

    // write blue data tag
    writer->BeginTag(blueTagName, blueFourCC);
    writer->WriteFloat(blue[0]);
    writer->WriteFloat(blue[1]);
    writer->WriteFloat(blue[2]);
    writer->WriteFloat(blue[3]);
    writer->WriteFloat(keyPos0);
    writer->WriteFloat(keyPos1);
    writer->WriteFloat(frequency);
    writer->WriteFloat(amplitude);
    writer->WriteInt(modFunc);
    writer->EndTag();
}


//------------------------------------------------------------------------------
/**
*/
String
N2Converter::MapShaderNameToModelNodeType(const String& shaderName)
{
    if (shaderName == "simple_alpha")
    {
        return "UnlitAlpha";
    }
    else if (shaderName == "additive_alpha")
    {
        return "Additive";
    }
    else if ("skinned_alpha" == shaderName)
    {
        return "BlendAlphaTest";
    }
    else if (String::MatchPattern(shaderName, "*alpha") ||
        String::MatchPattern(shaderName, "particle2") ||
        shaderName == "hair" ||
        shaderName == "volumefog")

    {
        return "Alpha";
    }
    else if ("particle2_additive" == shaderName)
    {
        return "Additive";
    }
    else if ("static_atest" == shaderName)
    {
        return "AlphaTest";
    }
    else if ("skybox" == shaderName)
    {
        return "Background";
    }
    else
    {
        return "Solid";
    }
}

//------------------------------------------------------------------------------
/**
*/
String
N2Converter::ConvertShaderName(const String& shdName)
{
    if ("lightmapped" == shdName
        || "lightmapped2" == shdName
        || "lightmapped_alpha" == shdName)
    {
        return "shd:lightmapped";
    }
    else if ("skybox" == shdName)
    {
        return "shd:skybox";
    }
    else if ("multilayered" == shdName 
        ||"multilayeredbump" == shdName)
    {
        return "shd:multilayer";
    }
    else if ("volumefog" == shdName)
    {
        return "shd:volumefog";
    }
    else if ("alpha_uvanimation" == shdName)
    {
        return "shd:uvanimated";
    }
    else if ("particle2" == shdName)
    {
        return "shd:particle";
    }
    else if ("particle2_additive" == shdName)
    {
        return "shd:particle";
    }
    else if ("simple_alpha" == shdName)
    {
        return "shd:unlit";
    }
    else if ("environment_skinned" == shdName)
    {
        return "shd:environment";
    }
    else
    {
        // map everything else to static
        return "shd:static";
    }
}

//------------------------------------------------------------------------------
/**
    Perform a file time check to decide whether a texture must be
    converted.
*/
bool
N2Converter::NeedsConversion(const String& srcPath, const String& dstPath)
{
    // file time check overriden?
    if (this->force)
    {
        return true;
    }

    // if this is a character, we would need to check skin lists as well,
    // instead we just assume that a conversion is needed
    if (String::MatchPattern(srcPath, "*characters*"))
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
    Loads the skin lists for the current character and writes them
    into the n3 file. This method is called by EndNode() when
    a Character3Node is finished.
*/
bool
N2Converter::ConvertSkinLists(const Ptr<ModelWriter>& writer)
{
    IoServer* ioServer = IoServer::Instance();
    
    // build skin list directory from current file name
    String skinListDir = this->srcPath;
    skinListDir.StripFileExtension();
    skinListDir.Append("/skinlists");
    if (!ioServer->DirectoryExists(skinListDir))
    {
        return false;
    }

    // get the skin lists files for this character
    Array<String> skinLists = ioServer->ListFiles(skinListDir, "*.xml");
    if (skinLists.Size() == 0)
    {
        return false;
    }

    // write number of skin lists to the N3 file
    writer->BeginTag("NumSkinLists", 'NSKL');
    writer->WriteInt(skinLists.Size());
    writer->EndTag();

    // parse skin list xml files
    IndexT skinListIndex;
    for (skinListIndex = 0; skinListIndex < skinLists.Size(); skinListIndex++)
    {   
        String skinListName = skinLists[skinListIndex];
        skinListName.StripFileExtension();
        Array<String> skins;
        
        // read skin list into an xml reader
        String skinListPath = skinListDir + "/" + skinLists[skinListIndex];
        Ptr<Stream> stream = ioServer->CreateStream(skinListPath);
        Ptr<XmlReader> xmlReader = XmlReader::Create();
        xmlReader->SetStream(stream);
        if (xmlReader->Open())
        {
            xmlReader->SetToNode("/skins");
            if (xmlReader->SetToFirstChild()) do
            {
                xmlReader->SetToFirstChild();
                xmlReader->SetToFirstChild();
                skins.Append(xmlReader->GetCurrentNodeName());
                xmlReader->SetToParent();
                xmlReader->SetToParent();
            }
            while (xmlReader->SetToNextChild());
            xmlReader->Close();
        }

        // write skin list to n3 file
        writer->BeginTag("SkinList", 'SKNL');
        writer->WriteString(skinListName);
        writer->WriteInt(skins.Size());
        IndexT skinIndex;
        for (skinIndex = 0; skinIndex < skins.Size(); skinIndex++)
        {
            writer->WriteString(skins[skinIndex]);
        }
        writer->EndTag();
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
N2Converter::AnimatorNodeType
N2Converter::FromStringToAnimatorNode(String animNode)
{
    if (animNode == "ntransformcurveanimator")      return TransformCurveAnimator;
    else if (animNode == "ntransformanimator")      return TransformAnimator;
    else if (animNode == "nfloatanimator")          return FloatAnimator;
    else if (animNode == "nintanimator")            return IntAnimator;
    else if (animNode == "nvectoranimator")         return Float4Animator;
    else if (animNode == "nuvanimator")             return UvAnimator;
    else return InvalidAnimatorNodeType;
}

//------------------------------------------------------------------------------
/**
*/
void
N2Converter::FillAnimatorInformation()
{
    if ((!this->lastAnimatorName.IsEmpty()) && (this->animInfo.animatorNode != InvalidAnimatorNodeType))
    {
        this->animatorInformation.Add(this->lastAnimatorName, this->animInfo);
        this->lastAnimatorName.Clear();

        //Cleanup BufferVariable for later usage
        this->animInfo.animation.Clear();
        this->animInfo.key.Clear();
        this->animInfo.loopType.Clear();
        this->animInfo.paramName.Clear();
        this->animInfo.animatorNode = InvalidAnimatorNodeType;
        this->animInfo.animationGroup = -1;
        this->animInfo.vectorName.Clear();
        this->animInfo.posKey.Clear();
        this->animInfo.eulerKey.Clear();
        this->animInfo.scaleKey.Clear();
        this->animInfo.layer.Clear();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
N2Converter::SetAnimatorPath(String objName, String objClass)
{
    // build the path
    if(path.Size() > 0)
        this->path.Append("/"+objName);
    else
        this->path.Append(objName);

    //save the objectClass for later use
    this->animInfo.animatorNode = this->FromStringToAnimatorNode(objClass);
}

//------------------------------------------------------------------------------
/**
*/
String
N2Converter::ConvertPathFromArrayToString(Array<String> stringArray)
{
    IndexT tokenIdx;
    String returnValue;

    for (tokenIdx = 0; tokenIdx < stringArray.Size(); tokenIdx++)
    {
        returnValue.Append(stringArray[tokenIdx]);
    }

    return returnValue;
}

//------------------------------------------------------------------------------
/**
    Converts the relative AnimatorPath to an Absolute AnimatorPath
*/
String
N2Converter::ConvertRelPathToAbsolute(String relPath, bool pathToAnimatorNode)
{
    Array<String> tokenized = relPath.Tokenize("/");
    IndexT tokenIdx;
    Array<String> nodePath = this->path;

    for(tokenIdx = tokenized.Size()-1; tokenIdx > -1; tokenIdx--)
    {
        if (tokenized[tokenIdx] == "..")
        {
            //Erase last ".." and last nodePath-segment
            nodePath.EraseIndex(nodePath.Size()-1);
            tokenized.EraseIndex(tokenIdx);
        }
    }
    String fragmentRelativePath = this->ConvertPathFromArrayToString(tokenized);
    String fragmentNodePath = this->ConvertPathFromArrayToString(nodePath);

    if (!pathToAnimatorNode)
    {
        // return Absolute Path
        if (fragmentNodePath.IsEmpty())
            return fragmentRelativePath;
        else
        {
            return fragmentNodePath + "/" + fragmentRelativePath;
        }
    }
    else
    {
        return fragmentNodePath;
    }
}

} // namespace ToolkitUtil