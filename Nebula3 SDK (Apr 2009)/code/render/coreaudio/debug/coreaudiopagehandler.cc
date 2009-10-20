//------------------------------------------------------------------------------
//  coreaudiopagehandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreaudio/debug/coreaudiopagehandler.h"
#include "coreaudio/audiodevice.h"

namespace Debug
{
__ImplementClass(Debug::CoreAudioPageHandler, 'CAPH', Http::HttpRequestHandler);

using namespace Util;
using namespace Http;
using namespace IO;
using namespace Resources;
using namespace CoreAudio;

//------------------------------------------------------------------------------
/**
*/
CoreAudioPageHandler::CoreAudioPageHandler()
{
    this->SetName("CoreAudio");
    this->SetDesc("show information about CoreAudio subsystem");
    this->SetRootLocation("coreaudio");
}

//------------------------------------------------------------------------------
/**
*/
void
CoreAudioPageHandler::HandleRequest(const Ptr<HttpRequest>& request)
{
    n_assert(HttpMethod::Get == request->GetMethod());

    // check if a soundbank data is requested
    Dictionary<String,String> query = request->GetURI().ParseQuery();
    if (query.Contains("soundbankinfo"))
    {
        request->SetStatus(this->HandleSoundBankInfoRequest(ResourceId(query["soundbankinfo"]), request->GetResponseContentStream()));
        return;
    }

    // build HTML page
    Ptr<HtmlPageWriter> htmlWriter = HtmlPageWriter::Create();
    htmlWriter->SetStream(request->GetResponseContentStream());
    htmlWriter->SetTitle("Nebula3 CoreAudio Subsystem");
    if (htmlWriter->Open())
    {
        htmlWriter->Element(HtmlElement::Heading1, "CoreAudio Subsystem");
        htmlWriter->AddAttr("href", "/index.html");
        htmlWriter->Element(HtmlElement::Anchor, "Home");

        AudioDevice* audioDevice = AudioDevice::Instance();

        SizeT numAdapters = audioDevice->GetNumAdapters();
        if (0 == numAdapters)
        {
            // no audio adapters in the system...
            htmlWriter->Element(HtmlElement::Heading3, "NO AUDIO ADAPTERS FOUND!");
        }
        else
        {
            // list audio adapters
            htmlWriter->Element(HtmlElement::Heading3, "Audio Adapters");
            htmlWriter->AddAttr("border", "1");
            htmlWriter->AddAttr("rules", "cols");
            htmlWriter->Begin(HtmlElement::Table);
                htmlWriter->AddAttr("bgcolor", "lightsteelblue");
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableHeader, "Adapter ID");
                    htmlWriter->Element(HtmlElement::TableHeader, "Name");
                    htmlWriter->Element(HtmlElement::TableHeader, "Default");
                htmlWriter->End(HtmlElement::TableRow);

                IndexT adapterIndex;
                for (adapterIndex = 0; adapterIndex < numAdapters; adapterIndex++)
                {
                    AudioAdapterInfo adapterInfo = audioDevice->GetAdapterInfo(adapterIndex);
                    htmlWriter->Begin(HtmlElement::TableRow);
                        htmlWriter->Element(HtmlElement::TableData, adapterInfo.GetAdapterId());
                        htmlWriter->Element(HtmlElement::TableData, adapterInfo.GetDisplayName());
                        htmlWriter->Element(HtmlElement::TableData, adapterInfo.IsDefaultAdapter() ? "yes" : "no");
                    htmlWriter->End(HtmlElement::TableRow);
                }
            htmlWriter->End(HtmlElement::Table);

            // list sound banks
            htmlWriter->Element(HtmlElement::Heading3, "Sound Banks");
            htmlWriter->AddAttr("border", "1");
            htmlWriter->AddAttr("rules", "cols");
            htmlWriter->Begin(HtmlElement::Table);
                htmlWriter->AddAttr("bgcolor", "lightsteelblue");
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableHeader, "Name");
                    htmlWriter->Element(HtmlElement::TableHeader, "Streaming");
                    htmlWriter->Element(HtmlElement::TableHeader, "NumCues");
                    htmlWriter->Element(HtmlElement::TableHeader, "NumWaves");
                htmlWriter->End(HtmlElement::TableRow);                

                IndexT soundBankIndex;
                SizeT numSoundBanks = audioDevice->GetNumSoundBanks();
                for (soundBankIndex = 0; soundBankIndex < numSoundBanks; soundBankIndex++)
                {
                    const Ptr<SoundBank>& soundBank = audioDevice->GetSoundBankByIndex(soundBankIndex);
                    htmlWriter->Begin(HtmlElement::TableRow);
                        htmlWriter->Begin(HtmlElement::TableData);
                            htmlWriter->AddAttr("href", "/coreaudio?soundbankinfo=" + soundBank->GetResourceId().Value());
                            htmlWriter->Element(HtmlElement::Anchor, soundBank->GetResourceId().Value());
                        htmlWriter->End(HtmlElement::TableData);
                        htmlWriter->Element(HtmlElement::TableData, soundBank->IsStreaming() ? "yes" : "no");
                        htmlWriter->Element(HtmlElement::TableData, String::FromInt(soundBank->GetNumCues()));
                        htmlWriter->Element(HtmlElement::TableData, String::FromInt(soundBank->GetNumWaves()));
                    htmlWriter->End(HtmlElement::TableRow);
                }
            htmlWriter->End(HtmlElement::Table);

            // display current listener position and velocity
            htmlWriter->Element(HtmlElement::Heading3, "Audio Listener");
            htmlWriter->Begin(HtmlElement::Table);
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableData, "Listener Position:");
                    htmlWriter->Element(HtmlElement::TableData, String::FromFloat4(audioDevice->GetListenerTransform().get_position()));
                htmlWriter->End(HtmlElement::TableRow);
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableData, "Listener Velocity:");
                    htmlWriter->Element(HtmlElement::TableData, String::FromFloat4(audioDevice->GetListenerVelocity()));
                htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->End(HtmlElement::Table);

            // display active cues
            htmlWriter->Element(HtmlElement::Heading3, "Active Cues");
            htmlWriter->AddAttr("border", "1");
            htmlWriter->AddAttr("rules", "cols");
            htmlWriter->Begin(HtmlElement::Table);
                htmlWriter->AddAttr("bgcolor", "lightsteelblue");
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableHeader, "Cue Name");
                    htmlWriter->Element(HtmlElement::TableHeader, "SoundBank");
                    htmlWriter->Element(HtmlElement::TableHeader, "State");
                    htmlWriter->Element(HtmlElement::TableHeader, "3D");
                    htmlWriter->Element(HtmlElement::TableHeader, "Position");
                    htmlWriter->Element(HtmlElement::TableHeader, "Velocity");
                htmlWriter->End(HtmlElement::TableRow);                

                const Array<Ptr<Cue>>& cues = audioDevice->GetActiveCues();
                IndexT cueIndex;
                String cueStateString;
                for (cueIndex = 0; cueIndex < cues.Size(); cueIndex++)
                {
                    htmlWriter->Begin(HtmlElement::TableRow);
                        htmlWriter->Element(HtmlElement::TableData, cues[cueIndex]->GetCueInfo().GetCueId().Value());
                        htmlWriter->Element(HtmlElement::TableData, cues[cueIndex]->GetSoundBank()->GetResourceId().Value());
                        cueStateString.Format("%04x", cues[cueIndex]->GetState());
                        htmlWriter->Element(HtmlElement::TableData, cueStateString);
                        htmlWriter->Element(HtmlElement::TableData, cues[cueIndex]->Is3DEnabled() ? "yes" : "no");
                        htmlWriter->Element(HtmlElement::TableData, String::FromFloat4(cues[cueIndex]->GetTransform().get_position()));
                        htmlWriter->Element(HtmlElement::TableData, String::FromFloat4(cues[cueIndex]->GetVelocity()));
                    htmlWriter->End(HtmlElement::TableRow);
                }
            htmlWriter->End(HtmlElement::Table);
        }
        htmlWriter->Close();
        request->SetStatus(HttpStatus::OK);
    }
    else
    {
        request->SetStatus(HttpStatus::InternalServerError);
    }
}

//------------------------------------------------------------------------------
/**
*/
HttpStatus::Code
CoreAudioPageHandler::HandleSoundBankInfoRequest(const ResourceId& resId, const Ptr<Stream>& responseStream)
{
    AudioDevice* audioDevice = AudioDevice::Instance();
    if (!audioDevice->HasSoundBank(resId))
    {
        return HttpStatus::NotFound;
    }
    const Ptr<SoundBank>& soundBank = audioDevice->GetSoundBankByName(resId);

    Ptr<HtmlPageWriter> htmlWriter = HtmlPageWriter::Create();
    htmlWriter->SetStream(responseStream);
    htmlWriter->SetTitle("Nebula3 SoundBank Info");
    if (htmlWriter->Open())
    {
        htmlWriter->Element(HtmlElement::Heading1, resId.Value());
        htmlWriter->AddAttr("href", "/index.html");
        htmlWriter->Element(HtmlElement::Anchor, "Home");
        htmlWriter->LineBreak();
        htmlWriter->AddAttr("href", "/coreaudio");
        htmlWriter->Element(HtmlElement::Anchor, "CoreAudio Home");
        htmlWriter->LineBreak();
        htmlWriter->LineBreak();

        // display some info about the sound bank
        htmlWriter->Begin(HtmlElement::Table);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Resource Id: ");
                htmlWriter->Element(HtmlElement::TableData, soundBank->GetResourceId().Value());
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Streaming: ");
                htmlWriter->Element(HtmlElement::TableData, soundBank->IsStreaming() ? "yes" : "no");
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Number of Cues: ");
                htmlWriter->Element(HtmlElement::TableData, String::FromInt(soundBank->GetNumCues()));
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Number of Waves: ");
                htmlWriter->Element(HtmlElement::TableData, String::FromInt(soundBank->GetNumWaves()));
            htmlWriter->End(HtmlElement::TableRow);
        htmlWriter->End(HtmlElement::Table);

        // show cue infos
        htmlWriter->Element(HtmlElement::Heading3, "Cues");
        htmlWriter->AddAttr("border", "1");
        htmlWriter->AddAttr("rules", "cols");
        htmlWriter->Begin(HtmlElement::Table);
            htmlWriter->AddAttr("bgcolor", "lightsteelblue");
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableHeader, "CueId");
                htmlWriter->Element(HtmlElement::TableHeader, "Interactive");
                htmlWriter->Element(HtmlElement::TableHeader, "NumVariations");
                htmlWriter->Element(HtmlElement::TableHeader, "MaxInstances");
            htmlWriter->End(HtmlElement::TableRow);                

            SizeT numCues = soundBank->GetNumCues();
            IndexT cueIndex;
            for (cueIndex = 0; cueIndex < numCues; cueIndex++)
            {
                const CueInfo& cueInfo = soundBank->GetCueInfo(cueIndex);
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableData, cueInfo.GetCueId().Value());
                    htmlWriter->Element(HtmlElement::TableData, cueInfo.IsInteractive() ? "yes" : "no");
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(cueInfo.GetNumVariations()));
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(cueInfo.GetMaxNumInstances()));
                htmlWriter->End(HtmlElement::TableRow);
            }
        htmlWriter->End(HtmlElement::Table);

        // show wave infos
        htmlWriter->Element(HtmlElement::Heading3, "Waves");
        htmlWriter->AddAttr("border", "1");
        htmlWriter->AddAttr("rules", "cols");
        htmlWriter->Begin(HtmlElement::Table);
            htmlWriter->AddAttr("bgcolor", "lightsteelblue");
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableHeader, "WaveId");
                htmlWriter->Element(HtmlElement::TableHeader, "Format");
                htmlWriter->Element(HtmlElement::TableHeader, "BitsPerSample");
                htmlWriter->Element(HtmlElement::TableHeader, "SamplesPerSec");
                htmlWriter->Element(HtmlElement::TableHeader, "NumChannels");
            htmlWriter->End(HtmlElement::TableRow);

            SizeT numWaves = soundBank->GetNumWaves();
            IndexT waveIndex;
            for (waveIndex = 0; waveIndex < numWaves; waveIndex++)
            {                
                const WaveInfo& waveInfo = soundBank->GetWaveInfo(waveIndex);
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableData, waveInfo.GetWaveId().Value());
                    htmlWriter->Element(HtmlElement::TableData, WaveInfo::FormatToString(waveInfo.GetFormat()));
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(waveInfo.GetBitsPerSample()));
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(waveInfo.GetSamplesPerSecond()));
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(waveInfo.GetNumChannels()));
                htmlWriter->End(HtmlElement::TableRow);
            }
        htmlWriter->End(HtmlElement::Table);
        htmlWriter->Close();
        return HttpStatus::OK;        
    }
    return HttpStatus::InternalServerError;
}

} // namespace Debug
