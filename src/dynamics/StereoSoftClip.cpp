
// SoftClip.cpp
// Nicola Pisanti, MIT License, 2019

#include "StereoSoftClip.h"

void np::dynamics::StereoSoftClip::patch(){

    addModuleInput( "signal", inputGain );
    addModuleOutput( "signal", outputGain );

    clip0.setOversampleLevel(2);
    clip1.setOversampleLevel(2);
    
    chan0.input >> inputGain.ch(0) >> upsampler0 >> clip0 >> downsampler0 >> outputGain.ch(0) >> chan0.output;
    
    chan1.input >> inputGain.ch(1) >> upsampler1 >> clip1 >> downsampler1 >> outputGain.ch(1) >> chan1.output;
    
    clipThreshold >> clip0.in_threshold();
    clipThreshold >> clip1.in_threshold();
    
    parameters.setName( "soft clip" );
    parameters.add( inputGain.set("input gain", 0, -48, 36) );
    parameters.add( clipThreshold.set("clip threshold", -3, 0, -12) );
    parameters.add( outputGain.set("output gain", 0, -48, 24 ) );    
}

pdsp::Patchable & np::dynamics::StereoSoftClip::ch( int i ){
    switch( i ){
        default: 
            std::cout<<"[pdsp] wrong index for StereoSoftClip ch, returning ch 0 \n"; 
        case 0:
            return chan0;
            break;
        case 1:
            return chan1;
            break;
    }
}

ofParameterGroup & np::dynamics::StereoSoftClip::label( std::string name ){
    parameters.setName(name);
    return parameters;
}
