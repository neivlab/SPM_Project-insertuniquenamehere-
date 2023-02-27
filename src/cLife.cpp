#include <string>
#include <vector>
#include "ofMain.h"
#include "cLife.h"

#if _DEBUG
int static pos2cell(int i)
{
   return (i - 16 / 2) / 16;
}
#endif

std::string cLife::ms_lifeName{ "life" };

//--------------------------------------------------------------
cLife::cLife()
{
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
cLife* cLife::spawn(int x, int y)
{
    cLife* pLife = new cLife;
    pLife->setPosition(x + ofRandom(-20, 20), y + ofRandom(-20, 20));
    return pLife;
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
