
#include "ofApp.h"

// ofxPDSP example but with ofx::patch classes

//--------------------------------------------------------------
void ofApp::setup(){
    
    //patching-------------------------------
    // WAVETABLE 
    wavesynth.wavetable.setVerbose( true ); // activate logs of waveform loadings and size increments
    
    wavesynth.wavetable.setup( 512, 128 ); // 512 samples, 128 max partials

    wavesynth.wavetable.addSawWave( pdsp::highestPartial(60.0f) ); // this saw wave won't alias before C3=60
    wavesynth.wavetable.addSquareWave( pdsp::highestPartial(72.0f) ); // this square wave won't alias before C4=72
    wavesynth.wavetable.addTriangleWave( 64 ); // Triangle wave with 64 partials
    wavesynth.wavetable.addSineWave();
    wavesynth.wavetable.addAdditiveWave ( { 1.0f, 1.0f, 1.0f, 1.0f } ); 
    wavesynth.wavetable.addAdditiveWave ({ 1.0f, 0.0f, -1.0f, 0.5f, 0.5f, 1.0f, -1.0f, 0.5f, 0.5f, 1.0f, -1.0f, 0.5f, 0.5f, 1.0f, -1.0f, 0.5f }, true ); 
      

    keyboard.setPolyMode( 8 );
    wavesynth.setup( keyboard.getVoicesNumber() );

    for(size_t i=0; i<wavesynth.voices.size(); ++i){
        // connect each voice to a pitch and trigger output
        keyboard.out_trig(i)  >> wavesynth.voices[i].in("trig");
        keyboard.out_pitch(i) >> wavesynth.voices[i].in("pitch");
        lfo >> wavesynth.voices[i].in("table");
    }
    
    wavesynth.out_L() >> chorus.in_R(); chorus.out_L() >> engine.audio_out(0);
    wavesynth.out_R() >> chorus.in_L(); chorus.out_R() >> engine.audio_out(1);


    // graphic setup---------------------------
    ofSetVerticalSync(true);
    ofDisableAntiAliasing();
    ofBackground(0);
    ofSetColor(ofColor(0,100,100));
    ofNoFill();
    ofSetLineWidth(1.0f);
    ofSetFrameRate(24);

    // GUI -----------------------------------
    ofColor mainColor = ofColor(255, 95, 95);
    gui.setHeaderBackgroundColor(mainColor);
    gui.setBorderColor(ofColor(40,40,40));
    gui.setDefaultFillColor(mainColor.getLerped(ofColor::black, 0.4f));
    gui.setDefaultBackgroundColor(ofColor(0,0,0));
    
    gui.setup("panel");
    gui.add( wavesynth.parameters );
    gui.add( lfo.setup( "wavetable mod", 4.0f) );
    gui.add( chorus.parameters );
    gui.setPosition(15, 20);
    gui.loadFromFile("settings.xml");
    
    // audio / midi setup----------------------------
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3);     
    
    engine.score.play();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    // draw GUI
    gui.draw();
    ofSetColor( 255, 95, 95 );
    
    // draw some keyboard keys and infos
    keyboard.draw(14, ofGetHeight() - 200, 200, 180);

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // sends key messages to ofxPDSPComputerKeyboard
    keyboard.keyPressed( key ); 

    if(key==' '){ // pause / play
        if(engine.score.isPlaying()){
            engine.score.pause();
        }else{
            engine.score.play();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    // sends key messages to ofxPDSPComputerKeyboard
    keyboard.keyReleased( key );
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
