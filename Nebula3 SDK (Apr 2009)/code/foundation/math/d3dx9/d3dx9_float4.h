#pragma once
//------------------------------------------------------------------------------
/**
    @class Math::float4
  
    The float4 class implemented on top of the D3DX math functions.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "core/types.h"
#include "math/scalar.h"

//------------------------------------------------------------------------------
namespace Math
{
class matrix44;

__declspec(align(16)) class float4
{
public:
    /// default constructor, NOTE: does NOT setup components!
    float4();
    /// construct from values
    float4(scalar x, scalar y, scalar z, scalar w);
    /// copy constructor
    float4(const float4& rhs);

    /// assignment operator
    void operator=(const float4& rhs);
    /// flip sign
    float4 operator-() const;
    /// inplace add
    void operator+=(const float4& rhs);
    /// inplace sub
    void operator-=(const float4& rhs);
    /// inplace scalar multiply
    void operator*=(scalar s);
    /// add 2 vectors
    float4 operator+(const float4& rhs) const;
    /// subtract 2 vectors
    float4 operator-(const float4& rhs) const;
    /// multiply with scalar
    float4 operator*(scalar s) const;
    /// equality operator
    bool operator==(const float4& rhs) const;
    /// inequality operator
    bool operator!=(const float4& rhs) const;

    /// load content from 16-byte-aligned memory
    void load(const scalar* ptr);
    /// load content from unaligned memory
    void loadu(const scalar* ptr);
    /// write content to 16-byte-aligned memory through the write cache
    void store(scalar* ptr) const;
    /// write content to unaligned memory through the write cache
    void storeu(scalar* ptr) const;
    /// stream content to 16-byte-aligned memory circumventing the write-cache
    void stream(scalar* ptr) const;

    /// load 3 floats into x,y,z from unaligned memory
    void load_float3(const void* ptr, float w);
    /// load from UByte4N packed vector, move range to -1..+1
    void load_ubyte4n_signed(const void* ptr, float w);

    /// set content
    void set(scalar x, scalar y, scalar z, scalar w);
    /// read/write access to x component
    scalar& x();
    /// read/write access to y component
    scalar& y();
    /// read/write access to z component
    scalar& z();
    /// read/write access to w component
    scalar& w();
    /// read-only access to x component
    scalar x() const;
    /// read-only access to y component
    scalar y() const;
    /// read-only access to z component
    scalar z() const;
    /// read-only access to w component
    scalar w() const;

    /// return length of vector
    scalar length() const;
    /// return squared length of vector
    scalar lengthsq() const;
    /// return compononent-wise absolute
    float4 abs() const;
    
    /// return 1.0 / vec
    static float4 reciprocal(const float4& v);
    /// component-wise multiplication
    static float4 multiply(const float4& v0, const float4& v1);
    /// return 3-dimensional cross product
    static float4 cross3(const float4& v0, const float4& v1);
    /// return 3d dot product of vectors
    static scalar dot3(const float4& v0, const float4& v1);
    /// return point in barycentric coordinates
    static float4 barycentric(const float4& v0, const float4& v1, const float4& v2, scalar f, scalar g);
    /// perform Catmull-Rom interpolation
    static float4 catmullrom(const float4& v0, const float4& v1, const float4& v2, const float4& v3, scalar s);
    /// perform Hermite spline interpolation
    static float4 hermite(const float4& v1, const float4& t1, const float4& v2, const float4& t2, scalar s);
    /// perform linear interpolation between 2 4d vectors
    static float4 lerp(const float4& v0, const float4& v1, scalar s);
    /// return 4d vector made up of largest components of 2 vectors
    static float4 maximize(const float4& v0, const float4& v1);
    /// return 4d vector made up of smallest components of 2 vectors
    static float4 minimize(const float4& v0, const float4& v1);
    /// return normalized version of 4d vector
    static float4 normalize(const float4& v);
    /// transform 4d vector by matrix44
    static float4 transform(const float4& v, const matrix44& m);
    /// reflect a vector v 
    static float4 reflect(const float4& normal, const float4& incident);
    /// clamp to min/max vector
    static float4 clamp(const float4& vClamp, const float4& vMin, const float4& vMax);
    /// angle between two vectors
    static scalar angle(const float4& v0, const float4& v1);

    /// return true if any XYZ component is less-then
    static bool less3_any(const float4& v0, const float4& v1);
    /// return true if all XYZ components are less-then
    static bool less3_all(const float4& v0, const float4& v1);
    /// return true if any XYZ component is less-or-equal 
    static bool lessequal3_any(const float4& v0, const float4& v1);
    /// return true if all XYZ components are less-or-equal
    static bool lessequal3_all(const float4& v0, const float4& v1);
    /// return true if any XYZ component is greater
    static bool greater3_any(const float4& v0, const float4& v1);
    /// return true if all XYZ components are greater
    static bool greater3_all(const float4& v0, const float4& v1);
    /// return true if any XYZ component is greater-or-equal
    static bool greaterequal3_any(const float4& v0, const float4& v1);
    /// return true if all XYZ components are greater-or-equal
    static bool greaterequal3_all(const float4& v0, const float4& v1);
    /// return true if any XYZ component is equal
    static bool equal3_any(const float4& v0, const float4& v1);
    /// return true if all XYZ components are equal
    static bool equal3_all(const float4& v0, const float4& v1);
    /// perform near equal comparison with given epsilon (3 components)
    static bool nearequal3(const float4& v0, const float4& v1, const float4& epsilon);

    /// return true if any XYZW component is less-then
    static bool less4_any(const float4& v0, const float4& v1);
    /// return true if all XYZW components are less-then
    static bool less4_all(const float4& v0, const float4& v1);
    /// return true if any XYZW component is less-or-equal 
    static bool lessequal4_any(const float4& v0, const float4& v1);
    /// return true if all XYZW components are less-or-equal
    static bool lessequal4_all(const float4& v0, const float4& v1);
    /// return true if any XYZW component is greater
    static bool greater4_any(const float4& v0, const float4& v1);
    /// return true if all XYZW components are greater
    static bool greater4_all(const float4& v0, const float4& v1);
    /// return true if any XYZW component is greater-or-equal
    static bool greaterequal4_any(const float4& v0, const float4& v1);
    /// return true if all XYZW components are greater-or-equal
    static bool greaterequal4_all(const float4& v0, const float4& v1);
    /// return true if any XYZW component is equal
    static bool equal4_any(const float4& v0, const float4& v1);
    /// return true if all XYZW components are equal
    static bool equal4_all(const float4& v0, const float4& v1);
    /// perform near equal comparison with given epsilon (4 components)
    static bool nearequal4(const float4& v0, const float4& v1, const float4& epsilon);

protected:
    friend class matrix44;
    friend class quaternion;

    scalar X;
    scalar Y;
    scalar Z;
    scalar W;
};

//------------------------------------------------------------------------------
/**
*/
__forceinline
float4::float4()
{
    //  empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline 
float4::float4(scalar x, scalar y, scalar z, scalar w) :
    X(x), Y(y), Z(z), W(w)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
float4::float4(const float4& rhs) :
    X(rhs.X), Y(rhs.Y), Z(rhs.Z), W(rhs.W)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
float4::operator=(const float4& rhs)
{
    this->X = rhs.X;
    this->Y = rhs.Y;
    this->Z = rhs.Z;
    this->W = rhs.W;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::operator==(const float4& rhs) const
{
    return (this->X == rhs.X) && (this->Y == rhs.Y) && (this->Z == rhs.Z) && (this->W == rhs.W);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::operator!=(const float4& rhs) const
{
    return (this->X != rhs.X) || (this->Y != rhs.Y) || (this->Z != rhs.Z) || (this->W != rhs.W);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
float4::load(const scalar* ptr)
{
    this->X = ptr[0];
    this->Y = ptr[1];
    this->Z = ptr[2];
    this->W = ptr[3];
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
float4::loadu(const scalar* ptr)
{
    this->X = ptr[0];
    this->Y = ptr[1];
    this->Z = ptr[2];
    this->W = ptr[3];
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
float4::store(scalar* ptr) const
{
    ptr[0] = this->X;
    ptr[1] = this->Y;
    ptr[2] = this->Z;
    ptr[3] = this->W;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
float4::storeu(scalar* ptr) const
{
    ptr[0] = this->X;
    ptr[1] = this->Y;
    ptr[2] = this->Z;
    ptr[3] = this->W;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
float4::stream(scalar* ptr) const
{
    ptr[0] = this->X;
    ptr[1] = this->Y;
    ptr[2] = this->Z;
    ptr[3] = this->W;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
float4::load_float3(const void* ptr, float w)
{
    const float* fltPtr = (const float*) ptr;
    this->X = fltPtr[0];
    this->Y = fltPtr[1];
    this->Z = fltPtr[2];
    this->W = w;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
float4::load_ubyte4n_signed(const void* ptr, float w)
{
    float mul = (1.0f / 255.0f);
    const uchar* ucharPtr = (const uchar*) ptr;
    this->X = ((float(ucharPtr[0]) * mul) * 2.0f) - 1.0f;
    this->Y = ((float(ucharPtr[1]) * mul) * 2.0f) - 1.0f;
    this->Z = ((float(ucharPtr[2]) * mul) * 2.0f) - 1.0f;
    this->W = w;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline float4
float4::operator-() const
{
    return float4(-this->X, -this->Y, -this->Z, -this->W);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline float4
float4::operator*(scalar t) const
{
    return float4(this->X * t, this->Y * t, this->Z * t, this->W * t);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
float4::operator+=(const float4& rhs)
{
    this->X += rhs.X;
    this->Y += rhs.Y;
    this->Z += rhs.Z;
    this->W += rhs.W;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
float4::operator-=(const float4& rhs)
{
    this->X -= rhs.X;
    this->Y -= rhs.Y;
    this->Z -= rhs.Z;
    this->W -= rhs.W;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
float4::operator*=(scalar s)
{
    this->X *= s;
    this->Y *= s;
    this->Z *= s;
    this->W *= s;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline float4
float4::operator+(const float4& rhs) const
{
    return float4(this->X + rhs.X, this->Y + rhs.Y, this->Z + rhs.Z, this->W + rhs.W);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline float4
float4::operator-(const float4& rhs) const
{
    return float4(this->X - rhs.X, this->Y - rhs.Y, this->Z - rhs.Z, this->W - rhs.W);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
float4::set(scalar x, scalar y, scalar z, scalar w)
{
    this->X = x;
    this->Y = y;
    this->Z = z;
    this->W = w;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar&
float4::x()
{
    return this->X;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
float4::x() const
{
    return this->X;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar&
float4::y()
{
    return this->Y;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
float4::y() const
{
    return this->Y;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar&
float4::z()
{
    return this->Z;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
float4::z() const
{
    return this->Z;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar&
float4::w()
{
    return this->W;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
float4::w() const
{
    return this->W;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
float4::length() const
{
    return D3DXVec4Length((CONST D3DXVECTOR4*)this);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
float4::lengthsq() const
{
    return D3DXVec4LengthSq((CONST D3DXVECTOR4*)this);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline float4
float4::reciprocal(const float4& v)
{
    return float4(1.0f / v.X, 1.0f / v.Y, 1.0f / v.Z, 1.0f / v.W);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline float4
float4::multiply(const float4& v0, const float4& v1)
{
    return float4(v0.X * v1.X, v0.Y * v1.Y, v0.Z * v1.Z, v0.W * v1.W);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline float4
float4::abs() const
{
    return float4(n_abs(this->X), n_abs(this->Y), n_abs(this->Z), n_abs(this->W));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline float4
float4::cross3(const float4& v0, const float4& v1)
{
    float4 res;
    D3DXVec3Cross((D3DXVECTOR3*)&res, (CONST D3DXVECTOR3*)&v0, (CONST D3DXVECTOR3*) &v1);
    res.W = 0.0f;
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
float4::dot3(const float4& v0, const float4& v1)
{
    return D3DXVec3Dot((CONST D3DXVECTOR3*)&v0, (CONST D3DXVECTOR3*) &v1);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline float4
float4::barycentric(const float4& v0, const float4& v1, const float4& v2, scalar f, scalar g)
{
    float4 res;
    D3DXVec4BaryCentric((D3DXVECTOR4*)&res, (CONST D3DXVECTOR4*)&v0, (CONST D3DXVECTOR4*)&v1, (CONST D3DXVECTOR4*)&v2, f, g);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline float4
float4::catmullrom(const float4& v0, const float4& v1, const float4& v2, const float4& v3, scalar s)
{
    float4 res;
    D3DXVec4CatmullRom((D3DXVECTOR4*)&res, (CONST D3DXVECTOR4*)&v0, (CONST D3DXVECTOR4*)&v1, (CONST D3DXVECTOR4*)&v2, (CONST D3DXVECTOR4*)&v3, s);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline float4
float4::hermite(const float4& v1, const float4& t1, const float4& v2, const float4& t2, scalar s)
{
    float4 res;
    D3DXVec4Hermite((D3DXVECTOR4*)&res, (CONST D3DXVECTOR4*)&v1, (CONST D3DXVECTOR4*)&t1, (CONST D3DXVECTOR4*)&v2, (CONST D3DXVECTOR4*)&t2, s);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline float4
float4::lerp(const float4& v0, const float4& v1, scalar s)
{
    float4 res;
    D3DXVec4Lerp((D3DXVECTOR4*)&res, (CONST D3DXVECTOR4*)&v0, (CONST D3DXVECTOR4*)&v1, s);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline float4
float4::maximize(const float4& v0, const float4& v1)
{
    float4 res;
    D3DXVec4Maximize((D3DXVECTOR4*)&res, (CONST D3DXVECTOR4*)&v0, (CONST D3DXVECTOR4*)&v1);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline float4
float4::minimize(const float4& v0, const float4& v1)
{
    float4 res;
    D3DXVec4Minimize((D3DXVECTOR4*)&res, (CONST D3DXVECTOR4*)&v0, (CONST D3DXVECTOR4*)&v1);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline float4
float4::normalize(const float4& v)
{
    float4 res;
    D3DXVec4Normalize((D3DXVECTOR4*)&res, (CONST D3DXVECTOR4*) &v);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline float4 
float4::reflect(const float4& normal, const float4& incident)
{
    scalar s = 2.0f * dot3(incident, normal);
    float4 result = incident - (normal * s);
    return result;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::less4_all(const float4& v0, const float4& v1)
{
    return ((v0.X < v1.X) && (v0.Y < v1.Y) && (v0.Z < v1.Z) && (v0.W < v1.W));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::less4_any(const float4& v0, const float4& v1)
{
    return ((v0.X < v1.X) || (v0.Y < v1.Y) || (v0.Z < v1.Z) || (v0.W < v1.W));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::lessequal4_all(const float4& v0, const float4& v1)
{
    return ((v0.X <= v1.X) && (v0.Y <= v1.Y) && (v0.Z <= v1.Z) && (v0.W <= v1.W));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::lessequal4_any(const float4& v0, const float4& v1)
{
    return ((v0.X <= v1.X) || (v0.Y <= v1.Y) || (v0.Z <= v1.Z) || (v0.W <= v1.W));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::greater4_all(const float4& v0, const float4& v1)
{
    return ((v0.X > v1.X) && (v0.Y > v1.Y) && (v0.Z > v1.Z) && (v0.W > v1.W));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::greater4_any(const float4& v0, const float4& v1)
{
    return ((v0.X > v1.X) || (v0.Y > v1.Y) || (v0.Z > v1.Z) || (v0.W > v1.W));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::greaterequal4_all(const float4& v0, const float4& v1)
{
    return ((v0.X >= v1.X) && (v0.Y >= v1.Y) && (v0.Z >= v1.Z) && (v0.W >= v1.W));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::greaterequal4_any(const float4& v0, const float4& v1)
{
    return ((v0.X >= v1.X) || (v0.Y >= v1.Y) || (v0.Z >= v1.Z) || (v0.W >= v1.W));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::equal4_all(const float4& v0, const float4& v1)
{
    return ((v0.X == v1.X) && (v0.Y == v1.Y) && (v0.Z == v1.Z) && (v0.W == v1.W));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::equal4_any(const float4& v0, const float4& v1)
{
    return ((v0.X == v1.X) || (v0.Y == v1.Y) || (v0.Z == v1.Z) || (v0.W == v1.W));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::nearequal4(const float4& v0, const float4& v1, const float4& epsilon)
{
    return ((v0.X >= v1.X - epsilon.X && v0.X <= v1.X + epsilon.X) &&
            (v0.Y >= v1.Y - epsilon.Y && v0.Y <= v1.Y + epsilon.Y) &&
            (v0.Z >= v1.Z - epsilon.Z && v0.Z <= v1.Z + epsilon.Z) &&
            (v0.W >= v1.W - epsilon.W && v0.W <= v1.W + epsilon.W));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::less3_all(const float4& v0, const float4& v1)
{
    return ((v0.X < v1.X) && (v0.Y < v1.Y) && (v0.Z < v1.Z));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::less3_any(const float4& v0, const float4& v1)
{
    return ((v0.X < v1.X) || (v0.Y < v1.Y) || (v0.Z < v1.Z));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::lessequal3_all(const float4& v0, const float4& v1)
{
    return ((v0.X <= v1.X) && (v0.Y <= v1.Y) && (v0.Z <= v1.Z));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::lessequal3_any(const float4& v0, const float4& v1)
{
    return ((v0.X <= v1.X) || (v0.Y <= v1.Y) || (v0.Z <= v1.Z));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::greater3_all(const float4& v0, const float4& v1)
{
    return ((v0.X > v1.X) && (v0.Y > v1.Y) && (v0.Z > v1.Z));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::greater3_any(const float4& v0, const float4& v1)
{
    return ((v0.X > v1.X) || (v0.Y > v1.Y) || (v0.Z > v1.Z));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::greaterequal3_all(const float4& v0, const float4& v1)
{
    return ((v0.X >= v1.X) && (v0.Y >= v1.Y) && (v0.Z >= v1.Z));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::greaterequal3_any(const float4& v0, const float4& v1)
{
    return ((v0.X >= v1.X) || (v0.Y >= v1.Y) || (v0.Z >= v1.Z));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::equal3_all(const float4& v0, const float4& v1)
{
    return ((v0.X == v1.X) && (v0.Y == v1.Y) && (v0.Z == v1.Z));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::equal3_any(const float4& v0, const float4& v1)
{
    return ((v0.X == v1.X) || (v0.Y == v1.Y) || (v0.Z == v1.Z));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
float4::nearequal3(const float4& v0, const float4& v1, const float4& epsilon)
{
    return ((v0.X >= v1.X - epsilon.X && v0.X <= v1.X + epsilon.X) &&
            (v0.Y >= v1.Y - epsilon.Y && v0.Y <= v1.Y + epsilon.Y) &&
            (v0.Z >= v1.Z - epsilon.Z && v0.Z <= v1.Z + epsilon.Z));
}

} // namespace Math
//------------------------------------------------------------------------------

    
    