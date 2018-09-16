
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("markov chain sequencer");
    
    mc.load( ofToDataPath("mc.chain") );
    
    // ---------------------SEQUENCING------------------------
    engine.score.init( 1, 1, 172.0f); // sections, sequences, tempo
    
    engine.score.sections[0].setCell( 0, &mc, pdsp::Behavior::Loop );
    
    engine.score.sections[0].launchCell(0);
    
    
    // ---------------------PATCHING------------------------

    engine.score.sections[0].out_trig(0) >> zap.in_trig();
    engine.score.sections[0].out_value(1) >> zap.in_pitch();
    engine.score.sections[0].out_trig(2) >> noise.in_trig();
    engine.score.sections[0].out_trig(3) >> click.in_trig();
    engine.score.sections[0].out_value(4) >> click.in_pitch();
    zap >> engine.audio_out(0);
    zap >> engine.audio_out(1);        
    noise >> engine.audio_out(0);
    noise >> engine.audio_out(1);        
    click.out_L() >> engine.audio_out(0);
    click.out_R() >> engine.audio_out(1);        
    
    
    // -------------------GRAPHIC SETUP--------------
    ofBackground(0);
    ofSetFrameRate(30);

    // -------------------GUI------------------------
    gui.setup("panel", "settings.xml", 20, 20 );
    gui.add( mc.parameters );
    gui.add( zap.parameters );
    gui.add( noise.label("noise") );
    gui.add( click.label("click") );
    gui.loadFromFile(ofToDataPath("settings.xml"));
    gui.minimizeAll();

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
    
    ofDrawBitmapString("for changing the sequence\nedit bin/data/mc.chain", 20, ofGetHeight()-40 );  
                        
    mc.draw( 20, ofGetHeight()-80);
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
