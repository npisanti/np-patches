
// SoftClip.cpp
// Nicola Pisanti, MIT License, 2019

#include "SoftClip.h"

void np::dynamics::SoftClip::patch(){

    addModuleInput( "signal", inputGain );
    addModuleOutput( "signal", outputGain );

    clip.setOversampleLevel(2);
    inputGain >> upsampler >> clip >> downsampler >> outputGain;
    
    clipThreshold >> clip.in_threshold();
    
    parameters.setName( "soft clip" );
    parameters.add( inputGain.set("input gain", 0, -48, 36) );
    parameters.add( clipThreshold.set("clip threshold", -3, 0, -12) );
    parameters.add( outputGain.set("output gain", 0, -48, 24 ) );    
}


ofParameterGroup & np::dynamics::SoftClip::label( std::string name ){
    parameters.setName(name);
    return parameters;
}
