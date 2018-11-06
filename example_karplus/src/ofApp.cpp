#include "ofApp.h"

// app_karplus
// Nicola Pisanti, GPLv3 License, 2016

// midi controlled polysynth example
// you need a midi keyboard for trying this! 
// remember also to select the right midi port and audio output device, as ususal.

//--------------------------------------------------------------
void ofApp::setup(){
    
    //patching-------------------------------

#ifdef OFAPP_KARPLUS_MIDI
    midiKeys.setPolyMode(8, 1);
    int voicesNum = midiKeys.getVoicesNumber();    
#else
    keyboard.setPolyMode(8);
    int voicesNum = keyboard.getVoicesNumber();       
#endif

    ksynth.setup(voicesNum);
    
    for( size_t i=0; i<ksynth.size(); ++i ){
#ifdef OFAPP_KARPLUS_MIDI
        midiKeys.outs_trig[i]  >> ksynth.in_trig( i );
        midiKeys.outs_pitch[i] >> ksynth.in_pitch( i );
#else
        keyboard.outs_trig[i]  >> ksynth.in_trig( i );
        keyboard.outs_pitch[i] >> ksynth.in_pitch( i );
#endif
    }
    
    ksynth.ch(0) >> chorus.ch(0) >> engine.audio_out(0);
    ksynth.ch(1) >> chorus.ch(1) >> engine.audio_out(1);

  
    // graphic setup---------------------------
    ofSetVerticalSync(true);
    ofDisableAntiAliasing();
    ofBackground(0);
    ofSetColor(ofColor(0,100,100));
    ofNoFill();
    ofSetLineWidth(1.0f);
    ofSetFrameRate(24);
    
#ifdef OFAPP_KARPLUS_MIDI     
    ofSetWindowShape(230, 350);
#else
    ofSetWindowShape(230, 530);
#endif 

    // GUI -----------------------------------
    gui.setHeaderBackgroundColor(ofColor(0,40,40));
    gui.setBorderColor(ofColor(0,40,40));
    
    gui.setDefaultFillColor(ofColor(0,90,90));
    gui.setDefaultBackgroundColor(ofColor(0,0,0));

    gui.setup("panel");
    gui.setName("karplus synth gui");
    gui.add( ksynth.parameters );
    gui.add( chorus.parameters );
    gui.setPosition(15, 20);
    
    
    // audio / midi setup----------------------------
#ifdef OFAPP_KARPLUS_MIDI    
    //get MIDI control
    midiIn.listPorts();
    midiIn.openPort(1); //set the right port !!!
    // for our midi controllers to work we have to add them to the engine, so it know it has to process them
    engine.addMidiController( midiKeys, midiIn ); // add midi processing to the engine
#endif

    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3);     
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    // draw GUI
    gui.draw();

#ifndef OFAPP_KARPLUS_MIDI
    // draw some keyboard keys and infos
    keyboard.draw(14, 330, 200, 180);
#endif

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
#ifndef OFAPP_KARPLUS_MIDI
    // sends key messages to ofxPDSPComputerKeyboard
    keyboard.keyPressed( key );
#endif
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
#ifndef OFAPP_KARPLUS_MIDI
    // sends key messages to ofxPDSPComputerKeyboard
    keyboard.keyReleased( key );
#endif
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
