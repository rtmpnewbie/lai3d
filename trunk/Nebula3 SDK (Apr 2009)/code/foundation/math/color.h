#pragma once
//------------------------------------------------------------------------------
/**
    @class Math::color

    A 4 byte ARGB color class.
    
    (C) 2008 xoyojank
*/
#include "core/types.h"
#include "math/scalar.h"
#include "math/float4.h"

//------------------------------------------------------------------------------
namespace Math
{
class color
{
public:
    /// default constructor, NOTE: does NOT setup components!
    color();
    /// construct from scalar values
    color(scalar r, scalar g, scalar b, scalar a);
	/// construct from byte values
	color(byte A, byte R, byte G, byte B);
	/// construct from float4 value
	color(const float4& clr);
	/// construct from uint value
	color(uint clr);
    /// copy constructor
    color(const color& rhs);
    /// assignment operator
    void operator=(const color& rhs);
    /// inplace add
    void operator+=(const color& rhs);
    /// inplace sub
    void operator-=(const color& rhs);
    /// inplace scalar multiply
    void operator*=(scalar s);
    /// add 2 vectors
    color operator+(const color& rhs) const;
    /// subtract 2 vectors
    color operator-(const color& rhs) const;
    /// multiply with scalar
    color operator*(scalar s) const;
    /// equality operator
    bool operator==(const color& rhs) const;
    /// inequality operator
    bool operator!=(const color& rhs) const;
    /// set content by scalar values
    void set(scalar r, scalar g, scalar b, scalar a);
	/// set content by byte values
	void set(byte A, byte R, byte G, byte B);
    /// read-only access to red component
    scalar r() const;
	/// read-only access to green component
	scalar g() const;
	/// read-only access to blue component
	scalar b() const;
	/// read-only access to alpha component
	scalar a() const;
	/// read-write access to alpha component
	byte& A();
	/// read-write access to red component
	byte& R();
	/// read-write access to green component
	byte& G();
	/// read-write access to blue component
	byte& B();
	/// read-only access to alpha component
	byte A() const;
	/// read-only access to red component
	byte R() const;
	/// read-only access to green component
	byte G() const;
	/// read-only access to blue component
	byte B() const;
	/// return an inverse copy (alpha do not inverse)
	color inverse() const;
	/// convert to luminance color
	color to_luminance() const;
	/// convert to RGBA format
	uint to_rgba() const;
	/// convert to ABGR format
	uint to_abgr() const;
	/// convert to float4
	float4 to_float4() const;
	/// convert to uint
	uint to_uint() const;

	/// convert from RGBA format
	static color from_rgba(uint clr);

public:
	/*********** ���õ���ɫֵ������ɫ���ο� http://www.ttpw.com/rgb.htm ************/
	static const uint LightPink			= 0xFFFFB6C1; ///< ǳ�ۺ�
	static const uint Pink				= 0xFFFFC0CB; ///< �ۺ�
	static const uint Crimson			= 0xFFDC143C; ///< ���/�ɺ�
	static const uint LavenderBlush		= 0xFFFFF0F5; ///< ���Ϻ�
	static const uint PaleVioletRed		= 0xFFDB7093; ///< ����������
	static const uint HotPink			= 0xFFFF69B4; ///< ����ķۺ�
	static const uint DeepPink			= 0xFFFF1493; ///< ��ۺ�
	static const uint MediumVioletRed	= 0xFFC71585; ///< ����������
	static const uint Orchid			= 0xFFDA70D6; ///< ����ɫ/������
	static const uint Thistle			= 0xFFD8BFD8; ///< ��ɫ
	static const uint Plum				= 0xFFDDA0DD; ///< ����ɫ/������
	static const uint Violet			= 0xFFEE82EE; ///< ������
	static const uint Magenta			= 0xFFFF00FF; ///< ���/õ���
	static const uint Fuchsia			= 0xFFFF00FF; ///< �Ϻ�/��������
	static const uint DarkMagenta		= 0xFF8B008B; ///< �����
	static const uint Purple			= 0xFF800080; ///< ��ɫ
	static const uint MediumOrchid		= 0xFFBA55D3; ///< ��������
	static const uint DarkViolet		= 0xFF9400D3; ///< ��������
	static const uint DarkOrchid		= 0xFF9932CC; ///< ��������
	static const uint Indigo			= 0xFF4B0082; ///< ����/����ɫ
	static const uint BlueViolet		= 0xFF8A2BE2; ///< ��������
	static const uint MediumPurple		= 0xFF9370DB; ///< ����ɫ
	static const uint MediumSlateBlue = 0xFF7B68EE; ///< �а���ɫ/�а�����
	static const uint SlateBlue			= 0xFF6A5ACD; ///< ʯ��ɫ/������
	static const uint DarkSlateBlue		= 0xFF483D8B; ///< ������ɫ/��������
	static const uint Lavender			= 0xFFE6E6FA; ///< ����ɫ/Ѭ�²ݵ���
	static const uint GhostWhite		= 0xFFF8F8FF; ///< �����
	static const uint Blue				= 0xFF0000FF; ///< ����
	static const uint MediumBlue		= 0xFF0000CD; ///< ����ɫ
	static const uint MidnightBlue		= 0xFF191970; ///< ��ҹ��
	static const uint DarkBlue			= 0xFF00008B; ///< ����ɫ
	static const uint Navy				= 0xFF000080; ///< ������
	static const uint RoyalBlue			= 0xFF4169E1; ///< �ʼ���/����
	static const uint CornflowerBlue	= 0xFF6495ED; ///< ʸ������
	static const uint LightSteelBlue	= 0xFFB0C4DE; ///< ������
	static const uint LightSlateGray	= 0xFF778899; ///< ������/��ʯ���
	static const uint SlateGray			= 0xFF708090; ///< ��ʯɫ/ʯ���
	static const uint DodgerBlue		= 0xFF1E90FF; ///< ����ɫ/������
	static const uint AliceBlue			= 0xFFF0F8FF; ///< ����˿��
	static const uint SteelBlue			= 0xFF4682B4; ///< ����/����
	static const uint LightSkyBlue		= 0xFF87CEFA; ///< ������ɫ
	static const uint SkyBlue			= 0xFF87CEEB; ///< ����ɫ
	static const uint DeepSkyBlue		= 0xFF00BFFF; ///< ������
	static const uint LightBlue			= 0xFFADD8E6; ///< ����
	static const uint PowderBlue		= 0xFFB0E0E6; ///< ����ɫ/��ҩ��
	static const uint CadetBlue			= 0xFF5F9EA0; ///< ����ɫ/������
	static const uint Azure				= 0xFFF0FFFF; ///< ε��ɫ
	static const uint LightCyan			= 0xFFE0FFFF; ///< ����ɫ
	static const uint PaleTurquoise		= 0xFFAFEEEE; ///< ���̱�ʯ
	static const uint Cyan				= 0xFF00FFFF; ///< ��ɫ
	static const uint Aqua				= 0xFF00FFFF; ///< ǳ��ɫ/ˮɫ
	static const uint DarkTurquoise		= 0xFF00CED1; ///< ���̱�ʯ
	static const uint DarkSlateGray		= 0xFF2F4F4F; ///< ���߻�ɫ/��ʯ���
	static const uint DarkCyan			= 0xFF008B8B; ///< ����ɫ
	static const uint Teal				= 0xFF008080; ///< ˮѼɫ
	static const uint MediumTurquoise	= 0xFF48D1CC; ///< ���̱�ʯ
	static const uint LightSeaGreen		= 0xFF20B2AA; ///< ǳ������
	static const uint Turquoise			= 0xFF40E0D0; ///< �̱�ʯ
	static const uint Aquamarine		= 0xFF7FFFD4; ///< ��ʯ����
	static const uint MediumAquamarine	= 0xFF66CDAA; ///< �б�ʯ����
	static const uint MediumSpringGreen = 0xFF00FA9A; ///< �д���ɫ
	static const uint MintCream			= 0xFFF5FFFA; ///< ��������
	static const uint SpringGreen		= 0xFF00FF7F; ///< ����ɫ
	static const uint MediumSeaGreen	= 0xFF3CB371; ///< �к�����
	static const uint SeaGreen			= 0xFF2E8B57; ///< ������
	static const uint Honeydew			= 0xFFF0FFF0; ///< ��ɫ/�۹�ɫ
	static const uint LightGreen		= 0xFF90EE90; ///< ����ɫ
	static const uint PaleGreen			= 0xFF98FB98; ///< ����ɫ
	static const uint DarkSeaGreen		= 0xFF8FBC8F; ///< ��������
	static const uint LimeGreen			= 0xFF32CD32; ///< ��������
	static const uint Lime				= 0xFF00FF00; ///< ������
	static const uint ForestGreen		= 0xFF228B22; ///< ɭ����
	static const uint Green				= 0xFF008000; ///< ����
	static const uint DarkGreen			= 0xFF006400; ///< ����ɫ
	static const uint Chartreuse		= 0xFF7FFF00; ///< ����ɫ/���ؾ���
	static const uint LawnGreen			= 0xFF7CFC00; ///< ����ɫ/��ƺ��
	static const uint GreenYellow		= 0xFFADFF2F; ///< �̻�ɫ
	static const uint DarkOliveGreen	= 0xFF556B2F; ///< �������
	static const uint YellowGreen		= 0xFF9ACD32; ///< ����ɫ
	static const uint OliveDrab			= 0xFF6B8E23; ///< ��魺�ɫ
	static const uint Beige				= 0xFFF5F5DC; ///< ��ɫ/����ɫ
	static const uint LightGoldenrodYellow = 0xFFFAFAD2; ///< ���ջ�
	static const uint Ivory				= 0xFFFFFFF0; ///< ����ɫ
	static const uint LightYellow		= 0xFFFFFFE0; ///< ǳ��ɫ
	static const uint Yellow			= 0xFFFFFF00; ///< ����
	static const uint Olive				= 0xFF808000; ///< ���
	static const uint DarkKhaki			= 0xFFBDB76B; ///< ���ƺ�ɫ/�ߴ��
	static const uint LemonChiffon		= 0xFFFFFACD; ///< ���ʳ�
	static const uint PaleGoldenrod		= 0xFFEEE8AA; ///< �Ҿջ�/������ɫ
	static const uint Khaki				= 0xFFF0E68C; ///< �ƺ�ɫ/��ߴ��
	static const uint Gold				= 0xFFFFD700; ///< ��ɫ
	static const uint Cornsilk			= 0xFFFFF8DC; ///< ����˿ɫ
	static const uint Goldenrod			= 0xFFDAA520; ///< ��ջ�
	static const uint DarkGoldenrod		= 0xFFB8860B; ///< ����ջ�
	static const uint FloralWhite		= 0xFFFFFAF0; ///< ���İ�ɫ
	static const uint OldLace			= 0xFFFDF5E6; ///< �ϻ�ɫ/����˿
	static const uint Wheat				= 0xFFF5DEB3; ///< ǳ��ɫ/С��ɫ
	static const uint Moccasin			= 0xFFFFE4B5; ///< ¹Ƥɫ/¹Ƥѥ
	static const uint Orange			= 0xFFFFA500; ///< ��ɫ
	static const uint PapayaWhip		= 0xFFFFEFD5; ///< ��ľɫ/��ľ��
	static const uint BlanchedAlmond	= 0xFFFFEBCD; ///< ����ɫ
	static const uint NavajoWhite		= 0xFFFFDEAD; ///< ���߰�/������
	static const uint AntiqueWhite		= 0xFFFAEBD7; ///< �Ŷ���
	static const uint Tan				= 0xFFD2B48C; ///< ��ɫ
	static const uint BurlyWood			= 0xFFDEB887; ///< Ӳľɫ
	static const uint Bisque			= 0xFFFFE4C4; ///< ������
	static const uint DarkOrange		= 0xFFFF8C00; ///< ���ɫ
	static const uint Linen				= 0xFFFAF0E6; ///< ���鲼
	static const uint Peru				= 0xFFCD853F; ///< ��³ɫ
	static const uint PeachPuff			= 0xFFFFDAB9; ///< ����ɫ
	static const uint SandyBrown		= 0xFFF4A460; ///< ɳ��ɫ
	static const uint Chocolate			= 0xFFD2691E; ///< �ɿ���ɫ
	static const uint SaddleBrown		= 0xFF8B4513; ///< �غ�ɫ/����ɫ
	static const uint Seashell			= 0xFFFFF5EE; ///< ������
	static const uint Sienna			= 0xFFA0522D; ///< ������ɫ
	static const uint LightSalmon		= 0xFFFFA07A; ///< ǳ������ɫ
	static const uint Coral				= 0xFFFF7F50; ///< ɺ��
	static const uint OrangeRed			= 0xFFFF4500; ///< �Ⱥ�ɫ
	static const uint DarkSalmon		= 0xFFE9967A; ///< ������/����ɫ
	static const uint Tomato			= 0xFFFF6347; ///< ���Ѻ�
	static const uint MistyRose			= 0xFFFFE4E1; ///< ǳõ��ɫ/����õ��
	static const uint Salmon			= 0xFFFA8072; ///< ����/����ɫ
	static const uint Snow				= 0xFFFFFAFA; ///< ѩ��ɫ
	static const uint LightCoral		= 0xFFF08080; ///< ��ɺ��ɫ
	static const uint RosyBrown			= 0xFFBC8F8F; ///< õ����ɫ
	static const uint IndianRed			= 0xFFCD5C5C; ///< ӡ�Ⱥ�
	static const uint Red				= 0xFFFF0000; ///< ����
	static const uint Brown				= 0xFFA52A2A; ///< ��ɫ
	static const uint FireBrick			= 0xFFB22222; ///< ��שɫ/�ͻ�ש
	static const uint DarkRed			= 0xFF8B0000; ///< ���ɫ
	static const uint Maroon			= 0xFF800000; ///< ��ɫ
	static const uint White				= 0xFFFFFFFF; ///< ����
	static const uint WhiteSmoke		= 0xFFF5F5F5; ///< ����
	static const uint Gainsboro			= 0xFFDCDCDC; ///< ����ɫ
	static const uint LightGrey			= 0xFFD3D3D3; ///< ǳ��ɫ
	static const uint Silver			= 0xFFC0C0C0; ///< ����ɫ
	static const uint DarkGray			= 0xFFA9A9A9; ///< ���ɫ
	static const uint Gray				= 0xFF808080; ///< ��ɫ
	static const uint DimGray			= 0xFF696969; ///< ������
	static const uint Black				= 0xFF000000; ///< ����

private:
	union
	{
		uint value;
		struct 
		{
			byte _B;
			byte _G;
			byte _R;
			byte _A;
		};
	};
};

//------------------------------------------------------------------------------
inline
color::color()
{
	// empty
}

//------------------------------------------------------------------------------
inline
color::color( scalar r, scalar g, scalar b, scalar a )
{
	n_assert(r >= 0.0f && r <= 1.0f);
	n_assert(g >= 0.0f && g <= 1.0f);
	n_assert(b >= 0.0f && b <= 1.0f);
	n_assert(a >= 0.0f && a <= 1.0f);
	this->_A = byte(a * 255.0f);
	this->_R = byte(r * 255.0f);
	this->_G = byte(g * 255.0f);
	this->_B = byte(b * 255.0f);
}

//------------------------------------------------------------------------------
inline
color::color( byte A, byte R, byte G, byte B )
: _A(A), _R(R), _G(G), _B(B)
{
	// empty
}

//------------------------------------------------------------------------------
inline
color::color( const float4& clr )
{
	new(this)color(clr.x(), clr.y(), clr.z(), clr.w());
}

//------------------------------------------------------------------------------
inline
color::color( uint clr )
: value(clr)
{
	// empty
}

//------------------------------------------------------------------------------
inline
color::color( const color& rhs )
: value(rhs.value)
{
	// empty
}

//------------------------------------------------------------------------------
inline void
color::operator=( const color& rhs )
{
	this->value = rhs.value;
}

//------------------------------------------------------------------------------
inline void
color::operator+=( const color& rhs )
{
	this->_A = (byte)n_min(int(this->_A) + rhs._A, 255);
	this->_R = (byte)n_min(int(this->_R) + rhs._R, 255);
	this->_G = (byte)n_min(int(this->_G) + rhs._G, 255);
	this->_B = (byte)n_min(int(this->_B) + rhs._B, 255);
}

//------------------------------------------------------------------------------
inline void
color::operator-=( const color& rhs )
{
	this->_A = (byte)n_max(int(this->_A) - rhs._A, 0);
	this->_R = (byte)n_max(int(this->_R) - rhs._R, 0);
	this->_G = (byte)n_max(int(this->_G) - rhs._G, 0);
	this->_B = (byte)n_max(int(this->_B) - rhs._B, 0);
}

//------------------------------------------------------------------------------
inline void
color::operator*=( scalar s )
{
	this->_A = byte(this->_A * s + 0.5f);
	this->_R = byte(this->_R * s + 0.5f);
	this->_G = byte(this->_G * s + 0.5f);
	this->_B = byte(this->_B * s + 0.5f);
}

//------------------------------------------------------------------------------
inline color
color::operator+( const color& rhs ) const
{
	return color((byte)n_min(int(this->_A) + rhs._A, 255),
		(byte)n_min(int(this->_R) + rhs._R, 255),
		(byte)n_min(int(this->_G) + rhs._G, 255),
		(byte)n_min(int(this->_B) + rhs._B, 255));
}

//------------------------------------------------------------------------------
inline color
color::operator-( const color& rhs ) const
{
	return color((byte)n_max(int(this->_A) - rhs._A, 0),
		(byte)n_max(int(this->_R) - rhs._R, 0),
		(byte)n_max(int(this->_G) - rhs._G, 0),
		(byte)n_max(int(this->_B) - rhs._B, 0));
}

//------------------------------------------------------------------------------
inline color
color::operator*( scalar s ) const
{
	return color(byte(this->_A * s + 0.5f),
		byte(this->_R * s + 0.5f),
		byte(this->_G * s + 0.5f),
		byte(this->_B * s + 0.5f));
}

//------------------------------------------------------------------------------
inline bool
color::operator==( const color& rhs ) const
{
	return this->value == rhs.value;
}

//------------------------------------------------------------------------------
inline bool
color::operator!=( const color& rhs ) const
{
	return this->value != rhs.value;
}

//------------------------------------------------------------------------------
inline void
color::set( scalar r, scalar g, scalar b, scalar a )
{
	n_assert(r >= 0.0f && r <= 1.0f);
	n_assert(g >= 0.0f && g <= 1.0f);
	n_assert(b >= 0.0f && b <= 1.0f);
	n_assert(a >= 0.0f && a <= 1.0f);
	this->_A = byte(a * 255.0f);
	this->_R = byte(r * 255.0f);
	this->_G = byte(g * 255.0f);
	this->_B = byte(b * 255.0f);
}

//------------------------------------------------------------------------------
inline void
color::set( byte A, byte R, byte G, byte B )
{
	this->_A = A;
	this->_R = R;
	this->_G = G;
	this->_B = B;
}

//------------------------------------------------------------------------------
inline scalar
color::r() const
{
	return this->_R / 255.0f;
}

//------------------------------------------------------------------------------
inline scalar
color::g() const
{
	return this->_G / 255.0f;
}

//------------------------------------------------------------------------------
inline scalar
color::b() const
{
	return this->_B / 255.0f;
}

//------------------------------------------------------------------------------
inline scalar
color::a() const
{
	return this->_A / 255.0f;
}

//------------------------------------------------------------------------------
inline byte
color::A() const
{
	return this->_A;
}

//------------------------------------------------------------------------------
inline byte
color::R() const
{
	return this->_R;
}

//------------------------------------------------------------------------------
inline byte
color::G() const
{
	return this->_G;
}

//------------------------------------------------------------------------------
inline byte
color::B() const
{
	return this->_B;
}

//------------------------------------------------------------------------------
inline byte&
color::A()
{
	return this->_A;
}

//------------------------------------------------------------------------------
inline byte&
color::R()
{
	return this->_R;
}

//------------------------------------------------------------------------------
inline byte&
color::G()
{
	return this->_G;
}

//------------------------------------------------------------------------------
inline byte&
color::B()
{
	return this->_B;
}

//------------------------------------------------------------------------------
inline color
color::inverse() const
{
	return color(this->_A, 255 - this->_R, 255 - this->_G, 255 - this->_B);
}

//------------------------------------------------------------------------------
inline color
color::to_luminance() const
{
	byte gray = byte(this->_R * 0.229f + this->_G * 0.587f + this->_B * 0.184f);
	return color(this->_A, gray, gray, gray);
}

//------------------------------------------------------------------------------
inline uint
color::to_rgba() const
{
	return (uint(this->_R) << 24) | (uint(this->_G) << 16) | (uint(this->_B) << 8) | (uint(this->_A));
}

//------------------------------------------------------------------------------
inline uint
color::to_abgr() const
{
	return (uint(this->_A) << 24) | (uint(this->_B) << 16) | (uint(this->_G) << 8) | (uint(this->_R));
}

//------------------------------------------------------------------------------
inline float4
color::to_float4() const
{
	return float4(this->r(), this->g(), this->b(), this->a());
}

//------------------------------------------------------------------------------
inline uint
color::to_uint() const
{
	return this->value;
}

//------------------------------------------------------------------------------
inline color
color::from_rgba(uint clr)
{
	union
	{
		struct
		{
			byte _a;
			byte _b;
			byte _g;
			byte _r;
		};
		uint rgba;
	} colorRGBA;
	colorRGBA.rgba = clr;
	return color(colorRGBA._a, colorRGBA._r, colorRGBA._g, colorRGBA._b);
}

} // namespace Math
//------------------------------------------------------------------------------
