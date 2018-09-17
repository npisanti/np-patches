
// Compressor.cpp
// Nicola Pisanti, MIT License, 2018

#include "Compressor.h"

np::dynamics::Compressor::Submodule::Submodule(){
    addModuleInput("signal", input );
    addModuleOutput( "signal", output);
}


void np::dynamics::Compressor::patch(){
    
    submodules.resize(2);

    addModuleInput( "signal", submodules[0].input );
    addModuleInput( "R", submodules[1].input );
    addModuleOutput( "signal", submodules[0].output );
    addModuleOutput( "R", submodules[1].output );
    
    comp.RMS( 25.0f );
    comp.analog();
    
    submodules[0].input >>comp.ch(0) >> makeup.ch(0) >> submodules[0].output;
    submodules[1].input >>comp.ch(1) >> makeup.ch(1) >> submodules[1].output;

    attackControl       >> comp.in_attack();
    releaseControl      >> comp.in_release();
    thresholdControl    >> comp.in_threshold();
    ratioControl        >> comp.in_ratio(); 
    
    parameters.setName( "rms compressor" );
    parameters.add(thresholdControl.set("threshold", 0, 0, -24));
    parameters.add(attackControl.set("attack", 10, 1, 200));
    parameters.add(releaseControl.set("release", 48, 10, 1000));
    parameters.add(ratioControl.set("ratio", 2, 1, 12));
    parameters.add(kneeControl.set("knee", 0, 0, 6));
    parameters.add(makeup.set("makeup gain", 0, -6, 12));
}

void np::dynamics::Compressor::enableScope( ofxPDSPEngine & engine ){   
    submodules[0].input  >> scope >> engine.blackhole();
    scope.set(512*8); 
}
    
void np::dynamics::Compressor::drawScope( int x, int y, int w, int h ) {
    ofPushMatrix();
    ofTranslate( x, y );
        ofDrawBitmapString( "limiter scope", 2, 14 );
        scope.draw( 0, 0, w, h);
        float yl = (1.0f-dB(threshold())) * h * 0.5f;        
        ofDrawLine(0, yl, w, yl );
        ofDrawLine(0, h-yl, w, h-yl );
    ofPopMatrix();
}

void np::dynamics::Compressor::drawMeter( int x, int y, int w, int h ) {
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

void np::dynamics::Compressor::draw( int x, int y, int w, int h ){
    int hscope = h - 30;
    drawScope( x, y, w, hscope );
    drawMeter( x, y+hscope+10, w, 20 );
}

ofParameterGroup & np::dynamics::Compressor::label( std::string name ){
    parameters.setName(name);
    return parameters;
}

float np::dynamics::Compressor::meter_GR() const {
    return comp.meter_GR();
}

pdsp::Patchable & np::dynamics::Compressor::ch( size_t index ) {
    pdsp::wrapChannelIndex( index );
    return submodules[ index ];
}
