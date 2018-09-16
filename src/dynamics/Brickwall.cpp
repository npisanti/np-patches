
// Brickwall.cpp
// Nicola Pisanti, MIT License, 2018

#include "Brickwall.h"

void np::dynamics::Brickwall::patch(){

    addModuleInput( "L", input0 );
    addModuleInput( "R", comp.in_1() );
    addModuleOutput( "L", downsampler0 );
    addModuleOutput( "R", downsampler1 );

    comp.digital( true );
    
    input0 >> comp.in_0();
    
    comp.out_0() >> makeup.in_0(); 
    comp.out_1() >> makeup.in_1();

    makeup.out_0() >> upsampler0;
    makeup.out_1() >> upsampler1;
    clip0.setOversampleLevel(2);
    clip1.setOversampleLevel(2);
    upsampler0 >> clip0 >> downsampler0;
    upsampler1 >> clip1 >> downsampler1;   

    attackControl       >> comp.in_attack();
    releaseControl      >> comp.in_release();
    thresholdControl    >> comp.in_threshold();

    45.0f               >> comp.in_ratio(); // limiter

    clipThreshold >> clip0.in_threshold();
    clipThreshold >> clip1.in_threshold();
    
    parameters.setName( "brickwall limiter" );
    
    parameters.add(thresholdControl.set("limiter threshold", 0, 0, -24));
    parameters.add(attackControl.set("attack", 20, 1, 50));
    parameters.add(releaseControl.set("release", 100, 10, 200));
    parameters.add(kneeControl.set("knee", 1.0f, 0.0f, 3.0f));
    parameters.add(makeup.set("makeup gain", 0, -6, 12));
    parameters.add(clipThreshold.set("soft clip threshold", -2, 0, -9));
    
}

void np::dynamics::Brickwall::enableScope( ofxPDSPEngine & engine ){   
    clip0 >> scope >> engine.blackhole();
    scope.set(512*8); 
}
    
void np::dynamics::Brickwall::drawScope( int x, int y, int w, int h ) {
    ofPushMatrix();
    ofTranslate( x, y );
        ofDrawBitmapString( "limiter scope", 2, 14 );
        scope.draw( 0, 0, w, h);
        float yl = (1.0f-dB(threshold())) * h * 0.5f;        
        ofDrawLine(0, yl, w, yl );
        ofDrawLine(0, h-yl, w, h-yl );
    ofPopMatrix();
}

void np::dynamics::Brickwall::drawMeter( int x, int y, int w, int h ) {
    ofPushMatrix();
    ofTranslate( x, y );
        float wm = ofMap(comp.meter_GR(), 0, -9, 0, w );
        ofNoFill();
        ofDrawRectangle( 0, 0, w, h);
        ofFill();
        ofDrawRectangle( 0, 0, wm, h);
        float x0 = 220*0.33333333f;
        float x1 = 220*0.66666666f;
        ofDrawLine( x0, 0, x0, h );
        ofDrawLine( x1, 0, x1, h );
        
                
        ofDrawBitmapString( "GR", 2, 14 );
        ofDrawBitmapString( "3", x0-8, 14 );
        ofDrawBitmapString( "6", x1-8, 14 );
        ofDrawBitmapString( "9", w-8, 14 );
    ofPopMatrix();
}

void np::dynamics::Brickwall::draw( int x, int y, int w, int h ){
    int hscope = h - 30;
    drawScope( x, y, w, hscope );
    drawMeter( x, y+hscope+10, w, 20 );
}

ofParameterGroup & np::dynamics::Brickwall::label( std::string name ){
    parameters.setName(name);
    return parameters;
}

float np::dynamics::Brickwall::meter_GR() const {
    return comp.meter_GR();
}

pdsp::Patchable & np::dynamics::Brickwall::in_L() {
    return in("L");
}

pdsp::Patchable & np::dynamics::Brickwall::in_R() {
    return in("R");
}

pdsp::Patchable & np::dynamics::Brickwall::out_L() {
    return out("L");
}

pdsp::Patchable & np::dynamics::Brickwall::out_R() {
    return out("R");
}
