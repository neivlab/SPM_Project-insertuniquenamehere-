#include <vector>
#include <map>
#include "ofApp.h"
#include "cLife.h"
#include "cBlob.h"
// #include any other life class headers here



//--------------------------------------------------------------
ofApp::ofApp()
    : ofBaseApp(),
    m_cellMatrix{ ofGetWidth(), ofGetHeight() }
{
}

//--------------------------------------------------------------
void ofApp::setup() {

    ofSeedRandom();

    m_cellMatrix.setup();
}

#if 0
cBlob* pBlob = new cBlob;
pBlob->setup("Bob");

// assign it to a cell - choose at random near the center
int row = (m_cellMatrix.getHeight() / 2) + ofRandom(-m_cellMatrix.getHeight() / 10, m_cellMatrix.getHeight() / 10);
int col = (m_cellMatrix.getWidth() / 2) + ofRandom(-m_cellMatrix.getWidth() / 10, m_cellMatrix.getWidth() / 10);

cLife* pLife = m_cellMatrix.assignLife(row, col, pBlob);
if (nullptr != pLife)
delete pLife;

// create a random number (4-7) of Blobs at a semi random distribution around the center
for (int i = ofRandom(4, 7); i > 0; i--)
{
    //ofLog(OF_LOG_NOTICE, "pLife created at %d %d", x, y);
}
#endif

#if 0

how to implement a simple factory for multiple types?
map of id to spawn function

vector of <r,c> assembled by the chosen pattern
then replace with derived class types using spawn method?


#endif

//--------------------------------------------------------------
void    ofApp::generate()
{
    /// get a random r,c posiition in the matrix
    int anchorRow = (int)ofRandom(m_cellMatrix.getHeight() * 0.1, m_cellMatrix.getHeight() * 0.8f);
    int anchorCol = (int)ofRandom(m_cellMatrix.getWidth() * 0.1, m_cellMatrix.getWidth() * 0.8f);

    // starting with that cell, give it health and then do the same with the rest of the pattern
    cLife* pLife = m_cellMatrix.getCellLife(anchorRow, anchorCol);
    pLife->addHealth(1);

    int pattern = ofRandom(Patterns::_First, Patterns::_Last);
    switch (pattern)
    {
    case Patterns::Blinker:
        pLife->addHealth(1);
        pLife = m_cellMatrix.getCellLife(anchorRow + 1, anchorCol);
        pLife->addHealth(1);
        pLife = m_cellMatrix.getCellLife(anchorRow + 2, anchorCol);
        pLife->addHealth(1);
        break;

    case Patterns::Toad:
        pLife = m_cellMatrix.getCellLife(anchorRow + 1, anchorCol);
        pLife->addHealth(1);
        pLife = m_cellMatrix.getCellLife(anchorRow + 2, anchorCol);
        pLife->addHealth(1);
        pLife = m_cellMatrix.getCellLife(anchorRow + 1, anchorCol+1);
        pLife->addHealth(1);
        pLife = m_cellMatrix.getCellLife(anchorRow + 2, anchorCol+1);
        pLife->addHealth(1);
        pLife = m_cellMatrix.getCellLife(anchorRow + 3, anchorCol+2);
        pLife->addHealth(1);
        break;

    case Patterns::Beacon:
    case Patterns::Pulsar:

    default:
    case Patterns::Peak:
        pLife->addHealth(1);
        pLife = m_cellMatrix.getCellLife(anchorRow, anchorCol + 1);
        pLife->addHealth(1);
        pLife = m_cellMatrix.getCellLife(anchorRow, anchorCol + 2);
        pLife->addHealth(1);
        pLife = m_cellMatrix.getCellLife(anchorRow - 1, anchorCol + 1);
        pLife->addHealth(1);
        break;
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    if (!m_runSim)
        return;

    m_cellMatrix.update();

    // if there aren't any living cells after a fixed number of updates, generate some new Life
    if (m_cellMatrix.getLivingCellCount() == 0)
    {
        if (--m_resetCountdown == 0)
        {
            generate();
            return;
        }
    }
    else
        m_resetCountdown = RESET_FRAME_COUNT;

    // pause execution for a bit - 1.5 seconds
    ofSleepMillis(1500);
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (!m_runSim)
        return;
    m_cellMatrix.draw();
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    switch (key)
    {
    case ' ':   // spacebar/pause
        m_runSim = !m_runSim;
        break;

    case 'r':   // reset
        break;

    default:    // ignore
        break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
