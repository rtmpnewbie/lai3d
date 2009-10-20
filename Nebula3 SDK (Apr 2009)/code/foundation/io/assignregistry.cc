//------------------------------------------------------------------------------
//  assignregistry.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/assignregistry.h"
#include "io/fswrapper.h"

namespace IO
{
__ImplementClass(IO::AssignRegistry, 'ASRG', Core::RefCounted);
__ImplementInterfaceSingleton(IO::AssignRegistry);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
AssignRegistry::AssignRegistry() :
    isValid(false),
    rootDirectory("home:")
{
    __ConstructInterfaceSingleton;
}

//------------------------------------------------------------------------------
/**
*/
AssignRegistry::~AssignRegistry()
{
    if (this->IsValid())
    {
        this->Discard();
    }
    __DestructInterfaceSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
AssignRegistry::Setup()
{
    this->critSect.Enter();

    n_assert(!this->IsValid());
    this->isValid = true;
    this->SetupSystemAssigns();
    this->SetupProjectAssigns();
    
    this->critSect.Leave();
}

//------------------------------------------------------------------------------
/**
*/
void
AssignRegistry::Discard()
{
    this->critSect.Enter();
    
    n_assert(this->IsValid());
    this->assignTable.Clear();
    this->isValid = false;
    
    this->critSect.Leave();
}

//------------------------------------------------------------------------------
/**
*/
bool
AssignRegistry::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
void
AssignRegistry::SetRootDirectory(const String& rootDir)
{
    this->critSect.Enter();
    this->rootDirectory = rootDir;
    this->SetAssign(Assign("root", rootDir));
    this->critSect.Leave();
}

//------------------------------------------------------------------------------
/**
*/
String
AssignRegistry::GetRootDirectory() const
{
    this->critSect.Enter();
    String result = this->rootDirectory;
    this->critSect.Leave();
    return result;
}

//------------------------------------------------------------------------------
/**
*/
void
AssignRegistry::SetupSystemAssigns()
{
    this->critSect.Enter();

    n_assert(this->IsValid()); 
    
    String homeLocation = Internal::FSWrapper::GetHomeDirectory();
    if (homeLocation.IsValid())
    {
        this->SetAssign(Assign("home", homeLocation));
    }
    #if !__WII__
    String binLocation = Internal::FSWrapper::GetBinDirectory();
    if (binLocation.IsValid())
    {
        this->SetAssign(Assign("bin", binLocation));
    }
    String tempLocation = Internal::FSWrapper::GetTempDirectory();
    if (tempLocation.IsValid())
    {
        this->SetAssign(Assign("temp", tempLocation));
    }
    String userLocation = Internal::FSWrapper::GetUserDirectory();
    if (userLocation.IsValid())
    {
        this->SetAssign(Assign("user", userLocation));
    }
    #endif
    
    #if __WIN32__
    String appDataLocation = Internal::FSWrapper::GetAppDataDirectory();
    if (appDataLocation.IsValid())
    {
        this->SetAssign(Assign("appdata", appDataLocation));
    }
    String programsLocation = Internal::FSWrapper::GetProgramsDirectory();
    if (programsLocation.IsValid())
    {
        this->SetAssign(Assign("programs", programsLocation));
    }
    #endif

    this->critSect.Leave();
}    

//------------------------------------------------------------------------------
/**
*/
void
AssignRegistry::SetupProjectAssigns()
{
    this->critSect.Enter();
        
    n_assert(this->IsValid());

    this->SetAssign(Assign("root", this->rootDirectory));
    this->SetAssign(Assign("msh", "root:export/meshes"));
    this->SetAssign(Assign("ani", "root:export/anims"));
    this->SetAssign(Assign("data", "root:export/data"));        
    this->SetAssign(Assign("export", "root:export"));
    this->SetAssign(Assign("video", "root:export/video"));

    // platform specific assigns
    #if __WIN32__
        this->SetAssign(Assign("stream", "root:export_win32/audio"));
        this->SetAssign(Assign("shd", "root:export_win32/shaders"));
        this->SetAssign(Assign("tex", "root:export_win32/textures"));
        this->SetAssign(Assign("textures", "root:export_win32/textures"));
        this->SetAssign(Assign("frame", "root:export_win32/frame"));
        this->SetAssign(Assign("mdl", "root:export_win32/models"));
        this->SetAssign(Assign("audio", "root:export_win32/audio"));
        this->SetAssign(Assign("sui", "root:export_win32/sui"));        
    #elif __XBOX360__
        this->SetAssign(Assign("stream", "root:export_xbox360/audio"));
        this->SetAssign(Assign("shd", "root:export_xbox360/shaders"));
        this->SetAssign(Assign("tex", "root:export_xbox360/textures"));
        this->SetAssign(Assign("textures", "root:export_xbox360/textures"));
        this->SetAssign(Assign("frame", "root:export_xbox360/frame"));
        this->SetAssign(Assign("mdl", "root:export_xbox360/models"));
        this->SetAssign(Assign("audio", "root:export_xbox360/audio"));
        this->SetAssign(Assign("sui", "root:export_xbox360/sui"));
    #elif __WII__
        this->SetAssign(Assign("stream", "root:export_wii/audio"));
        this->SetAssign(Assign("shd", "root:export_wii/shaders"));
        this->SetAssign(Assign("tex", "root:export_wii/textures"));
        this->SetAssign(Assign("textures", "root:export_wii/textures"));
        this->SetAssign(Assign("frame", "root:export_wii/frame"));
        this->SetAssign(Assign("mdl", "root:export_wii/models"));
        this->SetAssign(Assign("audio", "root:export_wii/audio"));
        this->SetAssign(Assign("sui", "root:export_wii/sui"));
    #else
    #error "Setup platform specific project assigns!");
    #endif

    // Nebul2 backward compat assigns:
    this->SetAssign(Assign("meshes", "root:export/meshes"));
    this->SetAssign(Assign("anims", "root:export/anims"));

    this->critSect.Leave();
}

//------------------------------------------------------------------------------
/**
*/
void
AssignRegistry::SetAssign(const Assign& assign)
{
    this->critSect.Enter();

    n_assert(this->IsValid());
    if (this->HasAssign(assign.GetName()))
    {
        this->ClearAssign(assign.GetName());
    }
    this->assignTable.Add(assign);
    
    this->critSect.Leave();
}

//------------------------------------------------------------------------------
/**
*/
bool
AssignRegistry::HasAssign(const String& assignName) const
{
    this->critSect.Enter();
    n_assert(assignName.IsValid());
    bool result = this->assignTable.Contains(assignName);
    this->critSect.Leave();
    return result;
}

//------------------------------------------------------------------------------
/**
*/
String
AssignRegistry::GetAssign(const String& assignName) const
{
    this->critSect.Enter();

    n_assert(assignName.IsValid());
    n_assert(this->HasAssign(assignName));
    String result = this->assignTable[assignName];
    
    this->critSect.Leave();
    return result;
}

//------------------------------------------------------------------------------
/**
*/
void
AssignRegistry::ClearAssign(const String& assignName)
{
    this->critSect.Enter();

    n_assert(assignName.IsValid());
    n_assert(this->HasAssign(assignName));
    this->assignTable.Erase(assignName);

    this->critSect.Leave();
}

//------------------------------------------------------------------------------
/**
*/
Array<Assign>
AssignRegistry::GetAllAssigns() const
{
    this->critSect.Enter();

    Array<KeyValuePair<String,String> > content = this->assignTable.Content();
    Array<Assign> assigns(content.Size(), 0);
    IndexT i;
    for (i = 0; i < content.Size(); i++)
    {
        assigns.Append(Assign(content[i].Key(), content[i].Value()));
    }

    this->critSect.Leave();
    return assigns;
}

//------------------------------------------------------------------------------
/**
    Resolves all assigns from an URI returning an URI. It is allowed to
    "stack" assigns, which means, defining an assign as pointing to
    another assign.
*/
URI
AssignRegistry::ResolveAssigns(const URI& uri) const
{
    this->critSect.Enter();
    URI resolvedUri = this->ResolveAssignsInString(uri.AsString());
    this->critSect.Leave();
    return resolvedUri;
}

//------------------------------------------------------------------------------
/**
    Resolves all assigns from a URI. It is allowed to
    "stack" assigns, which means, defining an assign as pointing to
    another assign.s
*/
String
AssignRegistry::ResolveAssignsInString(const String& uriString) const
{
    this->critSect.Enter();
    String result = uriString;

    // check for assigns
    int colonIndex;
    while ((colonIndex = result.FindCharIndex(':', 0)) > 0)
    {
        // special case: ignore one-caracter "assigns" because they are 
        // really DOS drive letters
        if (colonIndex > 1)
        {
            String assignString = result.ExtractRange(0, colonIndex);
            
            // ignore unknown assigns, because these may be part of an URI
            if (this->HasAssign(assignString))
            {
                String postAssignString = result.ExtractRange(colonIndex + 1, result.Length() - (colonIndex + 1));
                String replace = this->GetAssign(assignString);
                if (!replace.IsEmpty())
                {
                    if (replace[replace.Length()-1] != ':'
                        && (replace[replace.Length()-1] != '/'
                        || replace[replace.Length()-2] != ':'))
                    {
                        replace.Append("/");
                    }
                    replace.Append(postAssignString);
                }
                result = replace;
            }
            else break;
        }
        else break;
    }
    result.ConvertBackslashes();
    result.TrimRight("/");
    this->critSect.Leave();
    return result;
}

} // namespace IO