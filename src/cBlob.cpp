#include "ofMain.h"
#include "cBlob.h"


//--------------------------------------------------------------
cLife* cBlob::spawn(int x, int y)
{
	cBlob* pBlob = new cBlob;
	pBlob->setup();
	pBlob->setPosition(ofGetWidth() * .5 + ofRandom(-10, 10), ofGetHeight() * .5 + ofRandom(-10, 10));
	return pBlob;
}

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