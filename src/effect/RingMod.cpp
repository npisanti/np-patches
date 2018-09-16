
// RingMod.cpp
// Nicola Pisanti, MIT License, 2018

#include "RingMod.h"

void np::effect::RingMod::patch(){
    
    addModuleInput( "L", rm0 );
    addModuleInput( "R", rm1 );
    
    addModuleOutput( "L", wet0 );
    addModuleOutput( "R", wet1 );
    addModuleInput( "pitch", sine.in_pitch() );
    
    pitchControl >> sine.in_pitch();
                    sine >> rm0.in_mod() >> wet0.in_1();
                    sine >> rm1.in_mod() >> wet1.in_1();
    
    wetControl >> wet0.in_fade();
    wetControl >> wet1.in_fade();
    
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

pdsp::Patchable& np::effect::RingMod::in_L() {
    return in("L");
}

pdsp::Patchable& np::effect::RingMod::in_R() {
    return in("R");
}

pdsp::Patchable& np::effect::RingMod::out_L() {
    return out("L");
}

pdsp::Patchable& np::effect::RingMod::out_R() {
    return out("R");
}

pdsp::Patchable& np::effect::RingMod::in_pitch() {
    return in("pitch");
}

