#include "model.h"

#include <cassert>
#include <algorithm>
#include "util.h"

int globalTime = 0;
extern Model *g_selModel;

AnimManager::AnimManager(ModelAnimation *anim)
{
    AnimIDSecondary =  - 1;
    AnimIDMouth =  - 1;
    anims = anim;
    AnimParticles = false;

    Count = 1;
    PlayIndex = 0;
    CurLoop = 0;
    animList[0].AnimID = 0;
    animList[0].Loops = 0;

    if(anims != NULL)
    {
        Frame = anims[0].timeStart;
        TotalFrames = anims[0].timeEnd - anims[0].timeStart;
    }
    else
    {
        Frame = 0;
        TotalFrames = 0;
    }

    Speed = 1.0f;
    mouthSpeed = 1.0f;

    Paused = false;
}

//-------------------------------------------------------------------------

AnimManager::~AnimManager()
{
    anims = NULL;
}

//-------------------------------------------------------------------------


void AnimManager::AddAnim(unsigned int id, short loops)
{
    if(Count > 3)
    {
        return ;
    }

    animList[Count].AnimID = id;
    animList[Count].Loops = loops;
    Count++;
}

//-------------------------------------------------------------------------

void AnimManager::Set(short index, unsigned int id, short loops)
{
    // error check, we currently only support 4 animations.
    if(index > 3)
    {
        return ;
    }

    animList[index].AnimID = id;
    animList[index].Loops = loops;

    // Just an error check for our "auto animate"
    if(index == 0)
    {
        Count = 1;
        PlayIndex = index;
        Frame = anims[id].timeStart;
        TotalFrames = anims[id].timeEnd - anims[id].timeStart;
    }

    if(index + 1 > Count)
    {
        Count = index + 1;
    }
}

//-------------------------------------------------------------------------

void AnimManager::Play()
{
    PlayIndex = 0;
    //if (Frame == 0 && PlayID == 0) {
    CurLoop = animList[PlayIndex].Loops;
    Frame = anims[animList[PlayIndex].AnimID].timeStart;
    TotalFrames = anims[animList[PlayIndex].AnimID].timeEnd -
        anims[animList[PlayIndex].AnimID].timeStart;
    //}

    Paused = false;
    AnimParticles = false;
}

//-------------------------------------------------------------------------

void AnimManager::Stop()
{
    Paused = true;
    PlayIndex = 0;
    Frame = anims[animList[0].AnimID].timeStart;
    CurLoop = animList[0].Loops;
}

//-------------------------------------------------------------------------

void AnimManager::Pause(bool force)
{
    if(Paused && force == false)
    {
        Paused = false;
        AnimParticles = !Paused;
    }
    else
    {
        Paused = true;
        AnimParticles = !Paused;
    }
}

//-------------------------------------------------------------------------

void AnimManager::Next()
{
    if(CurLoop == 1)
    {
        PlayIndex++;
        if(PlayIndex >= Count)
        {
            Stop();
            return ;
        }

        CurLoop = animList[PlayIndex].Loops;
    }
    else if(CurLoop > 1)
    {
        CurLoop--;
    }
    else if(CurLoop == 0)
    {
        PlayIndex++;
        if(PlayIndex >= Count)
        {
            PlayIndex = 0;
        }
    }

    Frame = anims[animList[PlayIndex].AnimID].timeStart;
#ifdef WotLK
    TotalFrames = anims[animList[PlayIndex].AnimID].timeEnd -
        anims[animList[PlayIndex].AnimID].timeStart;
    g_selModel->currentAnim = animList[PlayIndex].AnimID;
#endif 
}

//-------------------------------------------------------------------------

void AnimManager::Prev()
{
    if(CurLoop >= animList[PlayIndex].Loops)
    {
        PlayIndex--;

        if(PlayIndex < 0)
        {
            Stop();
            return ;
        }

        CurLoop = animList[PlayIndex].Loops;
    }
    else if(CurLoop < animList[PlayIndex].Loops)
    {
        CurLoop++;
    }

    Frame = anims[animList[PlayIndex].AnimID].timeEnd;
#ifdef WotLK
    TotalFrames = anims[animList[PlayIndex].AnimID].timeEnd -
        anims[animList[PlayIndex].AnimID].timeStart;
    g_selModel->currentAnim = animList[PlayIndex].AnimID;
#endif 
}

//-------------------------------------------------------------------------

int AnimManager::Tick(int time)
{
    if( (Count < PlayIndex) )
    {
        return  - 1;
    }

    Frame += int(time *Speed);

    // animate our mouth animation
    if(AnimIDMouth >  - 1)
    {
        FrameMouth += (time *mouthSpeed);

        if(FrameMouth >= anims[AnimIDMouth].timeEnd)
        {
            FrameMouth -= (anims[AnimIDMouth].timeEnd -
                anims[AnimIDMouth].timeStart);
        }
        else if(FrameMouth < anims[AnimIDMouth].timeStart)
        {
            FrameMouth += (anims[AnimIDMouth].timeEnd -
                anims[AnimIDMouth].timeStart);
        }
    }

    // animate our second (upper body) animation
    if(AnimIDSecondary >  - 1)
    {
        FrameSecondary += (time *Speed);

        if(FrameSecondary >= anims[AnimIDSecondary].timeEnd)
        {
            FrameSecondary -= (anims[AnimIDSecondary].timeEnd -
                anims[AnimIDSecondary].timeStart);
        }
        else if(FrameSecondary < anims[AnimIDSecondary].timeStart)
        {
            FrameSecondary += (anims[AnimIDSecondary].timeEnd -
                anims[AnimIDSecondary].timeStart);
        }
    }

    if(Frame >= anims[animList[PlayIndex].AnimID].timeEnd)
    {
        Next();
        return 1;
    }
    else if(Frame < anims[animList[PlayIndex].AnimID].timeStart)
    {
        Prev();
        return 1;
    }

    return 0;
}

//-------------------------------------------------------------------------

unsigned int AnimManager::GetFrameCount()
{
    return (anims[animList[PlayIndex].AnimID].timeEnd -
        anims[animList[PlayIndex].AnimID].timeStart);
}

//-------------------------------------------------------------------------


void AnimManager::NextFrame()
{
    //AnimateParticles();
    int id = animList[PlayIndex].AnimID;
    Frame += int( (anims[id].timeEnd - anims[id].timeStart) / 60);
    TimeDiff = int( (anims[id].timeEnd - anims[id].timeStart) / 60);
}

//-------------------------------------------------------------------------

void AnimManager::PrevFrame()
{
    //AnimateParticles();
    int id = animList[PlayIndex].AnimID;
    Frame -= int( (anims[id].timeEnd - anims[id].timeStart) / 60);
    TimeDiff = int( (anims[id].timeEnd - anims[id].timeStart) / 60) * - 1;
}

//-------------------------------------------------------------------------

void AnimManager::SetFrame(unsigned int f)
{
    //TimeDiff = f - Frame;
    Frame = f;
}

//-------------------------------------------------------------------------

int AnimManager::GetTimeDiff()
{
    int t = TimeDiff;
    TimeDiff = 0;
    return t;
}

//-------------------------------------------------------------------------

void AnimManager::SetTimeDiff(int i)
{
    TimeDiff = i;
}

//-------------------------------------------------------------------------

void AnimManager::Clear()
{
    Stop();
    Paused = true;
    PlayIndex = 0;
    Count = 0;
    CurLoop = 0;
    Frame = 0;
}

//-------------------------------------------------------------------------

Model::Model(std::string name, bool forceAnim): ManagedItem(name), forceAnim
    (forceAnim)
{
    if(name == "")
    {
        return ;
    }

    // replace .MDX with .M2
    char tempname[256];
    strncpy(tempname, name.c_str(), sizeof(tempname) );

    if(tempname[name.length() - 1] != '2')
    {
        tempname[name.length() - 2] = '2';
        tempname[name.length() - 1] = 0;
    }

    // Initiate our model variables.
    trans = 1.0f;
    rad = 1.0f;
    pos = Vec3D(0.0f, 0.0f, 0.0f);
    rot = Vec3D(0.0f, 0.0f, 0.0f);

    for(int i = 0; i < 32; i++)
    {
        specialTextures[i] =  - 1;
        replaceTextures[i] = 0;
        useReplaceTextures[i] = false;
    }

    for(int i = 0; i < 40; i++)
    {
        attLookup[i] =  - 1;
    }

    for(int i = 0; i < 27; i++)
    {
        boneLookup[i] =  - 1;
    }


    dlist = 0;
    m_bounds = 0;
    m_boundTris = 0;
    showGeosets = 0;

    hasCamera = false;
    hasParticles = false;
    isWMO = false;
    isMount = false;

    showModel = false;
    showBones = false;
    showBounds = false;
    showWireframe = false;
    showParticles = false;
    showTexture = true;

    charModelDetails.Reset();

    m_vbuf = m_nbuf = m_tbuf = 0;

    m_origVertices = 0;
    vertices = 0;
    normals = 0;
    texCoords = 0;
    indices = 0;

    animtime = 0;
    anim = 0;
    anims = 0;
    animManager = NULL;
    bones = 0;
    m_bounds = 0;
    m_boundTris = 0;
    currentAnim = 0;
    m_colors = 0;
    m_globalSequences = 0;
    m_lights = 0;
    m_particleSystems = 0;
    m_ribbons = 0;
    m_texAnims = 0;
    textures = 0;
    m_transparency = 0;
    // --

    MPQFile f(tempname);
    ok = false;
    if(f.isEof() || (f.getSize() < sizeof(ModelHeader) ) )
    {
        wxLogMessage("Error: Unable to load model: [%s]", tempname);
        // delete this; //?
        f.close();
        return ;
    }
    ok = true;

    memcpy(&m_header, f.getBuffer(), sizeof(ModelHeader) );
    animated = isAnimated(f) || forceAnim;
    // isAnimated will set animGeometry and animTextures

    wxLogMessage(_T("Loading model: %s\n"), tempname);

    // Error check
    if(m_header.id[0] != 'M' && m_header.id[1] != 'D' && m_header.id[2] != '2' &&
        m_header.id[3] != '0')
    {
        wxLogMessage(_T("Error:\t\tInvalid model!  May be corrupted.") );
        ok = false;
        f.close();
        return ;
    }

#if 0
    if(header.nameOfs == 304 || header.nameOfs == 320)
    {
        fullname = name;
        ModelHeader2 header2;
        //memset(&header, 0, sizeof(ModelHeader));
        memcpy(&header2, f.getBuffer(), sizeof(ModelHeader2) );

        header.nBones = header2.nBones;
        header.ofsBones = header2.ofsBones;
        header.nBoneLookup = header2.nBoneLookup;
        header.ofsBoneLookup = header2.ofsBoneLookup;
        header.nVertices = header2.nVertices;
        header.ofsVertices = header2.ofsVertices;
        header.nViews = header2.nViews;
        header.nColors = header2.nColors;
        header.ofsColors = header2.ofsColors;
        header.nTextures = header2.nTextures;
        header.ofsTextures = header2.ofsTextures;
        header.nTransparency = header2.nTransparency;
        header.ofsTransparency = header2.ofsTransparency;
        header.nTexAnims = header2.nTexAnims;
        header.ofsTexAnims = header2.ofsTexAnims;
        header.nTexReplace = header2.nTexReplace;
        header.ofsTexReplace = header2.ofsTexReplace;
        header.nTexFlags = header2.nTexFlags;
        header.ofsTexFlags = header2.ofsTexFlags;
        header.nY = header2.nY;
        header.ofsY = header2.ofsY;
        header.nTexLookup = header2.nTexLookup;
        header.ofsTexLookup = header2.ofsTexLookup;
        header.nTexUnitLookup = header2.nTexUnitLookup;
        header.ofsTexUnitLookup = header2.ofsTexUnitLookup;
        header.nTransparencyLookup = header2.nTransparencyLookup;
        header.ofsTransparencyLookup = header2.ofsTransparencyLookup;
        header.nTexAnimLookup = header2.nTexAnimLookup;
        header.ofsTexAnimLookup = header2.ofsTexAnimLookup;
        header.floats[0] = header2.floats[0];
        header.floats[1] = header2.floats[1];
        header.floats[2] = header2.floats[2];
        header.floats[3] = header2.floats[3];
        header.floats[4] = header2.floats[4];
        header.floats[5] = header2.floats[5];
        header.floats[6] = header2.floats[6];
        header.floats[7] = header2.floats[7];
        header.floats[8] = header2.floats[8];
        header.floats[9] = header2.floats[9];
        header.floats[10] = header2.floats[10];
        header.floats[11] = header2.floats[11];
        header.floats[12] = header2.floats[12];
        header.floats[13] = header2.floats[13];
        header.nBoundingTriangles = header2.nBoundingTriangles;
        header.ofsBoundingTriangles = header2.ofsBoundingTriangles;
        header.nBoundingVertices = header2.nBoundingVertices;
        header.ofsBoundingVertices = header2.ofsBoundingVertices;
        header.nBoundingNormals = header2.nBoundingNormals;
        header.ofsBoundingNormals = header2.ofsBoundingNormals;
        header.nAttachments = header2.nAttachments;
        header.ofsAttachments = header2.ofsAttachments;
        header.nAttachLookup = header2.nAttachLookup;
        header.ofsAttachLookup = header2.ofsAttachLookup;
        header.nAttachments_2 = header2.nAttachments_2;
        header.ofsAttachments_2 = header2.ofsAttachments_2;
        header.nLights = header2.nLights;
        header.ofsLights = header2.ofsLights;
        header.nCameras = header2.nCameras;
        header.ofsCameras = header2.ofsCameras;
        header.nCameraLookup = header2.nCameraLookup;
        header.ofsCameraLookup = header2.ofsCameraLookup;
        header.nRibbonEmitters = header2.nRibbonEmitters;
        header.ofsRibbonEmitters = header2.ofsRibbonEmitters;
        header.nParticleEmitters = header2.nParticleEmitters;
        header.ofsParticleEmitters = header2.ofsParticleEmitters;
    }
#endif 

#ifdef WotLK
    fullname = name;

    if(m_header.nameOfs != 304 && m_header.nameOfs != 320)
    {
        wxLogMessage(_T(
            "Error:\t\tInvalid model nameOfs=%d/%d!  May be corrupted."),
            m_header.nameOfs, sizeof(ModelHeader) );
        ok = false;
        f.close();
        return ;
    }
#else 
    if(m_header.nameOfs != 336)
    {
        wxLogMessage(_T(
            "Error:\t\tInvalid model nameOfs=%d/%d!  May be corrupted."),
            m_header.nameOfs, sizeof(ModelHeader) );
        ok = false;
        f.close();
        return ;
    }
#endif 

    // Error check
    // 4 1 0 0 = WoW 2.0 models
    // 0 1 0 0 = WoW 1.0 models
    if(m_header.version[0] != 4 && m_header.version[1] != 1 && m_header.version[2] !=
        0 && m_header.version[3] != 0)
    {
        wxLogMessage(_T(
            "Error:\t\tModel version is incorrect!\n\t\tMake sure you are loading models from World of Warcraft 2.0.1 or newer client.") );
        ok = false;
        f.close();

        if(m_header.version[0] == 0)
        {
            wxMessageBox(wxString::Format(_T(
                "An error occured while trying to load the model %s.\nWoW Model Viewer 0.5.x only supports loading WoW 2.0 models\nModels from WoW 1.12 or earlier are not supported"), tempname), _T("Error: Unable to load model"), wxICON_ERROR);
        }

        return ;
    }

    if(f.getSize() < m_header.ofsParticleEmitters)
    {
        wxLogMessage(_T(
            "Error: Unable to load the Model \"%s\", appears to be corrupted."),
            tempname);
    }

    if(m_header.nGlobalSequences)
    {
        m_globalSequences = new int[m_header.nGlobalSequences];
        memcpy(m_globalSequences, (f.getBuffer() + m_header.ofsGlobalSequences),
            m_header.nGlobalSequences *4);
    }

    if(forceAnim)
    {
        animBones = true;
    }

    if(animated)
    {
        initAnimated(f);
    }
    else
    {
        initStatic(f);
    }

    f.close();

    // Ready to render.
    showModel = true;
    if(hasParticles)
    {
        showParticles = true;
    }
    alpha = 1.0f;
}

//-------------------------------------------------------------------------

Model::~Model()
{
    if(ok)
    {
#ifdef _DEBUG
        wxLogMessage(_T("Unloading model: %s\n"), name.c_str() );
#endif 

        // There is a small memory leak somewhere with the textures.
        // Especially if the texture was built into the model.
        // No matter what I try though I can't find the memory to unload.
        if(m_header.nTextures)
        {

            // For character models, the texture isn't loaded into the texture manager, manually remove it
            glDeleteTextures(1, &replaceTextures[1]);

            // Clears textures that were loaded from Model::InitCommon()
            for(size_t i = 0; i < m_header.nTextures; i++)
            {
                if(textures[i] > 0)
                {
                    g_texturemanager.del(textures[i]);
                }
            }

            for(size_t i = 0; i < 32; i++)
            {
                if(replaceTextures[i] > 0)
                {
                    g_texturemanager.del(replaceTextures[i]);
                }
            }



            wxDELETEA(textures);
        }

        wxDELETEA(m_globalSequences);

        wxDELETEA(m_bounds);
        wxDELETEA(m_boundTris);
        wxDELETEA(showGeosets);

        wxDELETE(animManager);

        if(animated)
        {
            // unload all sorts of crap
            // Need this if statement because VBO supported
            // cards have already deleted it.
            if(g_videoSetting.supportVBO)
            {
                glDeleteBuffersARB(1, &m_nbuf);
                glDeleteBuffersARB(1, &m_vbuf);
                glDeleteBuffersARB(1, &m_tbuf);

                vertices = NULL;
            }

            wxDELETEA(normals);
            wxDELETEA(vertices);
            wxDELETEA(texCoords);

            wxDELETEA(indices);
            wxDELETEA(anims);
            wxDELETEA(m_origVertices);

            wxDELETEA(bones);
            wxDELETEA(m_texAnims);
            wxDELETEA(m_colors);
            wxDELETEA(m_transparency);
            wxDELETEA(m_lights);
            wxDELETEA(m_particleSystems);
            wxDELETEA(m_ribbons);

        }
        else
        {
            glDeleteLists(dlist, 1);
        }
    }
}

//-------------------------------------------------------------------------


bool Model::isAnimated(MPQFile &f)
{
    // see if we have any animated bones
    ModelBoneDef *bo = (ModelBoneDef*) (f.getBuffer() + m_header.ofsBones);

    animGeometry = false;
    animBones = false;
    ind = false;

    ModelVertex *verts = (ModelVertex*) (f.getBuffer() + m_header.ofsVertices);
    for(size_t i = 0; i < m_header.nVertices && !animGeometry; i++)
    {
        for(size_t b = 0; b < 4; b++)
        {
            if(verts[i].weights[b] > 0)
            {
                ModelBoneDef &bb = bo[verts[i].bones[b]];
                if(bb.translation.type || bb.rotation.type || bb.scaling.type 
                    || (bb.flags &8) )
                {
                    if(bb.flags &8)
                    {
                        // if we have billboarding, the model will need per-instance animation
                        ind = true;
                    }
                    animGeometry = true;
                    break;
                }
            }
        }
    }

    if(animGeometry)
    {
        animBones = true;
    }
    else
    {
        for(size_t i = 0; i < m_header.nBones; i++)
        {
            ModelBoneDef &bb = bo[i];
            if(bb.translation.type || bb.rotation.type || bb.scaling.type)
            {
                animBones = true;
                break;
            }
        }
    }

    animTextures = m_header.nTexAnims > 0;

    bool animMisc = m_header.nCameras > 0 || 
    // why waste time, pretty much all models with cameras need animation
    m_header.nLights > 0 ||  // same here
    m_header.nParticleEmitters > 0 || m_header.nRibbonEmitters > 0;

    if(animMisc)
    {
        animBones = true;
    }

    // animated colors
    if(m_header.nColors)
    {
        ModelColorDef *cols = (ModelColorDef*) (f.getBuffer() +
            m_header.ofsColors);
        for(size_t i = 0; i < m_header.nColors; i++)
        {
            if(cols[i].color.type != 0 || cols[i].opacity.type != 0)
            {
                animMisc = true;
                break;
            }
        }
    }

    // animated opacity
    if(m_header.nTransparency && !animMisc)
    {
        ModelTransDef *trs = (ModelTransDef*) (f.getBuffer() +
            m_header.ofsTransparency);
        for(size_t i = 0; i < m_header.nTransparency; i++)
        {
            if(trs[i].trans.type != 0)
            {
                animMisc = true;
                break;
            }
        }
    }

    // guess not...
    return animGeometry || animTextures || animMisc;
}

//-------------------------------------------------------------------------


Vec3D fixCoordSystem(Vec3D v)
{
    return Vec3D(v.x, v.z,  - v.y);
}

//-------------------------------------------------------------------------

Vec3D fixCoordSystem2(Vec3D v)
{
    return Vec3D(v.x, v.z, v.y);
}

//-------------------------------------------------------------------------

Quaternion fixCoordSystemQuat(Quaternion v)
{
    return Quaternion( - v.x,  - v.z, v.y, v.w);
}

//-------------------------------------------------------------------------


void Model::initCommon(MPQFile &f)
{
    // assume: origVertices already set

    // This data is needed for both VBO and non-VBO cards.
    vertices = new Vec3D[m_header.nVertices];
    normals = new Vec3D[m_header.nVertices];

    // Correct the data from the model, so that its using the Y-Up axis mode.
    for(size_t i = 0; i < m_header.nVertices; i++)
    {
        m_origVertices[i].pos = fixCoordSystem(m_origVertices[i].pos);
        m_origVertices[i].normal = fixCoordSystem(m_origVertices[i].normal);

        // Set the data for our vertices, normals from the model data
        //if (!animGeometry || !supportVBO) {
        vertices[i] = m_origVertices[i].pos;
        normals[i] = m_origVertices[i].normal.normalize();
        //}

        float len = m_origVertices[i].pos.lengthSquared();
        if(len > rad)
        {
            rad = len;
        }
    }

    // model vertex radius
    rad = sqrtf(rad);

    // bounds
    if(m_header.nBoundingVertices > 0)
    {
        m_bounds = new Vec3D[m_header.nBoundingVertices];
        Vec3D *b = (Vec3D*) (f.getBuffer() + m_header.ofsBoundingVertices);
        for(size_t i = 0; i < m_header.nBoundingVertices; i++)
        {
            m_bounds[i] = fixCoordSystem(b[i]);
        }
    }
    if(m_header.nBoundingTriangles > 0)
    {
        m_boundTris = new uint16[m_header.nBoundingTriangles];
        memcpy(m_boundTris, f.getBuffer() + m_header.ofsBoundingTriangles,
            m_header.nBoundingTriangles *sizeof(uint16) );
    }

    // textures
    ModelTextureDef *texdef = (ModelTextureDef*) (f.getBuffer() +
        m_header.ofsTextures);
    if(m_header.nTextures)
    {

        // Error check
        if(m_header.nTextures <= 32)
        {
            textures = new TextureID[m_header.nTextures];
            for(size_t i = 0; i < m_header.nTextures; i++)
            {
                char texname[256];
                if(texdef[i].type == 0)
                {
                    strncpy(texname, (const char*)f.getBuffer() +
                        texdef[i].nameOfs, texdef[i].nameLen);
                    texname[texdef[i].nameLen] = 0;
                    std::string path(texname);
                    //fixname(path);
                    textures[i] = g_texturemanager.add(texname);
                }
                else
                {
                    // special texture - only on characters and such...
                    textures[i] = 0;
                    //while (texdef[i].type < 16 && specialTextures[texdef[i].type]!=-1) texdef[i].type++;
                    //if (texdef[i].type < 16)specialTextures[texdef[i].type] = (int)i;
                    specialTextures[i] = texdef[i].type;

                    if(texdef[i].type <= 32)
                    {
                        useReplaceTextures[texdef[i].type] = true;
                    }

                    if(texdef[i].type == 3)
                    {
                        // a fix for weapons with type-3 textures.
                        replaceTextures[3] = g_texturemanager.add(_T(
                            "Item\\ObjectComponents\\Weapon\\ArmorReflect4.BLP")
                            );
                    }
                }
            }
        }
    }
    /*
    // replacable textures - it seems to be better to get this info from the texture types
    if (header.nTexReplace) {
    size_t m = header.nTexReplace;
    if (m>16) m = 16;
    int16 *texrep = (int16*)(f.getBuffer() + header.ofsTexReplace);
    for (size_t i=0; i<m; i++) specialTextures[i] = texrep[i];
    }
     */

    // attachments
    // debug code here
    if(m_header.nAttachments)
    {
        ModelAttachmentDef *attachments = (ModelAttachmentDef*) (f.getBuffer() 
            + m_header.ofsAttachments);
        for(size_t i = 0; i < m_header.nAttachments; i++)
        {
            ModelAttachment att;
            att.model = this;
            att.init(f, attachments[i], m_globalSequences);
            atts.push_back(att);
        }
    }

    if(m_header.nAttachLookup)
    {
        int16 *p = (int16*) (f.getBuffer() + m_header.ofsAttachLookup);
        for(size_t i = 0; i < m_header.nAttachLookup; i++)
        {
            attLookup[i] = p[i];
        }
    }


    // init colors
    if(m_header.nColors)
    {
        m_colors = new ModelColor[m_header.nColors];
        ModelColorDef *colorDefs = (ModelColorDef*) (f.getBuffer() +
            m_header.ofsColors);
        for(size_t i = 0; i < m_header.nColors; i++)
        {
            m_colors[i].init(f, colorDefs[i], m_globalSequences);
        }
    }

    // init transparency
    int16 *transLookup = (int16*) (f.getBuffer() + m_header.ofsTransparencyLookup)
        ;
    if(m_header.nTransparency)
    {
        m_transparency = new ModelTransparency[m_header.nTransparency];
        ModelTransDef *trDefs = (ModelTransDef*) (f.getBuffer() +
            m_header.ofsTransparency);
        for(size_t i = 0; i < m_header.nTransparency; i++)
        {
            m_transparency[i].init(f, trDefs[i], m_globalSequences);
        }
    }

    // just use the first LOD/view
    // First LOD/View being the worst?
    // TODO: Add support for selecting the LOD.
    // indices - allocate space, too
    // header.nViews;
    // int viewLOD = 0; // sets LOD to worst
    // int viewLOD = header.nViews - 1; // sets LOD to best
    //setLOD(f, header.nViews - 1); // Set the default Level of Detail to the best possible. 

    // Old method - use this to try to determine a bug.
    // just use the first LOD/view

    // indices - allocate space, too
#ifdef WotLK
    // replace .MDX with .M2
    char tempname[256];
    strncpy(tempname, fullname.c_str(), sizeof(tempname) );

    if(tempname[fullname.length() - 1] != '2')
    {
        tempname[fullname.length() - 2] = '2';
        tempname[fullname.length() - 1] = 0;
    }
    int length = strlen(tempname);
    tempname[length - 3] = 0;
    fullname = tempname;
    strcat(tempname, "00.skin");
    MPQFile mpqFile(tempname);
    if(mpqFile.isEof() )
    {
        wxLogMessage("Error: Unable to load model: [%s]", tempname);
        mpqFile.close();
        return ;
    }

    ModelView *view = (ModelView*) (mpqFile.getBuffer() );

    // Indices,  Triangles
    uint16 *indexLookup = (uint16*) (mpqFile.getBuffer() + view->ofsIndex);
    uint16 *triangles = (uint16*) (mpqFile.getBuffer() + view->ofsTris);
    nIndices = view->nTris;
    indices = new uint16[nIndices];
    for(size_t i = 0; i < nIndices; i++)
    {
        indices[i] = indexLookup[triangles[i]];
    }

    // render ops
    ModelGeoset *ops = (ModelGeoset*) (mpqFile.getBuffer() + view->ofsSub);
    ModelTexUnit *tex = (ModelTexUnit*) (mpqFile.getBuffer() + view->ofsTex);
#else // not WotLK
    ModelView *view = (ModelView*) (f.getBuffer() + m_header.ofsViews);

    // Indices,  Triangles
    uint16 *indexLookup = (uint16*) (f.getBuffer() + view->ofsIndex);
    uint16 *triangles = (uint16*) (f.getBuffer() + view->ofsTris);
    nIndices = view->nTris;
    indices = new uint16[nIndices];
    for(size_t i = 0; i < nIndices; i++)
    {
        indices[i] = indexLookup[triangles[i]];
    }

    // render ops
    ModelGeoset *ops = (ModelGeoset*) (f.getBuffer() + view->ofsSub);
    ModelTexUnit *tex = (ModelTexUnit*) (f.getBuffer() + view->ofsTex);
#endif // WotLK

    ModelRenderFlags *renderFlags = (ModelRenderFlags*) (f.getBuffer() +
        m_header.ofsTexFlags);
    uint16 *texlookup = (uint16*) (f.getBuffer() + m_header.ofsTexLookup);
    uint16 *texanimlookup = (uint16*) (f.getBuffer() + m_header.ofsTexAnimLookup);
    int16 *texunitlookup = (int16*) (f.getBuffer() + m_header.ofsTexUnitLookup);


    showGeosets = new bool[view->nSub];
    for(size_t i = 0; i < view->nSub; i++)
    {
        m_geosets.push_back(ops[i]);
        showGeosets[i] = true;
    }

    for(size_t j = 0; j < view->nTex; j++)
    {
        ModelRenderPass pass;

        pass.useTex2 = false;
        pass.useEnvMap = false;
        pass.cull = false;
        pass.trans = false;
        pass.unlit = false;
        pass.noZWrite = false;
        pass.billboard = false;

        //pass.texture2 = 0;
        size_t geoset = tex[j].op;

        pass.geoset = (int)geoset;

        pass.indexStart = ops[geoset].istart;
        pass.indexCount = ops[geoset].icount;
        pass.vertexStart = ops[geoset].vstart;
        pass.vertexEnd = pass.vertexStart + ops[geoset].vcount;

        pass.order = tex[j].order; //pass.order = 0;

        //TextureID texid = textures[texlookup[tex[j].textureid]];
        //pass.texture = texid;
        pass.tex = texlookup[tex[j].textureid];

        /*
        // Render Flags
        0x01 = Unlit
        0x02 = ? glow effects ? no zwrite?
        0x04 = ?
        0x08 = ?
        0x10 = Billboard
        0x20 = ?
         */
        // TODO: figure out these flags properly -_-
        ModelRenderFlags &rf = renderFlags[tex[j].flagsIndex];

        pass.blendmode = rf.blend;
        //if (rf.blend == 0) // Test to disable/hide different blend types
        //	continue;

        pass.color = tex[j].colorIndex;
        pass.opacity = transLookup[tex[j].transid];

        pass.useEnvMap = (texunitlookup[tex[j].texunit] ==  - 1) && ( (rf.flags
            &0x10) != 0) && rf.blend > 2;
        //&& rf.blend<5; // Use environmental reflection effects?

        // Disable environmental mapping if its been unchecked.
        //if (pass.useEnvMap && !video.useEnvMapping)
        //	pass.useEnvMap = false;

        // This is wrong but meh.. best I could get it so far.
        //pass.cull = (rf.flags & 0x04)==0 && pass.blendmode!=1 && pass.blendmode!=4 && (rf.flags & 17)!=17;
        //pass.cull = false; // quick test
        pass.cull = (rf.flags &0x04) == 0 && pass.blendmode == 0;

        pass.unlit = (rf.flags &0x01) != 0;

        pass.billboard = (rf.flags &0x10) != 0;

        //pass.noZWrite = (texdef[pass.tex].flags & 3)!=0;
        if(name == "Creature\\Turkey\\turkey.m2")
        // manual fix as I just bloody give up.
        {
            pass.noZWrite = false;
        }
        else
        {
            pass.noZWrite = (pass.blendmode > 1);
        }
        //pass.noZWrite = (pass.blendmode>1) && !(rf.blend==4 && rf.flags==17);

        // ToDo: Work out the correct way to get the true/false of transparency
        pass.trans = (pass.blendmode > 0) && (pass.opacity > 0);
        // Transparency - not the correct way to get transparency

        pass.p = ops[geoset].v.z;

        // Texture flags
        pass.swrap = (texdef[pass.tex].flags &1) != 0;
        pass.twrap = (texdef[pass.tex].flags &2) != 0;

        if(animTextures)
        {
            //if (tex[j].flags & 16) {
            if(tex[j].flags &15)
            {
                pass.texanim =  - 1; // no texture animation
            }
            else
            {
                pass.texanim = texanimlookup[tex[j].texanimid];
            }
        }
        else
        {
            pass.texanim =  - 1; // no texture animation
        }

        m_passes.push_back(pass);
    }

    // transparent parts come later
    std::sort(m_passes.begin(), m_passes.end() );

    // zomg done
}

//-------------------------------------------------------------------------

void Model::initStatic(MPQFile &f)
{
    m_origVertices = (ModelVertex*) (f.getBuffer() + m_header.ofsVertices);

    initCommon(f);

    dlist = glGenLists(1);
    glNewList(dlist, GL_COMPILE);

    drawModel();

    glEndList();

    // clean up vertices, indices etc
    wxDELETEA(vertices);
    wxDELETEA(normals);
    wxDELETEA(indices);

    wxDELETEA(m_colors);
    wxDELETEA(m_transparency);
}

//-------------------------------------------------------------------------

void Model::initAnimated(MPQFile &f)
{
    if(m_origVertices)
    {
        delete [] m_origVertices;
        m_origVertices = NULL;
    }

    m_origVertices = new ModelVertex[m_header.nVertices];
    memcpy(m_origVertices, f.getBuffer() + m_header.ofsVertices,
        m_header.nVertices*sizeof(ModelVertex) );

    initCommon(f);

    if(m_header.nAnimations > 0)
    {
        anims = new ModelAnimation[m_header.nAnimations];

#ifndef WotLK
        memcpy(anims, f.getBuffer() + m_header.ofsAnimations,
            m_header.nAnimations*sizeof(ModelAnimation) );
#else 
        ModelAnimationWotLK animsWotLK;
        char tempname[256];
        animfiles = new MPQFile[m_header.nAnimations];
        for(size_t i = 0; i < m_header.nAnimations; i++)
        {
            memcpy(&animsWotLK, f.getBuffer() + m_header.ofsAnimations + i *
                sizeof(ModelAnimationWotLK), sizeof(ModelAnimationWotLK) );
            anims[i].animID = animsWotLK.animID;
            anims[i].timeStart = 0;
            anims[i].timeEnd = animsWotLK.length;
            anims[i].moveSpeed = animsWotLK.moveSpeed;
            anims[i].loopType = animsWotLK.loopType;
            anims[i].flags = animsWotLK.flags;
            anims[i].d1 = animsWotLK.d1;
            anims[i].d2 = animsWotLK.d2;
            anims[i].playSpeed = animsWotLK.playSpeed;
            anims[i].rad = animsWotLK.rad;
            anims[i].NextAnimation = animsWotLK.NextAnimation;
            anims[i].Index = animsWotLK.Index;

            sprintf(tempname, "%s%04d-%02d.anim", fullname.c_str(),
                anims[i].animID, animsWotLK.subAnimID);
            if(MPQFile::getSize(tempname) > 0)
            {
                animfiles[i].openFile(tempname);
            }
        }
#endif 

        animManager = new AnimManager(anims);
    }

    if(animBones)
    {
        // init bones...
        bones = new Bone[m_header.nBones];
        ModelBoneDef *mb = (ModelBoneDef*) (f.getBuffer() + m_header.ofsBones);
        for(size_t i = 0; i < m_header.nBones; i++)
        {
            //if (i==0) mb[i].rotation.ofsRanges = 1.0f;
#ifdef WotLK
            bones[i].init(f, mb[i], m_globalSequences, animfiles);
#else 
            bones[i].init(f, mb[i], m_globalSequences);
#endif 
        }

        // Block BoneLookup is a lookup table for Key Skeletal Bones, hands, arms, legs, etc.
        //header.nBoneLookup
        int16 *p = (int16*) (f.getBuffer() + m_header.ofsBoneLookup);
        for(size_t i = 0; i < m_header.nBoneLookup; i++)
        {
            boneLookup[i] = p[i];
        }
    }

#ifdef WotLK
    // free MPQFile
    if(m_header.nAnimations > 0)
    {
        for(size_t i = 0; i < m_header.nAnimations; i++)
        {
            if(animfiles[i].getSize() > 0)
            {
                animfiles[i].close();
            }
        }
        delete [] animfiles;
    }
#endif 

    const size_t size = (m_header.nVertices *sizeof(float) );
    m_vbufsize = (3 *size);
    // we multiple by 3 for the x, y, z positions of the vertex

    texCoords = new Vec2D[m_header.nVertices];
    for(size_t i = 0; i < m_header.nVertices; i++)
    {
        texCoords[i] = m_origVertices[i].texcoords;
    }

    if(g_videoSetting.supportVBO)
    {
        // Vert buffer
        glGenBuffersARB(1, &m_vbuf);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_vbufsize, vertices,
            GL_STATIC_DRAW_ARB);
        wxDELETEA(vertices);

        // Texture buffer
        glGenBuffersARB(1, &m_tbuf);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_tbuf);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, 2 *size, texCoords,
            GL_STATIC_DRAW_ARB);
        wxDELETEA(texCoords);

        // normals buffer
        glGenBuffersARB(1, &m_nbuf);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nbuf);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_vbufsize, normals,
            GL_STATIC_DRAW_ARB);
        wxDELETEA(normals);
    }

    if(animTextures)
    {
        m_texAnims = new TextureAnim[m_header.nTexAnims];
        ModelTexAnimDef *ta = (ModelTexAnimDef*) (f.getBuffer() +
            m_header.ofsTexAnims);
        for(size_t i = 0; i < m_header.nTexAnims; i++)
        {
            m_texAnims[i].init(f, ta[i], m_globalSequences);
        }
    }

    // particle systems
    if(m_header.nParticleEmitters)
    {
        ModelParticleEmitterDef *pdefs = (ModelParticleEmitterDef*)(f.getBuffer
            () + m_header.ofsParticleEmitters);
        m_particleSystems = new ParticleSystem[m_header.nParticleEmitters];
        hasParticles = true;
        for(size_t i = 0; i < m_header.nParticleEmitters; i++)
        {
            m_particleSystems[i].model = this;
            m_particleSystems[i].init(f, pdefs[i], m_globalSequences);
        }
    }

    // ribbons
    if(m_header.nRibbonEmitters)
    {
        ModelRibbonEmitterDef *rdefs = (ModelRibbonEmitterDef*) (f.getBuffer() 
            + m_header.ofsRibbonEmitters);
        m_ribbons = new RibbonEmitter[m_header.nRibbonEmitters];
        for(size_t i = 0; i < m_header.nRibbonEmitters; i++)
        {
            m_ribbons[i].model = this;
            m_ribbons[i].init(f, rdefs[i], m_globalSequences);
        }
    }

    // just use the first camera, meh
    if(m_header.nCameras > 0)
    {
        ModelCameraDef *camDefs = (ModelCameraDef*) (f.getBuffer() +
            m_header.ofsCameras);
        m_cam.init(f, camDefs[0], m_globalSequences);
        hasCamera = true;
    }

    // init lights
    if(m_header.nLights)
    {
        m_lights = new ModelLight[m_header.nLights];
        ModelLightDef *lDefs = (ModelLightDef*) (f.getBuffer() +
            m_header.ofsLights);
        for(size_t i = 0; i < m_header.nLights; i++)
        {
            m_lights[i].init(f, lDefs[i], m_globalSequences);
        }
    }


    animcalc = false;
}

//-------------------------------------------------------------------------

void Model::setLOD(MPQFile &f, int index)
{
    /*
    // I thought the view controlled the Level of detail,  but that doesn't seem to be the case.
    // Seems to only control the render order.  Which makes this function useless and not needed :(

    // Texture definitions
    ModelTextureDef *texdef = (ModelTextureDef*)(f.getBuffer() + header.ofsTextures);

    // Transparency
    int16 *transLookup = (int16*)(f.getBuffer() + header.ofsTransparencyLookup);

    // Level of Detail View
    ModelView *view = (ModelView*)(f.getBuffer() + header.ofsViews);

    uint16 *indexLookup = (uint16*)(f.getBuffer() + view[index].ofsIndex);
    uint16 *triangles = (uint16*)(f.getBuffer() + view[index].ofsTris);
    nIndices = view->nTris;

    wxDELETEA(indices);

    indices = new uint16[nIndices];
    for (size_t i = 0; i<nIndices; i++) {
    indices[i] = indexLookup[triangles[i]];
    }

    // render ops
    ModelGeoset *ops = (ModelGeoset*)(f.getBuffer() + view[index].ofsSub);
    ModelTexUnit *tex = (ModelTexUnit*)(f.getBuffer() + view[index].ofsTex);
    ModelRenderFlags *renderFlags = (ModelRenderFlags*)(f.getBuffer() + header.ofsTexFlags);
    uint16 *texlookup = (uint16*)(f.getBuffer() + header.ofsTexLookup);
    uint16 *texanimlookup = (uint16*)(f.getBuffer() + header.ofsTexAnimLookup);
    int16 *texunitlookup = (int16*)(f.getBuffer() + header.ofsTexUnitLookup);

    wxDELETEA(showGeosets);

    showGeosets = new bool[view[index].nSub];
    for (size_t i=0; i<view[index].nSub; i++) {
    geosets.push_back(ops[i]);
    showGeosets[i] = true;
    }

    passes.clear();
    for (size_t j=0; j<view[index].nTex; j++) {
    ModelRenderPass pass;

    pass.useTex2 = false;
    pass.useEnvMap = false;
    pass.cull = false;
    pass.Trans = false;
    pass.unlit = false;
    pass.noZWrite = false;

    //pass.texture2 = 0;
    size_t geoset = tex[j].op;

    pass.geoset = (int)geoset;

    pass.indexStart = ops[geoset].istart;
    pass.indexCount = ops[geoset].icount;
    pass.vertexStart = ops[geoset].vstart;
    pass.vertexEnd = pass.vertexStart + ops[geoset].vcount;

    pass.order = tex[j].order; //pass.order = 0;

    //TextureID texid = textures[texlookup[tex[j].textureid]];
    //pass.texture = texid;
    pass.tex = texlookup[tex[j].textureid];

    // TODO: figure out these flags properly -_-
    ModelRenderFlags &rf = renderFlags[tex[j].flagsIndex];

    pass.blendmode = rf.blend;
    pass.color = tex[j].colorIndex;
    pass.opacity = transLookup[tex[j].transid];

    //if (name == "Creature\\Kelthuzad\\kelthuzad.m2")
    pass.useEnvMap = (texunitlookup[tex[j].texunit] == -1) && ((rf.flags & 0x10) !=0) && rf.blend>2; // Use environmental reflection effects?
    //else
    //	pass.useEnvMap = (texunitlookup[tex[j].texunit] == -1) && ((rf.flags & 0x10) !=0);

    // This is wrong but meh.. best I could get it so far.
    //pass.cull = (rf.flags & 0x04)==0 && pass.blendmode!=1 && pass.blendmode!=4 && (rf.flags & 17)!=17;
    pass.cull = false; // quick test

    pass.unlit = (rf.flags & 0x01)!=0;

    //pass.noZWrite = (texdef[pass.tex].flags & 3)!=0;
    if (name == "Creature\\Turkey\\turkey.m2") // manual fix as I just bloody give up.
    pass.noZWrite = false;
    else
    pass.noZWrite = (pass.blendmode>1) && !(rf.blend==4 && rf.flags==17);

    //pass.noZWrite = false; // quick test

    // ToDo: Work out the correct way to get the true/false of transparency
    pass.Trans = (pass.blendmode>0) && (pass.opacity>0);	// Transparency - not the correct way to get transparency

    pass.p = ops[geoset].v.z;

    pass.swrap = (texdef[pass.tex].flags & 1) != 0;
    pass.twrap = (texdef[pass.tex].flags & 2) != 0;


    if (animTextures) {
    //if (tex[j].flags & 16) {
    if (tex[j].flags & 15) {
    pass.texanim = -1; // no texture animation
    } else {
    pass.texanim = texanimlookup[tex[j].texanimid];
    }
    } else {
    pass.texanim = -1; // no texture animation
    }

    passes.push_back(pass);
    }

    // transparent parts come later
    std::sort(passes.begin(), passes.end()); 
     */
}

//-------------------------------------------------------------------------

void Model::calcBones(int anim, int time)
{
    // Reset all bones to 'false' which means they haven't been animated yet.
    for(size_t i = 0; i < m_header.nBones; i++)
    {
        bones[i].calc = false;
    }

    // Character specific bone animation calculations.
    if(charModelDetails.isChar)
    {

        // Animate the "core" rotations and transformations for the rest of the model to adopt into their transformations
        if(boneLookup[BONE_ROOT] >  - 1)
        {
            for(int i = 0; i <= boneLookup[BONE_ROOT]; i++)
            {
                bones[i].calcMatrix(bones, anim, time);
            }
        }

        // Find the close hands animation id
        int closeFistID = 0;
        for(unsigned int i = 0; i < m_header.nAnimations; i++)
        {
            if(anims[i].animID == 15)
            {
                // closed fist
                closeFistID = i;
                break;
            }
        }

        // Animate key skeletal bones except the fingers which we do later.
        // -----
        int a, t;

        // if we have a "secondary animation" selected,  animate upper body using that.
        if(animManager->GetSecondaryID() >  - 1)
        {
            a = animManager->GetSecondaryID();
            t = animManager->GetSecondaryFrame();
        }
        else
        {
            a = anim;
            t = time;
        }

        for(size_t i = 0; i < 5; i++)
        {

            // only goto 5, otherwise it affects the hip/waist rotation for the lower-body.
            if(boneLookup[i] >  - 1)
            {
                bones[boneLookup[i]].calcMatrix(bones, a, t);
            }
        }

        if(animManager->GetMouthID() >  - 1)
        {
            // Animate the head and jaw
            if(boneLookup[BONE_HEAD] >  - 1)
            {
                bones[boneLookup[BONE_HEAD]].calcMatrix(bones, animManager
                    ->GetMouthID(), animManager->GetMouthFrame() );
            }
            if(boneLookup[BONE_JAW] >  - 1)
            {
                bones[boneLookup[BONE_JAW]].calcMatrix(bones, animManager
                    ->GetMouthID(), animManager->GetMouthFrame() );
            }
        }
        else
        {
            // Animate the head and jaw
            if(boneLookup[BONE_HEAD] >  - 1)
            {
                bones[boneLookup[BONE_HEAD]].calcMatrix(bones, a, t);
            }
            if(boneLookup[BONE_JAW] >  - 1)
            {
                bones[boneLookup[BONE_JAW]].calcMatrix(bones, a, t);
            }
        }

        // still not sure what 18-26 bone lookups are but I think its more for things like wrist, etc which are not as visually obvious.
        for(size_t i = 18; i < 27; i++)
        {
            if(boneLookup[i] >  - 1)
            {
                bones[boneLookup[i]].calcMatrix(bones, a, t);
            }
        }
        // =====



        if(charModelDetails.closeRHand)
        {
            a = closeFistID;
            t = anims[closeFistID].timeStart + 1;
        }
        else
        {
            a = anim;
            t = time;
        }

        for(unsigned int i = 0; i < 5; i++)
        {
            if(boneLookup[BONE_RFINGER1 + i] >  - 1)
            {
                bones[boneLookup[BONE_RFINGER1 + i]].calcMatrix(bones, a, t);
            }
        }

        if(charModelDetails.closeLHand)
        {
            a = closeFistID;
            t = anims[closeFistID].timeStart + 1;
        }
        else
        {
            a = anim;
            t = time;
        }

        for(unsigned int i = 0; i < 5; i++)
        {
            if(boneLookup[BONE_LFINGER1 + i] >  - 1)
            {
                bones[boneLookup[BONE_LFINGER1 + i]].calcMatrix(bones, a, t);
            }
        }
    }
    else
    {
        for(int i = 0; i < boneLookup[BONE_ROOT]; i++)
        {
            bones[i].calcMatrix(bones, anim, time);
        }

        // The following line fixes 'mounts' in that the character doesn't get rotated, but it also screws up the rotation for the entire model :(
        //bones[18].calcMatrix(bones, anim, time, false);

    }

    // Animate everything thats left with the 'default' animation
    for(size_t i = 0; i < m_header.nBones; i++)
    {
        bones[i].calcMatrix(bones, anim, time);
    }
}

//-------------------------------------------------------------------------

void Model::animate(int anim)
{
    int t = 0;

    ModelAnimation &a = anims[anim];
    int tmax = (a.timeEnd - a.timeStart);
    if(tmax == 0)
    {
        tmax = 1;
    }

    if(isWMO == true)
    {
        t = globalTime;
        t %= tmax;
        t += a.timeStart;
    }
    else
    {
        t = animManager->GetFrame();
    }

    this->animtime = t;
    this->anim = anim;

    if(animBones)
    // && (!animManager->IsPaused() || !animManager->IsParticlePaused()))
    {
        calcBones(anim, t);
    }

    if(animGeometry)
    {

        if(g_videoSetting.supportVBO)
        {
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf);
            glBufferDataARB(GL_ARRAY_BUFFER_ARB, 2 *m_vbufsize, NULL,
                GL_STREAM_DRAW_ARB);
            vertices = (Vec3D*)glMapBufferARB(GL_ARRAY_BUFFER_ARB,
                GL_WRITE_ONLY);

            // Something has been changed in the past couple of days that is causing nasty bugs
            // this is an extra error check to prevent the program from crashing.
            if(!vertices)
            {
                wxLogMessage(_T(
                    "Critical Error: void Model::animate(int anim), Vertex Buffer is null") );
                return ;
            }
        }

        // transform vertices
        ModelVertex *ov = m_origVertices;
        for(size_t i = 0; i < m_header.nVertices; ++i, ++ov)
        {
            //,k=0
            Vec3D v(0, 0, 0), n(0, 0, 0);

            for(size_t b = 0; b < 4; b++)
            {
                if(ov->weights[b] > 0)
                {
                    Vec3D tv = bones[ov->bones[b]].mat *ov->pos;
                    Vec3D tn = bones[ov->bones[b]].mrot *ov->normal;
                    v += tv *( (float)ov->weights[b] / 255.0f);
                    n += tn *( (float)ov->weights[b] / 255.0f);
                }
            }

            vertices[i] = v;
            if(g_videoSetting.supportVBO)
            {
                vertices[m_header.nVertices + i] = n.normalize();
            }
            // shouldn't these be normal by default?
            else
            {
                normals[i] = n;
            }
        }

        if(g_videoSetting.supportVBO)
        {
            glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
        }
    }

    for(size_t i = 0; i < m_header.nLights; i++)
    {
        if(m_lights[i].parent >= 0)
        {
            m_lights[i].tpos = bones[m_lights[i].parent].mat *m_lights[i].pos;
            m_lights[i].tdir = bones[m_lights[i].parent].mrot *m_lights[i].dir;
        }
    }

    for(size_t i = 0; i < m_header.nParticleEmitters; i++)
    {
        // random time distribution for teh win ..?
        int pt = a.timeStart + (t + (int) (tmax *m_particleSystems[i].tofs) ) %
            tmax;
        m_particleSystems[i].setup(anim, pt);
    }

    for(size_t i = 0; i < m_header.nRibbonEmitters; i++)
    {
        m_ribbons[i].setup(anim, t);
    }

    if(animTextures)
    {
        for(size_t i = 0; i < m_header.nTexAnims; i++)
        {
            m_texAnims[i].calc(anim, t);
        }
    }
}

//-------------------------------------------------------------------------


bool ModelRenderPass::init(Model *m)
{
    // May aswell check that we're going to render the geoset before doing all this crap.
    if(m->showGeosets[geoset])
    {

        // COLOUR
        // Get the colour and transparency and check that we should even render
        ocol = Vec4D(1.0f, 1.0f, 1.0f, m->trans);
        ecol = Vec4D(0.0f, 0.0f, 0.0f, 0.0f);

        //if (m->trans == 1.0f)
        //	return false;

        // emissive colors
        if(color !=  - 1)
        {
            Vec3D c = m->m_colors[color].color.getValue(m->anim, m->animtime);
            float o = m->m_colors[color].opacity.getValue(m->anim, m->animtime);
            ocol.w = o;

            if(unlit)
            {
                ocol.x = c.x;
                ocol.y = c.y;
                ocol.z = c.z;
            }
            else
            {
                ocol.x = ocol.y = ocol.z = 0;
            }

            ecol = Vec4D(c, ocol.w);
            glMaterialfv(GL_FRONT, GL_EMISSION, ecol);
        }

#ifndef WotLK /* Alfred 2008.10.02 buggy opacity make model invisable, TODO */
        // opacity
        if(opacity !=  - 1)
        {
            ocol.w *= m->m_transparency[opacity].trans.getValue(m->anim, m
                ->animtime);
        }
#endif 

        // exit and return false before affecting the opengl render state
        if(!( (ocol.w > 0) && (color ==  - 1 || ecol.w > 0) ) )
        {
            return false;
        }

        // TEXTURE
        // bind to our texture
        GLuint bindtex = 0;
        if(m->specialTextures[tex] ==  - 1)
        {
            bindtex = m->textures[tex];
        }
        else
        {
            bindtex = m->replaceTextures[m->specialTextures[tex]];
        }

        glBindTexture(GL_TEXTURE_2D, bindtex);
        // --

        // TODO: Add proper support for multi-texturing.

        // ALPHA BLENDING
        // blend mode
        switch(blendmode)
        {
            case BM_OPAQUE:
                // 0			
                break;
            case BM_TRANSPARENT:
                // 1
                glEnable(GL_ALPHA_TEST);
                glAlphaFunc(GL_GEQUAL, 0.7f);

                /*
                // Tex settings
                glTexEnvf(GL_TEXTURE_ENV,GL_ALPHA_SCALE,1.000000);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_ALPHA,GL_PREVIOUS);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE1_ALPHA,GL_TEXTURE);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE2_ALPHA,GL_CONSTANT);
                glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_ALPHA,GL_MODULATE);
                 */

                break;
            case BM_ALPHA_BLEND:
                // 2
                //glEnable(GL_ALPHA_TEST);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;
            case BM_ADDITIVE:
                // 3
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_COLOR, GL_ONE);
                break;
            case BM_ADDITIVE_ALPHA:
                // 4
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);

                /*
                glTexEnvf(GL_TEXTURE_ENV,GL_ALPHA_SCALE,1.000000);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_ALPHA,GL_PREVIOUS);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE1_ALPHA,GL_TEXTURE);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE2_ALPHA,GL_CONSTANT);
                glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_ALPHA,GL_MODULATE);
                 */

                break;
            case BM_MODULATE:
                // 5
                glEnable(GL_BLEND);
                glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);

                /*
                // Texture settings.
                glTexEnvf(GL_TEXTURE_ENV,GL_RGB_SCALE,1.000000);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_RGB,GL_TEXTURE);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE1_RGB,GL_PREVIOUS);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE2_RGB,GL_PREVIOUS);
                glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_RGB,GL_INTERPOLATE);
                glTexEnvf(GL_TEXTURE_ENV,GL_ALPHA_SCALE,1.000000);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_ALPHA,GL_PREVIOUS);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE1_ALPHA,GL_TEXTURE);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE2_ALPHA,GL_CONSTANT);
                glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_ALPHA,GL_REPLACE);
                 */

                break;
            case BM_MODULATEX2:
                // 6, not sure if this is right
                glEnable(GL_BLEND);
                glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);

                /*
                // Texture settings.
                glTexEnvf(GL_TEXTURE_ENV,GL_RGB_SCALE,1.000000);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_RGB,GL_TEXTURE);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE1_RGB,GL_PREVIOUS);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE2_RGB,GL_PREVIOUS);
                glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_RGB,GL_INTERPOLATE);
                glTexEnvf(GL_TEXTURE_ENV,GL_ALPHA_SCALE,1.000000);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_ALPHA,GL_PREVIOUS);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE1_ALPHA,GL_TEXTURE);
                glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE2_ALPHA,GL_CONSTANT);
                glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_ALPHA,GL_REPLACE);
                 */

                break;
            default:
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        //if (cull)
        //	glEnable(GL_CULL_FACE);

        // Texture wrapping around the geometry
        if(swrap)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        }
        if(twrap)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

        // no writing to the depth buffer.
        if(noZWrite)
        {
            glDepthMask(GL_FALSE);
        }

        // Environmental mapping, material, and effects
        if(useEnvMap)
        {
            // Turn on the 'reflection' shine, using 18.0f as that is what WoW uses based on the reverse engineering
            // This is now set in InitGL(); - no need to call it every render.
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 18.0f);

            // env mapping
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);

            const GLint maptype = GL_SPHERE_MAP;
            //const GLint maptype = GL_REFLECTION_MAP_ARB;

            glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, maptype);
            glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, maptype);
        }

        if(texanim !=  - 1)
        {
            glMatrixMode(GL_TEXTURE);
            glPushMatrix();

            m->m_texAnims[texanim].setup();
        }

        // color
        glColor4fv(ocol);
        //glMaterialfv(GL_FRONT, GL_SPECULAR, ocol);

        // don't use lighting on the surface
        if(unlit)
        {
            glDisable(GL_LIGHTING);
        }

        if(blendmode <= 1 && ocol.w < 1.0f)
        {
            glEnable(GL_BLEND);
        }

        return true;
    }

    return false;
}

//-------------------------------------------------------------------------

void ModelRenderPass::deinit()
{

    switch(blendmode)
    {
        case BM_OPAQUE:
            break;
        case BM_TRANSPARENT:
            glDisable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GEQUAL, 0.04f);

            /*
            glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE);
            glTexEnvf(GL_TEXTURE_ENV,GL_RGB_SCALE,1.000000);
            glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_RGB,GL_PREVIOUS);
            glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE1_RGB,GL_TEXTURE);
            glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE2_RGB,GL_CONSTANT);
            glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_RGB,GL_MODULATE);
            glTexEnvf(GL_TEXTURE_ENV,GL_ALPHA_SCALE,1.000000);
            glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_ALPHA,GL_PREVIOUS);
            glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE1_ALPHA,GL_TEXTURE);
            glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE2_ALPHA,GL_CONSTANT);
            glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_ALPHA,GL_REPLACE);
             */

            break;

        case BM_ALPHA_BLEND:
            glDisable(GL_BLEND);
            break;
        case BM_ADDITIVE:
            // 3
            glDisable(GL_BLEND);
            break;
        case BM_ADDITIVE_ALPHA:
            // 4
            //glDisable(GL_ALPHA_TEST);
            glDisable(GL_BLEND);
            break;

        case BM_MODULATE:
            // 5
            glDisable(GL_BLEND);
            break;

        case BM_MODULATEX2:
            // 6
            glDisable(GL_BLEND);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        default:
            glDisable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            // default blend func

            /*
            // Default texture settings.
            glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE);
            glTexEnvf(GL_TEXTURE_ENV,GL_RGB_SCALE, 1.000000);
            glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_RGB, GL_PREVIOUS);
            glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE1_RGB, GL_TEXTURE);
            glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE2_RGB, GL_CONSTANT);
            glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_RGB, GL_MODULATE);
            glTexEnvf(GL_TEXTURE_ENV,GL_ALPHA_SCALE, 1.000000);
            glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_ALPHA, GL_PREVIOUS);
            glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE1_ALPHA, GL_TEXTURE);
            glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE2_ALPHA, GL_CONSTANT);
            glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_ALPHA, GL_REPLACE);
             */
    }


    if(noZWrite)
    {
        glDepthMask(GL_TRUE);
    }

    if(texanim !=  - 1)
    {
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }

    if(unlit)
    {
        glEnable(GL_LIGHTING);
    }

    //if (billboard)
    //	glPopMatrix();

    if(cull)
    {
        glDisable(GL_CULL_FACE);
    }

    if(useEnvMap)
    {
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
    }

    if(swrap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    }

    if(twrap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    /*
    if (useTex2) {
    glDisable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE0);
    }
     */

    if(opacity !=  - 1 || color !=  - 1)
    {
        GLfloat czero[4] = 
        {
            0.0f, 0.0f, 0.0f, 1.0f
        };
        glMaterialfv(GL_FRONT, GL_EMISSION, czero);

        //glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        //glMaterialfv(GL_FRONT, GL_AMBIENT, ocol);
        //ocol = Vec4D(1.0f, 1.0f, 1.0f, 1.0f);
        //glMaterialfv(GL_FRONT, GL_DIFFUSE, ocol);
    }
}

//-------------------------------------------------------------------------


inline void Model::drawModel()
{
    // assume these client states are enabled: GL_VERTEX_ARRAY, GL_NORMAL_ARRAY, GL_TEXTURE_COORD_ARRAY
    if(g_videoSetting.supportVBO && animated)
    {
        // bind / point to the vertex normals buffer
        if(animGeometry)
        {
            glNormalPointer(GL_FLOAT, 0, GL_BUFFER_OFFSET(m_vbufsize) );
        }
        else
        {
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nbuf);
            glNormalPointer(GL_FLOAT, 0, 0);
        }

        // Bind the vertex buffer
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbuf);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        // Bind the texture coordinates buffer
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_tbuf);
        glTexCoordPointer(2, GL_FLOAT, 0, 0);

    }
    else if(animated)
    {
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glNormalPointer(GL_FLOAT, 0, normals);
        glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
    }

    // Display in wireframe mode?
    if(showWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // Render the various parts of the model.
    for(size_t i = 0; i < m_passes.size(); i++)
    {
        ModelRenderPass &p = m_passes[i];

        if(p.init(this) )
        {
            // we don't want to render completely transparent parts

            // render
            if(animated)
            {
                //glDrawElements(GL_TRIANGLES, p.indexCount, GL_UNSIGNED_SHORT, indices + p.indexStart);
                // a GDC OpenGL Performace Tuning paper recommended glDrawRangeElements over glDrawElements
                // I can't notice a difference but I guess it can't hurt
                if(g_videoSetting.supportVBO && g_videoSetting.supportDrawRangeElements)
                {
                    glDrawRangeElements(GL_TRIANGLES, p.vertexStart,
                        p.vertexEnd, p.indexCount, GL_UNSIGNED_SHORT, indices +
                        p.indexStart);

                    //} else if (!video.supportVBO) {
                    //	glDrawElements(GL_TRIANGLES, p.indexCount, GL_UNSIGNED_SHORT, indices + p.indexStart); 
                }
                else
                {
                    glBegin(GL_TRIANGLES);
                    for(size_t k = 0, b = p.indexStart; k < p.indexCount; k++, b++)
                    {
                        uint16 a = indices[b];
                        glNormal3fv(normals[a]);
                        glTexCoord2fv(m_origVertices[a].texcoords);
                        glVertex3fv(vertices[a]);
                    }
                    glEnd();
                }
            }
            else
            {
                glBegin(GL_TRIANGLES);
                for(size_t k = 0, b = p.indexStart; k < p.indexCount; k++, b++)
                {
                    uint16 a = indices[b];
                    glNormal3fv(normals[a]);
                    glTexCoord2fv(m_origVertices[a].texcoords);
                    glVertex3fv(vertices[a]);
                }
                glEnd();
            }

            p.deinit();
        }
    }

    if(showWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    // done with all render ops
}

//-------------------------------------------------------------------------

void TextureAnim::calc(int anim, int time)
{
    if(trans.used)
    {
        tval = trans.getValue(anim, time);
    }
    if(rot.used)
    {
        rval = rot.getValue(anim, time);
    }
    if(scale.used)
    {
        sval = scale.getValue(anim, time);
    }
}

//-------------------------------------------------------------------------

void TextureAnim::setup()
{
    glLoadIdentity();
    if(trans.used)
    {
        glTranslatef(tval.x, tval.y, tval.z);
    }
    if(rot.used)
    {
        glRotatef(rval.x, 0, 0, 1);
        // this is wrong, I have no idea what I'm doing here ;)
    }
    if(scale.used)
    {
        glScalef(sval.x, sval.y, sval.z);
    }
}

//-------------------------------------------------------------------------

void ModelCamera::init(MPQFile &f, ModelCameraDef &mcd, int *global)
{
    ok = true;
    nearclip = mcd.nearclip;
    farclip = mcd.farclip;
    fov = mcd.fov;
    pos = fixCoordSystem(mcd.pos);
    target = fixCoordSystem(mcd.target);
    tPos.init(mcd.transPos, f, global);
    tTarget.init(mcd.transTarget, f, global);
    rot.init(mcd.rot, f, global);
    tPos.fix(fixCoordSystem);
    tTarget.fix(fixCoordSystem);
}

//-------------------------------------------------------------------------

void ModelCamera::setup(int time)
{
    if(!ok)
    {
        return ;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov *34.5f, (GLfloat)g_videoSetting.xRes / (GLfloat)g_videoSetting.yRes,
        nearclip, farclip);

    Vec3D p = pos + tPos.getValue(0, time);
    Vec3D t = target + tTarget.getValue(0, time);

    Vec3D u(0, 1, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(p.x, p.y, p.z, t.x, t.y, t.z, u.x, u.y, u.z);
    //float roll = rot.getValue(0, time) / PI * 180.0f;
    //glRotatef(roll, 0, 0, 1);
}

//-------------------------------------------------------------------------

void ModelColor::init(MPQFile &f, ModelColorDef &mcd, int *global)
{
    color.init(mcd.color, f, global);
    opacity.init(mcd.opacity, f, global);
}

//-------------------------------------------------------------------------

void ModelTransparency::init(MPQFile &f, ModelTransDef &mcd, int *global)
{
    trans.init(mcd.trans, f, global);
}

//-------------------------------------------------------------------------

void ModelLight::init(MPQFile &f, ModelLightDef &mld, int *global)
{
    tpos = pos = fixCoordSystem(mld.pos);
    tdir = dir = Vec3D(0, 1, 0); // no idea
    type = mld.type;
    parent = mld.bone;
    ambColor.init(mld.ambColor, f, global);
    ambIntensity.init(mld.ambIntensity, f, global);
    diffColor.init(mld.color, f, global);
    diffIntensity.init(mld.intensity, f, global);
}

//-------------------------------------------------------------------------

void ModelLight::setup(int time, GLuint l)
{
    Vec4D ambcol(ambColor.getValue(0, time) *ambIntensity.getValue(0, time),
        1.0f);
    Vec4D diffcol(diffColor.getValue(0, time) *diffIntensity.getValue(0, time),
        1.0f);
    Vec4D p;
    if(type == 0)
    {
        // directional
        p = Vec4D(tdir, 0.0f);
    }
    else
    {
        // point
        p = Vec4D(tpos, 1.0f);
    }
    //gLog("Light %d (%f,%f,%f) (%f,%f,%f) [%f,%f,%f]\n", l-GL_LIGHT4, ambcol.x, ambcol.y, ambcol.z, diffcol.x, diffcol.y, diffcol.z, p.x, p.y, p.z);
    glLightfv(l, GL_POSITION, p);
    glLightfv(l, GL_DIFFUSE, diffcol);
    glLightfv(l, GL_AMBIENT, ambcol);
    glEnable(l);
}

//-------------------------------------------------------------------------

void TextureAnim::init(MPQFile &f, ModelTexAnimDef &mta, int *global)
{
    trans.init(mta.trans, f, global);
    rot.init(mta.rot, f, global);
    scale.init(mta.scale, f, global);
}

//-------------------------------------------------------------------------

#ifdef WotLK
void Bone::init(MPQFile &f, ModelBoneDef &b, int *global, MPQFile *animfiles)
{
    calc = false;

    parent = b.parent;
    pivot = fixCoordSystem(b.pivot);
    billboard = (b.flags &8) != 0;
    //billboard = false;

    boneDef = b;

    trans.init(b.translation, f, global, animfiles);
    rot.init(b.rotation, f, global, animfiles);
    scale.init(b.scaling, f, global, animfiles);
    trans.fix(fixCoordSystem);
    rot.fix(fixCoordSystemQuat);
    scale.fix(fixCoordSystem2);
}

//-------------------------------------------------------------------------

#else 
void Bone::init(MPQFile &f, ModelBoneDef &b, int *global)
{
    calc = false;

    parent = b.parent;
    pivot = fixCoordSystem(b.pivot);
    billboard = (b.flags &8) != 0;
    //billboard = false;

    boneDef = b;

    trans.init(b.translation, f, global);
    rot.init(b.rotation, f, global);
    scale.init(b.scaling, f, global);
    trans.fix(fixCoordSystem);
    rot.fix(fixCoordSystemQuat);
    scale.fix(fixCoordSystem2);
}

//-------------------------------------------------------------------------

#endif 

void ModelAttachment::init(MPQFile &f, ModelAttachmentDef &mad, int *global)
{
    pos = fixCoordSystem(mad.pos);
    bone = mad.bone;
    id = mad.id;
}

//-------------------------------------------------------------------------

void ModelAttachment::setup()
{
    Matrix m = model->bones[bone].mat;
    m.transpose();
    glMultMatrixf(m);
    glTranslatef(pos.x, pos.y, pos.z);
}

//-------------------------------------------------------------------------

void ModelAttachment::setupParticle()
{
    Matrix m = model->bones[bone].mat;
    m.transpose();
    glMultMatrixf(m);
    glTranslatef(pos.x, pos.y, pos.z);
}

//-------------------------------------------------------------------------

void Bone::calcMatrix(Bone *allbones, int anim, int time, bool rotate)
{
    if(calc)
    {
        return ;
    }

    Matrix m;
    Quaternion q;

    bool tr = rot.used || scale.used || trans.used || billboard;
    if(tr)
    {
        m.translation(pivot);

        if(trans.used)
        {
            Vec3D tr = trans.getValue(anim, time);
            m *= Matrix::newTranslation(tr);
        }

        if(rot.used && rotate)
        {
            q = rot.getValue(anim, time);
            m *= Matrix::newQuatRotate(q);
        }

        if(scale.used)
        {
            Vec3D sc = scale.getValue(anim, time);
            m *= Matrix::newScale(sc);
        }

        if(billboard)
        {
            float modelview[16];
            glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

            Vec3D vRight = Vec3D(modelview[0], modelview[4], modelview[8]);
            Vec3D vUp = Vec3D(modelview[1], modelview[5], modelview[9]);
            // Spherical billboarding
            //Vec3D vUp = Vec3D(0,1,0); // Cylindrical billboarding
            vRight = vRight *  - 1;
            m.m[0][2] = vRight.x;
            m.m[1][2] = vRight.y;
            m.m[2][2] = vRight.z;
            m.m[0][1] = vUp.x;
            m.m[1][1] = vUp.y;
            m.m[2][1] = vUp.z;
        }

        m *= Matrix::newTranslation(pivot * - 1.0f);

    }
    else
    {
        m.unit();
    }

    if(parent >  - 1)
    {
        allbones[parent].calcMatrix(allbones, anim, time, rotate);
        mat = allbones[parent].mat *m;
    }
    else
    {
        mat = m;
    }

    // transform matrix for normal vectors ... ??
    if(rot.used && rotate)
    {
        if(parent >= 0)
        {
            mrot = allbones[parent].mrot *Matrix::newQuatRotate(q);
        }
        else
        {
            mrot = Matrix::newQuatRotate(q);
        }
    }
    else
    {
        mrot.unit();
    }

    transPivot = mat * pivot;

    calc = true;
}

//-------------------------------------------------------------------------


inline void Model::draw()
{
    if(!ok)
    {
        return ;
    }

    if(!animated)
    {
        if(showModel)
        {
            glCallList(dlist);
        }

    }
    else
    {
        if(ind)
        {
            animate(currentAnim);
        }
        else
        {
            if(!animcalc)
            {
                animate(currentAnim);
                //animcalc = true; // Not sure what this is really for but it breaks WMO animation
            }
        }

        if(showModel)
        {
            drawModel();
        }
    }
}

// These aren't really needed in the model viewer.. only wowmapviewer
void Model::lightsOn(GLuint lbase)
{
    // setup lights
    for(unsigned int i = 0, l = lbase; i < m_header.nLights; i++)
    {
        m_lights[i].setup(animtime, l++);
    }
}

// These aren't really needed in the model viewer.. only wowmapviewer
void Model::lightsOff(GLuint lbase)
{
    for(unsigned int i = 0, l = lbase; i < m_header.nLights; i++)
    {
        glDisable(l++);
    }
}

// Updates our particles within models.
void Model::updateEmitters(float dt)
{
    if(!ok || !showParticles)
    {
        return ;
    }

    for(size_t i = 0; i < m_header.nParticleEmitters; i++)
    {
        m_particleSystems[i].update(dt);
    }
}


// Draws the "bones" of models  (skeletal animation)
void Model::drawBones()
{
    glDisable(GL_DEPTH_TEST);
    glBegin(GL_LINES);
    for(size_t i = 0; i < m_header.nBones; i++)
    {
        //for (size_t i=30; i<40; i++) {
        if(bones[i].parent !=  - 1)
        {
            glVertex3fv(bones[i].transPivot);
            glVertex3fv(bones[bones[i].parent].transPivot);
        }
    }
    glEnd();
    glEnable(GL_DEPTH_TEST);
}

// Sets up the models attachments
void Model::setupAtt(int id)
{
    int l = attLookup[id];
    if(l >  - 1)
    {
        atts[l].setup();
    }
}

// Sets up the models attachments
void Model::setupAtt2(int id)
{
    int l = attLookup[id];
    if(l >= 0)
    {
        atts[l].setupParticle();
    }
}

// Draws the Bounding Volume, which is used for Collision detection.
void Model::drawBoundingVolume()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLES);
    for(size_t i = 0; i < m_header.nBoundingTriangles; i++)
    {
        size_t v = m_boundTris[i];
        if(v < m_header.nBoundingVertices)
        {
            glVertex3fv(m_bounds[v]);
        }
        else
        {
            glVertex3f(0, 0, 0);
        }
    }
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// Renders our particles into the pipeline.
void Model::drawParticles()
{
    // draw particle systems
    for(size_t i = 0; i < m_header.nParticleEmitters; i++)
    {
        m_particleSystems[i].draw();
    }

    // draw ribbons
    for(size_t i = 0; i < m_header.nRibbonEmitters; i++)
    {
        m_ribbons[i].draw();
    }
}

// Adds models to the model manager, used by WMO's
int ModelManager::add(std::string name)
{
    int id;
    if(names.find(name) != names.end() )
    {
        id = names[name];
        items[id]->addref();
        return id;
    }
    // load new
    Model *model = new Model(name);
    id = nextID();
    do_add(name, id, model);
    return id;
}

// Resets the animation back to default.
void ModelManager::resetAnim()
{
    for(std::map < int, ManagedItem * > ::iterator it = items.begin(); it !=
        items.end(); ++it)
    {
        ( (Model*)it->second)->animcalc = false;
    }
}

// same as other updateEmitter except does it for the all the models being managed - for WMO's
void ModelManager::updateEmitters(float dt)
{
    for(std::map < int, ManagedItem * > ::iterator it = items.begin()
		; it != items.end()
		; ++it)
    {
        ( (Model*)it->second)->updateEmitters(dt);
    }
}

//-------------------------------------------------------------------------

void ModelManager::clear()
{
    for(std::map < int, ManagedItem * > ::iterator it = items.begin()
		; it != items.end()
		; ++it)
    {
        doDelete(it->first);
        delete it->second;
    }
    items.clear();
    names.clear();
}
