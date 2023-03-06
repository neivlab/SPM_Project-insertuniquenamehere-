#include <array>
#include "ofMain.h"
#include "cCellMatrix.h"
#include "cLife.h"

//--------------------------------------------------------------
// default constructor for the cell - unconventional but valid for structures
cCellMatrix::cCell::cCell()
{
}

//--------------------------------------------------------------
// preferred constructor for the cell - xy centre can be set here
cCellMatrix::cCell::cCell(int xCentre, int yCentre)
    : m_xCentre{ xCentre }, m_yCentre{ yCentre }
{
}

//--------------------------------------------------------------
cCellMatrix::cCell::cCell(const cCell& other)
    : cCellMatrix::cCell{ other.m_xCentre, other.m_yCentre }
{
}

//--------------------------------------------------------------
cCellMatrix::cCell::~cCell()
{
    // when a cell is destroyed, free any allocated life too
    if (mp_life)
    {
        delete mp_life;
        mp_life = nullptr;
    }
}

//--------------------------------------------------------------
void cCellMatrix::cCell::setup(int xCentre, int yCentre)
{
    m_xCentre = xCentre;
    m_yCentre = yCentre;
    // a cell must have a life object in it 
    mp_life = new cLife(m_xCentre, m_yCentre);
}

//--------------------------------------------------------------
void cCellMatrix::cCell::reset()
{
    if (mp_life)
    {
        delete mp_life;
        mp_life = new cLife(m_xCentre, m_yCentre);
    }
}

//--------------------------------------------------------------
// cCellMatrix constructor
//  initialize the 2d array of cells; this actually means an array of arrays of (width) cells
cCellMatrix::cCellMatrix(int width, int height)
    : m_cellsWidth{ width / CELL_SIZE }, m_cellsHeight{ height / CELL_SIZE }, 
    m_cells{ m_cellsHeight, std::vector<cCell>{ (const unsigned int)m_cellsWidth} }
{
}

//--------------------------------------------------------------
cCellMatrix::~cCellMatrix()
{
}

//--------------------------------------------------------------
void cCellMatrix::setup()
{
    // set the center of each cell during construction
    int xCellsCenter{ CELL_SIZE / 2 };
    int yCellsCenter{ CELL_SIZE / 2 };
    for (auto r = 0; r < m_cellsHeight; r++)
    {
        for (auto c = 0; c < m_cellsWidth; c++)
            m_cells[r][c].setup(c * CELL_SIZE + xCellsCenter, r * CELL_SIZE + yCellsCenter);
    }
}

//--------------------------------------------------------------
// kills all life in cells; resets life to base cLife
void cCellMatrix::reset()
{
    for (auto r = 0; r < m_cellsHeight; r++)
    {
        for (auto c = 0; c < m_cellsWidth; c++)
        {
            // kill off all non-base lifes
            if (m_cells[r][c].mp_life->getName() != cLife::getLifeName())
            {
                m_cells[r][c].reset();
            }

            m_cells[r][c].mp_life->addHealth(m_cells[r][c].mp_life->getHealth() * -1);    // set health to 0
        }
    }
}

//--------------------------------------------------------------
void    cCellMatrix::draw()
{
    // draw the grid
    ofSetColor(ofColor::gray);
    for (auto x = CELL_SIZE; x < ofGetWidth(); x += CELL_SIZE)
        ofDrawLine(x, 0, x, ofGetHeight());
    for (auto y = CELL_SIZE; y < ofGetHeight(); y += CELL_SIZE)
        ofDrawLine(0, y, ofGetWidth(), y);

    // draw all the cells - let life at the cell decide how to draw
    for (auto r = 0; r < m_cellsHeight; r++)
    {
        for (auto c = 0; c < m_cellsWidth; c++)
        {
            m_cells[r][c].mp_life->draw();
        }
    }
}

//--------------------------------------------------------------
cLife* cCellMatrix::setCellLife(int row, int col, cLife* pNewLife) 
{
    if ((nullptr == pNewLife ) || ((row < 0) || (row >= m_cellsHeight)) || ((col < 0) || (col >= m_cellsWidth)))
        return nullptr;
    cLife* pOld = m_cells[row][col].mp_life;
    m_cells[row][col].mp_life = pNewLife;
    // set the centre of the actual cell as the Life's position; the life class doesn't understand cell geometry
    pNewLife->setPosition(col * CELL_SIZE + CELL_SIZE / 2, row * CELL_SIZE + CELL_SIZE / 2);

    return pOld;
}

//--------------------------------------------------------------
void    cCellMatrix::update()
{
    // There are 3 stages within each generation of life:

    // 1. simulation of each cell - iterate by row from top to bottom, and column from left to right
    //   permanent buffer used to pass the neighbours to each cell's life for simulation 
    static std::array<cLife*, 8>   neighbourLife;
    for (auto r = 0, rowAbove = r - 1, rowBelow = r + 1; r < m_cellsHeight; r++, rowAbove++, rowBelow++)
    {
        // calc indices for row above and below current row
        for (auto c = 0, colLeft = c - 1, colRight = c + 1; c < m_cellsWidth; c++, colLeft++, colRight++)
        {
            // set the neighbours array - row above
            neighbourLife[0] = (rowAbove >= 0 && colLeft >= 0) ? (m_cells[rowAbove][colLeft]).mp_life : nullptr;
            neighbourLife[1] = (rowAbove >= 0) ? (m_cells[rowAbove][c]).mp_life : nullptr;
            neighbourLife[2] = (rowAbove >= 0 && colRight < m_cellsWidth) ? (m_cells[rowAbove][colRight]).mp_life : nullptr;

            // set the neighbours array - this row
            neighbourLife[3] = (colLeft >= 0) ? (m_cells[r][colLeft]).mp_life : nullptr;
            neighbourLife[4] = (colRight < m_cellsWidth) ? (m_cells[r][colRight]).mp_life : nullptr;

            // set the neighbours array - row below
            neighbourLife[5] = (rowBelow < m_cellsHeight&& colLeft >= 0) ? (m_cells[rowBelow][colLeft]).mp_life : nullptr;
            neighbourLife[6] = (rowBelow < m_cellsHeight) ? (m_cells[rowBelow][c]).mp_life : nullptr;
            neighbourLife[7] = (rowBelow < m_cellsHeight&& colRight < m_cellsWidth) ? (m_cells[rowBelow][colRight]).mp_life : nullptr;;

#if 0   // debugging
            if (mp_cells[r][c].mp_life->isAlive())
            {
                ofLog(OF_LOG_WARNING, "neighbours of r=%d,c=%d", r, c);
                for (auto n : neighbourLife)
                    ofLog(OF_LOG_WARNING, "  r=%d,c=%d", n->m_r, n->m_c);
                ofLog(OF_LOG_WARNING, "end neighbours");

            }
#endif
            // now that neighbours are correct, run simulation for life at this cell
            // the result is the change in health to be applied to the life at this cell, after all cells have been checked
            m_cells[r][c].m_healthChange = m_cells[r][c].mp_life->simulate(neighbourLife);
        }
    }

    // 2. apply life object changes, based on health values; evolutions, etc.

   
    // 3. apply health changes from simulation to each cell
    for (auto r = 0, rowAbove = r - 1, rowBelow = r + 1; r < m_cellsHeight; r++, rowAbove++, rowBelow++)
    {
        for (auto c = 0, colLeft = c - 1, colRight = c + 1; c < m_cellsWidth; c++, colLeft++, colRight++)
        {
            m_cells[r][c].mp_life->addHealth(m_cells[r][c].m_healthChange);
            m_cells[r][c].m_healthChange = 0;
        }
    }
}

//--------------------------------------------------------------
int     cCellMatrix::getLivingCellCount(void) const
{
    int count = 0;

    for (auto r = 0; r < m_cellsHeight; r++)
    {
        for (auto c = 0; c < m_cellsWidth; c++)
        {
            if (m_cells[r][c].mp_life->isAlive())
                count++;
        }
    }
    return count;
}

