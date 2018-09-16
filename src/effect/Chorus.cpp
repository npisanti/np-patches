
// Chorus.cpp
// Nicola Pisanti, MIT License, 2018

#include "Chorus.h"

void np::effect::Chorus::patch(){

    speedControl >> chorus.in_speed();
    depthControl >> chorus.in_depth();
    
    parameters.setName( "chorus" );
    parameters.add(speedControl.set("cho speed (hz)", 0.25f, 0.25f, 8.25f));
    parameters.add(depthControl.set("cho depth (ms)", 3.5f, 1.0f, 8.0f));
}


ofParameterGroup & np::effect::Chorus::label( std::string name ){
    parameters.setName(name);
    return parameters;
}

float np::effect::Chorus::meter_lfo() const {
    return chorus.meter_lfo();
}

pdsp::Patchable & np::effect::Chorus::in_L() {
    return chorus.in_0();
}

pdsp::Patchable & np::effect::Chorus::in_R() {
    return chorus.in_1();
}

pdsp::Patchable & np::effect::Chorus::out_L() {
    return chorus.out_0();
}

pdsp::Patchable & np::effect::Chorus::out_R() {
    return chorus.out_1();
}

