
// Cruncher.cpp
// Nicola Pisanti, MIT License, 2018

#include "Cruncher.h"

void np::effect::Cruncher::patch(){
    channels.resize(2);
    
    addModuleInput( "L", channels[0].input );
    addModuleInput( "R", channels[1].input );
    
    addModuleOutput( "L", channels[0].output );
    addModuleOutput( "R", channels[1].output );
    addModuleInput( "mod", cutoffModAmt );
    
    cutoffControl >> p2f >> decimate0.in_freq();
                     p2f >> decimate1.in_freq();
                     
    bitsControl      >> bitcrunch0.in_bits();
    bitsControl      >> bitcrunch1.in_bits();
    
    cutoffModAmt     >> p2f;
    cutoffModControl >> cutoffModAmt.in_mod();
    
    channels[0].input >> decimate0 >> bitcrunch0 >> channels[0].output;
    channels[1].input >> decimate1 >> bitcrunch1 >> channels[1].output;
    
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

pdsp::Patchable & np::effect::Cruncher::ch( size_t index ){
    pdsp::wrapChannelIndex( index );
    return channels[index];
}
