#include <array>
#include "ofMain.h"
#include "cCellMatrix.h"
#include "cLife.h"

// definition of the static member
int sCell::ms_count = 0;

//--------------------------------------------------------------
// constructor for the cell - unconventional but valid
sCell::sCell()
{
    // a cell has a life object in it - allocated on creation
    mp_life = new cLife;
    m_idx = sCell::ms_count++;
}

//--------------------------------------------------------------
sCell::~sCell()
{
    // when a cell is destroyed, free any allocated life too
    if (mp_life)
    {
        delete mp_life;
        mp_life = nullptr;
    }
}

//--------------------------------------------------------------
void sCell::setup(int x, int y)
{
    // get the x,y location of the cell's centre and pass to the life
    mp_life->setPosition(x, y);
    m_idx = sCell::ms_count++;
}

//--------------------------------------------------------------
void sCell::reset(int x, int y)
{
    if (mp_life)
    {
        delete mp_life;
        mp_life = new cLife;
        setup(x, y);
    }
}

//--------------------------------------------------------------
void sCell::draw()
{
    mp_life->draw();
}


//--------------------------------------------------------------
// cCellMatrix constructor
//  allocate a 2d array of cells; this actually means an array of arrays of (width) cells
cCellMatrix::cCellMatrix(int width, int height)
    : m_cellsWidth{ width / CELL_SIZE }, m_cellsHeight{ height / CELL_SIZE }
{
    // allocate the matrix of cells 
    mp_cells = new sCell * [m_cellsHeight];
    // pointer voodoo... have to allocate all the cells in a big block, the assign to each row
    mp_cells[0] = new sCell[m_cellsWidth * m_cellsHeight]();
    for (int r = 1; r < m_cellsHeight; r++)
        mp_cells[r] = mp_cells[r - 1] + m_cellsWidth;
}

//--------------------------------------------------------------
cCellMatrix::~cCellMatrix()
{
    // free the array cells, and array of arrays
    delete[] mp_cells[0];
    delete[] mp_cells;
    mp_cells = nullptr;
}

//--------------------------------------------------------------
void cCellMatrix::setup()
{
    // complete setup of the cell the position of each life to be the center of the cell they're in
    for (auto r = 0; r < m_cellsHeight; r++)
    {
        for (auto c = 0; c < m_cellsWidth; c++)
        {
            mp_cells[r][c].setup(c * CELL_SIZE + CELL_SIZE / 2, r * CELL_SIZE + CELL_SIZE / 2);
        }
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
            if (mp_cells[r][c].mp_life->getName() != cLife::getLifeName())
            {
                mp_cells[r][c].reset(c * CELL_SIZE + CELL_SIZE / 2, r * CELL_SIZE + CELL_SIZE / 2);
            }

            mp_cells[r][c].mp_life->addHealth(mp_cells[r][c].mp_life->getHealth() * -1);    // set health to 0
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
            mp_cells[r][c].draw();
        }
    }
}

//--------------------------------------------------------------
cLife* cCellMatrix::setCellLife(int row, int col, cLife* pNewLife) 
{
    if ((nullptr == pNewLife ) || ((row < 0) || (row >= m_cellsHeight)) || ((col < 0) || (col >= m_cellsWidth)))
        return nullptr;
    cLife* pOld = mp_cells[row][col].mp_life;
    mp_cells[row][col].mp_life = pNewLife;
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
            neighbourLife[0] = (rowAbove >= 0 && colLeft >= 0) ? (mp_cells[rowAbove][colLeft]).mp_life : nullptr;
            neighbourLife[1] = (rowAbove >= 0) ? (mp_cells[rowAbove][c]).mp_life : nullptr;
            neighbourLife[2] = (rowAbove >= 0 && colRight < m_cellsWidth) ? (mp_cells[rowAbove][colRight]).mp_life : nullptr;

            // set the neighbours array - this row
            neighbourLife[3] = (colLeft >= 0) ? (mp_cells[r][colLeft]).mp_life : nullptr;
            neighbourLife[4] = (colRight < m_cellsWidth) ? (mp_cells[r][colRight]).mp_life : nullptr;

            // set the neighbours array - row below
            neighbourLife[5] = (rowBelow < m_cellsHeight&& colLeft >= 0) ? (mp_cells[rowBelow][colLeft]).mp_life : nullptr;
            neighbourLife[6] = (rowBelow < m_cellsHeight) ? (mp_cells[rowBelow][c]).mp_life : nullptr;
            neighbourLife[7] = (rowBelow < m_cellsHeight&& colRight < m_cellsWidth) ? (mp_cells[rowBelow][colRight]).mp_life : nullptr;;

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
            mp_cells[r][c].m_healthChange = mp_cells[r][c].mp_life->simulate(neighbourLife);
        }
    }

    // 2. apply life object changes, based on health values; evolutions, etc.

   
    // 3. apply health changes from simulation to each cell
    for (auto r = 0, rowAbove = r - 1, rowBelow = r + 1; r < m_cellsHeight; r++, rowAbove++, rowBelow++)
    {
        for (auto c = 0, colLeft = c - 1, colRight = c + 1; c < m_cellsWidth; c++, colLeft++, colRight++)
        {
            mp_cells[r][c].mp_life->addHealth(mp_cells[r][c].m_healthChange);
            mp_cells[r][c].m_healthChange = 0;
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
            if (mp_cells[r][c].mp_life->isAlive())
                count++;
        }
    }
    return count;
}

