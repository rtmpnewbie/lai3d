//------------------------------------------------------------------------------
//  quadtreestagebuilder.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/quadtreestagebuilder.h"
#include "internalgraphics/internalstage.h"
#include "internalgraphics/cell.h"

namespace Attr
{
    DefineInt(QuadTreeStageBuilderDepth, 'QTBD', Attr::ReadOnly);
    DefineFloat4(QuadTreeStageBuilderCenter, 'QTBC', Attr::ReadOnly);
    DefineFloat4(QuadTreeStageBuilderExtents, 'QTBE', Attr::ReadOnly);
};

namespace InternalGraphics
{
__ImplementClass(InternalGraphics::QuadtreeStageBuilder, 'QSBL', InternalGraphics::StageBuilder);

using namespace Math;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
QuadtreeStageBuilder::QuadtreeStageBuilder()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
QuadtreeStageBuilder::~QuadtreeStageBuilder()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
QuadtreeStageBuilder::BuildStage(const Ptr<InternalStage>& stage)
{
    Math::bbox box(this->attrs.GetFloat4(Attr::QuadTreeStageBuilderCenter),
                   this->attrs.GetFloat4(Attr::QuadTreeStageBuilderExtents));
    uchar qtDepth = (uchar) this->attrs.GetInt(Attr::QuadTreeStageBuilderDepth);
    this->quadTree.Setup(box, qtDepth);
    this->numCellsBuilt = 0;

    Ptr<Cell> rootCell = this->CreateQuadTreeCell(0, 0, 0, 0);
    stage->SetRootCell(rootCell);
}

//------------------------------------------------------------------------------
/**
*/
Cell* 
QuadtreeStageBuilder::CreateQuadTreeCell(Cell* parentCell, uchar curLevel, ushort curCol, ushort curRow)
{
    // create a new cell
	Cell* cell = Cell::Create();
    int nodeIndex = this->quadTree.GetNodeIndex(curLevel, curCol, curRow);
    const QuadTree<Cell>::Node& node = this->quadTree.GetNodeByIndex(nodeIndex);
    cell->SetBoundingBox(node.GetBoundingBox());
    this->numCellsBuilt++;

    // create child cells
    uchar childLevel = curLevel + 1;
    if (childLevel < this->quadTree.GetDepth())
    {        
        ushort i;
        for (i = 0; i < 4; i++)
        {
            ushort childCol = 2 * curCol + (i & 1);
            ushort childRow = 2 * curRow + ((i & 2) >> 1);
            Ptr<Cell> childCell = this->CreateQuadTreeCell(cell, childLevel, childCol, childRow);
            cell->AttachChildCell(childCell);
        }        
    }
    return cell;
}

} // namespace InternalGraphics
