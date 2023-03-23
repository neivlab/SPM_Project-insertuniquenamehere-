#include <vector>
#include <map>
#include "ofApp.h"
#include "cLife.h"
#include "cBlob.h"
#include "cTracker.h"
// #include any other life class headers here

//--------------------------------------------------------------
ofApp::ofApp()
    : ofBaseApp(),
    m_cellMatrix{ CELL_SIZE, ofGetWidth(), ofGetHeight() }
{
}

//--------------------------------------------------------------
void ofApp::setup() {

    ofSeedRandom();
    
    // TODO: register your class here to enable it to be spawned
    m_factory.registerClassSpawner(cLife::getLifeName(), cLife::spawn);
    m_factory.registerClassSpawner(cBlob::getLifeName(), cBlob::spawn);
    m_factory.registerClassSpawner(cTracker::getLifeName(), cTracker::spawn);

    // TODO: set the default Life type for all cells here
    m_factory.setDefaultLife(cBlob::getLifeName());

    // share the Factory with the CellMatrix
    m_cellMatrix.setup(&m_factory);
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

    // TODO - choose the life type to spawn with a generation
    // select a randomly chosen life to spawn at the cells in the vector
    //std::string lifeName = m_factory.getRandomLife();
    std::string lifeName = m_factory.getDefaultLifeName();

#ifdef _DEV_DEBUG
    // override the randomness and force the created life to be ...
    // lifeName = cLife::getLifeName();
    lifeName = cBlob::getLifeName();
#endif
    for (auto& pair : lifeCellsList)
    {
        cLife * pLife = m_factory.spawn(lifeName, m_cellMatrix.getColX(pair.second), m_cellMatrix.getRowY(pair.first), 1);
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
    ofSleepMillis(FRAME_DELAY_MS);
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
void ofApp::mouseReleased(int x, int y, int button) {
    // gets the row and column of the mouse click
    int row = m_cellMatrix.getHeight() * y / ofGetWindowHeight();
    int col = m_cellMatrix.getWidth() * x / ofGetWindowWidth();

    if (row >= 0 && row < m_cellMatrix.getHeight() && col >= 0 && col < m_cellMatrix.getWidth()) {
        if (button == 0) {
            cLife* pLife = m_factory.spawn(cBlob::getLifeName(), m_cellMatrix.getColX(col), m_cellMatrix.getRowY(row), 1);
            m_cellMatrix.setLifeAtPos(pLife, row, col);
        }
        else {
            cLife* pLife = m_factory.spawn(cTracker::getLifeName(), m_cellMatrix.getColX(col), m_cellMatrix.getRowY(row), 1);
            m_cellMatrix.setLifeAtPos(pLife, row, col);
        }
    }
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
