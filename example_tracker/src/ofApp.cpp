
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("probabilistic tracker");
    
    tracker.load( ofToDataPath("tracker.seq") );
    
    // ---------------------SEQUENCING------------------------
    engine.score.init( 2, 4, 172.0f); // sections, sequences, tempo
    
    engine.score.sections[0].setCell( 0, &tracker, pdsp::Behavior::Loop );
    
    engine.score.sections[0].launchCell(0);
    
    
    // ---------------------PATCHING------------------------
    voices.resize(2);    
    for( size_t i=0; i<voices.size(); ++i){
        engine.score.sections[0].out_trig(i) >> voices[i].in("trig");

        voices[i] * dB(-12.0f) >> engine.audio_out(0);
        voices[i] * dB(-12.0f) >> engine.audio_out(1);        
    }
    
    
    // -------------GUI & GRAPHIC SETUP--------------
    gui.setup("panel", "settings.xml", 20, 20 );
    gui.add( tracker.parameters );
    for( size_t i=0; i<voices.size(); ++i){
        gui.add( voices[i].label( "perc "+ofToString(i) ) );
    }
    gui.loadFromFile("settings.xml");
    
    ofBackground(0);
    ofSetFrameRate(30);

    // ------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    //engine.graphics.draw();
    gui.draw();
    
    ofSetColor(255, 85, 85);
    ofDrawBitmapString("for changing the sequence\nedit the tracker.seq file", 
                        20, ofGetHeight()-40 );  
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
