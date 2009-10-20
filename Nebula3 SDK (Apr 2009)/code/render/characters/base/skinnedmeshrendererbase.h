#pragma once
//------------------------------------------------------------------------------
/**
    @class Base::SkinnedMeshRendererBase
    
    Wraps platform-specific rendering of a skinned mesh. For a platform
    which supports GPU skinning (e.g. anything except the Wii), simply
    call the DrawGPUSkinnedMesh(). For a software-skinned platform,
    call the UpdateSoftwareSkinnedMesh() ideally once per frame (although the 
    method makes sure, that a mesh isn't skinned twice
    even when the method is called multiple times), and then use the returned
    DrawHandle to call DrawSoftwareSkinnedMesh() several times per frame.

    See CharacterNodeInstance for details.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "util/array.h"
#include "characters/characterinstance.h"
#include "coregraphics/mesh.h"
#include "coregraphics/shadervariable.h"

//------------------------------------------------------------------------------
namespace Base
{
class SkinnedMeshRendererBase : public Core::RefCounted
{
    __DeclareClass(SkinnedMeshRendererBase);
    __DeclareSingleton(SkinnedMeshRendererBase);
public:
    /// an abstract draw handle
    typedef IndexT DrawHandle;
    static const DrawHandle InvalidDrawHandle = InvalidIndex;    

    /// constructor
    SkinnedMeshRendererBase();
    /// destructor
    virtual ~SkinnedMeshRendererBase();

    /// setup the renderer
    void Setup();
    /// discard the renderer
    void Discard();
    /// return true if renderer is valid
    bool IsValid() const;
    
    /// test if this platform requires software skinning (e.g. Wii)
    bool RequiresSoftwareSkinning() const;

    /// call once at beginning of frame
    void OnBeginFrame();
    /// update a software skinned mesh
    DrawHandle UpdateSoftwareSkinnedMesh(const Ptr<Characters::CharacterInstance>& charInst, const Ptr<CoreGraphics::Mesh>& mesh);
    /// draw a software skinned mesh
    void DrawSoftwareSkinnedMesh(DrawHandle h, IndexT primGroupIndex);
    /// draw a hardware skinned mesh
    void DrawGPUSkinnedMesh(const Ptr<Characters::CharacterInstance>& charInst, const Ptr<CoreGraphics::Mesh>& mesh, IndexT primGroupIndex, const Util::Array<IndexT>& jointPalette, const Ptr<CoreGraphics::ShaderVariable>& jointPaletteShdVar);
    /// call once at end of frame (after rendering)
    void OnEndFrame();

protected:
    class DrawKey
    {
    private:
        void* ptr0;
        void* ptr1;

    public:
        /// default constructor
        DrawKey() : ptr0(0), ptr1(0) { };
        /// construct from character instance and mesh pointers
        DrawKey(Characters::CharacterInstance* c, CoreGraphics::Mesh* m) : ptr0(c), ptr1(m) { };            
        /// equality operator
        bool operator==(const DrawKey& rhs) const 
        { 
            return (this->ptr0 == rhs.ptr0) && (this->ptr1 == rhs.ptr1); 
        };
        /// inequality operator
        bool operator!=(const DrawKey& rhs) const 
        { 
            return (this->ptr0 != rhs.ptr0) || (this->ptr1 != rhs.ptr1); 
        };            
        /// greater operator
        bool operator>(const DrawKey& rhs) const 
        { 
            if (this->ptr0 == rhs.ptr0) return this->ptr1 > rhs.ptr1; 
            else return this->ptr0 > rhs.ptr0; 
        };                         
        /// lesser operator
        bool operator<(const DrawKey& rhs) const
        {
            if (this->ptr0 == rhs.ptr0) return this->ptr1 < rhs.ptr1;
            else return this->ptr0 < rhs.ptr0;
        };        
    };
    struct DrawInfo
    {
        Ptr<Characters::CharacterInstance> charInst;
        Ptr<CoreGraphics::Mesh> mesh;
        #if __WII__
        IndexT startVertexIndex;        // only used by software-skinning platforms
        #endif
    };

    bool inFrame;
    bool isValid;
    Util::Array<DrawInfo> drawInfos;   
    Util::Dictionary<DrawKey,IndexT> keyIndexMap;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
SkinnedMeshRendererBase::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
    Override this method in a platform specific subclass!
*/
inline bool
SkinnedMeshRendererBase::RequiresSoftwareSkinning() const
{
    return false;
}

} // namespace Base
//------------------------------------------------------------------------------
