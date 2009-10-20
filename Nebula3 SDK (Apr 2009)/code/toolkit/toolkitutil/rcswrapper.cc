//------------------------------------------------------------------------------
//  rcswrapper.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "toolkitutil/rcswrapper.h"
#include "toolkitutil/applauncher.h"
#include "io/ioserver.h"
#include "io/textwriter.h"

namespace ToolkitUtil
{
using namespace IO;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
RCSWrapper::RCSWrapper()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
RCSWrapper::~RCSWrapper()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This writes a temporary config file to "temp:synctool3/". The config
    file instructs SVN to set the file timestamps to the commit-time.
*/
void
RCSWrapper::WriteTempSVNConfigFiles()
{
    IoServer* ioServer = IoServer::Instance();
    ioServer->CreateDirectory("temp:synctool3");
    Ptr<Stream> stream = ioServer->CreateStream("temp:synctool3/config");
    Ptr<TextWriter> textWriter = TextWriter::Create();
    textWriter->SetStream(stream);
    if (textWriter->Open())
    {
        textWriter->WriteLine("[auth]");
        textWriter->WriteLine("store-passwords = no");
        textWriter->WriteLine("store-auth-creds = no");
        textWriter->WriteLine("[helpers]");
        textWriter->WriteLine("[tunnels]");
        textWriter->WriteLine("[miscellany]");
        textWriter->WriteLine("use-commit-times = yes");
        textWriter->WriteLine("[auto-props]");
        textWriter->WriteLine("*.xls = svn:needs-lock");
        textWriter->WriteLine("*.xlsx = svn:needs-lock");
        textWriter->WriteLine("*.doc = svn:needs-lock");
        textWriter->WriteLine("*.docx = svn:needs-lock");
        textWriter->WriteLine("*.jpg = svn:needs-lock");
        textWriter->WriteLine("*.psd = svn:needs-lock");
        textWriter->WriteLine("*.mb = svn:needs-lock");
        textWriter->WriteLine("*.xml = svn:needs-lock");
        textWriter->WriteLine("*.exe = svn:needs-lock");
        textWriter->WriteLine("*.ppt = svn:needs-lock");
        textWriter->WriteLine("*.mpp = svn:needs-lock");
        textWriter->WriteLine("*.pdf = svn:needs-lock");
        textWriter->WriteLine("*.mmap = svn:needs-lock");
        textWriter->WriteLine("*.pst = svn:needs-lock");
        textWriter->WriteLine("*.msg = svn:needs-lock");
        textWriter->WriteLine("*.rtf = svn:needs-lock");
        textWriter->WriteLine("*.cab = svn:needs-lock");
        textWriter->WriteLine("*.dll = svn:needs-lock");
        textWriter->WriteLine("*.bat = svn:needs-lock");
        textWriter->WriteLine("*.bmp = svn:needs-lock");
        textWriter->WriteLine("*.ico = svn:needs-lock");
        textWriter->WriteLine("*.8bi = svn:needs-lock");
        textWriter->WriteLine("*.8bf = svn:needs-lock");
        textWriter->WriteLine("*.xsd = svn:needs-lock");
        textWriter->WriteLine("*.tga = svn:needs-lock");
        textWriter->WriteLine("*.dds = svn:needs-lock");
        textWriter->Close();

        ioServer->CopyFile("temp:synctool3/config", "temp:synctool3/config.txt");
    }

    // also need to write the user's authentification files
    this->CopyDirectory("appdata:Subversion/auth/svn.simple", "temp:synctool3/auth/svn.simple");
    this->CopyDirectory("appdata:Subversion/auth/svn.ssl.server", "temp:synctool3/auth/svn.ssl.server");
    this->CopyDirectory("appdata:Subversion/auth/svn.username", "temp:synctool3/auth/svn.username");
}

//------------------------------------------------------------------------------
/**
    Helper method to copy directory contents. Used when writing temporary 
    Subversion configuration data.
*/
void
RCSWrapper::CopyDirectory(const String& from, const String& to)
{
    IoServer* ioServer = IoServer::Instance();
    URI fromUri(from);
    URI toUri(to);
    if (ioServer->DirectoryExists(fromUri))
    {
        if (ioServer->CreateDirectory(toUri))
        {
            Array<String> files = ioServer->ListFiles(fromUri, "*");
            IndexT i;
            for (i = 0; i < files.Size(); i++)
            {
                String fromPath = from + "/" + files[i];
                String toPath = to + "/" + files[i];
                ioServer->CopyFile(URI(fromPath), URI(toPath));
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
RCSWrapper::Update(Logger& logger)
{
    IoServer* ioServer = IoServer::Instance();

    // write a SVN version file which switches the "use-commit-times" on
    this->WriteTempSVNConfigFiles();

    // first run the commit-timestamp update, this will set the 
    // timestamp of updated files to the commit time
    IndexT i;
    for (i = 0; i < this->commitTimeStampDirs.Size(); i++)
    {
        if (!this->UpdateDirectory(logger, this->commitTimeStampDirs[i], true))
        {
            return false;
        }
    }

    // then run the update-timestamp updates, this will set the
    // timestamp of updated files to the update time
    for (i = 0; i < this->updateTimeStampDirs.Size(); i++)
    {
        if (!this->UpdateDirectory(logger, this->updateTimeStampDirs[i], false))
        {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
RCSWrapper::UpdateDirectory(Logger& logger, const String& directory, bool commitTimeStampMode)
{
    IoServer* ioServer = IoServer::Instance();
    logger.Print("Updating %s...\n", directory.AsCharPtr());

    // check if the directory is under SVN control
    if (!ioServer->DirectoryExists(directory + "/.svn"))
    {
        logger.Error("Directory not under SVN or CVS control: %s\n", directory.AsCharPtr());
        return false;
    }

    // make sure the Subversion command line tool has been set and exists
    if (!this->svnToolLocation.IsValid())
    {
        logger.Error("SVN command line tool has not been set!\n");
        return false;
    }
    if (!ioServer->FileExists(this->svnToolLocation))
    {
        logger.Error("SVN command line tool not found: '%s'!\n", this->svnToolLocation.AsCharPtr());
        return false;
    }

    // launch SVN command line tool
    AppLauncher appLauncher;
    appLauncher.SetWorkingDirectory(directory);
    String svnArgs;
    if (commitTimeStampMode)
    {
        String resolvedConfigDir = AssignRegistry::Instance()->ResolveAssigns("temp:synctool3").LocalPath();
        svnArgs.Format("update --config-dir %s --non-interactive", resolvedConfigDir.AsCharPtr());
    }
    else
    {
        svnArgs = "update --non-interactive";
    }
    appLauncher.SetExecutable(this->svnToolLocation);
    appLauncher.SetArguments(svnArgs);

    logger.Print(("RCSWrapper::UpdateDirectory: " + this->svnToolLocation + " " + svnArgs + "\n").AsCharPtr());

    if (!appLauncher.LaunchWait())
    {
        logger.Error("Failed to launch SVN tool!\n");
        return false;
    }
    return true;;
}

} // namespace ToolkitUtil