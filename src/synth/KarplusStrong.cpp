
// KarplusSynth.cpp
// Nicola Pisanti, MIT License, 2018

#include "KarplusStrong.h"


void np::synth::KarplusStrong::Voice::patch() {
    addModuleInput("trig", triggers); 
    addModuleInput("pitch", comb.in_pitch() ); 
    addModuleInput("damping", comb.in_damping() ); 
    addModuleInput("fb", fbBoundaries );
    addModuleInput("pluck_attack", pluckEnv.in_attack() );
    addModuleInput("pluck_decay", pluckEnv.in_release() );

    addModuleInput("filter_type", filter.in_mode() );
    addModuleInput("filter_cutoff", filter.in_cutoff() );
    addModuleInput("filter_reso", filter.in_reso() );
    addModuleInput("filter_mod", filterEnvAmt.in_mod() );
    addModuleInput("mod_decay", filterEnv.in_release() );
    addModuleInput("mod_velo", filterEnv.in_velocity() );
    addModuleInput("drift", driftAmt.in_mod() );
    addModuleOutput("signal", comb ); 

    fbBoundaries.enableBoundaries(0.0f, 1.0f);
    fbBoundaries >> comb.in_feedback();
    pluckEnv.enableDBTriggering(-24.0f, 0.0f);
    //patching
    gateEnv.set(0.0f, 0.0f, 1.0f, 20.0f);
    0.0f >> gateEnv.in_velocity();
    gateEnv >> fbBoundaries;
    
    filterEnv.set(0.0f, 0.0f, 30.f);
    pluckEnv.set(0.0f, 0.0f, 30.f);
        
    triggers >> gateEnv;    
    triggers >>                           pluckEnv >> noiseAmp.in_mod();
                                    noise >> filter>> noiseAmp >> comb;
    triggers >> filterEnv >> filterEnvAmt >> filter.in_cutoff();  
    
    0.2f    >> phazorFree;
    0.05f  >> randomSlew.in_freq();   
    phazorFree.out_trig() >> rnd >> randomSlew >> driftAmt;
    driftAmt >> comb.in_pitch();
                                        
}

void np::synth::KarplusStrong::setup ( int numVoices, float spread ) {
    
    voices.resize( numVoices );
    
    for( size_t i=0; i<voices.size(); ++i ){
        // connect each voice to chorus
        if( spread > 0.0f ){                    
            voices[i] * pdsp::panL( pdsp::spread(i, voices.size(), spread ) ) >> ampL;
            voices[i] * pdsp::panR( pdsp::spread(i, voices.size(), spread ) ) >> ampR;
        }else{        
            voices[i] >> ampL;
            voices[i] >> ampR;            
        }

        fbControl       >> voices[i].in("fb");
        dampingControl  >> voices[i].in("damping");
        pluckAttackControl >> voices[i].in("pluck_attack");
        pluckDecayControl >> voices[i].in("pluck_decay");
        filterModeControl >> voices[i].in("filter_type");
        filterCutoffControl >> voices[i].in("filter_cutoff");
        filterResoControl >> voices[i].in("filter_reso");
        filterModControl >> voices[i].in("filter_mod");
        filterModDecayControl >> voices[i].in("mod_decay");
        filterModVeloControl >> voices[i].in("mod_velo");
        drift >> voices[i].in("drift");
    }
        
    masterFader >> dBtoLin >> ampL.in_mod();
                   dBtoLin >> ampR.in_mod();
    
    parameters.setName("karplus synth");
    parameters.add(masterFader.set( "master dB", -12, -48, 0 ) );
    parameters.add(pluckAttackControl.set( "pluck attack", 0, 0, 500 ) );
    parameters.add(pluckDecayControl.set( "pluck decay", 30, 1, 500 ) );
    parameters.add(fbControl.set( "feedback", 0.99f, 0.75f, 1.0f ) );
    parameters.add(dampingControl.set( "damping", 0.0f, 0.0f, 0.99f ) );
    parameters.add(filterModeControl.set( "filter mode", 0, 0, 5 ) );
    parameters.add(filterCutoffControl.set( "filter cutoff", 136, 20, 136 ) );
    parameters.add(filterResoControl.set( "filter reso", 0.0f, 0.0f, 1.0f ) );
    parameters.add(filterModControl.set( "filter mod", 0, 0, 120 ) );
    parameters.add(filterModDecayControl.set( "mod decay", 30, 1, 500 ) );
    parameters.add(filterModVeloControl.set( "mod dyn", 0.5f, 0.0f, 1.0f ) );
    parameters.add( drift.set("drift", 0.05f, 0.0f, 1.0f) );   

    
    masterFader.enableSmoothing(50.0f);
    fbControl.enableSmoothing(50.0f);
    dampingControl.enableSmoothing(50.0f);
    
}

size_t np::synth::KarplusStrong::size() {
    return voices.size();
}

ofParameterGroup & np::synth::KarplusStrong::label( std::string name ){
    parameters.setName( name );
    return parameters;
}

pdsp::Patchable & np::synth::KarplusStrong::in_trig( int voice ) {
    return voices[voice].in("trig");
}
pdsp::Patchable & np::synth::KarplusStrong::in_pitch( int voice ) {
    return voices[voice].in("pitch");
}
pdsp::Patchable & np::synth::KarplusStrong::out_voice( int voice ) {
    return voices[voice].out("signal");
}
    
pdsp::Patchable & np::synth::KarplusStrong::ch( size_t index ) {
    pdsp::wrapChannelIndex( index );
    switch( index ){
        case 0: return ampL; break;
        case 1: return ampR; break;
    }
    return ampL;
}
