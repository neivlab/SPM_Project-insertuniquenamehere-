#pragma once
#include <vector>
#include "cCellMatrix.h"

class cLife;

class cCellQuery
{
#if 0
    find adjacent @ distance
        get at offet by x, y
        nearest of same class
        numbers of same class
#endif

public:
    cCellQuery(cCellMatrix& cells);
    std::vector<cLife*> getNeighboursWithinDistance(int distance);
    cLife* getNeighbourAt(int xOffset, int yOffset);
    cLife* getNearestSibling();
    int    getSiblingCount();

private:
    cCellMatrix& m_cells;
    std::vector<cLife*>  m_neighbours;
};

