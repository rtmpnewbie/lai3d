#ifndef VIDEO_H
#define VIDEO_H

#include "RenderTexture.h"

#include "util.h"
#include "vec3d.h"
#include "manager.h"
#include "mpq.h"
#include "ddslib.h"


#ifndef PI
	#define PI 3.14159265358f
#endif

typedef GLuint TextureID;


class Texture : public ManagedItem {
public:
	int w,h;
	GLuint id;
	bool compressed;

	Texture(std::string name):ManagedItem(name), w(0), h(0), id(0), compressed(false) {}
	void getPixels(unsigned char *buf);

};

class TextureManager : public Manager<GLuint> {
	
public:
	virtual GLuint add(std::string name);
	void doDelete(GLuint id);

	void LoadBLP(GLuint id, Texture *tex);
};

struct VideoCaps
{
	int	colour;
	int alpha;
	int zBuffer;
	int accum;
	int stencil;
	int aaSamples;	// how many AA samples can this mode do?
	int hwAcc;	// Hardware Acceleration mode?
	GLboolean sampleBuffer;	// do we have an AA sample buffer?
	GLboolean doubleBuffer;	// double buffered?
};

class VideoSettings {

public:
	VideoSettings();
	~VideoSettings();

	// Functions
	bool Init();
	void InitGL();
	void EnumDisplayModes();

	bool GetCompatibleWinMode(VideoCaps caps);
	bool GetAvailableMode();
	HWND GetHandle() {return hWnd;}

	void ResizeGLScene(int width, int height);
	void SetHandle(HWND hwnd, int bpp);
	void SetMode();
	void SetCurrent();
	void SwapBuffers();
	void Release();
	
	bool init; 
	bool render;
	bool refresh;

	// Resources
	HDC hDC;
	HWND hWnd;
	HGLRC hRC;

	// OpenGL Settings
	int xRes, yRes;
	int pixelFormat;
	float fov;
	int desktopBPP;

	// Card capabilities
	std::vector<VideoCaps> capsList;
	VideoCaps curCap;
	int capIndex;

	int AnisofilterLevel;
	int numTextureUnits;

	// Card Info
	char *vendor;
	char *version;
	char *renderer;
	
	// Is there hardware support?
	bool hasHardware;
	bool secondPass;

	// Video card support for OGL Extensions
	bool supportFragProg;
	bool supportVertexProg;
	bool supportGLSL;
	bool supportCompression;// = false;
	bool supportMultiTex;//  = false;
	bool supportDrawRangeElements;//  = false;
	bool supportPointSprites;//  = false;
	//bool supportShaders;//  = false;
	bool supportAntiAlias;//  = false;
	bool supportVBO;//  = false;
	bool supportPBO;//  = false;
	bool supportFBO;//  = false;
	bool supportNPOT;//  = false;
	bool supportOGL20;//  = false;
	bool supportWGLPixelFormat; 
	bool supportTexRects;

	bool disableHWAcc;//  = false;
	bool useMasking;

	bool useAntiAlias;//  = true;
	bool useEnvMapping;//  = true;
	bool useShaders;
	bool useCompression;// = false;
	bool useVBO;//  = false;
	bool usePBO;//  = false;
	bool useFBO;//  = false;
	
};

void getTextureData(GLuint tex, unsigned char *buf);

extern VideoSettings g_videoSetting;
extern TextureManager g_texturemanager;

void decompressDXTC(GLint format, int w, int h, size_t size, unsigned char *src, unsigned char *dest);


#endif

