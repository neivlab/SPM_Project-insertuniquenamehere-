#pragma once

#include <vector>
#include "ofMain.h"
#include "cCellMatrix.h"
#include "cLife.h"

class ofApp : public ofBaseApp{

	public:
		// custom constructor to allow for easier initialisation
		ofApp::ofApp();

		void setup();
		void update();
		void draw();
		void exit();


		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	private:
		enum Patterns {
			Blinker = 0,
			Toad,
			Beacon,
			Glider,
			Peak,
			_First = Blinker,
			_Last = Peak,
		};

		const int INITIAL_RESET_COUNT = 2;
		const int RESET_FRAME_COUNT = 10;

		void    generate();
		void    generate2();
		cCellMatrix		m_cellMatrix{ ofGetWidth(), ofGetHeight() };
		bool			m_runSim{ true };		// prevent sim from running until cells are displayed once
		int				m_resetCountdown{ INITIAL_RESET_COUNT };

};
