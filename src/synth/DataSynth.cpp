
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
            voices[i] >> gain.in_0();
            voices[i] >> gain.in_1();
        }else{
            float spreadamt = pdsp::spread( i, numVoices, spread );
            voices[i] * pdsp::panL(spreadamt )>> gain.in_0();
            voices[i] * pdsp::panR(spreadamt )>> gain.in_1();
        }
    }
    
    
    // CONTROLS ----------------------------------------------------------------
    parameters.setName( name );

    parameters.add(filter_mode_ctrl.set("filter mode", 0, 0, 5) );
    parameters.add(cutoff_ctrl.set("filter cutoff", 120, 10, 120));
    parameters.add(reso_ctrl.set("filter reso", 0.0f, 0.0f, 1.0f) );
 
    cutoff_ctrl.enableSmoothing(200.0f);
    
    parameters.add(env_attack_ctrl.set( "env attack", 50, 5, 4000) );
    parameters.add(env_decay_ctrl.set(  "env decay", 400, 5, 1200) );
    parameters.add(env_sustain_ctrl.set("env sustain", 1.0f, 0.0f, 1.0f) );
    parameters.add(env_release_ctrl.set("env release", 900, 5, 5000));    
    parameters.add( env_filter_ctrl.set("env to filter", 30, 0, 60) );    

    parameters.add(gain.set("gain", -9, -48, 12));
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
    oscillator >> filter >> leakDC.out_hpf() >> voiceAmp;
    
    // MODULATIONS AND CONTROL
    voiceTrigger >> envelope >> voiceAmp.in_mod();
                    envelope >> envToFilter >> filter.in_pitch();
                              m.cutoff_ctrl >> filter.in_pitch();
                                m.reso_ctrl >> filter.in_reso();
                         m.filter_mode_ctrl >> filter.in_mode();


        m.env_attack_ctrl  >> envelope.in_attack();
        m.env_decay_ctrl   >> envelope.in_decay();
        m.env_sustain_ctrl >> envelope.in_sustain();
        m.env_release_ctrl >> envelope.in_release();

        m.env_filter_ctrl >> envToFilter.in_mod();
}

float np::synth::DataSynth::Voice::meter_mod_env() const{
    return envelope.meter_output();
}

float np::synth::DataSynth::Voice::meter_pitch() const{
    return oscillator.meter_pitch();
}

pdsp::Patchable& np::synth::DataSynth::out_L(){
    return gain.out_0();
}

pdsp::Patchable& np::synth::DataSynth::out_R(){
    return gain.out_1();
}

ofParameterGroup & np::synth::DataSynth::label (std::string name ){
    parameters.setName( name );
    return parameters;
}
