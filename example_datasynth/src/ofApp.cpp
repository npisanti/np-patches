
#include "ofApp.h"

// the same of the standar example, but with library classes 

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("data to waveform");
    
    camWidth = 320;  // try to grab at this size.
    camHeight = 240;
    camPosX = 230;
    camPosY = 10;
    col = 160;			// col for getting pixels to wave
    mode = 0;
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber( camWidth, camHeight );
    
    waveplot.allocate(camHeight*2 + 4, 170);
    
    //patching-------------------------------
    keyboard.setPolyMode( 8 );

    int voicesNum = keyboard.getVoicesNumber();
    
    dtsynth.datatable.setup( camHeight, camHeight ); // as many samples as the webcam width
	//dtsynth.datatable.smoothing(0.5f);

    dtsynth.setup( voicesNum );
    for(int i=0; i<voicesNum; ++i){
        // connect each voice to a pitch and trigger output
        keyboard.out_trig(i)  >> dtsynth.voices[i].in("trig");
        keyboard.out_pitch(i) >> dtsynth.voices[i].in("pitch");
    }
    
    dtsynth.ch(0) >> chorus.ch(0) >> engine.audio_out(0);
    dtsynth.ch(0) >> chorus.ch(1) >> engine.audio_out(1);

    // graphic setup---------------------------
    ofSetVerticalSync(true);
    ofDisableAntiAliasing();
    ofBackground(0);
    ofSetColor(ofColor(0,100,100));
    ofNoFill();
    ofSetLineWidth(1.0f);
    ofSetFrameRate(24);

    // GUI -----------------------------------
    gui.setup("panel", "settings.xml", 10, camPosY);
    gui.add( dtsynth.parameters );
    gui.add( smooth.set("wave smoothing", 0.0f, 0.0f, 0.95f) );
    gui.add( chorus.parameters );
    smooth.addListener(this, &ofApp::smoothCall );
    smooth.set(0.3f);
    
    // audio / midi setup----------------------------

    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3);     
    
}

//--------------------------------------------------------------
void ofApp::update(){
    vidGrabber.update();
    
    if(vidGrabber.isFrameNew() && dtsynth.datatable.ready() ){
		
        ofPixels & pixels = vidGrabber.getPixels();
        
        // ------------------ GENERATING THE WAVE ----------------------
		switch( mode ){
			case 0: // converting pixels to waveform samples
				dtsynth.datatable.begin();
				for(int n=0; n<camHeight; ++n){
					float sample = ofMap(pixels.getData()[col*3 + channel + n*3*camWidth], 0, 255, -0.5f, 0.5f);
					dtsynth.datatable.data(n, sample);
				}
				dtsynth.datatable.end(false);
			break; // remember, raw waveform could have DC offsets, we have filtered them in the dtsynth using an hpf
			
			case 1: // converting pixels to partials for additive synthesis
				dtsynth.datatable.begin();
				for(int n=0; n<camHeight; ++n){
					float partial = ofMap(pixels.getData()[col*3 + channel + n*3*camWidth], 0, 255, 0.0f, 1.0f);
					dtsynth.datatable.data(n, partial);
				}
				dtsynth.datatable.end(true);
			break;
		}
		
		// ----------------- PLOTTING THE WAVEFORM ---------------------
		waveplot.begin();
		ofClear(0, 0, 0, 0);
		
		ofSetColor(255);
		ofDrawRectangle(1, 1, waveplot.getWidth()-2, waveplot.getHeight()-2);
		ofTranslate(2, 2);
		switch( mode ){
			case 0: // plot the raw waveforms
				ofBeginShape();
				for(int n=0; n<camHeight; ++n){
					float y = ofMap(pixels.getData()[col*3 + channel + n*3*camWidth], 0, 255, camHeight, 0);
					ofVertex( n*2, y );
				}
				ofEndShape();
			break;
			
			case 1: // plot the partials
				for(int n=0; n<camHeight; ++n){
					float partial = ofMap(pixels.getData()[col*3 + channel + n*3*camWidth], 0, 255, 0.0f, 1.0f);
					int h = waveplot.getHeight() * partial;
					int y = waveplot.getHeight() - h;
					ofDrawLine(n*2, y, n*2, camHeight );
				}
			break;
		}
		waveplot.end();
		
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    // draw GUI
    gui.draw();

	ofPushMatrix();
    // draw some keyboard keys and infos
	switch(channel){
		case 0: ofSetColor(255, 0, 0); break;
		case 1: ofSetColor(0, 255, 0); break;
		case 2: ofSetColor(0, 0, 255); break;
		default: break;
	}
	ofTranslate(camPosX, camPosY);
    vidGrabber.draw(0, 0);
    
    string info = "press spacebar to change color channel\n";
    info += "datatable mode (press m to change): ";
    switch(mode){
		case 0: info+="raw waveform\n"; break;
		case 1: info+="additive synthesis\n"; break;
		default: break;
	}
	info += "click on the webcam image to change aquired pixel line";
	
	ofTranslate( 0, camHeight );
    ofDrawBitmapString(info, 0, 10);

	ofTranslate( 0, 50 );
	waveplot.draw(0, 0);
    ofPopMatrix();

    keyboard.draw( 10, 350, 200, 110);

	ofTranslate(camPosX, camPosY);
    ofSetColor(255);
    ofDrawLine(  col, 0, col, camHeight );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    keyboard.keyPressed( key ); 

    switch(key){
		case ' ':
			channel++;
			if(channel==3) channel = 0;
		break;
		case 'm': case 'M':
			mode++;
			if(mode==2) mode = 0;
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    keyboard.keyReleased( key );
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if( x> camPosX && x< camPosX+camWidth && y>camPosY && y<camPosY+camHeight){
		col = x - camPosX;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if( x> camPosX && x< camPosX+camWidth && y>camPosY && y<camPosY+camHeight){
		col = x - camPosX;
	}
}

//--------------------------------------------------------------
void ofApp::smoothCall( float & value ) {
	dtsynth.datatable.smoothing( value  );
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){}
//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){}
//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}
