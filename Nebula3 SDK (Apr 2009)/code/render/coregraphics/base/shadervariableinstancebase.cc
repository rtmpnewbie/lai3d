//------------------------------------------------------------------------------
//  shadervariableinstancebase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/shadervariableinstancebase.h"
#include "coregraphics/shadervariable.h"

namespace Base
{
__ImplementClass(Base::ShaderVariableInstanceBase, 'SVIB', Core::RefCounted);

using namespace CoreGraphics;
using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
ShaderVariableInstanceBase::ShaderVariableInstanceBase() :
    arrayCount(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ShaderVariableInstanceBase::~ShaderVariableInstanceBase()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderVariableInstanceBase::Setup(const Ptr<ShaderVariable>& var)
{
    this->Prepare(var->GetType(), var->GetNumArrayElements());
    this->Bind(var);
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderVariableInstanceBase::Bind(const Ptr<ShaderVariable>& var)
{
    n_assert(!this->shaderVariable.isvalid());
    this->shaderVariable = var;
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderVariableInstanceBase::Prepare(ShaderVariable::Type type, SizeT arraySize)
{
    n_assert(!this->shaderVariable.isvalid());
    switch (type)
    {
        case ShaderVariable::IntType:
            this->value.SetType(Variant::Int);
            if (arraySize > 0)
            {
                this->arrayCount = arraySize;
                this->arrayValues.Reserve(this->arrayCount * sizeof(int));
            }
            break;
        
        case ShaderVariable::FloatType:
            this->value.SetType(Variant::Float);
            if (arraySize > 0)
            {
                this->arrayCount = arraySize;
                this->arrayValues.Reserve(this->arrayCount * sizeof(float));
            }
            break;

        case ShaderVariable::VectorType:
            this->value.SetType(Variant::Float4);
            if (arraySize > 0)
            {
                this->arrayCount = arraySize;
                this->arrayValues.Reserve(this->arrayCount * sizeof(float4));
            }
            break;

        case ShaderVariable::MatrixType:
            this->value.SetType(Variant::Matrix44);
            if (arraySize > 0)
            {
                this->arrayCount = arraySize;
                this->arrayValues.Reserve(this->arrayCount * sizeof(matrix44));
            }
            break;

        case ShaderVariable::BoolType:
            this->value.SetType(Variant::Bool);
            if (arraySize > 0)
            {
                this->arrayCount = arraySize;
                this->arrayValues.Reserve(this->arrayCount * sizeof(bool));
            }
            break;

        case ShaderVariable::TextureType:
            this->value.SetType(Variant::Object);
            n_assert(0 == arraySize);
            break;
    }
}

//------------------------------------------------------------------------------
/**
    @todo: hmm, the dynamic type switch is sort of lame...
*/
void
ShaderVariableInstanceBase::Apply()
{
    n_assert(this->shaderVariable.isvalid());
    if (this->arrayCount > 0)
    {
        // contained data is an array
        SizeT count = this->arrayCount;
        void* ptr = this->arrayValues.GetPtr();
        n_assert(0 != ptr);
        switch (this->value.GetType())
        {
            case Variant::Int:
                this->shaderVariable->SetIntArray((int*)ptr, count);
                break;
            case Variant::Float:
                this->shaderVariable->SetFloatArray((float*)ptr, count);
                break;
            case Variant::Float4:
                this->shaderVariable->SetFloat4Array((float4*)ptr, count);
                break;
            case Variant::Matrix44:
                this->shaderVariable->SetMatrixArray((matrix44*)ptr, count);
                break;
            case Variant::Bool:
                this->shaderVariable->SetBoolArray((bool*)ptr, count);
                break;
            default:
                n_error("ShaderVariable::Apply(): invalid data type for arrays!");
                break;
        }
    }
    else
    {
        // contained data is a scalar
        switch (this->value.GetType())
        {
            case Variant::Int:
                this->shaderVariable->SetInt(this->value.GetInt());
                break;
            case Variant::Float:
                this->shaderVariable->SetFloat(this->value.GetFloat());
                break;
            case Variant::Float4:
                this->shaderVariable->SetFloat4(this->value.GetFloat4());
                break;
            case Variant::Matrix44:
                this->shaderVariable->SetMatrix(this->value.GetMatrix44());
                break;
            case Variant::Bool:
                this->shaderVariable->SetBool(this->value.GetBool());
                break;
            case Variant::Object:
                // @note: implicit Ptr<> creation!
                if (this->value.GetObject() != 0)
                {
                    this->shaderVariable->SetTexture((Texture*)this->value.GetObject());
                }
                break;
            default:
                n_error("ShaderVariable::Apply(): invalid data type for scalar!");
                break;
        }
    }
}

} // namespace Base

