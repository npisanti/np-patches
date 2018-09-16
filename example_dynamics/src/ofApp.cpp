#include "ofApp.h"

// brickwall limiter followed by a oversampled soft-clipper with adjustable treshold

// decomment TEST_COMPRESSOR in ofApp.h to test using a compressor instead of a brickwall limiter

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0);
    
    engine.score.init( 1, 1, 96.25f);
    
    engine.score.sections[0].sequence(0).set( { 1.0f, 0.0f }, 16.0, 1.0 );
    engine.score.sections[0].launchCell(0);
    
    engine.score.sections[0].out_trig(0) >> loop;

    loop.load( ofToDataPath("classic_bird_wave.wav") );
    
    loop >> dyn.out_L() >> engine.audio_out(0);
            dyn.out_L() >> engine.audio_out(1);
   
    
    dyn.enableScope( engine );
   
    gui.setup("channel strip");
    gui.add( loop.parameters );
    gui.add( dyn.parameters );
    gui.setPosition(15, 20);      
   
    // start your audio engines!
    engine.listDevices();    
    engine.setDeviceID(0); 
    engine.setup( 44100, 512, 3); 
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    gui.draw();
    
    dyn.draw( 240, 20, 220, 150 );
    
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
