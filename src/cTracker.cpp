#include "ofMain.h"
#include "cTracker.h"
#include "cBlob.h"

const std::string  cTracker::m_LifeName{ "tracker" };

//--------------------------------------------------------------
cTracker::cTracker(int xpos, int ypos)
    : cLife(xpos, ypos)
{
    m_color = ofColor::yellow;
    m_drawSize += 3;
    m_name = m_LifeName;
}

//--------------------------------------------------------------
void cTracker::draw()
{
    if (m_health == 0)
        return;

    ofFill();
    ofSetColor(m_color);
    ofDrawCircle(m_xCentre, m_yCentre, m_drawSize);

    if (m_nearestBlob != nullptr)
    {
        int blobX, blobY;
        m_nearestBlob->getPosition(blobX, blobY);

        ofFill();
        ofSetColor(ofColor::blue);
        ofDrawCircle(blobX, blobY, 5);
    }
}

//--------------------------------------------------------------
NeighbourClassMap cTracker::countNeighbours(const std::vector<cLife*>& simNeighbours)
{
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
int cTracker::interactWithNeighbours(const std::vector<cLife*>& simNeighbours, NeighbourClassMap& livingNeighbours)
{
    cLife* nearestBlob = nullptr;
    float minDistance = std::numeric_limits<float>::max();

    for (cLife* pLife : simNeighbours)
    {
        if (nullptr == pLife || pLife->getName() != cBlob::getLifeName() && pLife->getHealth()>0)
            continue;

        int lifeX, lifeY;
        pLife->getPosition(lifeX, lifeY);
        int X, Y;
        this->getPosition(X, Y);
        float distance = ofDist(X, Y, lifeX, lifeY);
        if (distance < minDistance)
        {
            minDistance = distance;
            nearestBlob = pLife;
        }
    }

    if (nearestBlob != nullptr)
    {
        m_nearestBlob = nearestBlob;
        int blobX, blobY;
        nearestBlob->getPosition(blobX, blobY);

        int X, Y;
        this->getPosition(X, Y);

        int deltaX = blobX - X;
        int deltaY = blobY - Y;

        int mouseX = ofGetMouseX();
        int mouseY = ofGetMouseY();

        int deltaMouseX = mouseX - X;
        int deltaMouseY = mouseY - Y;

        if (deltaMouseX != 0)
            X += (deltaMouseX > 0) ? 20 : -20;
        if (deltaMouseY != 0)
            Y += (deltaMouseY > 0) ? 20 : -20;

        this->setPosition(X, Y);
    }

    return 0;
}


//--------------------------------------------------------------
int  cTracker::addPendingHealthChange(int health)
{
    m_healthChange += health;
    if (m_healthChange > 0)
        return m_healthChange;
    return m_healthChange;
}

//--------------------------------------------------------------
void cTracker::applySimulationChanges()
{
    m_health += m_healthChange;

    // for base life, limit health to 0 or 1
    m_health = (m_health < 0) ? 0 : (m_health > MAX_LIFE) ? MAX_LIFE : m_health;

    m_healthChange = 0;
}

//--------------------------------------------------------------
cLife* cTracker::spawn(int x, int y, int health)
{
    cTracker* pTracker = new cTracker(x, y);
    pTracker->setup();
    pTracker->addPendingHealthChange(health);
    return pTracker;
}
