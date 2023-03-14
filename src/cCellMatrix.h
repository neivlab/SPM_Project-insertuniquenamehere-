#pragma once
#include <vector>
#include <array>
#include "iCellQuery.h"
#include "cLife.h"

// forward declaration
class cFactory;

class cCellMatrix : public iCellQuery
{
public:
    cCellMatrix() = delete;     // no default constructor - must set width & height
    cCellMatrix(int cellSize, int width, int height);
    virtual ~cCellMatrix();
    void    setup(cFactory* factory);
    void    reset();
    void    update();
    void    draw();
    int     getWidth() const { return m_lifeWidth; }
    int     getHeight() const { return m_lifeHeight; }
    int     getColX(int col) { return col * m_cellSize + m_cellSize / 2; }
    int     getRowY(int row) { return row * m_cellSize + m_cellSize / 2; }
    cLife*  getLifeAtPos(int row, int col) const { return m_life[row][col]; }
    void    setLifeAtPos(cLife* pLife, int row, int col);
    int     getLivingCellCount(void) const;

    virtual std::vector<cLife*> queryNeighboursWithinDistance(cLife* pLife, int distance);
    virtual cLife* queryNeighbourAt(cLife* pLife, int xOffset, int yOffset);
    virtual cLife* queryNearestSibling(cLife* pLife);
    virtual int    querySiblingCount(cLife* pLife);

private:
    const unsigned int  NUM_ADJACENT_CELLS = 8;

    // grid drawing data
    int m_cellSize{ 0 };                // size of each cell
    int	m_lifeWidth{ 0 };               // width of the matrix, in cells
    int m_lifeHeight{ 0 };              // height of the matrix, in cells

    //LifeSpawnFunction   m_baseSpawner{ nullptr };
    cFactory* mp_factory{ nullptr };
    std::vector <std::vector<cLife*>> m_life;   // the cell matrix is a container for a 2D array of cells
};

