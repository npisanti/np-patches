
// FM2.cpp
// Nicola Pisanti, MIT License, 2018

#include "FM2.h"

ofParameterGroup & np::synth::FM2::setup( int numVoices, float spread, std::string name ){
    
    // -------------------------- PATCHING ------------------------------------
    voices.resize( numVoices );
 
    self_mod.channels(numVoices);
    fm_mod.channels(numVoices);
 
    for(int i=0; i<numVoices; ++i){
        // setup voice
        voices[i].setup( *this, i );
        
        if( spread==0.0f ){
            voices[i] >> gain.ch(0);
            voices[i] >> gain.ch(1);
        }else{
            float spreadamt = pdsp::spread( i, numVoices, spread );
            voices[i] * pdsp::panL(spreadamt )>> gain.ch(0);
            voices[i] * pdsp::panR(spreadamt )>> gain.ch(1);
        }
    }

    // CONTROLS ---------------------------------------------------------------
    parameters.setName( name );
    
    parameters.add(gain.set("gain", -12, -48, 12));
    
    parameters.add(ratio_ctrl.set( "ratio coarse", 1, 1, 13) );
    parameters.add(ratio_ctrl.set( "ratio fine", 0.0f, -1.0f, 0.0f) );
    parameters.add( fm_ctrl.set( "fm", 0.0f, 0.0f, 4.0f) );    
    parameters.add( fm_mod.set( "fm mod", 0.0f, 0.0f, 4.0f) );
    parameters.add( self_ctrl.set("self fb", 0.0f, 0.0f, 1.0f) );
    parameters.add( self_mod.set("self fb mod", 0.2f, 0.0f, 0.5f) );    
    parameters.add( drift.set("drift", 0.0f, 0.0f, 1.0f) );    
    
    parameters.add(env_attack_ctrl.set( "env attack", 400, 0, 4000) );
    parameters.add(env_decay_ctrl.set(  "env decay", 400, 5, 10000) );
    parameters.add(env_sustain_ctrl.set("env sustain", 1.0f, 0.0f, 1.0f) );
    parameters.add(env_release_ctrl.set("env release", 900, 5, 8000));    

    gain.enableSmoothing(50.f);
    self_ctrl.enableSmoothing( 200.0f );
    fm_ctrl.enableSmoothing( 200.0f );
    ratio_ctrl.enableSmoothing( 200.0f );
    
    return parameters;
}


void np::synth::FM2::Voice::setup( FM2 & m, int i ){

    addModuleInput("trig", voiceTrigger);
    addModuleInput("pitch", pitchNode);
    addModuleInput("fm", fmAmp.in_mod());
    addModuleInput("self", carrier.in_fb());
    addModuleOutput("signal", voiceAmp);

    
    pitchNode >> carrier.in_pitch();
    pitchNode >> modulator.in_pitch();
    m.ratio_ctrl >> modulator.in_ratio();
    m.fm_ctrl >> fmAmp.in_mod();
    m.self_ctrl >> carrier.in_fb();
    
    
    // SIGNAL PATH
    modulator >> fmAmp >> carrier.in_fm() >> voiceAmp;
    
        
    // MODULATIONS AND CONTROL
    envelope >> m.fm_mod.ch(i)   >> fmAmp.in_mod();
    envelope >> m.self_mod.ch(i) >> carrier.in_fb();
    
    voiceTrigger >> envelope >> voiceAmp.in_mod();

        m.env_attack_ctrl  >> envelope.in_attack();
        m.env_decay_ctrl   >> envelope.in_decay();
        m.env_sustain_ctrl >> envelope.in_sustain();
        m.env_release_ctrl >> envelope.in_release();


        
    0.2f    >> phazorFree;
    0.05f  >> randomSlew.in_freq();
                                       m.drift >> driftAmt.in_mod();        
    phazorFree.out_trig() >> rnd >> randomSlew >> driftAmt;
                                                  driftAmt >> carrier.in_pitch();
                                                  driftAmt >> modulator.in_pitch();
        
}

float np::synth::FM2::Voice::meter_mod_env() const{
    return envelope.meter_output();
}

float np::synth::FM2::Voice::meter_pitch() const{
    return carrier.meter_pitch();
}

pdsp::Patchable& np::synth::FM2::ch( size_t index ){
    pdsp::wrapChannelIndex( index );
    switch( index ){
        case 0: return gain.ch(0); break;
        case 1: return gain.ch(1); break;
    }
    return gain.ch(0);
}

pdsp::Patchable& np::synth::FM2::Voice::in_trig(){
    return in("trig");
}

pdsp::Patchable& np::synth::FM2::Voice::in_pitch(){
    return in("pitch");
}
pdsp::Patchable& np::synth::FM2::Voice::in_fm(){
    return in("fm");
}

pdsp::Patchable& np::synth::FM2::Voice::in_self(){
    return in("self");
}

ofParameterGroup & np::synth::FM2::label (std::string name ){
    parameters.setName( name );
    return parameters;
}
