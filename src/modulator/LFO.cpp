
// LFO.cpp
// Nicola Pisanti, MIT License, 2019

#include "LFO.h"


ofParameterGroup & np::modulator::LFO::label( std::string name ){
    parameters.setName(name);
    return parameters;
}

void np::modulator::LFO::patch(){
    
    addModuleInput( "pitch",  p2f );
    addModuleOutput( "signal", amountControl );

    lfoSwitch >> amountControl;
    pitchControl >> p2f >> lfo.in_freq();
    shapeControl >> lfoSwitch.in_select();
    
    lfoSwitch.resize(5);  // resize input channels
    lfo.out_triangle()          >> lfoSwitch.input(0); // you cannot use this input() method in a chain
    lfo.out_saw()               >> lfoSwitch.input(1); // because: API reasons
    lfo.out_square()            >> lfoSwitch.input(2);
    lfo.out_sine()              >> lfoSwitch.input(3);
    lfo.out_sample_and_hold()   >> lfoSwitch.input(4);
    
    parameters.setName( "LFO" );
    parameters.add( pitchControl.set( "speed", -20, -120, 29 ) );
    parameters.add( shapeControl.set( "shape", 0, 0, 4) );
    parameters.add( amountControl.set( "amount", .0f, 0.0f, 1.0f) );
        
}

float np::modulator::LFO::meter_output() const {
    return lfoSwitch.meter_output();
}
