#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::N2Converter
    
    A utility class to convert N2 binary object files to Nebula3 object
    files.
    
    (C) 2008 Radon Labs GmbH
*/
#include "toolkitutil/platform.h"
#include "io/uri.h"
#include "util/string.h"
#include "util/stack.h"
#include "io/stream.h"
#include "io/binaryreader.h"
#include "toolkitutil/modelwriter.h"
#include "util/variant.h"

//------------------------------------------------------------------------------
namespace ToolkitUtil
{
class N2Converter
{
public:
    /// constructor
    N2Converter();
    /// destructor
    ~N2Converter();
    
    /// set target platform
    void SetPlatform(Platform::Code platform);
    /// set source directory
    void SetSrcDir(const Util::String& srcDir);
    /// set destination directory
    void SetDstDir(const Util::String& dstDir);
    /// set force conversion flag (otherwise check timestamps)
    void SetForceFlag(bool b);
    /// set binary mode
    void SetBinaryFlag(bool b);

    /// convert a single N2 file
    bool ConvertFile(const Util::String& category, const Util::String& srcFile);
    /// convert all files in a category
    bool ConvertCategory(const Util::String& category);
    /// convert all files
    bool ConvertAll();

private:
    /// supported Nebula2 node types
    enum NodeType
    {
        TransformNode = 0,
        ShapeNode,
        Character3Node,
        Character3SkinAnimator,
        Character3SkinShapeNode,
        EmbeddedNodeType,
        AnimatorNode,
        ParticleSystemNode,        

        InvalidNodeType,
    };

    enum AnimatorNodeType
    {
        IntAnimator = 0,
        FloatAnimator,
        Float4Animator,
        TransformAnimator,
        TransformCurveAnimator,
        UvAnimator,

        InvalidAnimatorNodeType,
    };

    struct KeyValues
    {
        float time;
        Util::Variant value;
    };

    struct AnimatorInfo
    {
        Util::String loopType;
        AnimatorNodeType animatorNode;
        Util::String animation;
        Util::Array<KeyValues> key;
        Util::String paramName;
        Util::String vectorName;
        Util::Array<KeyValues> posKey;
        Util::Array<KeyValues> eulerKey;
        Util::Array<KeyValues> scaleKey;
        int animationGroup;
        Util::Array<int> layer;
    };    

    /// test if a conversion is needed (checks file time stamps and force flag)
    bool NeedsConversion(const Util::String& srcPath, const Util::String& dstPath);
    /// perform conversion for a single file
    bool PerformConversion(const Util::String& modelName, const Ptr<IO::BinaryReader>& reader, const Ptr<ModelWriter>& writer);
    /// actual node-parser
    bool ParseNodes(const Ptr<IO::BinaryReader>& reader, const Ptr<ModelWriter>& writer);
    /// called when a new node is encountered in the source file
    void BeginNode(const Util::String& objClass, const Util::String& objName, const Ptr<ModelWriter>& writer);
    /// called when a node is left in the source file
    void EndNode(const Ptr<ModelWriter>& writer);
    /// called for each data element encountered in the source file
    void ReadDataTag(Util::FourCC fourCC, const Ptr<IO::BinaryReader>& reader, const Ptr<ModelWriter>& writer);
    /// map a shader name to a ModelNodeType
    Util::String MapShaderNameToModelNodeType(const Util::String& shaderName);
    /// convert Nebula2 shader name to Nebula3 shader name
    Util::String ConvertShaderName(const Util::String& shaderName);
    /// convert skin lists of a character3 (skin lists are written to the n3 file)
    bool ConvertSkinLists(const Ptr<ModelWriter>& writer);
   /// write a particle system envelope curve tag
    void WriteParticleEnvelopeCurve(const Util::String& tag, Util::FourCC fourCC, const Ptr<IO::BinaryReader>& reader, const Ptr<ModelWriter>& writer);
    /// write a particle system RGB envelope curve tag
    void WriteParticleRGBEnvelopeCurve(const Util::String& redTag, Util::FourCC redFourCC, const Util::String& greenTag, Util::FourCC greenFourCC, const Util::String& blueTag, Util::FourCC blueFourCC, const Ptr<IO::BinaryReader>& reader, const Ptr<ModelWriter>& writer);
    /// write the animatornodes to nebula3-file
    void WriteAnimatorNode(const Ptr<ModelWriter>& writer);
    /// add an additional information to dictionary
    void FillAnimatorInformation();
    /// Set Animator Path
    void SetAnimatorPath(Util::String objName, Util::String objClass);
    /// convert from string to animatorNode
    AnimatorNodeType FromStringToAnimatorNode(Util::String animNode);
    /// Converts Relative AnimatorPath to Absolute AnimatorPath
    Util::String ConvertRelPathToAbsolute(Util::String relPath, bool pathToAnimatorNode=false);
    /// Paths are saved in Array of String-> Converts paths from Array to simpleString 
    Util::String ConvertPathFromArrayToString(Util::Array<Util::String> stringArray);

    Platform::Code platform;
    Util::String srcDir;
    Util::String dstDir;
    Util::String srcPath;
    Util::String dstPath;
    bool force;
    bool binary;
    NodeType nodeType;
    Util::Stack<NodeType> nodeTypeStack;

    Util::FixedArray<IndexT> skinFragmentPrimGroupIndices;
    Util::FixedArray<Util::FixedArray<IndexT>> skinFragmentJointPalettes;
    Util::FixedArray<Util::String> characterClips;

    Util::Dictionary<Util::String, Util::Array<Util::String> > animatedNodesToAnimator;
    Util::Dictionary<Util::String, AnimatorInfo> animatorInformation;
    Util::Array<Util::String> path;
    Util::String lastAnimatorName;
    AnimatorInfo animInfo;
    AnimatorNodeType animNodeType;
};

//------------------------------------------------------------------------------
/**
*/
inline void
N2Converter::SetPlatform(Platform::Code p)
{
    this->platform = p;
}

//------------------------------------------------------------------------------
/**
*/
inline void
N2Converter::SetSrcDir(const Util::String& dir)
{
    this->srcDir = dir;
}

//------------------------------------------------------------------------------
/**
*/
inline void
N2Converter::SetDstDir(const Util::String& dir)
{
    this->dstDir = dir;
}

//------------------------------------------------------------------------------
/**
*/
inline void
N2Converter::SetForceFlag(bool b)
{
    this->force = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
N2Converter::SetBinaryFlag(bool b)
{
    this->binary = b;
}

} // namespace ToolkitUtil
//------------------------------------------------------------------------------
    
