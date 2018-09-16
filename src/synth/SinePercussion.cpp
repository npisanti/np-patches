
// SinePercussion.cpp
// Nicola Pisanti, MIT License, 2018

#include "SinePercussion.h"

void np::synth::SinePercussion::patch(){
    addModuleInput("trig", triggers); 
    addModuleInput("pitch", osc.in_pitch()); 
    addModuleOutput("signal", gain ); 

    //patching
                     triggers >> aEnv >> amp.in_mod();
                                                           osc >> amp >> gain;
    
 
                                        pEnv >>fbModAmt >> osc.in_fb();
    triggers >> pEnv.set(0.0f, 0.0f,  50.0f) >> pModAmt >> osc.in_pitch();
                                           pitchControl >> osc.in_pitch();

    //triggers >> osc.in_sync();
    
    gainControl >> dBtoLin  >> gain.in_mod();
    veloToEnv       >> pEnv.in_velocity();
    pEnvAmtControl     >> pModAmt.in_mod();
    
    envAttackControl  >> aEnv.in_attack();
    envHoldControl    >> aEnv.in_hold();
    envReleaseControl >> aEnv.in_release();
    
    pEnvAttackControl  >> pEnv.in_attack();
    pEnvReleaseControl >> pEnv.in_release();
    
    fbControl >> osc.in_fb();
    fbModControl >> fbModAmt.in_mod();

    parameters.setName("sine percussion");
    parameters.add( gainControl.set("gain", -12, -48, 24) );
    parameters.add( pitchControl.set("pitch", 36, 24, 132 ));
    parameters.add( pEnvAmtControl.set("pitch env", 48, 0, 72 ) );
    parameters.add( pEnvAttackControl.set("p env attack",  0, 0, 200 ) );
    parameters.add( pEnvReleaseControl.set("p env release", 50, 5, 500 ) );
    parameters.add( veloToEnv.set( "p env dyn amt", 0.5f, 0.0f, 1.0f ) );
    parameters.add( fbControl.set( "feedback", 0.0f, 0.0f, 0.25f ) );
    parameters.add( fbModControl.set( "env to fb", 0.0f, 0.0f, 0.25f ) );
    parameters.add( envAttackControl.set("attack",  3, 0, 200 ) );
    parameters.add( envHoldControl.set( "hold",  40, 0, 300 ) );
    parameters.add( envReleaseControl.set("release", 20, 5, 600 ) );
    
}   

ofParameterGroup & np::synth::SinePercussion::label( string name ){
    parameters.setName( name );
    return parameters;
}

void np::synth::SinePercussion::longerEnvelopes() {
    envAttackControl.set("attack",  3, 0, 2000 );
    envHoldControl.set( "hold",  40, 0, 2000 );
    envReleaseControl.set("release", 20, 5, 4000 );
    pEnvAttackControl.set("p env attack",  0, 0, 2000 );
    pEnvReleaseControl.set("p env release", 50, 5, 2000 ); 
}

void np::synth::SinePercussion::enableDB( float minValue ){
    aEnv.enableDBTriggering( minValue, 0.0f);
}
void np::synth::SinePercussion::disableDB( ){
    aEnv.disableDBTriggering( );
}

float np::synth::SinePercussion::meter() const {
    return aEnv.meter_output();
}

pdsp::Patchable & np::synth::SinePercussion::in_trig() {
    return in("trig");
}
pdsp::Patchable & np::synth::SinePercussion::in_pitch() {
    return in("pitch");
}

