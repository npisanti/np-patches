
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle( "graindrone");
    
    ofxGuiSetFillColor(ofColor(255, 80, 80));
    
    drone.setup( 420, 80 );
    drone.addSample( sample0 );
    drone.addSample( sample1 );
    drone.addSample( sample2 );
    drone.addSample( sample3 );
    drone.smoothing( 100.0f );

    drone.ch(0) >> engine.audio_out(0);
    drone.ch(1) >> engine.audio_out(1);
    
    
    gui.setup( "gui", "settings.xml");
    gui.add( drone.parameters );
    gui.setPosition( 20, 120 );
    gui.loadFromFile("joystick.xml");
   
    samples.setup( "samples", "samples.xml", 240, 120);   
    samples.add( sample0.parameters );
    samples.add( sample1.parameters );
    samples.add( sample2.parameters );
    samples.add( sample3.parameters );
    samples.loadFromFile( "samples.xml");
    
    //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID( 0 );
    engine.setup( 44100, 512, 3); 

}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground( 0 ); 
    ofSetColor( 255, 80, 80 );
    gui.draw();
    samples.draw();
    drone.draw( 20, 20 );
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){ }

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseMoved(int x, int y ){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){ }
//--------------------------------------------------------------
