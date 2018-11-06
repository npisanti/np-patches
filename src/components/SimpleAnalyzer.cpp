
#include "SimpleAnalyzer.h"

void np::SimpleAnalyzer::setup( int sampleRate, int frames, int id ) {
    
    engine.setChannels(2, 2);
    engine.audio_in(0) >> pm;    
    
    pm.activate( engine );

    gui.setup("", "sanalyzer.xml", ofGetWidth()-220, 20 );
    gui.setName("simple analyzer");
    pm.ui.setName("parameters");
    gui.add( pm.ui );
    gui.loadFromFile("sanalyzer.xml");
    

    engine.setDeviceID(id);
    engine.setup( sampleRate, frames, 3); 
    
}

float np::SimpleAnalyzer::meter() const {
    return pm.meter_output();
}

void np::SimpleAnalyzer::draw( ) {
    gui.draw();
    ofDrawRectangle( gui.getPosition().x, gui.getPosition().y - 30, meter()*200, 20 );
}

int np::SimpleAnalyzer::PeakMeter::number = 0;

np::SimpleAnalyzer::PeakMeter::PeakMeter( ){
    
    addModuleInput( "signal", gain );
    
    gain >> absolute >> envFollowerPeak ;
    
    peakAttackControl  >> envFollowerPeak.in_attack();
    peakReleaseControl >> envFollowerPeak.in_release();
        
    ui.setName( "peak meter " + ofToString(number++) );
    ui.add( gain.set( "band input gain", 0, -48, 24 ));  
    ui.add( peakAttackControl.set( "peak env attack ms", 5, 1, 25 ));
    ui.add( peakReleaseControl.set( "peak env release ms", 10, 1, 50 ));
    
}

float np::SimpleAnalyzer::PeakMeter::meter_output() const { 
    return envFollowerPeak.meter_output(); 
}
    
void np::SimpleAnalyzer::PeakMeter::activate( ofxPDSPEngine & engine ) {
     envFollowerPeak  >> engine.blackhole();
}
