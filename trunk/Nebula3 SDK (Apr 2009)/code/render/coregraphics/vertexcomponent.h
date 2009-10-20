#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::VertexComponent
  
    Describes a single vertex component in a vertex layout description.
    
    (C) 2006 Radon Labs GmbH
*/    
#include "core/types.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class VertexComponent
{
public:
    /// component semantic
    enum SemanticName
    {
        Position = 0,
        Normal,
        Tangent,
        Binormal,
        TexCoord,        
        Color,
        SkinWeights,
        SkinJIndices,

        Invalid,
    };

    /// component format
    enum Format
    {
        Float,      //> one-component float, expanded to (float, 0, 0, 1)
        Float2,     //> two-component float, expanded to (float, float, 0, 1)
        Float3,     //> three-component float, expanded to (float, float, float, 1)
        Float4,     //> four-component float
        UByte4,     //> four-component unsigned byte
        Short2,     //> two-component signed short, expanded to (value, value, 0, 1)
        Short4,     //> four-component signed short
        UByte4N,    //> four-component normalized unsigned byte (value / 255.0f)
        Short2N,    //> two-component normalized signed short (value / 32767.0f)
        Short4N,    //> four-component normalized signed short (value / 32767.0f)
    };
    
    /// access type hint, this is only relevant on the Wii
    enum AccessType
    {
        None,
        Direct,     //> component has direct value (non-indexed)
        Index8,     //> component is indexed with 8-bit indices           
        Index16,    //> component is indexed with 16-bit indices
        Index32,    //> component is indexed with 32-bit indices
    };

    /// default constructor
    VertexComponent();
    /// constructor
    VertexComponent(SemanticName semName, IndexT semIndex, Format format, IndexT streamIndex=0);
    /// get semantic name
    SemanticName GetSemanticName() const;
    /// get semantic index
    IndexT GetSemanticIndex() const;
    /// get vertex component format
    Format GetFormat() const;
    /// get stream index
    IndexT GetStreamIndex() const;
    /// get the byte size of the vertex component
    SizeT GetByteSize() const;
    /// get a unique signature of the vertex component
    Util::String GetSignature() const;
    /// get access type
    AccessType GetAccessType() const;
    /// convert semantic name to string
    static Util::String SemanticNameToString(SemanticName n);
    /// convert format to string
    static Util::String FormatToString(Format f);

private:
    SemanticName semName;
    IndexT semIndex;
    Format format;
    AccessType accessType;
    IndexT streamIndex;
};

//------------------------------------------------------------------------------
/**
*/
inline
VertexComponent::VertexComponent() :
    semName(Invalid),
    semIndex(0),
    format(Float),
    accessType(Index16),
    streamIndex(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
VertexComponent::VertexComponent(SemanticName semName_, IndexT semIndex_, Format format_, IndexT streamIndex_) :
    semName(semName_),
    semIndex(semIndex_),
    format(format_),
    accessType(Index16),
    streamIndex(streamIndex_)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline VertexComponent::SemanticName
VertexComponent::GetSemanticName() const
{
    return this->semName;
}

//------------------------------------------------------------------------------
/**
*/
inline VertexComponent::AccessType
VertexComponent::GetAccessType() const
{
    return this->accessType;     
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
VertexComponent::GetSemanticIndex() const
{
    return this->semIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline VertexComponent::Format
VertexComponent::GetFormat() const
{
    return this->format;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
VertexComponent::GetStreamIndex() const
{
    return this->streamIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
VertexComponent::GetByteSize() const
{
    switch (this->format)
    {
        case Float:     return 4;
        case Float2:    return 8;
        case Float3:    return 12;
        case Float4:    return 16;
        case UByte4:    return 4;
        case Short2:    return 4;
        case Short4:    return 8;
        case UByte4N:   return 4;
        case Short2N:   return 4;
        case Short4N:   return 8;
    }
    n_error("Can't happen");
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
inline Util::String
VertexComponent::SemanticNameToString(SemanticName n)
{
    switch (n)
    {
        case Position:      return "Position";
        case Normal:        return "Normal";
        case Tangent:       return "Tangent";
        case Binormal:      return "Binormal";
        case TexCoord:      return "TexCoord";
        case Color:         return "Color";
        case SkinWeights:   return "SkinWeights";
        case SkinJIndices:  return "SkinJIndices";
        default:
            n_error("VertexComponent::SemanticNameToString(): invalid SemanticName code!");
            return "";
    }
}

//------------------------------------------------------------------------------
/**
*/
inline Util::String
VertexComponent::FormatToString(VertexComponent::Format f)
{
    switch (f)
    {
        case Float:     return "Float";
        case Float2:    return "Float2";
        case Float3:    return "Float3";
        case Float4:    return "Float4";
        case UByte4:    return "UByte4";
        case Short2:    return "Short2";
        case Short4:    return "Short4";
        case UByte4N:   return "UByte4N";
        case Short2N:   return "Short2N";
        case Short4N:   return "Short4N";
        default:
            n_error("VertexComponent::FormatToString(): invalid Format code!");
            return "";
    }
}

//------------------------------------------------------------------------------
/**
*/
inline Util::String
VertexComponent::GetSignature() const
{
    Util::String str;
    switch (this->semName)
    {
        case Position:      str = "pos"; break;
        case Normal:        str = "nrm"; break;
        case Tangent:       str = "tan"; break;
        case Binormal:      str = "bin"; break;
        case TexCoord:      str = "tex"; break;
        case Color:         str = "clr"; break;
        case SkinWeights:   str = "skw"; break;
        case SkinJIndices:  str = "sji"; break;
        default:
            n_error("can't happen!");
            break;
    }
    str.AppendInt(this->semIndex);    
    str.Append(FormatToString(this->format));
    return str;
}

} // namespace CoreGraphics
//------------------------------------------------------------------------------

