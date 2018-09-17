
// QuickSampler.cpp
// Nicola Pisanti, MIT License, 2018

#include "QuickSampler.h"

void np::synth::QuickSampler::patch(){
   
    addModuleInput("trig", triggers );
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
    
    startControl >> sampler0.in_start();
    startControl >> sampler1.in_start();
    startModControl >> sampler0.in_start_mod();
    startModControl >> sampler1.in_start_mod();
    
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
    
    sampler0.addSample( &sample, 0 );
    sampler1.addSample( &sample, 1 );

    attackControl  >> env.in_attack();
    holdControl    >> env.in_hold();
    releaseControl >> env.in_release();

    attackControl  >> fEnv.in_attack();
    holdControl    >> fEnv.in_hold();
    releaseControl >> fEnv.in_release();

    parameters.setName( "quicksampler");
    parameters.add( faderControl.set("volume", 0, -48, 24) );
    parameters.add( loadButton.set( "load", false ));
    parameters.add( sampleName.set("sample", "no sample"));
    parameters.add( samplePath.set("path", "no path"));
    parameters.add( startControl.set("start", 0.0f, 0.0f, 1.0f));
    parameters.add( startModControl.set("dyn start mod", 0.0f, 0.0f, 1.0f));
    parameters.add( pitchControl.set("pitch", 0, -24, 24));
    parameters.add( driftControl.set("pitch drift", 0.0f, 0.0f, 0.5f));
    parameters.add( attackControl.set("attack time", 0, 0, 600));
    parameters.add( holdControl.set("hold time", 0, 0, 600));
    parameters.add( releaseControl.set("release time", 0, 0, 600));
    parameters.add( filterControl.set("filter cutoff", 136, 20, 136));
    parameters.add( envFilterControl.set("env to cutoff", 0, 0, 120));
    parameters.add( envDynControl.set("env dynamics", 1.0f, 0.0f, 1.0f));
    
    loadButton.addListener(this, &QuickSampler::loadButtonCall );
    samplePath.addListener(this, &QuickSampler::sampleChangedCall );
    
}

ofParameterGroup & np::synth::QuickSampler::label( std::string name ){
    parameters.setName( name );
    return parameters;
}

void np::synth::QuickSampler::loadButtonCall( bool & value ) {
    if(value){

        float fvalue = faderControl.get();
        faderControl.setv(0.0f);
        
        //Open the Open File Dialog
        ofFileDialogResult openFileResult= ofSystemLoadDialog("select an audio sample"); 
        
        //Check if the user opened a file
        if (openFileResult.bSuccess){
            
            std::string path = openFileResult.getPath();
            
            samplePath = path;
        
            ofLogVerbose("file loaded");
            
        }else {
            ofLogVerbose("User hit cancel");
        }


        loadButton = false;
        
        faderControl.setv(fvalue);
    }
}

void np::synth::QuickSampler::sampleChangedCall( std::string & value ) {
    
    ofLogVerbose( "loading" + value );
    loadSample( samplePath );    

    auto v = ofSplitString(samplePath, "/" );
    sampleName = v[v.size()-1];
    
}
    
void np::synth::QuickSampler::loadSample( std::string path ) {
    
    sample.load( path );
    
    int sample_length_ms = ((double) sample.length * 1000.0) / sample.fileSampleRate;
    
    holdControl.set("hold time", sample_length_ms, 0, sample_length_ms*2);
    releaseControl.set("release time", 50, 0, sample_length_ms*2);
    
    if( sample.channels==2 ){
        sampler1.setSample( &sample, 0, 1 ); // stereo
    }else{
        sampler1.setSample( &sample, 0, 0 ); // mono
    }
}

void np::synth::QuickSampler::load( std::string path ) {
    samplePath = path;
}

void np::synth::QuickSampler::dBTriggering( bool enable ) {
    if(enable){
        env.enableDBTriggering(-24.0f, 0.0f);
    }else{
        env.disableDBTriggering();
    }
}

pdsp::Patchable & np::synth::QuickSampler::in_trig() {
    return in("trig");
}
pdsp::Patchable & np::synth::QuickSampler::in_pitch() {
    return in("pitch");
}

pdsp::Patchable & np::synth::QuickSampler::ch( size_t index ) {
    pdsp::wrapChannelIndex( index );
    switch( index ){
        case 0: return out("L"); break;
        case 1: return out("R"); break;
    }
    return out("L"); 
}
