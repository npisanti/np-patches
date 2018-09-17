
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

namespace np { namespace synth {
    
class QuickSampler : public pdsp::Patchable {
public:
    QuickSampler() { patch(); } 
    QuickSampler( const QuickSampler & other ){ patch(); } 
    
    ofParameterGroup    parameters;
    
    pdsp::Patchable & in_trig();
    pdsp::Patchable & in_pitch();
    
    pdsp::Patchable & ch( size_t index );

    void dBTriggering( bool enable );
    void load(string path);
    
    ofParameterGroup & label( std::string name );
    
private:
    void patch();

    void loadButtonCall( bool & value );
    void sampleChangedCall( std::string & value );
    void loadSample( std::string path );


    pdsp::PatchNode     triggers;
    pdsp::Sampler       sampler0;
    pdsp::Sampler       sampler1;
    pdsp::AHR           env;
    pdsp::AHR           fEnv;
    pdsp::Amp           amp0;
    pdsp::Amp           amp1;
    
    pdsp::Amp           fader0;
    pdsp::Amp           fader1;
    pdsp::DBtoLin       dBtoLin;
    pdsp::Parameter     faderControl;    
    
    pdsp::OnePole       lp0a;
    pdsp::OnePole       lp0b;
    pdsp::OnePole       lp1a;
    pdsp::OnePole       lp1b;
    
    pdsp::TriggeredRandom   drift;
    pdsp::Amp               driftAmt;
    pdsp::Parameter         driftControl;
    
    pdsp::PatchNode     pitchNode;
    pdsp::Parameter     pitchControl;
        
    pdsp::Parameter     startControl;
    pdsp::Parameter     startModControl;
        
    pdsp::Parameter     attackControl;
    pdsp::Parameter     holdControl;
    pdsp::Parameter     releaseControl;
    
    pdsp::Parameter     envFilterControl;
    pdsp::Amp           envFilterAmt;
    pdsp::Parameter     filterControl;
    pdsp::PitchToFreq   p2f;
    
    pdsp::Parameter        envDynControl;

    pdsp::SampleBuffer  sample;    
    ofParameter<bool>   loadButton;
    ofParameter<string> samplePath;
    ofParameter<string> sampleName;
    
};   
    
}} // end namespaces 
