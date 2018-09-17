
// Ducker.cpp
// Nicola Pisanti, MIT License, 2018

#include "Ducker.h"

void np::effect::Ducker::patch(){

    addModuleInput( "signal", ducker.ch(0) );
    addModuleInput( "trig", ducker.in_trig() );
    addModuleOutput( "signal", ducker.ch(0) );
    
    attackControl  >> ducker.in_attack();
    holdControl    >> ducker.in_hold();
    releaseControl >> ducker.in_release();
    duckingControl >> ducker.in_ducking();
    veloControl    >> ducker.in_velocity();
    
    parameters.setName( "ducker" );
    parameters.add(duckingControl.set("ducking", -12, -48, 0) );
    parameters.add(attackControl.set("attack", 50, 10, 600));
    parameters.add(holdControl.set("hold", 0, 10, 600));
    parameters.add(releaseControl.set("release", 100, 20, 1200));
    parameters.add(veloControl.set("sensitivity", 0.0f, 0.0f, 1.0f));
}

ofParameterGroup & np::effect::Ducker::label( std::string name ){
    parameters.setName(name);
    return parameters;
}

float np::effect::Ducker::meter_env() const {
    return ducker.meter_env();
}

pdsp::Patchable & np::effect::Ducker::ch( size_t index ) {
    return ducker.ch( index );
}

pdsp::Patchable & np::effect::Ducker::in_trig() {
    return in("trig");
}
