#include "ofMain.h"
#include "cRgbcell.h"

const std::string  cRgbcell::m_LifeName{ "rgb" };

//--------------------------------------------------------------
cRgbcell::cRgbcell(int xpos, int ypos)
    : cLife(xpos, ypos)
{
    m_color = ofColor(0, 0, 0);
    m_drawSize += 3;
    m_name = m_LifeName;
}

//--------------------------------------------------------------
void cRgbcell::draw()
{
    if (m_health == 0)
        return;
    ofFill();
    ofSetColor(255 / 10 * red, 255 / 10 * green, 255 / 10 * blue);
    ofDrawCircle(m_xCentre, m_yCentre, m_drawSize);
}

//--------------------------------------------------------------
NeighbourClassMap cRgbcell::countNeighbours(const std::vector<cLife*>& simNeighbours)
{
    // map of neighbor classes for each type, how many alive?
    NeighbourClassMap livingNeighbours;
    for (cLife* pLife : simNeighbours)
    {
        if (nullptr == pLife)
            continue;
        if (pLife->isAlive())
        {
            livingNeighbours[pLife->getName()]++;
        }
    }
    return livingNeighbours;
}

//--------------------------------------------------------------
int cRgbcell::interactWithNeighbours(const std::vector<cLife*>& simNeighbours, NeighbourClassMap& livingNeighbours)
{
    // count the number of live neighbors this cell has; go through the map
    size_t livingNeighbourCount{ 0 };
    for (auto life : livingNeighbours)
        livingNeighbourCount += life.second;

    int healthChange{ 0 };
    switch (livingNeighbourCount)
    {
        // over/under population if neighbours < 2 or > 3
    default:
        if (red <= 10)
        {
            red++;
        }
        else
        {
            red = 0;
        }
        if (green <= 10)
        {
            green++;
        }
        else
        {
            green = 0;
        }
        if (blue <= 10)
        {
            blue++;
        }
        else
        {
            blue = 0;
        }
        break;
    case 0:
        red = rand() % 11;
        green = rand() % 11;
        blue = rand() % 11;
        break;
    case 1:
        if (isAlive())
        {
            if (red <= 10)
            {
                red++;
            }
            else
            {
                red = 0;
            }
#ifdef _DEV_DEBUG
            ofLog(OF_LOG_NOTICE, "death at x=%d,y=%d", m_xCentre, m_yCentre);
#endif
        }
        break;

        // if alive, this life lives on if 2 neighbours - do nothing
    case 2:
#ifdef _DEV_DEBUG
        if (isAlive())
            ofLog(OF_LOG_NOTICE, "living at x=%d,y=%d", m_xCentre, m_yCentre);
#endif
        if (green <= 10)
        {
            green++;
        }
        else
        {
            green = 0;
        }
        break;

        // if alive, this life lives on if 3 neighbours - do nothing
    case 3:
        // if !alive, this life springs to life - as if through reproduction of the neighbours
        if (!isAlive())
        {
#ifdef _DEV_DEBUG
            ofLog(OF_LOG_NOTICE, "birth at x=%d,y=%d", m_xCentre, m_yCentre);
#endif
            healthChange = 1;

        }
#ifdef _DEV_DEBUG
        else
        {

            ofLog(OF_LOG_NOTICE, "living at x=%d,y=%d", m_xCentre, m_yCentre);
        }
#endif
        if (blue <= 10)
        {
            blue++;
        }
        else
        {
            blue = 0;
        }
        break;
    }
    return healthChange;   // no change
}

//--------------------------------------------------------------
int  cRgbcell::addPendingHealthChange(int health)
{
    m_healthChange += health;
    if (m_healthChange > 0)
        return m_healthChange;
    return m_healthChange;
}

//--------------------------------------------------------------
void cRgbcell::applySimulationChanges()
{
    m_health += m_healthChange;

    // for base life, limit health to 0 or 1
    m_health = (m_health < 0) ? 0 : (m_health > MAX_LIFE) ? MAX_LIFE : m_health;

    m_healthChange = 0;
}

//--------------------------------------------------------------
cLife* cRgbcell::spawn(int x, int y, int health)
{
    cRgbcell* pRgbcell = new cRgbcell(x, y);
    pRgbcell->setup();
    pRgbcell->addPendingHealthChange(health);
    return pRgbcell;
}

