//------------------------------------------------------------------------------
//  objectinspectorhandler.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "basegamefeature/debug/objectinspectorhandler.h"
#include "http/html/htmlpagewriter.h"
#include "basegamefeature/basegamefeatureunit.h"
#include "managers/entitymanager.h"
#include "http/html/htmlpagewriter.h"

namespace Debug
{
__ImplementClass(Debug::ObjectInspectorHandler, 'OBIH', Http::HttpRequestHandler);

using namespace BaseGameFeature;
using namespace Http;
using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
ObjectInspectorHandler::ObjectInspectorHandler()
{
    this->SetName("Game Entities");
    this->SetDesc("show game entities debug information");
    this->SetRootLocation("objectinspector");
}

//------------------------------------------------------------------------------
/**
*/
void
ObjectInspectorHandler::HandleRequest(const Ptr<Http::HttpRequest>& request)
{
    n_assert(HttpMethod::Get == request->GetMethod());

    // configure a HTML page writer
    Ptr<HtmlPageWriter> htmlWriter = HtmlPageWriter::Create();
    htmlWriter->SetStream(request->GetResponseContentStream());
    htmlWriter->SetTitle("Nebula3 Game Entities Info");

    if (htmlWriter->Open())
    {
        // write page header
        htmlWriter->Element(HtmlElement::Heading1, "Game Entities");
        htmlWriter->AddAttr("href", "/index.html");
        htmlWriter->Element(HtmlElement::Anchor, "Home");

        // first check if a command has been defined in the URI
        Dictionary<String,String> query = request->GetURI().ParseQuery();

        // command ls, show entities of the category in the query
        if (query.Contains("ls"))
        {
            htmlWriter->LineBreak();
            htmlWriter->AddAttr("href", "/objectinspector");
            htmlWriter->Element(HtmlElement::Anchor, "Game Entities Home");
            this->WriteEntityInfo(query["ls"], htmlWriter);
            
            // close htmlWriter
            htmlWriter->Close();
            request->SetStatus(HttpStatus::OK);      
            return;
        }
        
        // no commands, view objectinspectors mainpage
        htmlWriter->Element(HtmlElement::Heading3, "Available Categories");
        
        // write table header
        htmlWriter->AddAttr("border", "1");
        htmlWriter->AddAttr("rules", "cols");
        htmlWriter->Begin(HtmlElement::Table);
            htmlWriter->AddAttr("bgcolor", "lightsteelblue");
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableHeader, "Category");
                htmlWriter->Element(HtmlElement::TableHeader, "Number of Entities");
            htmlWriter->End(HtmlElement::TableRow);
        
        // list entity categories and number of entities in the categories
        IndexT categoryIndex;
        for(categoryIndex = 0; categoryIndex < CategoryManager::Instance()->GetNumCategories(); categoryIndex++)
        {
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Begin(HtmlElement::TableData);
                    htmlWriter->AddAttr("href", "/objectinspector?ls=" + CategoryManager::Instance()->GetCategoryByIndex(categoryIndex).GetName());
                    htmlWriter->Element(HtmlElement::Anchor, CategoryManager::Instance()->GetCategoryByIndex(categoryIndex).GetName());
                htmlWriter->End(HtmlElement::TableData);

                htmlWriter->Begin(HtmlElement::TableData);

                    Util::String categoryName = CategoryManager::Instance()->GetCategoryByIndex(categoryIndex).GetName();
                    if (CategoryManager::Instance()->HasInstanceTable(categoryName))
                    {
                        Ptr<Db::ValueTable> instanceTable =  CategoryManager::Instance()->GetInstanceTable(categoryName);
                        htmlWriter->Element(HtmlElement::Anchor, Util::String::FromInt(instanceTable->GetNumRows()));
                    }
                    else
                    {
                        htmlWriter->Element(HtmlElement::Anchor, "0");
                    }
                    
                htmlWriter->End(HtmlElement::TableData);
            htmlWriter->End(HtmlElement::TableRow); 
        }
        htmlWriter->End(HtmlElement::Table);

        // close htmlWriter
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
void ObjectInspectorHandler::WriteEntityInfo( const Util::String &category, 
                                              const Ptr<Http::HtmlPageWriter>& htmlWriter )
{
    SizeT numEntities = EntityManager::Instance()->GetEntities().Size();
    SizeT entityIndex = 0;
    bool firstMatch = true;
    Ptr<Game::Entity> gameEntity;
    Ptr<Db::ValueTable> attrTable;
    SizeT numColumns = 0;
    SizeT colIndex = 0;
    SizeT numRows = 0;
    SizeT rowIndex = 0;
    Util::Blob blobValue;
    SizeT blobSize;
    Util::Guid guidValue;
    Attr::AttrId attrId;

    // html format
    htmlWriter->LineBreak();
    htmlWriter->LineBreak();
    htmlWriter->Text("Category Name: " + category);
    htmlWriter->LineBreak();
    htmlWriter->LineBreak();

    htmlWriter->AddAttr("border", "1");
    htmlWriter->AddAttr("rules", "all");

    // begin write table
    htmlWriter->Begin(HtmlElement::Table);

    // write table data
    // iterate all game entities
    for (entityIndex = 0; entityIndex<numEntities; entityIndex++)
    {
        // check if game entity has the right category
        if (EntityManager::Instance()->GetEntities()[entityIndex]->GetCategory() == category)
        {
            gameEntity = EntityManager::Instance()->GetEntities()[entityIndex];
            rowIndex = gameEntity->GetAttrTableRowIndex();
            attrTable = gameEntity->GetAttrTable();
            numColumns = attrTable->GetNumColumns();
            numRows = attrTable->GetNumRows();

            // when the first entity from the category is found we build the table header
            if (firstMatch == true)
            {
                firstMatch = false;
                // write table header
                htmlWriter->AddAttr("bgcolor", "lightsteelblue");
                htmlWriter->Begin(HtmlElement::TableRow);
                for (colIndex = 0; colIndex<numColumns; colIndex++)
                {
                    attrId = attrTable->GetColumnId( colIndex );
                    if (attrId.IsValid())
                    {
                        // write attribute name
                        htmlWriter->Element(HtmlElement::TableData, attrId.GetName() );                    
                    }
                }
                htmlWriter->End(HtmlElement::TableRow);
            }
            // write entity data table row
            htmlWriter->Begin(HtmlElement::TableRow);
            for (colIndex = 0; colIndex<numColumns; colIndex++)
            {
                attrId = attrTable->GetColumnId( colIndex );
                if (attrId.IsValid())
                {
                    Attr::ValueType valueType = attrId.GetValueType();
                    htmlWriter->Begin( HtmlElement::TableData );
                    // variables for string convertions
                    Util::String valueString;
                    Math::float4 valueFloat4;
                    float valueFloat;
                    Math::matrix44 matrix;
                    switch (valueType)
                    {
                        case Attr::IntType:
                            htmlWriter->Text(Util::String::FromInt( 
                                attrTable->GetInt(colIndex, rowIndex ) ));
                            break;
                        case Attr::FloatType:
                            valueFloat = attrTable->GetFloat(colIndex, rowIndex);
                            valueString.Format("%.3f", valueFloat);
                            htmlWriter->Text(valueString);
                            break;
                        case Attr::BoolType:
                            htmlWriter->Text(Util::String::FromBool(
                                attrTable->GetBool(colIndex, rowIndex ) ));
                            break;
                        case Attr::Float4Type:
                            valueFloat4 = attrTable->GetFloat4(colIndex, rowIndex);
                            valueString.Format("(%.3f, %.3f, %.3f, %.3f)",
                                valueFloat4.x(), valueFloat4.y(), 
                                valueFloat4.z(), valueFloat4.w());
                            htmlWriter->Text(valueString);
                            break;
                        case Attr::StringType:
                            htmlWriter->Text( attrTable->GetString(colIndex,  rowIndex ));
                            break;
                        case Attr::Matrix44Type:
                            // to make the matrix more readable create a table
                            matrix = attrTable->GetMatrix44(colIndex, rowIndex );
                            htmlWriter->AddAttr("cellpadding", "3");
                            //htmlWriter->AddAttr("border", "0");
                            //htmlWriter->AddAttr("rules", "cols");
                            htmlWriter->Begin(HtmlElement::Table);
                            htmlWriter->Begin(HtmlElement::TableRow);
                            valueString.Format("%.3f,", matrix.getrow0().x() );
                            htmlWriter->Element( HtmlElement::TableData, valueString );
                            valueString.Format("%.3f,", matrix.getrow0().y() );
                            htmlWriter->Element( HtmlElement::TableData, valueString );
                            valueString.Format("%.3f,", matrix.getrow0().z() );
                            htmlWriter->Element( HtmlElement::TableData, valueString );
                            valueString.Format("%.3f,", matrix.getrow0().w() );
                            htmlWriter->Element( HtmlElement::TableData, valueString );
                            htmlWriter->End(HtmlElement::TableRow);
                            htmlWriter->Begin(HtmlElement::TableRow);
                            valueString.Format("%.3f,", matrix.getrow1().x() );
                            htmlWriter->Element( HtmlElement::TableData, valueString );
                            valueString.Format("%.3f,", matrix.getrow1().y() );
                            htmlWriter->Element( HtmlElement::TableData, valueString );
                            valueString.Format("%.3f,", matrix.getrow1().z() );
                            htmlWriter->Element( HtmlElement::TableData, valueString );
                            valueString.Format("%.3f,", matrix.getrow1().w() );
                            htmlWriter->Element( HtmlElement::TableData, valueString );
                            htmlWriter->End(HtmlElement::TableRow);
                            htmlWriter->Begin(HtmlElement::TableRow);
                            valueString.Format("%.3f,", matrix.getrow2().x() );
                            htmlWriter->Element( HtmlElement::TableData, valueString );
                            valueString.Format("%.3f,", matrix.getrow2().y() );
                            htmlWriter->Element( HtmlElement::TableData, valueString );
                            valueString.Format("%.3f,", matrix.getrow2().z() );
                            htmlWriter->Element( HtmlElement::TableData, valueString );
                            valueString.Format("%.3f,", matrix.getrow2().w() );
                            htmlWriter->Element( HtmlElement::TableData, valueString );
                            htmlWriter->End(HtmlElement::TableRow);
                            htmlWriter->Begin(HtmlElement::TableRow);
                            valueString.Format("%.3f,", matrix.getrow3().x() );
                            htmlWriter->Element( HtmlElement::TableData, valueString );
                            valueString.Format("%.3f,", matrix.getrow3().y() );
                            htmlWriter->Element( HtmlElement::TableData, valueString );
                            valueString.Format("%.3f,", matrix.getrow3().z() );
                            htmlWriter->Element( HtmlElement::TableData, valueString );
                            valueString.Format("%.3f", matrix.getrow3().w() );
                            htmlWriter->Element( HtmlElement::TableData, valueString );
                            htmlWriter->End(HtmlElement::TableRow);
                            htmlWriter->End( HtmlElement::Table );
                            break;
                        case Attr::BlobType:
                            // there is no implementation to write out blob data yet
                            blobValue = attrTable->GetBlob(colIndex, rowIndex );
                            blobSize = blobValue.Size();
                            htmlWriter->Text( "Blob size: " 
                                + Util::String::FromInt(blobSize) );
                            break;
                        case Attr::GuidType:
                            guidValue = attrTable->GetGuid(colIndex, rowIndex );
                            htmlWriter->Text( guidValue.AsString() );
                            break;
                        default:
                            n_error("Debug::ObjectInspectorHandler: invalid attribute type!");
                    }
                    htmlWriter->End(HtmlElement::TableData );
                } // end if attrId.IsValid()
            } // end for colIndex
            htmlWriter->End(HtmlElement::TableRow);
        } // end if
    } // end for rowIndex
    htmlWriter->End(HtmlElement::Table);        
}

} // namespace Debug
