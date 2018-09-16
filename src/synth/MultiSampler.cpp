
// MultiSampler.cpp
// Nicola Pisanti, MIT License, 2016

#include "MultiSampler.h"

void np::synth::MultiSampler::patch (){
   
    addModuleInput("trig", triggers );
    addModuleInput("select", selectNode );
    addModuleInput("pitch", pitchNode );
    addModuleOutput("L", fader0 );
    addModuleOutput("R", fader1 );
    
    env.enableDBTriggering(-24.0f, 0.0f);
    
    drift        >> driftAmt;
    driftControl >> driftAmt.in_mod();
                    driftAmt >> sampler0.in_pitch();
                    driftAmt >> sampler1.in_pitch();
    
                    pitchNode >> sampler0.in_pitch();
                    pitchNode >> sampler1.in_pitch();
    
                 pitchControl >> sampler0.in_pitch();
                 pitchControl >> sampler1.in_pitch();
    
    triggers >> drift.in_trig();
    triggers >> sampler0 >> amp0;
    triggers >> env      >> amp0.in_mod();
    triggers >> sampler1 >> amp1;
                env      >> amp1.in_mod();
    
                sampler0 >> amp0 >> lp0a >> lp0b >> fader0;
                sampler1 >> amp1 >> lp1a >> lp1b >> fader1;
                
    faderControl >> dBtoLin  >> fader0.in_mod();
                    dBtoLin  >> fader1.in_mod();
    
    filterControl >> p2f >> lp0a.in_freq();
                     p2f >> lp0b.in_freq();
                     p2f >> lp1a.in_freq();
                     p2f >> lp1b.in_freq();
    envFilterAmt >>  p2f;
    
    triggers >>fEnv >> envFilterAmt;
    envFilterControl >> envFilterAmt.in_mod();
    
    p2f.enableBoundaries(20, 136);
    
    envDynControl >> env.in_velocity();
    envDynControl >> fEnv.in_velocity();

    attackControl  >> env.in_attack();
    holdControl    >> env.in_hold();
    releaseControl >> env.in_release();

    attackControl  >> fEnv.in_attack();
    holdControl    >> fEnv.in_hold();
    releaseControl >> fEnv.in_release();

    selectNode >> sampler0.in_select();
    selectNode >> sampler1.in_select();

    parameters.setName( "multisampler");
    parameters.add( faderControl.set("volume", 0, -48, 24) );
    parameters.add( pitchControl.set("pitch", 0, -24, 24));
    parameters.add( driftControl.set("pitch drift", 0.0f, 0.0f, 0.5f));
    parameters.add( attackControl.set("attack time", 0, 0, 1000));
    parameters.add( holdControl.set("hold time", 4000, 0, 4000));
    parameters.add( releaseControl.set("release time", 4000, 0, 4000));
    parameters.add( filterControl.set("filter cutoff", 136, 20, 136));
    parameters.add( envFilterControl.set("env to cutoff", 0, 0, 120));
    parameters.add( envDynControl.set("env dynamics", 1.0f, 0.0f, 1.0f));

}

ofParameterGroup & np::synth::MultiSampler::label( std::string name ){
    parameters.setName( name );
    return parameters;
}

void np::synth::MultiSampler::addSample( pdsp::SampleBuffer & sample) {

    if(sample.channels > 1 ){
        sampler0.addSample( &sample, 0 );
        sampler1.addSample( &sample, 1 );
    }else{
        sampler0.addSample( &sample, 0 );
        sampler1.addSample( &sample, 0 );
    }
}


void np::synth::MultiSampler::dBTriggering( bool enable ) {
    if(enable){
        env.enableDBTriggering(-24.0f, 0.0f);
    }else{
        env.disableDBTriggering();
    }
}


pdsp::Patchable & np::synth::MultiSampler::in_trig() {
    return in("trig");
}

pdsp::Patchable & np::synth::MultiSampler::in_select() {
    return in("select");
}

pdsp::Patchable & np::synth::MultiSampler::in_pitch() {
    return in("pitch");
}


pdsp::Patchable & np::synth::MultiSampler::out_L() {
    return out("L");
}
pdsp::Patchable & np::synth::MultiSampler::out_R() {
    return out("R");
}

