#pragma once
#ifndef MATH_TRANSFORM44_H
#define MATH_TRANSFORM44_H
//------------------------------------------------------------------------------
/**
    @class Math::transform44

    A 4x4 matrix which is described by translation, rotation and scale.
    
    (C) 2004 RadonLabs GmbH
*/
#include "math/matrix44.h"
#include "math/vector.h"
#include "math/point.h"
#include "math/quaternion.h"

//------------------------------------------------------------------------------
namespace Math
{
class transform44
{
public:
    /// constructor
    transform44();
    /// set position
    void setposition(const point& p);
    /// get position
    const point& getposition() const;
    /// set rotate
    void setrotate(const quaternion& r);
    /// get rotate
    const quaternion& getrotate() const;
    /// set scale
    void setscale(const vector& s);
    /// get scale
    const vector& getscale() const;
    /// set optional rotate pivot
    void setrotatepivot(const point& p);
    /// get optional rotate pivot
    const point& getrotatepivot() const;
    /// set optional scale pivot
    void setscalepivot(const point& p);
    /// get optional scale pivot
    const point& getscalepivot() const;
    /// get resulting 4x4 matrix
    const matrix44& getmatrix();
    /// return true if the transformation matrix is dirty
    bool isdirty() const;

private:
    point position;
    quaternion rotate;
    vector scale;
    point rotatePivot;
    point scalePivot;
    matrix44 matrix;
    bool isDirty;
};

//------------------------------------------------------------------------------
/**
*/
inline
transform44::transform44() :
    position(0.0f, 0.0f, 0.0f),
    rotate(0.0f, 0.0f, 0.0f, 1.0f),
    scale(1.0f, 1.0f, 1.0f),
    rotatePivot(0.0f, 0.0f, 0.0f),
    scalePivot(0.0f, 0.0f, 0.0f),
    isDirty(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void
transform44::setposition(const point& p)
{
    this->position = p;
    this->isDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline const point&
transform44::getposition() const
{
    return this->position;
}

//------------------------------------------------------------------------------
/**
*/
inline void
transform44::setrotate(const quaternion& r)
{
    this->rotate = r;
    this->isDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline const quaternion&
transform44::getrotate() const
{
    return this->rotate;
}

//------------------------------------------------------------------------------
/**
*/
inline void
transform44::setscale(const vector& s)
{
    this->scale = s;
    this->isDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline const vector&
transform44::getscale() const
{
    return this->scale;
}

//------------------------------------------------------------------------------
/**
*/
inline void
transform44::setrotatepivot(const point& p)
{
	this->rotatePivot = p;
    this->isDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline const point& 
transform44::getrotatepivot() const
{
    return this->rotatePivot;
}

//------------------------------------------------------------------------------
/**
*/
inline void
transform44::setscalepivot(const point& p)
{
    this->scalePivot = p;
    this->isDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline const point&
transform44::getscalepivot() const
{
    return this->scalePivot;
}

//------------------------------------------------------------------------------
/**
*/
inline const matrix44&
transform44::getmatrix()
{
    if (this->isDirty)
    {
        quaternion ident = quaternion::identity();
        this->matrix = matrix44::transformation(this->scalePivot, ident, this->scale, this->rotatePivot, this->rotate, this->position);
        this->isDirty = false;
    }
    return this->matrix;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
transform44::isdirty() const
{
    return this->isDirty;
}

} // namespace Math
//------------------------------------------------------------------------------
#endif

    
    