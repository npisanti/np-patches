#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "ofxGui.h"

#include "synth/QuickSampler.h"
#include "dynamics/Compressor.h"
#include "dynamics/Brickwall.h"

//#define TEST_COMPRESSOR

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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
        
        // pdsp modules
        pdsp::Engine   engine;
        
        np::synth::QuickSampler loop;
     
        #ifdef TEST_COMPRESSOR
        np::dynamics::Compressor dyn;
        #else
        np::dynamics::Brickwall dyn;
        #endif
            
        ofxPanel gui;

};
