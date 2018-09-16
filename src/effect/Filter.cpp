
// Filter.cpp
// Nicola Pisanti, MIT License, 2018

#include "Filter.h"

void np::effect::Filter::patch(){
    
    addModuleInput( "L", filter.in_0() );
    addModuleInput( "R", filter.in_1() );
    
    addModuleOutput( "L", filter.out_0() );
    addModuleOutput( "R", filter.out_1() );
    addModuleInput( "cutoff", filter.in_cutoff() );
    
    cutoffControl    >> filter.in_cutoff();
    resoControl      >> filter.in_reso();
    
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

pdsp::Patchable & np::effect::Filter::in_L() {
    return in("L");
}

pdsp::Patchable & np::effect::Filter::in_R() {
    return in("R");
}

pdsp::Patchable & np::effect::Filter::out_L() {
    return out("L");
}

pdsp::Patchable & np::effect::Filter::out_R() {
    return out("R");
}
