﻿#ifndef ENUMS_H
#define ENUMS_H

enum ObjectID {

	// Model Canvas
	ID_TIMER = 2000,

	// Model Viewer Frame
	ID_MODELVIEWERFRAME, // = 2000,
	ID_FILE_NPC,
	ID_FILE_SCREENSHOT,
	ID_FILE_SCREENSHOTCONFIG,
	ID_FILE_EXPORTGIF,
	ID_FILE_EXPORTAVI,
	ID_FILE_TEXIMPORT,
	ID_FILE_TEXEXPORT,
	ID_FILE_MODELEXPORT,
	ID_FILE_RESETLAYOUT,
	ID_FILE_EXIT,
	ID_FILELIST,

	ID_SHOW_FILE_LIST,
	ID_SHOW_ANIM,
	ID_SHOW_CHAR,
	ID_SHOW_VIEW,
	ID_SHOW_LIGHT,
	ID_SHOW_MODEL,
	ID_SHOW_MODELBANK,

	ID_SHOW_MASK,

	//ID_SHOW_BONES,
	ID_SHOW_BOUNDS,
	//ID_SHOW_PARTICLES,
	//ID_SHOW_WIREFRAME,
	ID_USE_CAMERA,

	ID_CAMERA,
	ID_CAM_FRONT,
	ID_CAM_SIDE,
	ID_CAM_BACK,
	ID_CAM_ISO,

	ID_LT_AMBIENT,
	ID_LT_DIRECTIONAL,
	ID_LT_MODEL,
	ID_LT_COLOR,
	ID_LT_DIRECTION,
	ID_LT_TRUE,
	ID_LT_SAVE,
	ID_LT_LOAD,
	
	ID_BACKGROUND,
	ID_BG_COLOR,
	ID_SKYBOX,
	ID_SHOW_GRID,

	ID_CANVASSIZE,
	ID_CANVAS120,
	ID_CANVAS512,
	ID_CANVAS640,
	ID_CANVAS800,
	ID_CANVAS1024,
	ID_CANVAS1152,
	ID_CANVAS1280,
	ID_CANVAS1600,

	ID_ZOOM_IN,
	ID_ZOOM_OUT,

	ID_ENCHANTS,
	ID_SPELLS,
	ID_EQCREATURE_R,
	ID_EQCREATURE_L,
	ID_SHADER_DEATH,
	ID_TEST,

	ID_USE_NPCSKINS,
	ID_DEFAULT_DOODADS,
	ID_USE_ANTIALIAS,
	ID_USE_ENVMAP,
	ID_USE_HWACC,
	ID_SHOW_SETTINGS,
	//ID_RESET,
	
	ID_CHECKFORUPDATE,
	ID_HELP,
	ID_ABOUT,

	ID_MOUNT_CHARACTER,

	ID_SAVE_TEMP1,
	ID_SAVE_TEMP2,
	ID_SAVE_TEMP3,
	ID_SAVE_TEMP4,
	ID_LOAD_TEMP1,
	ID_LOAD_TEMP2,
	ID_LOAD_TEMP3,
	ID_LOAD_TEMP4,

	// ---------------------------------------
	// Settings
	ID_SETTINGS_FRAME,
	ID_SETTINGS_PAGE1,
	ID_SETTINGS_PAGE2,
	ID_SETTINGS_TABS,
	// Settings Page1
	ID_SETTINGS_LOCALFILES,
	ID_SETTINGS_UP,
	ID_SETTINGS_DOWN,
	ID_SETTINGS_ADD,
	ID_SETTINGS_REMOVE,
	ID_SETTINGS_CLEAR,
	ID_SETTINGS_RANDOMCHAR,
	ID_SETTINGS_RANDOMSKIN,
	ID_SETTINGS_INITSTARTUP,
	ID_SETTINGS_SAVELAYOUT,
	ID_SETTINGS_APPLY,

	// -------------------------------------
	// GIF Exporter frame
	ID_GIF_FRAME, // = 3500,
	ID_GIFSTART,
	ID_GIFEXIT,
	ID_GIFFILENAME,
	ID_GIFCURFRAME,
	ID_GIFTOTALFRAME,
	ID_GIFTRANSPARENT,
	ID_GIFDIFFUSE,
	ID_GIFSHRINK,
	ID_GIFGREYSCALE,

	// ------------------------------------
	// Anim Frame
	ID_ANIM_FRAME,// = 3000,
	ID_PAUSE,
	ID_PLAY,
	ID_STOP,
	ID_CLEARANIM,
	ID_LOADANIM,
	ID_ADDANIM,
	ID_PREVANIM,
	ID_NEXTANIM,
	ID_OLDSTYLE,
	ID_LOOPS,
	ID_ANIM,
	ID_ANIM_SECONDARY,
	ID_ANIM_LOCK,
	ID_SKIN,
	ID_ITEMSET,
	ID_SPEED,
	ID_FRAME,
	ID_PAUSE_MOUTH,
	ID_ANIM_MOUTH,
	ID_SPEED_MOUTH,

	// ---------------------------------------------
	// Light Frame
	ID_LIGHT_FRAME, // = 3950,
	ID_LIGHTSEL,
	ID_LIGHTENABLED,
	ID_LIGHTRELATIVE,
	ID_LIGHTCOLOUR,
	ID_LIGHTAMBIENCE,
	ID_LIGHTDIFFUSE,
	ID_LIGHTSPECULAR,
	ID_LIGHTPOSX,
	ID_LIGHTPOSY,
	ID_LIGHTPOSZ,
	ID_LIGHTTARX,
	ID_LIGHTTARY,
	ID_LIGHTTARZ,
	ID_LIGHTCINTENSITY,
	ID_LIGHTLINTENSITY,
	ID_LIGHTQINTENSITY,
	ID_LIGHTALPHA,
	ID_LIGHTRESET,
	ID_LIGHTPOSITIONAL,
	ID_LIGHTSPOT,
	ID_LIGHTDIRECTIONAL,

	// ----------------------------------------
	// Model Control Frame
	ID_MODEL_FRAME,
	ID_MODEL_NAME,
	ID_MODEL_LOD,
	ID_MODEL_ALPHA,
	ID_MODEL_SCALE,
	ID_MODEL_BONES,
	ID_MODEL_BOUNDS,
	ID_MODEL_RENDER,
	ID_MODEL_WIREFRAME,
	ID_MODEL_PARTICLES,
	ID_MODEL_TEXTURE,
	ID_MODEL_GEOSETS,
	ID_MODEL_X,
	ID_MODEL_Y,
	ID_MODEL_Z,

	// ----------------------------------------
	// Model Bank Control Frame
	ID_MODELBANK_FRAME,
	ID_MODELBANK_NAME,
	ID_MODELBANK_ADD,
	ID_MODELBANK_REMOVE,
	ID_MODELBANK_DISPLAY,

	// ----------------------------------------
	// Arrow Frame
	ID_ARROW_FRAME,
	ID_ARROW_ATTACH,
	ID_ARROW_JOINT,
	ID_ARROW_MODEL,
	ID_ARROW_TEXTURE,
	ID_ARROW_ROTATION,
	ID_ARROW_SCALE,
	ID_ARROW_POSITION,
	ID_ARROW_CLEAR,

	// -----------------------------------------
	// effects/enhants frame
	ID_ENCHANTSFRAME, // = 3600,
	ID_ENCHANTSOK,
	ID_ENCHANTSCANCEL,

	// -----------------------------------------
	// Image Control Frame
	ID_IMAGE_FRAME,
	ID_IMAGE_FILENAME,
	ID_IMAGE_CANVASWIDTH,
	ID_IMAGE_CANVASHEIGHT,
	ID_IMAGE_LOCKASPECT,
	ID_IMAGE_SAVE,
	ID_IMAGE_CANCEL,

	// Sound Control Frame
	//ID_SOUND_FRAME,
	//ID_SOUND_PLAY,
	//ID_SOUND_STOP,

	// --------------------------------------
	// Char control frame
	ID_CHAR_FRAME, // = 3400,
	ID_SKIN_COLOR,
	ID_FACE_TYPE,
	ID_HAIR_COLOR,
	ID_HAIR_STYLE,
	ID_FACIAL_HAIR,
	ID_FACIAL_COLOR,
	ID_CHAR_RANDOMISE,

	ID_TABARD_ICON,
	ID_TABARD_ICONCOLOR,
	ID_TABARD_BORDER,
	ID_TABARD_BORDERCOLOR,
	ID_TABARD_BACKGROUND,

	ID_SHOW_UNDERWEAR,
	ID_SHOW_EARS,
	ID_SHOW_HAIR,
	ID_SHOW_FACIALHAIR,
	ID_SHOW_FEET,
	ID_SHEATHE,

	ID_SAVE_EQUIPMENT,
	ID_LOAD_EQUIPMENT,
	ID_CLEAR_EQUIPMENT,

	ID_SAVE_CHAR,
	ID_LOAD_CHAR,
	ID_IMPORT_CHAR, // From Blizzards Armory website

	ID_LOAD_SET,
	ID_LOAD_START,

	ID_MOUNT,

	ID_EQUIPMENT = 5000
};

enum {
	LIGHT_AMBIENT,
	LIGHT_DYNAMIC,
	LIGHT_MODEL_ONLY
};

enum {
	LIGHT_POSITIONAL,
	LIGHT_SPOT,
	LIGHT_DIRECTIONAL
};

enum ModelType {
	MT_NORMAL,
	MT_CHAR,
	MT_WMO,
	MT_NPC
};

enum CharRegions {
	CR_BASE = 0,
	CR_ARM_UPPER,
	CR_ARM_LOWER,
	CR_HAND,
	CR_FACE_UPPER,
	CR_FACE_LOWER,
	CR_TORSO_UPPER,
	CR_TORSO_LOWER,
	CR_PELVIS_UPPER,
	CR_PELVIS_LOWER,
	CR_FOOT,
	NUM_REGIONS,

	CR_LEG_UPPER = CR_PELVIS_UPPER,
	CR_LEG_LOWER = CR_PELVIS_LOWER
};

enum {
	UPDATE_ITEM,
	UPDATE_SET,
	UPDATE_START,
	UPDATE_MOUNT,
	UPDATE_CREATURE_ITEM,
	UPDATE_NPC
};

enum CharSlots {
	CS_HEAD,
	CS_NECK,
	CS_SHOULDER,
	CS_BOOTS,
	CS_BELT,
	CS_SHIRT,
	CS_PANTS,
	CS_CHEST,
	CS_BRACERS,
	CS_GLOVES,
	CS_HAND_RIGHT,
	CS_HAND_LEFT,
	CS_CAPE,
	CS_TABARD,
	CS_QUIVER,
	
	NUM_CHAR_SLOTS
};

// Item type values as referred to by the items.csv list
enum ItemTypes {
	IT_ALL = 0,
	IT_HEAD,
	IT_NECK,
	IT_SHOULDER,
	IT_SHIRT,
	IT_CHEST,
	IT_BELT,
	IT_PANTS,
	IT_BOOTS,
	IT_BRACERS,
	IT_GLOVES,
	IT_RINGS,
	IT_ACCESSORY,
	IT_DAGGER,
	IT_SHIELD,
	IT_BOW,
	IT_CAPE,
	IT_2HANDED,
	IT_QUIVER,
	IT_TABARD,
	IT_ROBE,
	IT_LEFTHANDED, // IT_1HANDED
	IT_RIGHTHANDED, // IT_CLAW
	IT_OFFHAND,
	IT_UNUSED, // unused?
	IT_THROWN,
	IT_GUN,

	NUM_ITEM_TYPES
};

enum {
	CHECK_RANDOMCHAR,
	CHECK_RANDOMSKIN,
	CHECK_LOCALFILES,
	CHECK_INITSTARTUP,
	CHECK_SAVELAYOUT
};

enum {
	CHECK_COMPRESSEDTEX,
	CHECK_MULTITEX,
	CHECK_VBO,
	CHECK_FBO,
	CHECK_PBO,
	CHECK_DRAWRANGEELEMENTS,
	CHECK_ENVMAPPING,
	CHECK_NPOT,
	CHECK_PIXELSHADERS,
	CHECK_VERTEXSHADERS,
	CHECK_GLSLSHADERS
};

enum {
	SPIN_SKIN_COLOR,
	SPIN_FACE_TYPE,
	SPIN_HAIR_COLOR,
	SPIN_HAIR_STYLE,
	SPIN_FACIAL_HAIR,
	SPIN_FACIAL_COLOR,

	NUM_SPIN_BTNS
};

enum {
	SPIN_TABARD_ICON = 0,
	SPIN_TABARD_ICONCOLOR,
	SPIN_TABARD_BORDER,
	SPIN_TABARD_BORDERCOLOR,
	SPIN_TABARD_BACKGROUND,

	NUM_TABARD_BTNS
};

enum WOW_LOCALE {
	enUS = 0,
	koKR,
	frFR,
	deDE,
	zhCN,
	zhTW,
	esES,
	ruRU,

	NUM_LOCALES = 16
};

// copied from the .mdl docs? this might be completely wrong
enum BlendModes {
	BM_OPAQUE,
	BM_TRANSPARENT,
	BM_ALPHA_BLEND,
	BM_ADDITIVE,
	BM_ADDITIVE_ALPHA,
	BM_MODULATE,
	BM_MODULATEX2
};

enum BoneTable {
	//Block F - Bone lookup table.
	//---------------------------------
	BONE_LARM = 0,		// 0, Left upper arm
	BONE_RARM,			// 1, Right upper arm
	BONE_LSHOULDER,		// 2, Left Shoulder / deltoid area
	BONE_RSHOULDER,		// 3, Right Shoulder / deltoid area
	BONE_STOMACH,		// 4, (upper?) abdomen
	BONE_WAIST,			// 5, (lower abdomen?) waist
	BONE_HEAD,			// 6, head
	BONE_JAW,			// 7, jaw/mouth
	BONE_RFINGER1,		// 8, (Trolls have 3 "fingers", this points to the 2nd one.
	BONE_RFINGER2,		// 9, center finger - only used by dwarfs.. don't know why
	BONE_RFINGER3,		// 10, (Trolls have 3 "fingers", this points to the 3rd one.
	BONE_RFINGERS,		// 11, Right fingers -- this is -1 for trolls, they have no fingers, only the 3 thumb like thingys
	BONE_RTHUMB,		// 12, Right Thumb
	BONE_LFINGER1,		// 13, (Trolls have 3 "fingers", this points to the 2nd one.
	BONE_LFINGER2,		// 14, Center finger - only used by dwarfs.
	BONE_LFINGER3,		// 15, (Trolls have 3 "fingers", this points to the 3rd one.
	BONE_LFINGERS,		// 16, Left fingers
	BONE_LTHUMB,		// 17, Left Thumb
	UnK19,	// ?
	UnK20,	// ?
	UnK21,	// ?
	UnK22,	// ?
	UnK23,	// ?
	UnK24,	// ?
	UnK25,	// ?
	UnK26,	// ?
	BONE_ROOT			// 26, The "Root" bone,  this controls rotations, transformations, etc of the whole model and all subsequent bones.
};

enum GRAPHIC_EFFECTS {
	FRAGMENT_SHADER,
	VERTEX_SHADER,
	GLSL,
	COMPRESSED_TEXTURES,
	MULTI_TEXTURES,
	DRAW_RANGE_ELEMENTS,
	POINT_SPRITES,
	//bool supportShaders;//  = false;
	ANTI_ALIAS,
	VERTEX_BUFFER,
	PIXEL_BUFFER,
	FRAME_BUFFER,
	NON_POWEROFTWO,
	OPENGL_20,
	WGLPIXELFORMAT,

	NUM_EFFECTS
};

#endif

