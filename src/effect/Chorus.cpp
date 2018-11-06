
// Chorus.cpp
// Nicola Pisanti, MIT License, 2018

#include "Chorus.h"

void np::effect::Chorus::patch(){

    speedControl >> chorus.in_speed();
    depthControl >> chorus.in_depth();
    
    parameters.setName( "chorus" );
    parameters.add(speedControl.set("cho speed (hz)", 0.25f, 0.25f, 3.0f));
    parameters.add(depthControl.set("cho depth (ms)", 3.5f, 1.0f, 8.0f));
}


ofParameterGroup & np::effect::Chorus::label( std::string name ){
    parameters.setName(name);
    return parameters;
}

float np::effect::Chorus::meter_lfo() const {
    return chorus.meter_lfo();
}

pdsp::Patchable & np::effect::Chorus::ch( size_t index ) {
    return chorus.ch( index );
}
