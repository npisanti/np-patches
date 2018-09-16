#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "ofxGui.h"
#include "synth/DataSynth.h"
#include "effect/Chorus.h"

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
        
        pdsp::Engine   engine;

        np::synth::DataSynth dtsynth;
        np::effect::Chorus chorus;
        
        pdsp::ComputerKeyboard     keyboard;

        ofxPanel                    gui;

        ofVideoGrabber vidGrabber;
        int camWidth;
        int camHeight;
        int camPosX;
        int camPosY;
        
        int col;
        int channel;
        
        int mode;
        
        ofParameter<float> smooth;
		void smoothCall( float & value );
		
		ofFbo waveplot;
		
};
