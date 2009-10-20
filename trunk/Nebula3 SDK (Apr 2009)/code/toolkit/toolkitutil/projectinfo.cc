//------------------------------------------------------------------------------
//  projectinfo.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "projectinfo.h"
#include "io/ioserver.h"
#include "io/stream.h"
#include "io/xmlreader.h"
#include "system/win32/win32registry.h"

namespace ToolkitUtil
{
using namespace Util;
using namespace IO;
using namespace System;

//------------------------------------------------------------------------------
/**
*/
ProjectInfo::ProjectInfo() :
    defPlatform(Platform::InvalidPlatform),
    curPlatform(Platform::InvalidPlatform)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ProjectInfo::~ProjectInfo()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ProjectInfo::HasAttr(const Util::String& attrName) const
{
    n_assert(attrName.IsValid());
    n_assert(Platform::InvalidPlatform != this->curPlatform);
    return this->platformAttrs[this->curPlatform].Contains(attrName);
}

//------------------------------------------------------------------------------
/**
*/
const Util::String&
ProjectInfo::GetAttr(const Util::String& attrName) const
{
    n_assert(attrName.IsValid());
    n_assert(Platform::InvalidPlatform != this->curPlatform);
    return this->platformAttrs[this->curPlatform][attrName];
}

//------------------------------------------------------------------------------
/**
*/
bool
ProjectInfo::IsValid() const
{
    return (this->platformAttrs.Size() > 0);
}

//------------------------------------------------------------------------------
/**
*/
ProjectInfo::Result
ProjectInfo::Setup()
{
    n_assert(!this->IsValid());

    // setup project and toolkit path assign
    String projPath = this->QueryProjectPathFromRegistry();
    if (projPath.IsEmpty())
    {
        return NoProjPathInRegistry;
    }
    String toolkitPath = this->QueryToolkitPathFromRegistry();
    n_assert(toolkitPath.IsValid());
    AssignRegistry::Instance()->SetAssign(Assign("proj", projPath));
    AssignRegistry::Instance()->SetAssign(Assign("toolkit", toolkitPath));

    // parse project info XML file
    Result res = ProjectInfo::Success;
    if (IoServer::Instance()->FileExists("proj:projectinfo.xml"))
    {
        res = this->ParseProjectInfoFile();
    }
    else
    {
        return NoProjectInfoFile;
    }
    return res;
}

//------------------------------------------------------------------------------
/**
    Parse the projectinfo.xml file which is expected in the project root 
    directory.
*/
ProjectInfo::Result
ProjectInfo::ParseProjectInfoFile()
{
    n_assert(!this->IsValid());

    Ptr<Stream> stream = IoServer::Instance()->CreateStream(URI("proj:projectinfo.xml"));
    Ptr<XmlReader> xmlReader = XmlReader::Create();
    xmlReader->SetStream(stream);
    if (xmlReader->Open())
    {
        // check if it's a valid project info file
        if (!xmlReader->HasNode("/Nebula3/Project"))
        {
            return ProjectFileContentInvalid;
        }
        xmlReader->SetToNode("/Nebula3/Project");
        n_assert(xmlReader->HasAttr("defaultPlatform"));
        this->defPlatform = Platform::FromString(xmlReader->GetString("defaultPlatform"));
        
        // for each platform...
        if (xmlReader->SetToFirstChild("Platform")) do
        {
            // setup a new set of platform attributes
            n_assert(xmlReader->HasAttr("name"));
            Platform::Code platform = Platform::FromString(xmlReader->GetString("name"));
            n_assert(!this->platformAttrs.Contains(platform));
            Dictionary<String,String> emptyDict;
            this->platformAttrs.Add(platform, emptyDict);

            // load attributes
            if (xmlReader->SetToFirstChild("Attr")) do
            {
                n_assert(xmlReader->HasAttr("name"));
                n_assert(xmlReader->HasAttr("value"));
                String attrName = xmlReader->GetString("name");
                String attrValue = xmlReader->GetString("value");
                n_assert(!this->platformAttrs[platform].Contains(attrName));
                this->platformAttrs[platform].Add(attrName, attrValue);
            }
            while (xmlReader->SetToNextChild("Attr"));
        }
        while (xmlReader->SetToNextChild("Platform"));
        return Success;
    }
    else
    {
        return CouldNotOpenProjectInfoFile;
    }
}

//------------------------------------------------------------------------------
/**
    Query the project path from the registry, the registry key is set
    by the "Nebula2 Toolkit For Maya". If no key is found, the method
    will return "home:".
*/
String
ProjectInfo::QueryProjectPathFromRegistry()
{
    String projDirectory;
    if (Win32Registry::Exists(Win32Registry::CurrentUser, "Software\\RadonLabs\\Toolkit", "project"))
    {
        projDirectory = Win32Registry::ReadString(Win32Registry::CurrentUser, "Software\\RadonLabs\\Toolkit", "project");
    }
    else
    {
        projDirectory = "home:";
    }
    return projDirectory;
}

//------------------------------------------------------------------------------
/**
    Query the toolkit path from the registry, the registry key is set
    by the "Nebula2 Toolkit For Maya". If no key is found, the method
    will return "home:".
*/
String
ProjectInfo::QueryToolkitPathFromRegistry()
{
    String toolkitDirectory;
    if (Win32Registry::Exists(Win32Registry::CurrentUser, "Software\\RadonLabs\\Toolkit", "path"))
    {
        toolkitDirectory = Win32Registry::ReadString(Win32Registry::CurrentUser, "Software\\RadonLabs\\Toolkit", "path");
    }
    else
    {
        toolkitDirectory = "home:";
    }
    return toolkitDirectory;
}

//------------------------------------------------------------------------------
/**
*/
String
ProjectInfo::GetErrorString(Result res) const
{
    switch (res)
    {
        case ProjectInfo::NoProjPathInRegistry:
            return String("ERROR: no Nebula toolkit entries found in registry!");
        case ProjectInfo::CouldNotOpenProjectInfoFile:
            return String("ERROR: could not open projectinfo.xml file in project directory!");
        case ProjectInfo::ProjectFileContentInvalid:
            return String("ERROR: content of projectinfo.xml is invalid!");
        case ProjectInfo::NoProjectInfoFile:
            return String("ERROR: No projectinfo.xml file found in current project!");
        default:
            return String("ERROR: unknown error from ProjectInfo object!\n");
    }
}

} // namespace ToolkitUtil