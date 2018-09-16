
// Cruncher.cpp
// Nicola Pisanti, MIT License, 2018

#include "Cruncher.h"

void np::effect::Cruncher::patch(){
    
    addModuleInput( "L", decimate0 );
    addModuleInput( "R", decimate1 );
    
    addModuleOutput( "L", bitcrunch0 );
    addModuleOutput( "R", bitcrunch1 );
    addModuleInput( "mod", cutoffModAmt );
    
    cutoffControl >> p2f >> decimate0.in_freq();
                     p2f >> decimate1.in_freq();
                     
    bitsControl      >> bitcrunch0.in_bits();
    bitsControl      >> bitcrunch1.in_bits();
    
    cutoffModAmt     >> p2f;
    cutoffModControl >> cutoffModAmt.in_mod();
    
    decimate0 >> bitcrunch0;
    decimate1 >> bitcrunch1;
    
    parameters.setName( "cruncher" );
    parameters.add( cutoffControl.set( "decimate", 152, 20, 148) );
    parameters.add( cutoffModControl.set( "decimate mod", 0, 0, 127) );
    parameters.add( bitsControl.set( "bits", 24, 24, 1) );
    
    smoothing(50.0f);
}

ofParameterGroup & np::effect::Cruncher::label( std::string name ){
    parameters.setName(name);
    return parameters;
}

float np::effect::Cruncher::meter_decimate_pitch() const {
    return p2f.meter_input();
}

pdsp::Patchable& np::effect::Cruncher::in_decimate_mod() {
    return in("mod");
}

void np::effect::Cruncher::smoothing( float ms ){
    cutoffControl.enableSmoothing( ms );
    bitsControl.enableSmoothing( ms );
}


pdsp::Patchable & np::effect::Cruncher::in_L() {
    return in("L");
}

pdsp::Patchable & np::effect::Cruncher::in_R() {
    return in("R");
}

pdsp::Patchable & np::effect::Cruncher::out_L() {
    return out("L");
}

pdsp::Patchable & np::effect::Cruncher::out_R() {
    return out("R");
}
