
#include "NoiseDevice.h"

void np::synth::NoiseDevice::patch() {
  
    addModuleInput ( "trig", trigger );
    addModuleInput ( "pitch", noise.in_pitch() );
    addModuleInput ( "decimate", noise.in_decimation() );
    addModuleInput ( "filter", filter.in_cutoff() );
    addModuleInput ( "hold", ampEnv.in_hold() );
    //addModuleInput ( "release", ampEnv.in_release() );

    addModuleOutput( "L", gain0 );
    addModuleOutput( "R", gain1 );
    
    noise.ch(0) >> filter.ch(0) >> amp0 >> gain0;
    noise.ch(1) >> filter.ch(1) >> amp1 >> gain1;
    
    trigger >> noise.in_trig();
    trigger >> ampEnv >> amp0.in_mod();
               ampEnv >> amp1.in_mod();
    trigger >> modEnv;
    trigger >> random0 >> randomPitch >> noise.in_pitch();
    trigger >> random1 >> randomDecimate >> noise.in_decimation();
    trigger >> random2 >> randomCutoff >> filter.in_cutoff();
    
    // map controls 
    noisePitchControl    >> noise.in_pitch();
    noiseDecimateControl >> noise.in_decimation();
    
    envAttackControl  >> ampEnv.in_attack();
    envHoldControl    >> ampEnv.in_hold();
    envReleaseControl >> ampEnv.in_release();
    
    modEnv.set(0.0f, 0.0f, 20.0f);
    modEnvVeloControl >> modEnv.in_velocity();
    modEnvAttackControl  >> modEnv.in_attack();
    modEnvReleaseControl >> modEnv.in_release();
    modEnv >> modFilterAmt >> filter.in_cutoff();
    modEnv >> modPitchAmt >> noise.in_pitch();
    modEnv >> modDecimateAmt >> noise.in_decimation();

    filterTypeControl   >> filter.in_mode();
    filterCutoffControl >> filter.in_cutoff();
    filterResoControl   >> filter.in_reso();

    bitsControl >> noise.in_bits();

    randomControl >> randomPitch.in_mod();
    randomControl >> randomDecimate.in_mod();
    randomCutoffControl >> randomCutoff.in_mod();
    
    gainControl >> dBtoLin  >> gain0.in_mod();    
                   dBtoLin  >> gain1.in_mod();    
    
    parameters.setName( "noise device");
    parameters.add( gainControl.set("gain", -12, -48, 24) );
    parameters.add( noisePitchControl.set ("pitch", -36, -36, 148) );
    parameters.add( noiseDecimateControl.set ("decimate", 160, 1, 160) );
    parameters.add( bitsControl.set ("bits", 24, 24, 1) );
    parameters.add( envAttackControl.set("attack",  3, 0, 200 ) );
    parameters.add( envHoldControl.set( "hold",  40, 0, 300 ) );
    parameters.add( envReleaseControl.set("release", 20, 5, 600 ) );
    parameters.add( filterTypeControl.set("filter mode", 0, 0, 5 ) );
    parameters.add( filterCutoffControl.set("filter cutoff", 136, 20, 136 ) );
    parameters.add( filterResoControl.set("filter reso", 0.0f, 0.0f, 1.0f ) );
    
    parameters.add( modEnvAttackControl.set("mod attack",  0, 0, 200 ) );
    parameters.add( modEnvReleaseControl.set("mod release", 50, 5, 600 ) );    
    parameters.add( modEnvVeloControl.set("mod velo", 0.5f, 0.0f, 1.0f ) );    
    parameters.add( modPitchAmt.set("mEnv to pitch", 0, 0, 120 ) );    
    parameters.add( modDecimateAmt.set("mEnv to decimation", 0, 0, 120 ) );    
    parameters.add( modFilterAmt.set("mEnv to filter", 0, 0, 120 ) );    
    parameters.add( randomControl.set("random", 0, 0, 66 ) );    
    parameters.add( randomCutoffControl.set("random cutoff", 0, 0, 66 ) );    
    
}

ofParameterGroup & np::synth::NoiseDevice::label( string name ){
    parameters.setName(name);
    return parameters;
}    
    
float np::synth::NoiseDevice::meter() const {
    return ampEnv.meter_output();
}

pdsp::Patchable & np::synth::NoiseDevice::in_trig(){
    return in("trig");
}
pdsp::Patchable & np::synth::NoiseDevice::in_pitch(){
    return in("pitch");
}
pdsp::Patchable & np::synth::NoiseDevice::in_cutoff(){
    return in("cutoff");
}
pdsp::Patchable & np::synth::NoiseDevice::in_decimate(){
    return in("decimate");
}

pdsp::Patchable & np::synth::NoiseDevice::ch( size_t index ) {
    pdsp::wrapChannelIndex( index );
    switch( index ){
        case 0: return out("L"); break;
        case 1: return out("R"); break;
    }
    return out("L"); 
}
