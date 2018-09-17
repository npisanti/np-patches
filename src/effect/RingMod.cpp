
// RingMod.cpp
// Nicola Pisanti, MIT License, 2018

#include "RingMod.h"

np::effect::RingMod::Submodule::Submodule(){
    addModuleInput( "signal", input );
    addModuleOutput( "signal", mix );
    
    input >>       mix.in_A();
    input >> rm >> mix.in_B();
}

void np::effect::RingMod::patch(){
    
    channels.resize(2);
    
    addModuleInput( "L", channels[0] );
    addModuleInput( "R", channels[1] );
    
    addModuleOutput( "L", channels[0] );
    addModuleOutput( "R", channels[1] );
    addModuleInput( "pitch", sine.in_pitch() );
    
    pitchControl >> sine.in_pitch();
                    sine >> b2u >> channels[0].rm.in_mod();
                            b2u >> channels[1].rm.in_mod();

    wetControl >> channels[0].mix.in_fade();
    wetControl >> channels[1].mix.in_fade();
    
    parameters.setName( "ring modulator" );
    parameters.add( pitchControl.set( "pitch", 72, 0, 148) );
    parameters.add( wetControl.set( "depth amt", 0.5f, 0.0f, 1.0f) );
    
    smoothing(50.0f);
}

void np::effect::RingMod::smoothing( float ms ){
    pitchControl.enableSmoothing( ms );
}

ofParameterGroup & np::effect::RingMod::label( std::string name ){
    parameters.setName(name);
    return parameters;
}

pdsp::Patchable& np::effect::RingMod::ch( size_t index ) {
    pdsp::wrapChannelIndex( index );
    return channels[index];
}

pdsp::Patchable& np::effect::RingMod::in_pitch() {
    return in("pitch");
}

