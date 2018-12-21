
// DataSynth.cpp
// Nicola Pisanti, MIT License, 2018

#include "DataSynth.h"

ofParameterGroup & np::synth::DataSynth::setup(int numVoices, float spread, std::string name ){

    // -------------------------- PATCHING ------------------------------------
    voices.resize( numVoices );
 
    for(int i=0; i<numVoices; ++i){
        // setup voice
        voices[i].setup( *this );
        
        if( spread==0.0f ){
            voices[i] >> gain.ch(0);
            voices[i] >> gain.ch(1);
        }else{
            float spreadamt = pdsp::spread( i, numVoices, spread );
            voices[i] * pdsp::panL(spreadamt )>> gain.ch(0);
            voices[i] * pdsp::panR(spreadamt )>> gain.ch(1);
        }
    }
    
    cutoff_ctrl >> cutoff_p2f;
    
    // CONTROLS ----------------------------------------------------------------
    parameters.setName( name );

    parameters.add(gain.set("gain", -9, -48, 12));
    smooth.addListener(this, &np::synth::DataSynth::smoothCall );
    parameters.add( smooth.set("wave smoothing", 0.4f, 0.0f, 0.95f) );
    parameters.add( cutoff_ctrl.set("filter cutoff", 136, 10, 136) ); 
    parameters.add( env_attack_ctrl.set( "env attack", 50, 0, 4000) );
    parameters.add( env_decay_ctrl.set(  "env decay", 400, 5, 1200) );
    parameters.add( env_sustain_ctrl.set("env sustain", 1.0f, 0.0f, 1.0f) );
    parameters.add( env_release_ctrl.set("env release", 900, 5, 5000));    
    parameters.add( drift.set("drift", 0.05f, 0.0f, 1.0f) );   

    gain.enableSmoothing(50.f);
    // ------------------------------------------------------------------------
    
    return parameters;
}

pdsp::Patchable& np::synth::DataSynth::Voice::in_trig(){
    return in("trig");
}

pdsp::Patchable& np::synth::DataSynth::Voice::in_pitch(){
    return in("pitch");
}

void np::synth::DataSynth::Voice::setup( DataSynth & m ){

    addModuleInput("trig", voiceTrigger);
    addModuleInput("pitch", oscillator.in_pitch());
    addModuleOutput("signal", voiceAmp);

    oscillator.setTable( m.datatable );
    
    // we filter the frequency below 20 hz (not audible) just to remove DC offsets
    leakDC.set(20.0f);
    // SIGNAL PATH
    oscillator >> leakDC.out_hpf() >> filter.out_lpf() >> voiceAmp;
    
    m.cutoff_p2f >> filter.in_freq();
    
    // MODULATIONS AND CONTROL
    voiceTrigger >> envelope >> voiceAmp.in_mod();

    m.env_attack_ctrl  >> envelope.in_attack();
    m.env_decay_ctrl   >> envelope.in_decay();
    m.env_sustain_ctrl >> envelope.in_sustain();
    m.env_release_ctrl >> envelope.in_release();

    0.2f    >> phazorFree;
    0.05f  >> randomSlew.in_freq();
                                       m.drift >> driftAmt.in_mod();        
    phazorFree.out_trig() >> rnd >> randomSlew >> driftAmt;
                                                  driftAmt >> oscillator.in_pitch();
}

float np::synth::DataSynth::Voice::meter_mod_env() const{
    return envelope.meter_output();
}

float np::synth::DataSynth::Voice::meter_pitch() const{
    return oscillator.meter_pitch();
}

pdsp::Patchable& np::synth::DataSynth::ch( size_t index ){
    pdsp::wrapChannelIndex( index );
    switch( index ){
        case 0: return gain.ch(0); break;
        case 1: return gain.ch(1); break;
    }
    return gain.ch(0);
}

ofParameterGroup & np::synth::DataSynth::label (std::string name ){
    parameters.setName( name );
    return parameters;
}

void np::synth::DataSynth::smoothCall( float & value ) {
	datatable.smoothing( value  );
}
