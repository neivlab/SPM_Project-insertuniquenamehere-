#include <vector>
#include <map>
#include "ofApp.h"
#include "cLife.h"
#include "cBlob.h"
// #include any other life class headers here


/*
A simple factory pattern to create multiple types of Life at random

std::map of life "id"  to its spawn function

A vector of pairs of <r, c> assembled by the chosen pattern
then replace with derived class types using spawn method ?

*/
typedef cLife* (*LifeSpawnFunction)(int x, int y);
std::map<std::string, LifeSpawnFunction>    lifeFactory =
{
    {cLife::getLifeName(), cLife::spawn },
    {cBlob::getLifeName(), cBlob::spawn },

    // your class' static getName function and spawn function get added here
};


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

//--------------------------------------------------------------
void    ofApp::createNewGeneration()
{
    /// get a random r,c posiition in the matrix
    int anchorRow = (int)ofRandom(m_cellMatrix.getHeight() * 0.1, m_cellMatrix.getHeight() * 0.8f);
    int anchorCol = (int)ofRandom(m_cellMatrix.getWidth() * 0.1, m_cellMatrix.getWidth() * 0.8f);

    static std::vector<std::pair<int, int>> lifeCellsList;
    lifeCellsList.push_back(std::make_pair(anchorRow, anchorCol));

    int pattern = ofRandom(Patterns::_First, Patterns::_Last);
#ifdef _DEV_DEBUG
    // debug - force specific patterns
    //pattern = Patterns::Blinker;
    //pattern = Patterns::Beacon;
    //pattern = Patterns::Peak;
    //pattern = Patterns::Glider;
    //pattern = Patterns::Toad;
#endif

    switch (pattern)
    {
    case Patterns::Blinker:
        lifeCellsList.push_back(std::make_pair(anchorRow + 1, anchorCol));
        lifeCellsList.push_back(std::make_pair(anchorRow + 2, anchorCol));
        break;

    case Patterns::Toad:
        lifeCellsList.push_back(std::make_pair(anchorRow + 1, anchorCol));
        lifeCellsList.push_back(std::make_pair(anchorRow + 2, anchorCol));
        lifeCellsList.push_back(std::make_pair(anchorRow + 1, anchorCol + 1));
        lifeCellsList.push_back(std::make_pair(anchorRow + 2, anchorCol + 1));
        lifeCellsList.push_back(std::make_pair(anchorRow + 3, anchorCol + 1));
        break;

    case Patterns::Beacon:
        lifeCellsList.push_back(std::make_pair(anchorRow, anchorCol + 1));
        lifeCellsList.push_back(std::make_pair(anchorRow + 1, anchorCol)); 
        lifeCellsList.push_back(std::make_pair(anchorRow + 1, anchorCol + 1));
        lifeCellsList.push_back(std::make_pair(anchorRow+ 2, anchorCol + 2));
        lifeCellsList.push_back(std::make_pair(anchorRow + 2, anchorCol + 3));
        lifeCellsList.push_back(std::make_pair(anchorRow + 3, anchorCol + 2));
        lifeCellsList.push_back(std::make_pair(anchorRow + 3, anchorCol + 3));

        break;

    case Patterns::Glider:
        lifeCellsList.push_back(std::make_pair(anchorRow + 1, anchorCol + 1));
        lifeCellsList.push_back(std::make_pair(anchorRow + 2, anchorCol - 1));
        lifeCellsList.push_back(std::make_pair(anchorRow + 2, anchorCol));
        lifeCellsList.push_back(std::make_pair(anchorRow + 2, anchorCol + 1));
        break;

    default:
    case Patterns::Peak:
        lifeCellsList.push_back(std::make_pair(anchorRow, anchorCol + 1));
        lifeCellsList.push_back(std::make_pair(anchorRow, anchorCol + 2));
        lifeCellsList.push_back(std::make_pair(anchorRow+1, anchorCol + 1));
        break;
    }

    // spawn randomly chosen life at the cells in the vector
    std::map<std::string, LifeSpawnFunction>::const_iterator itr = lifeFactory.begin();
    std::advance(itr, ofRandom(0, lifeFactory.size() - 1));
    std::string lifeName{ itr->first };
    // force lifeName to be "Life"
    lifeName = cLife::getLifeName();
    for (auto& pair : lifeCellsList)
    {
        cLife * pLife = lifeFactory[lifeName](m_cellMatrix.getColX(pair.second), m_cellMatrix.getRowY(pair.first));
        m_cellMatrix.setLifeAtPos(pLife, pair.first, pair.second);
    }
    lifeCellsList.clear();
}


//--------------------------------------------------------------
void ofApp::update(){
    if (!m_runSim)
        return;

    // tell all cells to update
    m_cellMatrix.update();

    // if there aren't any living cells after a fixed number of updates, generate some new Life
    if (m_cellMatrix.getLivingCellCount() == 0)
    {
        if (--m_resetCountdown <= 0)
        {
            createNewGeneration();
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
