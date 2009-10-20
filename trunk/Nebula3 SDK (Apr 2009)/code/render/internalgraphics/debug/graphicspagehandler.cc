//------------------------------------------------------------------------------
//  graphicspagehandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/debug/graphicspagehandler.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "internalgraphics/internalstage.h"
#include "internalgraphics/internalview.h"
#include "characters/character.h"

namespace Debug
{
__ImplementClass(Debug::GraphicsPageHandler, 'GPHD', Http::HttpRequestHandler);

using namespace Util;
using namespace Http;
using namespace Math;
using namespace Resources;
using namespace InternalGraphics;
using namespace Lighting;
using namespace Characters;
using namespace CoreAnimation;

//------------------------------------------------------------------------------
/**
*/
GraphicsPageHandler::GraphicsPageHandler()
{
    this->SetName("Graphics");
    this->SetDesc("show graphics subsystem internals");
    this->SetRootLocation("graphics");
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsPageHandler::HandleRequest(const Ptr<HttpRequest>& request)
{
    n_assert(HttpMethod::Get == request->GetMethod());
    InternalGraphicsServer* server = InternalGraphicsServer::Instance();
    
    // check if a command has been defined in the URI
    Dictionary<String,String> query = request->GetURI().ParseQuery();
    if (query.Contains("stage"))
    {
        request->SetStatus(this->WriteStageInfo(request, ResourceId(query["stage"])));
        return;
    }
    else if (query.Contains("view"))
    {
        request->SetStatus(this->WriteViewInfo(request, ResourceId(query["view"])));
        return;
    }
    else if (query.Contains("entity"))
    {
        request->SetStatus(this->WriteEntityInfo(request, query["entity"].AsInt()));
        return;
    }
    else if (query.Contains("playAnim"))
    {
        request->SetStatus(this->PlayAnim(request, query["playAnim"]));
        return;
    }
    else if (query.Contains("toggleSkin"))
    {
        request->SetStatus(this->ToggleSkin(request, query["toggleSkin"]));
        return;
    }
    else if (query.Contains("applySkinList"))
    {
        request->SetStatus(this->ApplySkinList(request, query["applySkinList"]));
        return;
    }

    // configure a HTML page writer
    Ptr<HtmlPageWriter> htmlWriter = HtmlPageWriter::Create();
    htmlWriter->SetStream(request->GetResponseContentStream());
    htmlWriter->SetTitle("Nebula3 Graphics Subsystem");
    if (htmlWriter->Open())
    {
        htmlWriter->Element(HtmlElement::Heading1, "Graphics Subsystem");
        htmlWriter->AddAttr("href", "/index.html");
        htmlWriter->Element(HtmlElement::Anchor, "Home");

        // create a table of all stages
        htmlWriter->Element(HtmlElement::Heading3, "Stages");
        htmlWriter->AddAttr("border", "1");
        htmlWriter->AddAttr("rules", "cols");
        htmlWriter->Begin(HtmlElement::Table);
            htmlWriter->AddAttr("bgcolor", "lightsteelblue");
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableHeader, "Name");
                htmlWriter->Element(HtmlElement::TableHeader, "Entities (All)");
                htmlWriter->Element(HtmlElement::TableHeader, "Models");
                htmlWriter->Element(HtmlElement::TableHeader, "Lights");
                htmlWriter->Element(HtmlElement::TableHeader, "Cameras");
            htmlWriter->End(HtmlElement::TableRow);

            const Array<Ptr<InternalStage> >& stages = server->GetStages();
            IndexT stageIndex;
            for (stageIndex = 0; stageIndex < stages.Size(); stageIndex++)
            {
                const Ptr<InternalStage>& stage = stages[stageIndex];
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Begin(HtmlElement::TableData);
                        htmlWriter->AddAttr("href", "/graphics?stage=" + stage->GetName().Value());
                        htmlWriter->Element(HtmlElement::Anchor, stage->GetName().Value());
                    htmlWriter->End(HtmlElement::TableData);
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(stage->GetEntities().Size()));
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(stage->GetEntitiesByType(InternalGraphicsEntityType::Model).Size()));
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(stage->GetEntitiesByType(InternalGraphicsEntityType::Light).Size()));
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(stage->GetEntitiesByType(InternalGraphicsEntityType::Camera).Size()));
                htmlWriter->End(HtmlElement::TableRow);
            }
        htmlWriter->End(HtmlElement::Table);

        // create a table of all views
        htmlWriter->Element(HtmlElement::Heading3, "Views");
        htmlWriter->AddAttr("border", "1");
        htmlWriter->AddAttr("rules", "cols");
        htmlWriter->Begin(HtmlElement::Table);
            htmlWriter->AddAttr("bgcolor", "lightsteelblue");
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableHeader, "Name");
                htmlWriter->Element(HtmlElement::TableHeader, "Stage");
                htmlWriter->Element(HtmlElement::TableHeader, "RenderTarget");
                htmlWriter->Element(HtmlElement::TableHeader, "FrameShader");
            htmlWriter->End(HtmlElement::TableRow);

            const Array<Ptr<InternalView> >& views = server->GetViews();
            IndexT viewIndex;
            for (viewIndex = 0; viewIndex < views.Size(); viewIndex++)
            {
                const Ptr<InternalView>& view = views[viewIndex];
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Begin(HtmlElement::TableData);
                        htmlWriter->AddAttr("href", "/graphics?view=" + view->GetName().Value());
                        htmlWriter->Element(HtmlElement::Anchor, view->GetName().Value());
                    htmlWriter->End(HtmlElement::TableData);
                    htmlWriter->Begin(HtmlElement::TableData);
                        htmlWriter->AddAttr("href", "/graphics?stage=" + view->GetStage()->GetName().Value());
                        htmlWriter->Element(HtmlElement::Anchor, view->GetStage()->GetName().Value());
                    htmlWriter->End(HtmlElement::TableData);
                    htmlWriter->Begin(HtmlElement::TableData);                        
                        if (view->GetRenderTarget().isvalid())
                        {
                            const ResourceId& rtResId = view->GetRenderTarget()->GetResolveTexture()->GetResourceId();
                            htmlWriter->AddAttr("href", "/texture?texinfo=" + rtResId.Value());
                            htmlWriter->Element(HtmlElement::Anchor, rtResId.Value());
                        }
                        else
                        {
                            htmlWriter->AddAttr("href", "/display");
                            htmlWriter->Element(HtmlElement::Anchor, "default");
                        }
                    htmlWriter->End(HtmlElement::TableData);
                    htmlWriter->Element(HtmlElement::TableData, view->GetFrameShader()->GetName().Value());
                htmlWriter->End(HtmlElement::TableRow);
            }
        htmlWriter->End(HtmlElement::Table);

        // create table of all entities
        htmlWriter->Element(HtmlElement::Heading3, "Entities");
        const Array<Ptr<InternalGraphicsEntity> > entities = server->GetEntities();
        this->WriteEntityTable(entities, htmlWriter);

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
    Write a table with InternalGraphicsEntities.
*/
void
GraphicsPageHandler::WriteEntityTable(const Array<Ptr<InternalGraphicsEntity> >& entities, const Ptr<HtmlPageWriter>& htmlWriter)
{
    htmlWriter->AddAttr("border", "1");
    htmlWriter->AddAttr("rules", "cols");
    htmlWriter->Begin(HtmlElement::Table);
        htmlWriter->AddAttr("bgcolor", "lightsteelblue");
        htmlWriter->Begin(HtmlElement::TableRow);                
            htmlWriter->Element(HtmlElement::TableHeader, "Id");
            htmlWriter->Element(HtmlElement::TableHeader, "Type");
            htmlWriter->Element(HtmlElement::TableHeader, "ResId");
            htmlWriter->Element(HtmlElement::TableHeader, "Visible");
            htmlWriter->Element(HtmlElement::TableHeader, "Stage");
            htmlWriter->Element(HtmlElement::TableHeader, "Pos");
            htmlWriter->Element(HtmlElement::TableHeader, "Size");
        htmlWriter->End(HtmlElement::TableRow);

        IndexT entityIndex;
        for (entityIndex = 0; entityIndex < entities.Size(); entityIndex++)
        {
            const Ptr<InternalGraphicsEntity>& entity = entities[entityIndex];
            InternalGraphicsEntityType::Code type = entity->GetType();     
            const point& pos = entity->GetTransform().get_position();
            const vector& size = entity->GetGlobalBoundingBox().size();

            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Begin(HtmlElement::TableData);
                    htmlWriter->AddAttr("href", "/graphics?entity=" + String::FromInt(entity->GetId()));
                    htmlWriter->Element(HtmlElement::Anchor, String::FromInt(entity->GetId()));
                htmlWriter->End(HtmlElement::TableData);                    
                if (InternalGraphicsEntityType::Model == type)
                {
                    const Ptr<InternalModelEntity>& modelEntity = entity.downcast<InternalModelEntity>();
                    if (modelEntity->HasCharacter())
                    {
                        htmlWriter->Element(HtmlElement::TableData, "Model (Char)");
                    }
                    else
                    {
                        htmlWriter->Element(HtmlElement::TableData, "Model");
                    }
                    htmlWriter->Element(HtmlElement::TableData, modelEntity->GetResourceId().Value());
                }
                else if (InternalGraphicsEntityType::Camera == type)
                {
                    htmlWriter->Element(HtmlElement::TableData, "Camera");
                    htmlWriter->Element(HtmlElement::TableData, "-");
                }
                else if (InternalGraphicsEntityType::Light == type)
                {
                    htmlWriter->Element(HtmlElement::TableData, "Light");
                    htmlWriter->Element(HtmlElement::TableData, "-");
                }
                else
                {
                    htmlWriter->Element(HtmlElement::TableData, "UNKNOWN");
                    htmlWriter->Element(HtmlElement::TableData, "-");
                }
                htmlWriter->Element(HtmlElement::TableData, String::FromBool(entity->IsVisible()));
                if (entity->IsAttachedToStage())
                {
                    htmlWriter->Begin(HtmlElement::TableData);
                        htmlWriter->AddAttr("href", "/graphics?stage=" + entity->GetStage()->GetName().Value());
                        htmlWriter->Element(HtmlElement::Anchor, entity->GetStage()->GetName().Value());
                    htmlWriter->End(HtmlElement::TableData);
                }
                else
                {
                    htmlWriter->Element(HtmlElement::TableData, "-");
                }
                String posString, sizeString;
                posString.Format("%.3f,%.3f,%.3f", pos.x(), pos.y(), pos.z());
                sizeString.Format("%.3f,%.3f,%.3f", size.x(), size.y(), size.z());
                htmlWriter->Element(HtmlElement::TableData, posString);
                htmlWriter->Element(HtmlElement::TableData, sizeString);
            htmlWriter->End(HtmlElement::TableRow);
        }
    htmlWriter->End(HtmlElement::Table);
}

//------------------------------------------------------------------------------
/**
*/
HttpStatus::Code
GraphicsPageHandler::WriteStageInfo(const Ptr<HttpRequest>& request, const ResourceId& stageName)
{
    InternalGraphicsServer* server = InternalGraphicsServer::Instance();
    if (server->HasStage(stageName))
    {
        Ptr<HtmlPageWriter> htmlWriter = HtmlPageWriter::Create();
        htmlWriter->SetStream(request->GetResponseContentStream());
        htmlWriter->SetTitle("Graphics Stage Info");
        if (htmlWriter->Open())
        {
            htmlWriter->Element(HtmlElement::Heading1, "Graphics Stage: " + stageName.Value());
            htmlWriter->AddAttr("href", "/index.html");
            htmlWriter->Element(HtmlElement::Anchor, "Home");
            htmlWriter->LineBreak();
            htmlWriter->AddAttr("href", "/graphics");
            htmlWriter->Element(HtmlElement::Anchor, "Graphics Home");
            htmlWriter->LineBreak();

            const Ptr<InternalStage>& stage = server->GetStageByName(stageName);
            const Array<Ptr<InternalGraphicsEntity> >& entities = stage->GetEntities();
            this->WriteEntityTable(entities, htmlWriter);

            htmlWriter->Close();
            return HttpStatus::OK;            
        }
        else
        {
            return HttpStatus::InternalServerError;
        }
    }
    else
    {
        return HttpStatus::NotFound;
    }
}

//------------------------------------------------------------------------------
/**
*/
HttpStatus::Code
GraphicsPageHandler::WriteViewInfo(const Ptr<HttpRequest>& request, const ResourceId& viewName)
{
    // @todo: implement GraphicsPageHandler::WriteViewInfo
    return HttpStatus::NotFound;
}

//------------------------------------------------------------------------------
/**
*/
HttpStatus::Code
GraphicsPageHandler::WriteEntityInfo(const Ptr<HttpRequest>& request, InternalGraphicsEntity::Id entityId)
{
    InternalGraphicsServer* server = InternalGraphicsServer::Instance();
    if (server->HasEntity(entityId))
    {
        const Ptr<InternalGraphicsEntity>& entity = server->GetEntityById(entityId);
        Ptr<HtmlPageWriter> htmlWriter = HtmlPageWriter::Create();
        htmlWriter->SetStream(request->GetResponseContentStream());
        htmlWriter->SetTitle("Graphics Entity Info");
        if (htmlWriter->Open())
        {
            htmlWriter->Element(HtmlElement::Heading1, "Graphics Entity Info");
            htmlWriter->AddAttr("href", "/index.html");
            htmlWriter->Element(HtmlElement::Anchor, "Home");
            htmlWriter->LineBreak();
            htmlWriter->AddAttr("href", "/graphics");
            htmlWriter->Element(HtmlElement::Anchor, "Graphics Home");
            htmlWriter->LineBreak();

            htmlWriter->Element(HtmlElement::Heading3, "Generic Info:");
            htmlWriter->Begin(HtmlElement::Table);
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableData, "Entity Id: ");
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(entityId));
                htmlWriter->End(HtmlElement::TableRow);
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableData, "Active: ");
                    htmlWriter->Element(HtmlElement::TableData, entity->IsActive() ? "yes" : "no");
                htmlWriter->End(HtmlElement::TableRow);
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableData, "Valid: ");
                    htmlWriter->Element(HtmlElement::TableData, entity->IsValid() ? "yes" : "no");
                htmlWriter->End(HtmlElement::TableRow);
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableData, "Visible: ");
                    htmlWriter->Element(HtmlElement::TableData, entity->IsVisible() ? "yes" : "no");
                htmlWriter->End(HtmlElement::TableRow);
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableData, "Position: ");
                    htmlWriter->Element(HtmlElement::TableData, String::FromFloat4(entity->GetTransform().get_position()));
                htmlWriter->End(HtmlElement::TableRow);
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableData, "Size: ");
                    htmlWriter->Element(HtmlElement::TableData, String::FromFloat4(entity->GetGlobalBoundingBox().size()));
                htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->End(HtmlElement::Table);

            if (InternalGraphicsEntityType::Model == entity->GetType())
            {
                htmlWriter->Element(HtmlElement::Heading3, "Model Entity Info:");
                this->WriteModelEntityInfo(entity.downcast<InternalModelEntity>(), htmlWriter);
            }
            else if (InternalGraphicsEntityType::Light == entity->GetType())
            {
                htmlWriter->Element(HtmlElement::Heading3, "Light Entity Info:");
                this->WriteLightEntityInfo(entity.downcast<InternalAbstractLightEntity>(), htmlWriter);
            }
            else if (InternalGraphicsEntityType::Camera == entity->GetType())
            {
                htmlWriter->Element(HtmlElement::Heading3, "Camera Entity Info:");
                this->WriteCameraEntityInfo(entity.downcast<InternalCameraEntity>(), htmlWriter);
            }                    
            htmlWriter->Close();
            return HttpStatus::OK;
        }
        else
        {
            return HttpStatus::InternalServerError;
        }
    }
    else
    {
        return HttpStatus::NotFound;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsPageHandler::WriteModelEntityInfo(const Ptr<InternalModelEntity>& modelEntity, const Ptr<HtmlPageWriter>& htmlWriter)
{
    htmlWriter->Begin(HtmlElement::Table);
        htmlWriter->Begin(HtmlElement::TableRow);
            htmlWriter->Element(HtmlElement::TableData, "Resource: ");
            htmlWriter->Element(HtmlElement::TableData, modelEntity->GetResourceId().Value());
        htmlWriter->End(HtmlElement::TableRow);
    htmlWriter->End(HtmlElement::Table);
    
    // write character specific info
    if (modelEntity->HasCharacter())
    {
        const Ptr<Character>& character = modelEntity->GetCharacter();

        htmlWriter->Element(HtmlElement::Heading3, "Character Instance");
        htmlWriter->AddAttr("border", "1");
        htmlWriter->AddAttr("rules", "cols");
        htmlWriter->Begin(HtmlElement::Table);
            htmlWriter->AddAttr("bgcolor", "lightsteelblue");
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableHeader, "Clips");
                htmlWriter->Element(HtmlElement::TableHeader, "Variations");
                htmlWriter->Element(HtmlElement::TableHeader, "Skins");
                htmlWriter->Element(HtmlElement::TableHeader, "Skin Lists");
            htmlWriter->End(HtmlElement::TableRow);

            htmlWriter->Begin(HtmlElement::TableRow);
                // write animations
                htmlWriter->AddAttr("valign", "top");
                htmlWriter->Begin(HtmlElement::TableData);
                    IndexT clipIndex;
                    const Ptr<AnimResource>& anim = character->AnimationLibrary().GetAnimResource();
                    for (clipIndex = 0; clipIndex < anim->GetNumClips(); clipIndex++)
                    {
                        const AnimClip& clip = anim->GetClipByIndex(clipIndex);
                        String playAnimLink;
                        playAnimLink.Format("/graphics?playAnim=%d,%s", modelEntity->GetId(), clip.GetName().Value().AsCharPtr());
                        htmlWriter->AddAttr("href", playAnimLink);
                        htmlWriter->Element(HtmlElement::Anchor, clip.GetName().Value());
                        htmlWriter->LineBreak();
                    }
                htmlWriter->End(HtmlElement::TableData);

                // FIXME: write variations
                htmlWriter->AddAttr("valign", "top");
                htmlWriter->Element(HtmlElement::TableData, "FIXME");
                
                // write skins
                htmlWriter->AddAttr("valign", "top");
                htmlWriter->Begin(HtmlElement::TableData);
                    IndexT skinIndex;
                    for (skinIndex = 0; skinIndex < character->SkinLibrary().GetNumSkins(); skinIndex++)
                    {
                        const CharacterSkin& skin = character->SkinLibrary().GetSkin(skinIndex);
                        String toggleSkinLink;
                        toggleSkinLink.Format("/graphics?toggleSkin=%d,%s", modelEntity->GetId(), skin.GetName().Value().AsCharPtr());
                        htmlWriter->AddAttr("href", toggleSkinLink);
                        htmlWriter->Element(HtmlElement::Anchor, skin.GetName().Value());
                        htmlWriter->LineBreak();
                    }
                htmlWriter->End(HtmlElement::TableData);

                // write skin lists
                htmlWriter->AddAttr("valign", "top");
                htmlWriter->Begin(HtmlElement::TableData);
                    IndexT skinListIndex;
                    for (skinListIndex = 0; skinListIndex < character->SkinLibrary().GetNumSkinLists(); skinListIndex++)
                    {
                        const CharacterSkinList& skinList = character->SkinLibrary().GetSkinList(skinListIndex);
                        String applySkinListLink;
                        applySkinListLink.Format("/graphics?applySkinList=%d,%s", modelEntity->GetId(), skinList.GetName().Value().AsCharPtr());
                        htmlWriter->AddAttr("href", applySkinListLink);
                        htmlWriter->Element(HtmlElement::Anchor, skinList.GetName().Value());
                        htmlWriter->LineBreak();
                    }
                htmlWriter->End(HtmlElement::TableData);
            htmlWriter->End(HtmlElement::TableRow);
        htmlWriter->End(HtmlElement::Table);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsPageHandler::WriteLightEntityInfo(const Ptr<InternalAbstractLightEntity>& lightEntity, const Ptr<HtmlPageWriter>& htmlWriter)
{
    const matrix44& m = lightEntity->GetTransform();
    float xSize = m.get_xaxis().length();
    float ySize = m.get_yaxis().length();
    float zSize = m.get_zaxis().length();
    vector size(xSize, ySize, zSize);

    htmlWriter->Begin(HtmlElement::Table);
        htmlWriter->Begin(HtmlElement::TableRow);
            htmlWriter->Element(HtmlElement::TableData, "Type: ");
            htmlWriter->Element(HtmlElement::TableData, LightType::ToString(lightEntity->GetLightType()));
        htmlWriter->End(HtmlElement::TableRow);
        htmlWriter->Begin(HtmlElement::TableRow);
            htmlWriter->Element(HtmlElement::TableData, "Position: ");
            htmlWriter->Element(HtmlElement::TableData, String::FromFloat4(m.get_position()));
        htmlWriter->End(HtmlElement::TableRow);
        htmlWriter->Begin(HtmlElement::TableRow);
            htmlWriter->Element(HtmlElement::TableData, "Size: ");
            htmlWriter->Element(HtmlElement::TableData, String::FromFloat4(size));
        htmlWriter->End(HtmlElement::TableRow);
        htmlWriter->Begin(HtmlElement::TableRow);
            htmlWriter->Element(HtmlElement::TableData, "Direction: ");
            htmlWriter->Element(HtmlElement::TableData, String::FromFloat4(m.get_zaxis()));
        htmlWriter->End(HtmlElement::TableRow);
        htmlWriter->Begin(HtmlElement::TableRow);
            htmlWriter->Element(HtmlElement::TableData, "Color: ");
            htmlWriter->Element(HtmlElement::TableData, String::FromFloat4(lightEntity->GetColor()));             
        htmlWriter->End(HtmlElement::TableRow);
        htmlWriter->Begin(HtmlElement::TableRow);
            htmlWriter->Element(HtmlElement::TableData, "Cast Shadows: ");
            htmlWriter->Element(HtmlElement::TableData, String::FromBool(lightEntity->GetCastShadows()));
        htmlWriter->End(HtmlElement::TableRow);
    htmlWriter->End(HtmlElement::Table);
}

//------------------------------------------------------------------------------
/**
*/
void
GraphicsPageHandler::WriteCameraEntityInfo(const Ptr<InternalCameraEntity>& cameraEntity, const Ptr<HtmlPageWriter>& htmlWriter)
{
    // FIXME!
}

//------------------------------------------------------------------------------
/**
*/
HttpStatus::Code
GraphicsPageHandler::PlayAnim(const Ptr<HttpRequest>& request, const String& argString)
{
    InternalGraphicsServer* server = InternalGraphicsServer::Instance();

    // parse arg string, must be "entityId,clipIndex"
    Array<String> tokens = argString.Tokenize(",");
    if (tokens.Size() != 2)
    {
        return HttpStatus::BadRequest;
    }
    InternalGraphicsEntity::Id entityId = tokens[0].AsInt();
    const StringAtom& clipName = tokens[1];
    if (!server->HasEntity(entityId))
    {
        return HttpStatus::NotFound;
    }
    
    // lookup entity and make sure it has a character
    const Ptr<InternalGraphicsEntity>& entity = server->GetEntityById(entityId);
    n_assert(entity.isvalid());
    if (entity->GetType() != InternalGraphicsEntityType::Model)
    {
        return HttpStatus::BadRequest;
    }
    const Ptr<InternalModelEntity>& modelEntity = entity.downcast<InternalModelEntity>();
    if (!modelEntity->HasCharacter())
    {
        return HttpStatus::NotFound;
    }

    // check if the animation exists...
    if (!modelEntity->GetCharacter()->AnimationLibrary().GetAnimResource()->HasClip(clipName))
    {
        return HttpStatus::NotFound;
    }

    // finally... start the clip!    
    modelEntity->GetCharacterInstance()->AnimController().StopAll();
    modelEntity->GetCharacterInstance()->AnimController().PlayLoop(clipName,    // clipName
                                                                   StringAtom("default"),   // category
                                                                   0,           // loopCount (infinity)
                                                                   0,           // start time
                                                                   200,         // fadeInTime
                                                                   200,         // fadeOutTime
                                                                   0,           // sampleTimeOffset,
                                                                   0,           // blendPriority
                                                                   1.0f,        // blendWeight
                                                                   1.0f);       // timefactor

    // finally we need to write the entity page again
    return this->WriteEntityInfo(request, entityId);
}

//------------------------------------------------------------------------------
/**
*/
HttpStatus::Code
GraphicsPageHandler::ToggleSkin(const Ptr<HttpRequest>& request, const String& argString)
{
    InternalGraphicsServer* server = InternalGraphicsServer::Instance();

    // parse arg string, must be "entityId,skinName"
    Array<String> tokens = argString.Tokenize(",");
    if (tokens.Size() != 2)
    {
        return HttpStatus::BadRequest;
    }
    InternalGraphicsEntity::Id entityId = tokens[0].AsInt();
    const StringAtom& skinName = tokens[1];
    if (!server->HasEntity(entityId))
    {
        return HttpStatus::NotFound;
    }
    
    // lookup entity and make sure it has a character
    const Ptr<InternalGraphicsEntity>& entity = server->GetEntityById(entityId);
    n_assert(entity.isvalid());
    if (entity->GetType() != InternalGraphicsEntityType::Model)
    {
        return HttpStatus::BadRequest;
    }
    const Ptr<InternalModelEntity>& modelEntity = entity.downcast<InternalModelEntity>();
    if (!modelEntity->HasCharacter())
    {
        return HttpStatus::NotFound;
    }

    // check if the skin exists...
    if (!modelEntity->GetCharacter()->SkinLibrary().HasSkin(skinName))
    {
        return HttpStatus::NotFound;
    }

    // finally toggle skin visibility
    modelEntity->GetCharacterInstance()->SkinSet().ToggleSkin(skinName);

    // finally we need to write the entity page again
    return this->WriteEntityInfo(request, entityId);
}

//------------------------------------------------------------------------------
/**
*/
HttpStatus::Code
GraphicsPageHandler::ApplySkinList(const Ptr<HttpRequest>& request, const String& argString)
{
    InternalGraphicsServer* server = InternalGraphicsServer::Instance();

    // parse arg string, must be "entityId,skinListName"
    Array<String> tokens = argString.Tokenize(",");
    if (tokens.Size() != 2)
    {
        return HttpStatus::BadRequest;
    }
    InternalGraphicsEntity::Id entityId = tokens[0].AsInt();
    const StringAtom& skinListName = tokens[1];
    if (!server->HasEntity(entityId))
    {
        return HttpStatus::NotFound;
    }
    
    // lookup entity and make sure it has a character
    const Ptr<InternalGraphicsEntity>& entity = server->GetEntityById(entityId);
    n_assert(entity.isvalid());
    if (entity->GetType() != InternalGraphicsEntityType::Model)
    {
        return HttpStatus::BadRequest;
    }
    const Ptr<InternalModelEntity>& modelEntity = entity.downcast<InternalModelEntity>();
    if (!modelEntity->HasCharacter())
    {
        return HttpStatus::NotFound;
    }

    // check if the skin exists...
    if (!modelEntity->GetCharacter()->SkinLibrary().HasSkinList(skinListName))
    {
        return HttpStatus::NotFound;
    }

    // finally toggle skin visibility
    modelEntity->GetCharacterInstance()->SkinSet().ApplySkinList(skinListName);

    // finally we need to write the entity page again
    return this->WriteEntityInfo(request, entityId);
}

} // namespace Characters