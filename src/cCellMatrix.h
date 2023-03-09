#pragma once
#include <vector>
#include <array>
#include "iCellQuery.h"

// Forward declaration of the base cell class.
// Since only a cLife pointer is used in this file, the whole cLife class isn't necessary to include
class cLife;

class cCellMatrix : public iCellQuery
{
public:
    cCellMatrix() = delete;     // no default constructor - must set width & height
    cCellMatrix(int width, int height);
    virtual ~cCellMatrix();
    void    setup();
    void    reset();
    void    update();
    void    draw();
    int     getWidth() const { return m_lifeWidth; }
    int     getHeight() const { return m_lifeHeight; }
    int     getColX(int col) { return col * CELL_SIZE + CELL_SIZE / 2; }
    int     getRowY(int row) { return row * CELL_SIZE + CELL_SIZE / 2; }
    cLife*  getLifeAtPos(int row, int col) const { return m_life[row][col]; }
    void    setLifeAtPos(cLife* pLife, int row, int col);
    int     getLivingCellCount(void) const;

    virtual std::vector<cLife*> queryNeighboursWithinDistance(cLife* pLife, int distance);
    virtual cLife* queryNeighbourAt(cLife* pLife, int xOffset, int yOffset);
    virtual cLife* queryNearestSibling(cLife* pLife);
    virtual int    querySiblingCount(cLife* pLife);

private:
    const int           CELL_SIZE = 16;
    const unsigned int NUM_ADJACENT_CELLS = 8;
    // grid drawing data
    int	m_lifeWidth{ 0 };              // width of the matrix, in cells
    int m_lifeHeight{ 0 };             // height of the matrix, in cells
    
    std::vector <std::vector<cLife*>> m_life;   // the cell matrix is a container for a 2D array of cells
};

