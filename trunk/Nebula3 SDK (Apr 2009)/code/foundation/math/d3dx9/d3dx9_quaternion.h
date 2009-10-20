#pragma once
//------------------------------------------------------------------------------
/**
    @class Math::quaternion
  
    A quaternion class on top of the D3DX9 math functions.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "core/types.h"
#include "math/scalar.h"
#include "math/float4.h"

//------------------------------------------------------------------------------
namespace Math
{
__declspec(align(16)) class quaternion
{
public:
    /// default constructor, NOTE: does NOT setup components!
    quaternion();
    /// construct from components
    quaternion(scalar x, scalar y, scalar z, scalar w);
    /// construct from float4
    quaternion(const float4& rhs);
    /// copy constructor
    quaternion(const quaternion& rhs);

    /// assignment operator
    void operator=(const quaternion& rhs);
    /// equality operator
    bool operator==(const quaternion& rhs) const;
    /// inequality operator
    bool operator!=(const quaternion& rhs) const;

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

    /// set content
    void set(scalar x, scalar y, scalar z, scalar w);
    /// set from float4
    void set(const float4& f4);
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
    
    /// return true if quaternion is identity
    bool isidentity() const;
    /// returns length
    scalar length() const;
    /// returns length squared
    scalar lengthsq() const;
    /// un-denormalize quaternion (this is sort of a hack since Maya likes to return denormal quaternions)
    void undenormalize();

    /// return quaternion in barycentric coordinates
    static quaternion barycentric(const quaternion& q0, const quaternion& q1, const quaternion& q2, scalar f, scalar g);
    /// return conjugate of a normalized quaternion
    static quaternion conjugate(const quaternion& q);
    /// return dot product of two normalized quaternions
    static scalar dot(const quaternion& q0, const quaternion& q1);
    /// calculate the exponential
    static quaternion exp(const quaternion& q0);
    /// returns an identity quaternion
    static quaternion identity();
    /// conjugates and renormalizes quaternion
    static quaternion inverse(const quaternion& q);
    /// calculate the natural logarithm
    static quaternion ln(const quaternion& q);
    /// multiply 2 quaternions
    static quaternion multiply(const quaternion& q0, const quaternion& q1);
    /// compute unit length quaternion
    static quaternion normalize(const quaternion& q);
    /// build quaternion from axis and clockwise rotation angle in radians
    static quaternion rotationaxis(const float4& axis, scalar angle);
    /// build quaternion from rotation matrix
    static quaternion rotationmatrix(const matrix44& m);
    /// build quaternion from yaw, pitch and roll
    static quaternion rotationyawpitchroll(scalar yaw, scalar pitch, scalar roll);
    /// interpolate between 2 quaternion using spherical interpolation
    static quaternion slerp(const quaternion& q1, const quaternion& q2, scalar t);
    /// setup control points for spherical quadrangle interpolation
    static void squadsetup(const quaternion& q0, const quaternion& q1, const quaternion& q2, const quaternion& q3, quaternion& aOut, quaternion& bOut, quaternion& cOut);
    /// interpolate between quaternions using spherical quadrangle interpolation
    static quaternion squad(const quaternion& q1, const quaternion& a, const quaternion& b, const quaternion& c, scalar t);
    /// convert quaternion to axis and angle
    static void to_axisangle(const quaternion& q, float4& outAxis, scalar& outAngle);

private:
    scalar X;
    scalar Y;
    scalar Z;
    scalar W;
};

//------------------------------------------------------------------------------
/**
*/
__forceinline
quaternion::quaternion()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
quaternion::quaternion(scalar x, scalar y, scalar z, scalar w) :
    X(x), Y(y), Z(z), W(w)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
quaternion::quaternion(const float4& rhs)
{
    this->X = rhs.x();
    this->Y = rhs.y();
    this->Z = rhs.z();
    this->W = rhs.w();
}
    
//------------------------------------------------------------------------------
/**
*/
__forceinline
quaternion::quaternion(const quaternion& rhs) :
    X(rhs.X), Y(rhs.Y), Z(rhs.Z), W(rhs.W)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
quaternion::operator=(const quaternion& rhs)
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
quaternion::operator==(const quaternion& rhs) const
{
    return (this->X == rhs.X) && (this->Y == rhs.Y) && (this->Z == rhs.Z) && (this->W == rhs.W);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
quaternion::operator!=(const quaternion& rhs) const
{
    return (this->X != rhs.X) || (this->Y != rhs.Y) || (this->Z != rhs.Z) || (this->W != rhs.W);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
quaternion::load(const scalar* ptr)
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
quaternion::loadu(const scalar* ptr)
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
quaternion::store(scalar* ptr) const
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
quaternion::storeu(scalar* ptr) const
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
quaternion::stream(scalar* ptr) const
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
quaternion::set(scalar x, scalar y, scalar z, scalar w)
{
    this->X = x;
    this->Y = y;
    this->Z = z;
    this->W = w;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
quaternion::set(const float4& f4)
{
    this->X = f4.X;
    this->Y = f4.Y;
    this->Z = f4.Z;
    this->W = f4.W;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar&
quaternion::x()
{
    return this->X;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
quaternion::x() const
{
    return this->X;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar&
quaternion::y()
{
    return this->Y;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
quaternion::y() const
{
    return this->Y;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar&
quaternion::z()
{
    return this->Z;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
quaternion::z() const
{
    return this->Z;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar&
quaternion::w()
{
    return this->W;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
quaternion::w() const
{
    return this->W;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
quaternion::isidentity() const
{
    return TRUE == D3DXQuaternionIsIdentity((CONST D3DXQUATERNION*)this);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
quaternion::length() const
{
    return D3DXQuaternionLength((CONST D3DXQUATERNION*)this);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
quaternion::lengthsq() const
{
    return D3DXQuaternionLengthSq((CONST D3DXQUATERNION*)this);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
quaternion::undenormalize()
{
    this->X = n_undenormalize(this->X);
    this->Y = n_undenormalize(this->Y);
    this->Z = n_undenormalize(this->Z);
    this->W = n_undenormalize(this->W);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline quaternion
quaternion::barycentric(const quaternion& q0, const quaternion& q1, const quaternion& q2, scalar f, scalar g)
{
    quaternion res;
    D3DXQuaternionBaryCentric((D3DXQUATERNION*)&res, (CONST D3DXQUATERNION*)&q0, (CONST D3DXQUATERNION*)&q1, (CONST D3DXQUATERNION*)&q2, f, g);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline quaternion
quaternion::conjugate(const quaternion& q)
{
    quaternion res;
    D3DXQuaternionConjugate((D3DXQUATERNION*)&res, (CONST D3DXQUATERNION*)&q);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
quaternion::dot(const quaternion& q0, const quaternion& q1)
{
    return D3DXQuaternionDot((CONST D3DXQUATERNION*)&q0, (CONST D3DXQUATERNION*)&q1);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline quaternion
quaternion::exp(const quaternion& q)
{
    quaternion res;
    D3DXQuaternionExp((D3DXQUATERNION*)&res, (CONST D3DXQUATERNION*)&q);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline quaternion
quaternion::identity()
{
    quaternion res;
    D3DXQuaternionIdentity((D3DXQUATERNION*)&res);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline quaternion
quaternion::inverse(const quaternion& q)
{
    quaternion res;
    D3DXQuaternionInverse((D3DXQUATERNION*)&res, (CONST D3DXQUATERNION*)&q);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline quaternion
quaternion::ln(const quaternion& q)
{
    quaternion res;
    D3DXQuaternionLn((D3DXQUATERNION*)&res, (CONST D3DXQUATERNION*)&q);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline quaternion
quaternion::multiply(const quaternion& q0, const quaternion& q1)
{
    quaternion res;
    D3DXQuaternionMultiply((D3DXQUATERNION*)&res, (CONST D3DXQUATERNION*)&q0, (CONST D3DXQUATERNION*)&q1);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline quaternion
quaternion::normalize(const quaternion& q)
{
    quaternion res;
    D3DXQuaternionNormalize((D3DXQUATERNION*)&res, (CONST D3DXQUATERNION*)&q);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline quaternion
quaternion::rotationaxis(const float4& axis, scalar angle)
{
    quaternion res;
    D3DXQuaternionRotationAxis((D3DXQUATERNION*)&res, (CONST D3DXVECTOR3*)&axis, angle);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline quaternion
quaternion::rotationmatrix(const matrix44& m)
{
    quaternion res;
    D3DXQuaternionRotationMatrix((D3DXQUATERNION*)&res, (CONST D3DXMATRIX*)&m);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline quaternion
quaternion::rotationyawpitchroll(scalar yaw, scalar pitch, scalar roll)
{
    quaternion res;
    D3DXQuaternionRotationYawPitchRoll((D3DXQUATERNION*)&res, yaw, pitch, roll);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline quaternion
quaternion::slerp(const quaternion& q1, const quaternion& q2, scalar t)
{
    quaternion res;
    D3DXQuaternionSlerp((D3DXQUATERNION*)&res, (CONST D3DXQUATERNION*)&q1, (CONST D3DXQUATERNION*)&q2, t);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
quaternion::squadsetup(const quaternion& q0, const quaternion& q1, const quaternion& q2, const quaternion& q3, quaternion& aOut, quaternion& bOut, quaternion& cOut)
{
    D3DXQuaternionSquadSetup((D3DXQUATERNION*)&aOut,
                             (D3DXQUATERNION*)&bOut,
                             (D3DXQUATERNION*)&cOut,
                             (CONST D3DXQUATERNION*)&q0,
                             (CONST D3DXQUATERNION*)&q1,
                             (CONST D3DXQUATERNION*)&q2,
                             (CONST D3DXQUATERNION*)&q3);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline quaternion
quaternion::squad(const quaternion& q1, const quaternion& a, const quaternion& b, const quaternion& c, scalar t)
{
    quaternion res;
    D3DXQuaternionSquad((D3DXQUATERNION*)&res, (CONST D3DXQUATERNION*)&q1, (CONST D3DXQUATERNION*)&a, (CONST D3DXQUATERNION*)&b, (CONST D3DXQUATERNION*)&c, t);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
quaternion::to_axisangle(const quaternion& q, float4& outAxis, scalar& outAngle)
{
    outAxis.set(0.0f, 0.0f, 0.0f, 0.0f);
    D3DXQuaternionToAxisAngle((CONST D3DXQUATERNION*)&q, (D3DXVECTOR3*)&outAxis, &outAngle);
}

} // namespace Math
//------------------------------------------------------------------------------
