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
	/*********** ³£ÓÃµÄÑÕÉ«Öµ£¬¾ßÌåÉ«Ñù²Î¿¼ http://www.ttpw.com/rgb.htm ************/
	static const uint LightPink			= 0xFFFFB6C1; ///< Ç³·Ûºì
	static const uint Pink				= 0xFFFFC0CB; ///< ·Ûºì
	static const uint Crimson			= 0xFFDC143C; ///< Éîºì/ÐÉºì
	static const uint LavenderBlush		= 0xFFFFF0F5; ///< µ­×Ïºì
	static const uint PaleVioletRed		= 0xFFDB7093; ///< Èõ×ÏÂÞÀ¼ºì
	static const uint HotPink			= 0xFFFF69B4; ///< ÈÈÇéµÄ·Ûºì
	static const uint DeepPink			= 0xFFFF1493; ///< Éî·Ûºì
	static const uint MediumVioletRed	= 0xFFC71585; ///< ÖÐ×ÏÂÞÀ¼ºì
	static const uint Orchid			= 0xFFDA70D6; ///< °µ×ÏÉ«/À¼»¨×Ï
	static const uint Thistle			= 0xFFD8BFD8; ///< ¼»É«
	static const uint Plum				= 0xFFDDA0DD; ///< ÑóÀîÉ«/Àî×Ó×Ï
	static const uint Violet			= 0xFFEE82EE; ///< ×ÏÂÞÀ¼
	static const uint Magenta			= 0xFFFF00FF; ///< Ñóºì/Ãµ¹åºì
	static const uint Fuchsia			= 0xFFFF00FF; ///< ×Ïºì/µÆÁýº£ÌÄ
	static const uint DarkMagenta		= 0xFF8B008B; ///< ÉîÑóºì
	static const uint Purple			= 0xFF800080; ///< ×ÏÉ«
	static const uint MediumOrchid		= 0xFFBA55D3; ///< ÖÐÀ¼»¨×Ï
	static const uint DarkViolet		= 0xFF9400D3; ///< °µ×ÏÂÞÀ¼
	static const uint DarkOrchid		= 0xFF9932CC; ///< °µÀ¼»¨×Ï
	static const uint Indigo			= 0xFF4B0082; ///< µåÇà/×ÏÀ¼É«
	static const uint BlueViolet		= 0xFF8A2BE2; ///< À¶×ÏÂÞÀ¼
	static const uint MediumPurple		= 0xFF9370DB; ///< ÖÐ×ÏÉ«
	static const uint MediumSlateBlue = 0xFF7B68EE; ///< ÖÐ°µÀ¶É«/ÖÐ°åÑÒÀ¶
	static const uint SlateBlue			= 0xFF6A5ACD; ///< Ê¯À¶É«/°åÑÒÀ¶
	static const uint DarkSlateBlue		= 0xFF483D8B; ///< °µ»ÒÀ¶É«/°µ°åÑÒÀ¶
	static const uint Lavender			= 0xFFE6E6FA; ///< µ­×ÏÉ«/Ñ¬ÒÂ²Ýµ­×Ï
	static const uint GhostWhite		= 0xFFF8F8FF; ///< ÓÄÁé°×
	static const uint Blue				= 0xFF0000FF; ///< ´¿À¶
	static const uint MediumBlue		= 0xFF0000CD; ///< ÖÐÀ¶É«
	static const uint MidnightBlue		= 0xFF191970; ///< ÎçÒ¹À¶
	static const uint DarkBlue			= 0xFF00008B; ///< °µÀ¶É«
	static const uint Navy				= 0xFF000080; ///< º£¾üÀ¶
	static const uint RoyalBlue			= 0xFF4169E1; ///< »Ê¼ÒÀ¶/±¦À¶
	static const uint CornflowerBlue	= 0xFF6495ED; ///< Ê¸³µ¾ÕÀ¶
	static const uint LightSteelBlue	= 0xFFB0C4DE; ///< ÁÁ¸ÖÀ¶
	static const uint LightSlateGray	= 0xFF778899; ///< ÁÁÀ¶»Ò/ÁÁÊ¯°å»Ò
	static const uint SlateGray			= 0xFF708090; ///< »ÒÊ¯É«/Ê¯°å»Ò
	static const uint DodgerBlue		= 0xFF1E90FF; ///< ÉÁÀ¼É«/µÀÆæÀ¶
	static const uint AliceBlue			= 0xFFF0F8FF; ///< °®ÀöË¿À¶
	static const uint SteelBlue			= 0xFF4682B4; ///< ¸ÖÀ¶/ÌúÇà
	static const uint LightSkyBlue		= 0xFF87CEFA; ///< ÁÁÌìÀ¶É«
	static const uint SkyBlue			= 0xFF87CEEB; ///< ÌìÀ¶É«
	static const uint DeepSkyBlue		= 0xFF00BFFF; ///< ÉîÌìÀ¶
	static const uint LightBlue			= 0xFFADD8E6; ///< ÁÁÀ¶
	static const uint PowderBlue		= 0xFFB0E0E6; ///< ·ÛÀ¶É«/»ðÒ©Çà
	static const uint CadetBlue			= 0xFF5F9EA0; ///< ¾üÀ¼É«/¾ü·þÀ¶
	static const uint Azure				= 0xFFF0FFFF; ///< ÎµÀ¶É«
	static const uint LightCyan			= 0xFFE0FFFF; ///< µ­ÇàÉ«
	static const uint PaleTurquoise		= 0xFFAFEEEE; ///< ÈõÂÌ±¦Ê¯
	static const uint Cyan				= 0xFF00FFFF; ///< ÇàÉ«
	static const uint Aqua				= 0xFF00FFFF; ///< Ç³ÂÌÉ«/Ë®É«
	static const uint DarkTurquoise		= 0xFF00CED1; ///< °µÂÌ±¦Ê¯
	static const uint DarkSlateGray		= 0xFF2F4F4F; ///< °µÍß»ÒÉ«/°µÊ¯°å»Ò
	static const uint DarkCyan			= 0xFF008B8B; ///< °µÇàÉ«
	static const uint Teal				= 0xFF008080; ///< Ë®Ñ¼É«
	static const uint MediumTurquoise	= 0xFF48D1CC; ///< ÖÐÂÌ±¦Ê¯
	static const uint LightSeaGreen		= 0xFF20B2AA; ///< Ç³º£ÑóÂÌ
	static const uint Turquoise			= 0xFF40E0D0; ///< ÂÌ±¦Ê¯
	static const uint Aquamarine		= 0xFF7FFFD4; ///< ±¦Ê¯±ÌÂÌ
	static const uint MediumAquamarine	= 0xFF66CDAA; ///< ÖÐ±¦Ê¯±ÌÂÌ
	static const uint MediumSpringGreen = 0xFF00FA9A; ///< ÖÐ´ºÂÌÉ«
	static const uint MintCream			= 0xFFF5FFFA; ///< ±¡ºÉÄÌÓÍ
	static const uint SpringGreen		= 0xFF00FF7F; ///< ´ºÂÌÉ«
	static const uint MediumSeaGreen	= 0xFF3CB371; ///< ÖÐº£ÑóÂÌ
	static const uint SeaGreen			= 0xFF2E8B57; ///< º£ÑóÂÌ
	static const uint Honeydew			= 0xFFF0FFF0; ///< ÃÛÉ«/ÃÛ¹ÏÉ«
	static const uint LightGreen		= 0xFF90EE90; ///< µ­ÂÌÉ«
	static const uint PaleGreen			= 0xFF98FB98; ///< ÈõÂÌÉ«
	static const uint DarkSeaGreen		= 0xFF8FBC8F; ///< °µº£ÑóÂÌ
	static const uint LimeGreen			= 0xFF32CD32; ///< ÉÁ¹âÉîÂÌ
	static const uint Lime				= 0xFF00FF00; ///< ÉÁ¹âÂÌ
	static const uint ForestGreen		= 0xFF228B22; ///< É­ÁÖÂÌ
	static const uint Green				= 0xFF008000; ///< ´¿ÂÌ
	static const uint DarkGreen			= 0xFF006400; ///< °µÂÌÉ«
	static const uint Chartreuse		= 0xFF7FFF00; ///< »ÆÂÌÉ«/²éÌØ¾ÆÂÌ
	static const uint LawnGreen			= 0xFF7CFC00; ///< ²ÝÂÌÉ«/²ÝÆºÂÌ
	static const uint GreenYellow		= 0xFFADFF2F; ///< ÂÌ»ÆÉ«
	static const uint DarkOliveGreen	= 0xFF556B2F; ///< °µéÏé­ÂÌ
	static const uint YellowGreen		= 0xFF9ACD32; ///< »ÆÂÌÉ«
	static const uint OliveDrab			= 0xFF6B8E23; ///< éÏé­ºÖÉ«
	static const uint Beige				= 0xFFF5F5DC; ///< Ã×É«/»Ò×ØÉ«
	static const uint LightGoldenrodYellow = 0xFFFAFAD2; ///< ÁÁ¾Õ»Æ
	static const uint Ivory				= 0xFFFFFFF0; ///< ÏóÑÀÉ«
	static const uint LightYellow		= 0xFFFFFFE0; ///< Ç³»ÆÉ«
	static const uint Yellow			= 0xFFFFFF00; ///< ´¿»Æ
	static const uint Olive				= 0xFF808000; ///< éÏé­
	static const uint DarkKhaki			= 0xFFBDB76B; ///< °µ»ÆºÖÉ«/Éî¿¨ß´²¼
	static const uint LemonChiffon		= 0xFFFFFACD; ///< ÄûÃÊ³ñ
	static const uint PaleGoldenrod		= 0xFFEEE8AA; ///< »Ò¾Õ»Æ/²Ô÷è÷ëÉ«
	static const uint Khaki				= 0xFFF0E68C; ///< »ÆºÖÉ«/¿¨ß´²¼
	static const uint Gold				= 0xFFFFD700; ///< ½ðÉ«
	static const uint Cornsilk			= 0xFFFFF8DC; ///< ÓñÃ×Ë¿É«
	static const uint Goldenrod			= 0xFFDAA520; ///< ½ð¾Õ»Æ
	static const uint DarkGoldenrod		= 0xFFB8860B; ///< °µ½ð¾Õ»Æ
	static const uint FloralWhite		= 0xFFFFFAF0; ///< »¨µÄ°×É«
	static const uint OldLace			= 0xFFFDF5E6; ///< ÀÏ»¨É«/¾ÉÀÙË¿
	static const uint Wheat				= 0xFFF5DEB3; ///< Ç³»ÆÉ«/Ð¡ÂóÉ«
	static const uint Moccasin			= 0xFFFFE4B5; ///< Â¹Æ¤É«/Â¹Æ¤Ñ¥
	static const uint Orange			= 0xFFFFA500; ///< ³ÈÉ«
	static const uint PapayaWhip		= 0xFFFFEFD5; ///< ·¬Ä¾É«/·¬Ä¾¹Ï
	static const uint BlanchedAlmond	= 0xFFFFEBCD; ///< °×ÐÓÉ«
	static const uint NavajoWhite		= 0xFFFFDEAD; ///< ÄÉÍß°×/ÍÁÖø°×
	static const uint AntiqueWhite		= 0xFFFAEBD7; ///< ¹Å¶­°×
	static const uint Tan				= 0xFFD2B48C; ///< ²èÉ«
	static const uint BurlyWood			= 0xFFDEB887; ///< Ó²Ä¾É«
	static const uint Bisque			= 0xFFFFE4C4; ///< ÌÕÅ÷»Æ
	static const uint DarkOrange		= 0xFFFF8C00; ///< Éî³ÈÉ«
	static const uint Linen				= 0xFFFAF0E6; ///< ÑÇÂé²¼
	static const uint Peru				= 0xFFCD853F; ///< ÃØÂ³É«
	static const uint PeachPuff			= 0xFFFFDAB9; ///< ÌÒÈâÉ«
	static const uint SandyBrown		= 0xFFF4A460; ///< É³×ØÉ«
	static const uint Chocolate			= 0xFFD2691E; ///< ÇÉ¿ËÁ¦É«
	static const uint SaddleBrown		= 0xFF8B4513; ///< ÖØºÖÉ«/Âí°°×ØÉ«
	static const uint Seashell			= 0xFFFFF5EE; ///< º£±´¿Ç
	static const uint Sienna			= 0xFFA0522D; ///< »ÆÍÁô÷É«
	static const uint LightSalmon		= 0xFFFFA07A; ///< Ç³öÙÓãÈâÉ«
	static const uint Coral				= 0xFFFF7F50; ///< Éºº÷
	static const uint OrangeRed			= 0xFFFF4500; ///< ³ÈºìÉ«
	static const uint DarkSalmon		= 0xFFE9967A; ///< ÉîÏÊÈâ/öÙÓãÉ«
	static const uint Tomato			= 0xFFFF6347; ///< ·¬ÇÑºì
	static const uint MistyRose			= 0xFFFFE4E1; ///< Ç³Ãµ¹åÉ«/±¡ÎíÃµ¹å
	static const uint Salmon			= 0xFFFA8072; ///< ÏÊÈâ/öÙÓãÉ«
	static const uint Snow				= 0xFFFFFAFA; ///< Ñ©°×É«
	static const uint LightCoral		= 0xFFF08080; ///< µ­Éºº÷É«
	static const uint RosyBrown			= 0xFFBC8F8F; ///< Ãµ¹å×ØÉ«
	static const uint IndianRed			= 0xFFCD5C5C; ///< Ó¡¶Èºì
	static const uint Red				= 0xFFFF0000; ///< ´¿ºì
	static const uint Brown				= 0xFFA52A2A; ///< ×ØÉ«
	static const uint FireBrick			= 0xFFB22222; ///< »ð×©É«/ÄÍ»ð×©
	static const uint DarkRed			= 0xFF8B0000; ///< ÉîºìÉ«
	static const uint Maroon			= 0xFF800000; ///< ÀõÉ«
	static const uint White				= 0xFFFFFFFF; ///< ´¿°×
	static const uint WhiteSmoke		= 0xFFF5F5F5; ///< °×ÑÌ
	static const uint Gainsboro			= 0xFFDCDCDC; ///< µ­»ÒÉ«
	static const uint LightGrey			= 0xFFD3D3D3; ///< Ç³»ÒÉ«
	static const uint Silver			= 0xFFC0C0C0; ///< Òø»ÒÉ«
	static const uint DarkGray			= 0xFFA9A9A9; ///< Éî»ÒÉ«
	static const uint Gray				= 0xFF808080; ///< »ÒÉ«
	static const uint DimGray			= 0xFF696969; ///< °µµ­»Ò
	static const uint Black				= 0xFF000000; ///< ´¿ºÚ

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
