
// example_lead_reverb
// Nicola Pisanti, MIT License, 2018

// example to test BasiVerb and StereoDelay, 
// compile, play the keyboard and tweak the parameters

// requires a midi keyboard, remember to set the right port for midi

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("fm lead + rev/delay");
    engine.score.setTempo(86.0f); // the delay times are clocked

    lead.setup(1);

    midiKeys.setMonoMode(1, true);

    // activate portamento, in poly mode you can notice portamento only on note stealing
    midiKeys.setPortamento(pdsp::On, 150.0f, pdsp::Rate);

    midiKeys.outs_trig[0]  >> lead.voices[0].in("trig");
    midiKeys.outs_pitch[0] >> lead.voices[0].in("pitch");
    
    lead.out_L() >> reverb.in();
    lead.out_L() >> delays.in_L();
    lead.out_R() >> delays.in_R();    

    delays.out_L() >> reverb.in();
    delays.out_R() >> reverb.in();
    
    lead.out_L() >> engine.audio_out(0);
    lead.out_L() >> engine.audio_out(1);
    
    reverb.out_L() >> engine.audio_out(0);
    reverb.out_R() >> engine.audio_out(1);
  
    delays.out_L() >> engine.audio_out(0);
    delays.out_R() >> engine.audio_out(1);
  
    // graphic setup---------------------------
    ofSetVerticalSync(true);
    ofDisableAntiAliasing();
    ofBackground(0);
    ofSetColor(ofColor(0,100,100));
    ofNoFill();
    ofSetLineWidth(1.0f);
    ofSetFrameRate(24);

    // GUI -----------------------------------
    gui.setHeaderBackgroundColor(ofColor( 40, 0, 0));
    gui.setBorderColor(ofColor(40, 0, 0));
    
    gui.setDefaultFillColor(ofColor(255, 85, 85));
    gui.setDefaultBackgroundColor(ofColor(0,0,0));

    gui.setup("gui", "settings.xml", 20, 20);
    gui.add( lead.label("fm lead") );
    gui.add( reverb.parameters );
    gui.add( delays.parameters );
    gui.loadFromFile("settings.xml");
    
    // audio setup----------------------------
    // for our midi controllers to work we have to add them to the engine, so it know it has to process them
    midiIn.listPorts();
    midiIn.openPort(1); //set the right port !!!
    engine.addMidiController( midiKeys, midiIn ); // add midi processing to the engine
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3);     
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    gui.draw();
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
