#ifndef MODEL_H
#define MODEL_H

// C++ files
#include <vector>
//#include <stdlib.h>
//#include <crtdbg.h>

// Our files
#include "video.h"
#include "displayable.h"

#include "vec3d.h"

class Model;
class Bone;
Vec3D fixCoordSystem(Vec3D v);

#include "manager.h"
#include "mpq.h"

#include "modelheaders.h"
#include "quaternion.h"
#include "matrix.h"

#include "animated.h"
#include "particle.h"

#include "enums.h"

// This will be our animation manager
// instead of using a STL vector or list or table, etc.  
// Decided to just limit it upto 4 animations to loop through - for experimental testing.
// The second id and loop count will later be used for being able to have a primary and secondary animation.

// Currently, this is more of a "Wrapper" over the existing code
// but hopefully over time I can remove and re-write it so this is the core.
struct AnimInfo {
	short Loops;
	unsigned int AnimID;
};

class AnimManager {
	ModelAnimation *anims;
	
	bool Paused;
	bool AnimParticles;

	AnimInfo animList[4];

	unsigned int Frame;		// Frame number we're upto in the current animation
	unsigned int TotalFrames;

	int AnimIDSecondary;
	unsigned int FrameSecondary;

	int AnimIDMouth;
	unsigned int FrameMouth;
	
	short Count;			// Total index of animations
	short PlayIndex;		// Current animation index we're upto
	short CurLoop;			// Current loop that we're upto.

	int TimeDiff;			// Difference in time between each frame

	float Speed;			// The speed of which to multiply the time given for Tick();
	float mouthSpeed;

public:
	AnimManager(ModelAnimation *anim);
	~AnimManager();
	
	void AddAnim(unsigned int id, short loop); // Adds an animation to our array.
	void Set(short index, unsigned int id, short loop); // sets one of the 4 existing animations and changes it (not really used currently)
	
	void SetSecondary(int id) {
		AnimIDSecondary = id;
		FrameSecondary = anims[id].timeStart;
	}
	void ClearSecondary() { AnimIDSecondary = -1; }
	int GetSecondaryID() { return AnimIDSecondary; }
	unsigned int GetSecondaryFrame() { return FrameSecondary; }

	// For independent mouth movement.
	void SetMouth(int id) {
		AnimIDMouth = id;
		FrameMouth = anims[id].timeStart;
	}
	void ClearMouth() { AnimIDMouth = -1; }
	int GetMouthID() { return AnimIDMouth; }
	unsigned int GetMouthFrame() { return FrameMouth; }
	void SetMouthSpeed(float speed) {
		mouthSpeed = speed;
	}

	void Play(); // Players the animation, and reconfigures if nothing currently inputed
	void Stop(); // Stops and resets the animation
	void Pause(bool force = false); // Toggles 'Pause' of the animation, use force to pause the animation no matter what.
	
	void Next(); // Plays the 'next' animation or loop
	void Prev(); // Plays the 'previous' animation or loop

	int Tick(int time);

	unsigned int GetFrameCount();
	unsigned int GetFrame() {return Frame;}
	void SetFrame(unsigned int f);
	void SetSpeed(float speed) {Speed = speed;}
	float GetSpeed() {return Speed;}
	
	void PrevFrame();
	void NextFrame();

	void Clear();
	void Reset() { Count = 0; }

	bool IsPaused() { return Paused; }
	bool IsParticlePaused() { return !AnimParticles; }
	void AnimateParticles() { AnimParticles = true; }

	unsigned int GetAnim() { return animList[PlayIndex].AnimID; }

	int GetTimeDiff();
	void SetTimeDiff(int i);
};

class Bone {
	/*
	Animated<Vec3D> trans;
	Animated<Quaternion> rot;
	Animated<Vec3D> scale;
	*/
public:
	Animated<Vec3D> trans;
	//Animated<Quaternion> rot;
	Animated<Quaternion, PACK_QUATERNION, Quat16ToQuat32> rot;
	Animated<Vec3D> scale;

	Vec3D pivot, transPivot;
	int parent;

	bool billboard;
	Matrix mat;
	Matrix mrot;

	ModelBoneDef boneDef;

	bool calc;
	void calcMatrix(Bone* allbones, int anim, int time, bool rotate=true);
#ifdef WotLK
	void init(MPQFile &f, ModelBoneDef &b, int *global, MPQFile *animfiles);
#else
	void init(MPQFile &f, ModelBoneDef &b, int *global);
#endif
};

class TextureAnim {
	Animated<Vec3D> trans, rot, scale;

public:
	Vec3D tval, rval, sval;

	void calc(int anim, int time);
	void init(MPQFile &f, ModelTexAnimDef &mta, int *global);
	void setup();
};

struct ModelColor {
	Animated<Vec3D> color;
	AnimatedShort opacity;

	void init(MPQFile &f, ModelColorDef &mcd, int *global);
};

struct ModelTransparency {
	AnimatedShort trans;

	void init(MPQFile &f, ModelTransDef &mtd, int *global);
};

struct ModelRenderPass {
	uint32 indexStart, indexCount, vertexStart, vertexEnd;
	//TextureID texture, texture2;
	int tex;
	bool useTex2, useEnvMap, cull, trans, unlit, noZWrite, billboard;
	float p;
	
	int16 texanim, color, opacity, blendmode;
	uint16 order;

	// Geoset ID
	int geoset;

	// texture wrapping
	bool swrap, twrap;

	// colours
	Vec4D ocol, ecol;

	bool init(Model *m);
	void deinit();

	bool operator< (const ModelRenderPass &m) const
	{
		// This is the old sort order method which I'm pretty sure is wrong - need to try something else.
		//return !trans;
		if (order<m.order)
			return true;
		else if (order>m.order)
			return false;
		else
			return blendmode == m.blendmode ? (p<m.p) : (blendmode<m.blendmode);
	}
};

struct ModelCamera {
	bool ok;

	Vec3D pos, target;
	float nearclip, farclip, fov;
	Animated<Vec3D> tPos, tTarget;
	Animated<float> rot;

	void init(MPQFile &f, ModelCameraDef &mcd, int *global);
	void setup(int time=0);

	ModelCamera():ok(false) {}
};

struct ModelLight {
	int type, parent;
	Vec3D pos, tpos, dir, tdir;
	Animated<Vec3D> diffColor, ambColor;
	Animated<float> diffIntensity, ambIntensity;

	void init(MPQFile &f, ModelLightDef &mld, int *global);
	void setup(int time, GLuint l);
};


struct ModelAttachment {
	int id;
	Vec3D pos;
	int bone;
	Model *model;

	void init(MPQFile &f, ModelAttachmentDef &mad, int *global);
	void setup();
	void setupParticle();
};

class Model: public ManagedItem, public Displayable
{
	// VBO Data
	GLuint m_vbuf, m_nbuf, m_tbuf;
	size_t m_vbufsize;

	// Non VBO Data
	GLuint dlist;

	bool animGeometry,animTextures,animBones;
	bool forceAnim;

	void init(MPQFile &f);

	TextureAnim		*m_texAnims;
	int				*m_globalSequences;
	ModelColor		*m_colors;
	ModelTransparency *m_transparency;
	ModelLight		*m_lights;
	ParticleSystem	*m_particleSystems;
	RibbonEmitter	*m_ribbons;

	inline void drawModel();
	void initCommon(MPQFile &f);
	bool isAnimated(MPQFile &f);
	void initAnimated(MPQFile &f);
	void initStatic(MPQFile &f);

	void animate(int anim);
	void calcBones(int anim, int time);

	void lightsOn(GLuint lbase);
	void lightsOff(GLuint lbase);

	Vec3D *m_bounds;
	uint16 *m_boundTris;

public:
	// Raw Data
	ModelVertex *m_origVertices;

	Vec3D *vertices, *normals;
	Vec2D *texCoords;
	uint16 *indices;
	size_t nIndices;
	// --

public:
	Model(std::string name, bool forceAnim=false);
	~Model();

	ModelHeader m_header;
	ModelCamera m_cam;
#ifdef WotLK
	string fullname;
#endif
	
	std::vector<ModelRenderPass> m_passes;
	std::vector<ModelGeoset> m_geosets;

	// ===============================
	// Toggles
	bool *showGeosets;
	bool showBones;
	bool showBounds;
	bool showWireframe;
	bool showParticles;
	bool showModel;
	bool showTexture;
	float alpha;

	// Position and rotation vector
	Vec3D pos;
	Vec3D rot;

	//
	bool ok;
	bool ind;
	bool hasCamera;
	bool hasParticles;
	bool isWMO;
	bool isMount;
	bool animated;

	// Misc values
	float rad;
	float trans;
	// -------------------------------


	// ===============================
	// Bone & Animation data
	// ===============================
	ModelAnimation *anims;
	AnimManager *animManager;
	Bone *bones;
	MPQFile *animfiles;

	int currentAnim;
	bool animcalc;
	int anim, animtime;

	void reset() 
	{ 
		animcalc = false; 
	}

	
	void update(int dt) 
	{  
		// (float dt)
		if (animated)
			animManager->Tick(dt);

		updateEmitters((dt/1000.0f)); 
	};
	// -------------------------------

	// ===============================
	// Texture data
	// ===============================
	TextureID *textures;
	int specialTextures[32];
	GLuint replaceTextures[32];
	bool useReplaceTextures[32];
	// -------------------------------

	// ===============================
	// 

	// ===============================
	// Rendering Routines
	// ===============================
	void drawBones();
	void drawBoundingVolume();
	void drawParticles();
	inline void draw();
	// -------------------------------
	
	void updateEmitters(float dt);
	void setLOD(MPQFile &f, int index);

	void setupAtt(int id);
	void setupAtt2(int id);

	std::vector<ModelAttachment> atts;
	int attLookup[40];
	int boneLookup[27];

	ModelType modelType;
	CharModelDetails charModelDetails;

	friend struct ModelRenderPass;
};

class ModelManager: public SimpleManager {
public:
	int add(std::string name);

	ModelManager() : v(0) {}

	int v;

	void resetAnim();
	void updateEmitters(float dt);
	void clear();

};

#endif
