#include "ofMain.h"
#include "cBlob.h"

const std::string  cBlob::m_LifeName{ "blob" };

//--------------------------------------------------------------
cBlob::cBlob(int xpos, int ypos)
	: cLife(xpos, ypos)
{
	m_color = ofColor::blueSteel;
	m_health = 1;
	m_drawSize += 3;
}

//--------------------------------------------------------------
void cBlob::draw()
{
	ofFill();
	ofSetColor(m_color);
    ofDrawCircle(m_xCentre, m_yCentre, m_drawSize);
}

//--------------------------------------------------------------
cLife* cBlob::spawn(int x, int y)
{
	cBlob* pBlob = new cBlob(x, y);
	pBlob->setup();
	pBlob->addHealth(1);
	return pBlob;
}

