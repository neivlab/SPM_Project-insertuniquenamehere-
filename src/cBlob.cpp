#include "ofMain.h"
#include "cBlob.h"

const std::string  cBlob::m_LifeName{ "blob" };

//--------------------------------------------------------------
cBlob::cBlob()
	: cLife()
{
	m_color = ofColor::blueSteel;
	m_health = 1;
}

//--------------------------------------------------------------
void cBlob::draw()
{
	ofFill();
	ofSetColor(m_color);
    ofDrawCircle(m_xPos, m_yPos, m_drawSize);
}

//--------------------------------------------------------------
cLife* cBlob::spawn(int x, int y)
{
	cBlob* pBlob = new cBlob;
	pBlob->setup();
	pBlob->setPosition(x + ofRandom(-20, 20), y + ofRandom(-20, 20));
	pBlob->addHealth(1);
	return pBlob;
}

