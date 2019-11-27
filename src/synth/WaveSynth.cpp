
// WaveSynth.cpp
// Nicola Pisanti, MIT License, 2018

#include "WaveSynth.h"

ofParameterGroup & np::synth::WaveSynth::setup( int numVoices, float spread, std::string name ){
    
    // -------------------------- PATCHING ---------------------------
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

    // CONTROLS ------------------------------------------------------
    parameters.setName( name );
    parameters.add(table_ctrl.set("table index", 8.0f, 0.0f, (float)(wavetable.size()-1) ) );
    table_ctrl.enableSmoothing(200.0f);

    parameters.add(filter_mode_ctrl.set("filter mode", 0, 0, 5) );
    parameters.add(cutoff_ctrl.set("filter cutoff", 82, 10, 120));
    parameters.add(reso_ctrl.set("filter reso", 0.0f, 0.0f, 1.0f) );
 
    cutoff_ctrl.enableSmoothing(200.0f);
    
    parameters.add(env_attack_ctrl.set( "env attack", 400, 0, 1200) );
    parameters.add(env_decay_ctrl.set(  "env decay", 400, 5, 1200) );
    parameters.add(env_sustain_ctrl.set("env sustain", 1.0f, 0.0f, 1.0f) );
    parameters.add(env_release_ctrl.set("env release", 900, 0, 2000));    
    parameters.add( env_table_ctrl.set( "env to table", 0.0f, 0.0f, 2.0f) );
    parameters.add( env_filter_ctrl.set("env to filter", 30, 0, 60) );    
    parameters.add( drift.set("drift", 0.0f, 0.0f, 1.0f) );    

    parameters.add(gain.set("gain", -12, -48, 12));
    gain.enableSmoothing(50.f);
    
    return parameters;
}


void np::synth::WaveSynth::Voice::setup( WaveSynth & m ){

    addModuleInput("trig", voiceTrigger);
    addModuleInput("pitch", oscillator.in_pitch());
    addModuleInput("table", oscillator.in_table());
    addModuleInput("cutoff", filter.in_pitch());
    addModuleOutput("signal", voiceAmp);

    oscillator.setTable( m.wavetable );

    // SIGNAL PATH
    oscillator >> filter >> voiceAmp;
    
    // MODULATIONS AND CONTROL
    envelope >> envToTable >> oscillator.in_table();
              m.table_ctrl >> oscillator.in_table();

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
        m.env_table_ctrl  >> envToTable.in_mod();
        
        
    0.2f    >> phazorFree;
    0.05f  >> randomSlew.in_freq();
                                       m.drift >> driftAmt.in_mod();        
    phazorFree.out_trig() >> rnd >> randomSlew >> driftAmt;
                                                  driftAmt >> oscillator.in_pitch();
                                                  driftAmt * 7.0f >> filter.in_pitch();
}

float np::synth::WaveSynth::Voice::meter_env() const{
    return envelope.meter_output();
}

float np::synth::WaveSynth::Voice::meter_pitch() const{
    return oscillator.meter_pitch();
}

pdsp::Patchable& np::synth::WaveSynth::ch( size_t index ){
    pdsp::wrapChannelIndex( index );
    return gain.ch( index );
}

pdsp::Patchable& np::synth::WaveSynth::Voice::in_trig(){
    return in("trig");
}

pdsp::Patchable& np::synth::WaveSynth::Voice::in_pitch(){
    return in("pitch");
}
pdsp::Patchable& np::synth::WaveSynth::Voice::in_table(){
    return in("table");
}

pdsp::Patchable& np::synth::WaveSynth::Voice::in_cutoff(){
    return in("cutoff");
}

ofParameterGroup & np::synth::WaveSynth::label (std::string name ){
    parameters.setName( name );
    return parameters;
}
