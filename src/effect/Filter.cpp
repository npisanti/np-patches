
// Filter.cpp
// Nicola Pisanti, MIT License, 2018

#include "Filter.h"

void np::effect::Filter::patch(){
    
    addModuleInput( "signal", filter.ch(0) );
    addModuleOutput( "signal", filter.ch(0) );
    
    addModuleInput( "cutoff", filter.in_cutoff() );
    
    cutoffControl    >> filter.in_cutoff();
    resoControl      >> filter.in_reso();
    modeControl      >> filter.in_mode();
    
    parameters.setName( "filter" );
    parameters.add( cutoffControl.set( "cutoff", 136, 20, 136) );
    parameters.add( resoControl.set( "reso", 0.0f, 0.0f, 1.0f) );
    parameters.add( modeControl.set( "filter type", 0, 0, 5) );
    
    smoothing(50.0f);
}

ofParameterGroup & np::effect::Filter::label( std::string name ){
    parameters.setName(name);
    return parameters;
}

float np::effect::Filter::meter_cutoff() const {
    return filter.meter_cutoff();
}

pdsp::Patchable& np::effect::Filter::in_cutoff() {
    return in("cutoff");
}

void np::effect::Filter::smoothing( float ms ){
    cutoffControl.enableSmoothing( ms );
    resoControl.enableSmoothing( ms );
}

pdsp::Patchable & np::effect::Filter::ch( size_t index ) {
    return filter.ch( index );
}
