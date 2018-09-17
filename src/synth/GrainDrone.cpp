
// GrainDrone.cpp
// Nicola Pisanti, MIT License, 2018

#include "GrainDrone.h"

void np::synth::GrainDrone::resize(int w, int h) {

    uiWidth = w;
    uiHeight = h;
    grainW = 10;
    grainH = (h*6)/10;

}
    
void np::synth::GrainDrone::addSample( util::SampleControl & sample ) {
    cloud.addSample( &sample.sample );
    samples.push_back( &sample );
    selectSampleControl.set("select sample", 0, 0, samples.size()-1 );
}

    
void np::synth::GrainDrone::sample( int select ) {
    selectSampleControl.getOFParameterInt() = select;
}
    
void np::synth::GrainDrone::pitch( int newpitch ) {
    pitchControl.getOFParameterInt() = newpitch;
}

ofParameterGroup & np::synth::GrainDrone::setup( int w, int h, std::string name, bool guicontrol ){

    this->showLabel = true;
    resize( w, h);
    
    //--------GRAINCLOUD-----------------------
    grainVoices = cloud.getVoicesNum();
    
    cloud.setWindowType(pdsp::Tukey); // select the shape of the grain envelope
            // available windows: Rectangular, Triangular, Hann, Hamming, Blackman, BlackmanHarris, SineWindow, Welch
            
    selectSampleControl >> cloud.in_select();
    pitchControl >> cloud.in_pitch();
    positionControl >> cloud.in_position();
    positionJitterControl >> cloud.in_position_jitter();  
    lengthControl >> cloud.in_length();
    densityControl >> cloud.in_density();
    distanceJitterControl >> cloud.in_distance_jitter();
    pitchJitterControl >> cloud.in_pitch_jitter();
    // 0.0f >> cloud.in_pitch();  

    fader >> dBtoLin >> voiceAmpL.in_mod(); 
                   dBtoLin >> voiceAmpR.in_mod(); 
    
    // SET UP RESONATORS----------------------------------------
    resonators.resize(8);
    resonators_sends.resize(8);
    resonators_control.resize(4);
    sendnode.resize(4);
    
    cloud.ch(0) >> dryL >> voiceAmpL;
    cloud.ch(1) >> dryR >> voiceAmpR;
    
    drynode >> dryL.in_mod();
    drynode >> dryR.in_mod();
    
    if(guicontrol) dry_control >> drynode;
    
    for(int i=0; i<8; ++i){
        fbInput >> resonators[i].in_feedback();
        resonatorsFBControl >> resonators[i].in_feedback();
        resonatorsDampControl >> resonators[i].in_damping();
        if(i%2==0){
            cloud.ch(0) >> resonators_sends[i] >> resonators[i] >> voiceAmpL;
        }else{
            cloud.ch(1) >> resonators_sends[i] >> resonators[i] >> voiceAmpR;
        }
    }
    
    resonatorsPitchControls.resize(4);
    
    pitchModSpeed >> lfoPhazor.in_freq() >> lfo >> pitchModAmt;
                                pitchModControl >> pitchModAmt.in_mod();
    
    
    
    for(int i=0; i<4; ++i){
        if(guicontrol) resonators_control[i] >> sendnode[i];

        sendnode[i] >> resonators_sends[i*2].in_mod();
        sendnode[i] >> resonators_sends[i*2 + 1].in_mod();

        resonatorsLRSpreadControl         >> resonators[i*2].in_pitch();
        resonatorsPitchMaster             >> resonators[i*2].in_pitch();
        pitchModAmt                       >> resonators[i*2].in_pitch();
        resonatorsPitchControls[i]        >> resonators[i*2].in_pitch();
        resonatorsLRSpreadControl * -1.0f >> resonators[i*2 + 1].in_pitch();
        resonatorsPitchMaster             >> resonators[i*2 + 1].in_pitch();
        pitchModAmt               * -1.0f >> resonators[i*2 + 1].in_pitch();
        resonatorsPitchControls[i]        >> resonators[i*2 + 1].in_pitch();
    }
    // FINISHED WITH RESONATORS ------------------------------    


    parameters.setName( name );
    
    if(guicontrol){
        parameters.add( dry_control.set("dry control", 0.0f, 0.0f, 1.0f) );
        for(int i=0; i<4; ++i){
            parameters.add( resonators_control[i].set( "r " + ofToString(i+1) + " control", 0.25f, 0.0f, 1.0f));
        }        
    }

    parameters.add( fader.set( "gain", 0, -48, 30) );
    parameters.add( selectSampleControl.set("select sample", 0, 0, samples.size()-1) );
    parameters.add( pitchControl.set("pitch", 0, -36, 36));
    parameters.add( pitchControl.set("p fine", 0.0f, -0.5f, 0.5f));
    parameters.add( pitchJitterControl.set("p jitter", 0.05f, 0.0f, 1.0f));
    parameters.add( positionControl.set("position", 0.5f, 0.0f, 1.0f));
    parameters.add( positionJitterControl.set("pos jitter", 0.02f, 0.0f, 0.5f));
    parameters.add( lengthControl.set("length (ms)", 250, 5, 700) );
    parameters.add( densityControl.set("density", 0.9f, 0.0f, 1.0f));
    parameters.add( distanceJitterControl.set("distance jit", 20, 0, 300));
    
    parameters.add( resonatorsPitchMaster.set("resonators pitch", 48, 0, 127));
    parameters.add( resonatorsLRSpreadControl.set("reson. LR spread", 0.05f, 0.0f, 0.5f));
    for(int i=0; i<4; ++i){
        int p;
        switch(i){
            case 0: p = -12; break;
            case 1: p = -5; break;
            case 2: p = 0; break;
            case 3: p = 7; break;
        }
        
        string name = "r";
        name += ofToString(i+1);
        name += " offset";
        parameters.add( resonatorsPitchControls[i].set( name, p, -24, 24));
        name += " fine";
        parameters.add( resonatorsPitchControls[i].set( name, 0.0f, -0.5f, 0.5f));
    }
    
    parameters.add( resonatorsFBControl.set("resonators fb", 0.5f, 0.0f, 1.0f));
    parameters.add( resonatorsDampControl.set("resonators damp", 0.5f, 0.0f, 1.0f));
    parameters.add( pitchModControl.set("lfo mod", 0.0f, 0.0f, 2.0f));
    parameters.add( pitchModSpeed.set("lfo speed", 3.5f, 0.25f, 10.25f) );
    fader.enableSmoothing(50.0f);

    return parameters;
}


ofParameterGroup & np::synth::GrainDrone::label( std::string name ){
    parameters.setName( name );
    return parameters;
}

void np::synth::GrainDrone::draw( int x, int y ){
    
    int sampleIndex = selectSampleControl.get();
    if( !samples.empty() && !samples[sampleIndex]->loading() ){
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(x, y);
        
            ofSetRectMode(OF_RECTMODE_CORNER);
            ofNoFill();

            ofDrawRectangle( 0, 0, uiWidth, uiHeight);
            
            ofSetRectMode(OF_RECTMODE_CORNER);        
            samples[sampleIndex]->graphics.draw(0, 0, uiWidth, uiHeight);
            if(showLabel) ofDrawBitmapString( samples[sampleIndex]->sample.filePath, 5, 14);
        
            ofSetRectMode(OF_RECTMODE_CENTER);

            int grainsY = uiHeight/2;
            for (int i=0; i<grainVoices; ++i){
                float xpos = uiWidth * cloud.meter_position(i);
                float dimensionX = cloud.meter_env(i)*10;
                float dimensionY = cloud.meter_env(i)*grainH;
                ofDrawRectangle(xpos, grainsY, dimensionX, dimensionY);
            }       

        ofPopMatrix();
        ofPopStyle();
    }
    
}

pdsp::Patchable& np::synth::GrainDrone::in_dry_control(){
    return drynode;
}

pdsp::Patchable& np::synth::GrainDrone::in_resonator_control( int i ){
    if(i>=0 && i<4){
        return sendnode[i];
    }else{
        ofLogError()<<"[graindrone] wrong index for resonator, min 0, max 3";
        return drynode;
    }
}

pdsp::Patchable& np::synth::GrainDrone::ch( size_t index ) {
    pdsp::wrapChannelIndex( index );
    switch( index ){
        case 0: return voiceAmpL; break;
        case 1: return voiceAmpR; break;
    }
    return voiceAmpL;
}

pdsp::Patchable& np::synth::GrainDrone::in_feedback(){
    return fbInput;
}

pdsp::Patchable& np::synth::GrainDrone::in_position(){
    return cloud.in_position();
}
    
void np::synth::GrainDrone::smoothing( float ms ) {
    for(size_t i=0; i<4; ++i) resonators_control[i].enableSmoothing( ms );
    dry_control.enableSmoothing( ms );
    fader.enableSmoothing( ms );
}
