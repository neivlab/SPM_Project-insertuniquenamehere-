#include <string>
#include <vector>
#include <algorithm>
#include "ofMain.h"
#include "cLife.h"

#if _DEBUG
int static pos2cell(int i)
{
   return (i - 16 / 2) / 16;
}
#endif

const std::string   cLife::mk_LifeName{ "life" };
//const int           cLife::mk_MaxLife{ 1 };

//--------------------------------------------------------------
cLife::cLife(int x, int y)
    : m_xPos{ x }, m_yPos{y}
{
}

//--------------------------------------------------------------
cLife::~cLife()
{
    m_health = -1;
}

//--------------------------------------------------------------
cLife::cLife(const cLife& other)
    : cLife{other.m_xPos, other.m_yPos}
{
}

//--------------------------------------------------------------
cLife& cLife::operator=(const cLife& other)
{
    if (this == &other)
        return *this;
    this->m_health = other.m_health;
    this->m_name = other.m_name;
    this->m_xPos = other.m_xPos;
    this->m_yPos = other.m_yPos;
    this->m_drawSize = other.m_drawSize;
    this->m_color = other.m_color;
    return *this;
}

//--------------------------------------------------------------
void cLife::draw()
{
    if (m_health <= 0)
        return;
    ofFill();
    ofSetColor(m_color);
    ofDrawRectangle(m_xPos - m_drawSize * .5, m_yPos - m_drawSize * .5, m_drawSize, m_drawSize);
}

//--------------------------------------------------------------
int cLife::simulate(std::array<cLife*, 8>& simNeighbours)
{
    size_t neighbours{ 0 };
    for (cLife* pLife : simNeighbours)
    {
        if (nullptr == pLife)
            continue;
        if (pLife->isAlive())
            neighbours++;
    }

    switch (neighbours)
    {
    // over/under population if neighbours < 2 or > 3
    default:
    case 0:
    case 1:
        if (isAlive())
        {
            return -1; // decrement health 
#if _DEBUG
            ofLog(OF_LOG_NOTICE, "death at r=%d,c=%d", pos2cell(m_yPos), pos2cell(m_xPos));
#endif
        }
        break;

    // if alive, this life lives on if 2 neighbours - do nothing
    case 2:
#if _DEBUG
        if (isAlive())
            ofLog(OF_LOG_NOTICE, "living at r=%d,c=%d", pos2cell(m_yPos), pos2cell(m_xPos));
#endif
        break;
    
    // if alive, this life lives on if 3 neighbours - do nothing
    case 3:
        // if !alive, this life springs to life - as if through reproduction of the neighbours
        if (!isAlive())
        {
#if _DEBUG
            ofLog(OF_LOG_NOTICE, "birth at r=%d,c=%d", pos2cell(m_yPos), pos2cell(m_xPos));
#endif
            return +1;
        }
#if _DEBUG
        else
        {

            ofLog(OF_LOG_NOTICE, "living at r=%d,c=%d", pos2cell(m_yPos), pos2cell(m_xPos));
        }
#endif
        break;

    }
    return 0;   // no change
}

//--------------------------------------------------------------
void cLife::destroy()
{
}

//--------------------------------------------------------------
int  cLife::addHealth(int health) 
{ 
    m_health += health;
    // for base life, limit health to 0 or 1
    m_health = (m_health < 0) ? 0 : (m_health > mk_MaxLife) ? mk_MaxLife : m_health;
    return m_health;
}

//--------------------------------------------------------------
void cLife::setPosition(int x, int y)
{
    m_xPos = x;
    m_yPos = y;
}

//--------------------------------------------------------------
void cLife::getPosition(int& x, int& y)
{
    x = m_xPos;
    y = m_yPos;
}

//--------------------------------------------------------------
void  cLife::setupQuery(cCellQuery& query)
{
    ms_query = &query;
}

//--------------------------------------------------------------
cLife* cLife::spawn(int x, int y)
{
    cLife* pLife = new cLife(x, y);
    pLife->addHealth(1);
    return pLife;
}

