
// Ducker.cpp
// Nicola Pisanti, MIT License, 2018

#include "Ducker.h"

void np::effect::Ducker::patch(){

    addModuleInput( "L", ducker.in_0() );
    addModuleInput( "R", ducker.in_1() );
    addModuleInput( "trig", ducker.in_trig() );
    addModuleOutput( "L", ducker.out_0() );
    addModuleOutput( "R", ducker.out_1() );
    
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

pdsp::Patchable & np::effect::Ducker::in_L() {
    return in("L");
}

pdsp::Patchable & np::effect::Ducker::in_R() {
    return in("R");
}

pdsp::Patchable & np::effect::Ducker::out_L() {
    return out("L");
}

pdsp::Patchable & np::effect::Ducker::out_R() {
    return out("R");
}

pdsp::Patchable & np::effect::Ducker::in_trig() {
    return in("trig");
}
