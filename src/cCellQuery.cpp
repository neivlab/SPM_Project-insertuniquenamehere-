#include <vector>
#include "cCellMatrix.h"
#include "cCellQuery.h"

//--------------------------------------------------------------
cCellQuery::cCellQuery(cCellMatrix& cells)
    : m_cells{ cells } 
{}

//--------------------------------------------------------------
std::vector<cLife*> cCellQuery::getNeighboursWithinDistance(int distance)
{
    return std::vector<cLife*>();
}

//--------------------------------------------------------------
cLife* cCellQuery::getNeighbourAt(int xOffset, int yOffset)
{
    return nullptr;
}

//--------------------------------------------------------------
cLife* cCellQuery::getNearestSibling()
{
    return nullptr;
}

//--------------------------------------------------------------
int cCellQuery::getSiblingCount()
{
    return 0;
}
