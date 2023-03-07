#include <array>
#include "ofMain.h"
#include "cCellMatrix.h"
#include "cLife.h"

#if 0
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
#endif

//--------------------------------------------------------------
// cCellMatrix constructor
//  initialize the 2d array of cells; this actually means an array of arrays of (width) cells
//  initialise the Query object the life classes can use to find out about life around themselves
cCellMatrix::cCellMatrix(int width, int height)
    : iCellQuery{},  m_lifeWidth{ width / CELL_SIZE }, m_lifeHeight{ height / CELL_SIZE },
    m_life { m_lifeHeight, std::vector<cLife*>{ (const unsigned int) m_lifeWidth, nullptr} }
{
    cLife::setupQuery(*this);
}

//--------------------------------------------------------------
// destroy all life! Go through the vectors and delete the life pointers.
// assume OF won't allow draw or update to be called during destruction. that would be bad.
cCellMatrix::~cCellMatrix()
{
    for (auto row : m_life)
    {
        for (auto life : row)
        {
            if (life != nullptr)
                delete life;
        }
        row.resize(0);
    }
    m_life.resize(0);
}

//--------------------------------------------------------------
// create each Life and set the center position of each during construction
void cCellMatrix::setup()
{
    
    int xCellsCenter{ CELL_SIZE / 2 };
    int yCellsCenter{ CELL_SIZE / 2 };
    for (auto r = 0; r < m_lifeHeight; r++)
    {
        for (auto c = 0; c < m_lifeWidth; c++)
        {
            m_life[r][c] = new cLife(c * CELL_SIZE + xCellsCenter, r * CELL_SIZE + yCellsCenter);
            m_life[r][c]->setup();
        }
    }
}

//--------------------------------------------------------------
// kills all life in cells; resets life to base cLife with health 0
void cCellMatrix::reset()
{
    int xCellsCenter{ CELL_SIZE / 2 };
    int yCellsCenter{ CELL_SIZE / 2 };
    for (auto r = 0; r < m_lifeHeight; r++)
    {
        for (auto c = 0; c < m_lifeWidth; c++)
        {
            // kill off all non-base lifes
            if (m_life[r][c]->getName() != cLife::getLifeName())
            {
                delete  m_life[r][c];
                m_life[r][c] = new cLife(c * CELL_SIZE + xCellsCenter, r * CELL_SIZE + yCellsCenter);
                m_life[r][c]->setup();
            }
            m_life[r][c]->addHealth(m_life[r][c]->getHealth() * -1);    // set health to 0
        }
    }
}

//--------------------------------------------------------------
// draw all the cells
void    cCellMatrix::draw()
{
    // draw the grid
    ofSetColor(ofColor::gray);
    for (auto x = CELL_SIZE; x < ofGetWidth(); x += CELL_SIZE)
        ofDrawLine(x, 0, x, ofGetHeight());
    for (auto y = CELL_SIZE; y < ofGetHeight(); y += CELL_SIZE)
        ofDrawLine(0, y, ofGetWidth(), y);

    for (auto row : m_life)
    {
        for (auto life : row)
        {
            if (life != nullptr)
                life->draw();
        }
    }
}

//--------------------------------------------------------------
void    cCellMatrix::update()
{
    // There are 3 stages within each generation of life:

    // 1. simulation of each cell - iterate by row from top to bottom, and column from left to right
    //   permanent buffer used to pass the neighbours to each cell's life for simulation 
    static std::array<cLife*, 8>   neighbourLife;
    for (auto r = 0, rowAbove = r - 1, rowBelow = r + 1; r < m_lifeHeight; r++, rowAbove++, rowBelow++)
    {
        // calc indices for row above and below current row
        for (auto c = 0, colLeft = c - 1, colRight = c + 1; c < m_lifeWidth; c++, colLeft++, colRight++)
        {
            // set the neighbours array - row above
            neighbourLife[0] = (rowAbove >= 0 && colLeft >= 0) ? (m_life[rowAbove][colLeft]) : nullptr;
            neighbourLife[1] = (rowAbove >= 0) ? (m_life[rowAbove][c]) : nullptr;
            neighbourLife[2] = (rowAbove >= 0 && colRight < m_lifeWidth) ? (m_life[rowAbove][colRight]) : nullptr;

            // set the neighbours array - this row
            neighbourLife[3] = (colLeft >= 0) ? (m_life[r][colLeft]) : nullptr;
            neighbourLife[4] = (colRight < m_lifeWidth) ? (m_life[r][colRight]) : nullptr;

            // set the neighbours array - row below
            neighbourLife[5] = (rowBelow < m_lifeHeight&& colLeft >= 0) ? (m_life[rowBelow][colLeft]) : nullptr;
            neighbourLife[6] = (rowBelow < m_lifeHeight) ? (m_life[rowBelow][c]) : nullptr;
            neighbourLife[7] = (rowBelow < m_lifeHeight&& colRight < m_lifeWidth) ? (m_life[rowBelow][colRight]) : nullptr;;

            // now that neighbours are correct, run simulation for life at this cell
            // the result is the change in health to be applied to the life at this cell, after all cells have been checked
            m_life[r][c]->addHealth(m_life[r][c]->simulate(neighbourLife));
        }
    }

    // 2. apply life object changes, based on health values; evolutions, etc.
    //      add code here if needed to call additional life functions

   
    // 3. apply health changes from simulation to each cell
    for (auto r = 0, rowAbove = r - 1, rowBelow = r + 1; r < m_lifeHeight; r++, rowAbove++, rowBelow++)
    {
        for (auto c = 0, colLeft = c - 1, colRight = c + 1; c < m_lifeWidth; c++, colLeft++, colRight++)
        {
            m_life[r][c]->updateHealth();
        }
    }
}

//--------------------------------------------------------------
// assign a new life to a location in the matrix
// previous life is deleted immediately
void cCellMatrix::setLifeAtPos(cLife* pLife, int row, int col)
{
    cLife* pOld = m_life[row][col];
    m_life[row][col] = pLife;
    delete pOld;
}

//--------------------------------------------------------------
// return the number of cells with active life (health != 0)
int     cCellMatrix::getLivingCellCount(void) const
{
    int count = 0;
    for (auto row : m_life)
    {
        for (auto life : row)
        {
            if (life->isAlive())
                count++;
        }
    }
    return count;
}

//--------------------------------------------------------------
std::vector<cLife*> cCellMatrix::queryNeighboursWithinDistance(cLife* pLife, int distance)
{
    int xPos, yPos;
    pLife->getPosition(xPos, yPos);

    // clamp the search range to with the cell bounds: 0 to cellWidth or cellHeight
    xPos = (xPos - distance < 0) ? 0 : xPos - distance;
    yPos = (yPos - distance < 0) ? 0 : yPos - distance;

    cLife* neighbour{ nullptr };
    m_neighbours.clear();

#if 0
    for (auto r = yPos, rowAbove = r - distance, rowBelow = r + distance; r < m_lifeHeight; r++, rowAbove++, rowBelow++)
    {
        // calc indices for row above and below current row
        for (auto c = xPos, colLeft = c - 1, colRight = c + 1; c < m_lifeWidth; c++, colLeft++, colRight++)
        {
            // set the neighbours array - rows above
            neighbour = (rowAbove >= 0 && colLeft >= 0) ? (m_life[rowAbove][colLeft]) : nullptr;


            m_neighbours[0] = (rowAbove >= 0 && colLeft >= 0) ? (m_life[rowAbove][colLeft]) : nullptr;
            neighbours[1] = (rowAbove >= 0) ? (m_life[rowAbove][c]) : nullptr;
            neighbours[2] = (rowAbove >= 0 && colRight < m_lifeWidth) ? (m_life[rowAbove][colRight]) : nullptr;

            // set the neighbours array - this row
            neighbours[3] = (colLeft >= 0) ? (m_life[r][colLeft]) : nullptr;
            neighbours[4] = (colRight < m_lifeWidth) ? (m_life[r][colRight]) : nullptr;

            // set the neighbours array - row below
            neighbours[5] = (rowBelow < m_lifeHeight&& colLeft >= 0) ? (m_life[rowBelow][colLeft]) : nullptr;
            neighbours[6] = (rowBelow < m_lifeHeight) ? (m_life[rowBelow][c]) : nullptr;
            neighbours[7] = (rowBelow < m_lifeHeight&& colRight < m_lifeWidth) ? (m_life[rowBelow][colRight]) : nullptr;;
#if 0   // debugging
            if (mp_cells[r][c]->isAlive())
            {
                ofLog(OF_LOG_WARNING, "neighbours of r=%d,c=%d", r, c);
                for (auto n : neighbourLife)
                    ofLog(OF_LOG_WARNING, "  r=%d,c=%d", n->m_r, n->m_c);
                ofLog(OF_LOG_WARNING, "end neighbours");

            }
#endif

            m_neighbours.push_back(neighbour);
        }
    }
#endif

    return m_neighbours;
}

//--------------------------------------------------------------
cLife* cCellMatrix::queryNeighbourAt(cLife* pLife, int xOffset, int yOffset)
{
    return nullptr;
}

//--------------------------------------------------------------
cLife* cCellMatrix::queryNearestSibling(cLife* pLife)
{
    return nullptr;
}

//--------------------------------------------------------------
int    cCellMatrix::querySiblingCount(cLife* pLife)
{
    return 0;
}
