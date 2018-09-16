#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    trig >> sampler.in("trig") >> engine.audio_out(0);
                       sampler >> engine.audio_out(1);

    // ------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 
    
    gui.setup("panel");
    gui.add( sampler.parameters );
    gui.setPosition( 20, 20);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    gui.draw();
    ofDrawBitmapString("press load to choose a sample\nthen press 1-8 to trigger the sample\nwith different intensities", 20, ofGetHeight()-40 );
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key){
        case '1': trig.trigger(1.0f); break;
        case '2': trig.trigger(0.875f); break;
        case '3': trig.trigger(0.75f); break;
        case '4': trig.trigger(0.625f); break;
        case '5': trig.trigger(0.5f); break;
        case '6': trig.trigger(0.375f); break;
        case '7': trig.trigger(0.25f); break;
        case '8': trig.trigger(0.25f); break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){ }
//--------------------------------------------------------------
