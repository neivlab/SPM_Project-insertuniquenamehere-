#pragma once
#include <vector>

class cLife;

class iCellQuery
{
#if 0
    find adjacent @ distance
        get at offet by x, y
        nearest of same class
        numbers of same class
#endif

public:
    virtual ~iCellQuery() {};
    virtual std::vector<cLife*> queryNeighboursWithinDistance(cLife* pLife, int distance) = 0;
    virtual cLife* queryNeighbourAt(cLife* pLife, int xOffset, int yOffset) = 0;
    virtual cLife* queryNearestSibling(cLife* pLife) = 0;
    virtual int    querySiblingCount(cLife* pLife) = 0;

protected:
    iCellQuery() = default;
    std::vector<cLife*>  m_neighbours;
};

