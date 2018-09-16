
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

namespace np { namespace synth {
    
class MultiSampler : public pdsp::Patchable {
    
public:
    MultiSampler() { patch(); } 
    MultiSampler( const MultiSampler & other ) { patch(); } 
    
    ofParameterGroup    parameters;
    
    pdsp::Patchable & in_trig();
    pdsp::Patchable & in_select();
    pdsp::Patchable & in_pitch();
    
    pdsp::Patchable & out_L();
    pdsp::Patchable & out_R();

    void dBTriggering( bool enable );
    void addSample( pdsp::SampleBuffer & sample);
    
    ofParameterGroup & label( std::string name );
        
private:
    void patch ();
    
    pdsp::PatchNode     selectNode;
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
    
    pdsp::PatchNode         pitchNode;
    pdsp::Parameter         pitchControl;
        
        
    pdsp::Parameter        attackControl;
    pdsp::Parameter        holdControl;
    pdsp::Parameter        releaseControl;
    
    pdsp::Parameter        envFilterControl;
    pdsp::Amp              envFilterAmt;
    pdsp::Parameter        filterControl;
    pdsp::PitchToFreq      p2f;
    
    pdsp::Parameter        envDynControl;
    
};   
    
}} // end namespaces
