#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxPDSP.h"
#include "ofxGui.h"

#include "effect/Chorus.h"
#include "synth/WaveSynth.h"
#include "modulator/ClockedLFO.h"

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
        
        np::effect::Chorus      chorus;
        np::synth::WaveSynth    wavesynth;
        np::modulator::ClockedLFO lfo;
                
        pdsp:: ComputerKeyboard     keyboard;
        ofxPanel                    gui;
      
};
