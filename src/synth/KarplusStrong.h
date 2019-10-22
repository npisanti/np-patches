

#pragma once

#include "ofxPDSP.h"
#include "ofMain.h"

namespace np { namespace synth {


class KarplusStrong {
    
// ----------------------------- Voice class ----------------------------------
public:

class Voice : public pdsp::Patchable{ 
public:
    Voice() { patch(); }
    Voice(const Voice & other) { patch(); }

    void patch();
    void connectFBGate();
    
    float meter_pluck() const { return pluckEnv.meter_output(); }
    
private:
    pdsp::PatchNode         fbBoundaries;
    pdsp::PatchNode         triggers;
    pdsp::PRNoiseGen        noise;
    pdsp::AHR               pluckEnv;
    pdsp::ADSR              gateEnv;
    pdsp::Amp               noiseAmp;
    
    pdsp::AHR               filterEnv;
    pdsp::Amp               filterEnvAmt;
    pdsp::VAFilter          filter;

    pdsp::CombFilter        comb;        
            
    pdsp::Amp               driftAmt;
    pdsp::LFOPhazor         phazorFree;
    pdsp::TriggeredRandom   rnd;
    pdsp::OnePole           randomSlew;
};    


    
public: // ---------------------- public API ----------------------------------    
    
    void setup ( int numVoices, float spread = 0.0f, bool gatectrl=true );
    
    pdsp::Patchable & in_trig( int voice );
    pdsp::Patchable & in_pitch( int voice );

    pdsp::Patchable & out_voice( int voice );

    pdsp::Patchable & ch( size_t index );
    
    size_t size();

    ofParameterGroup parameters;

    ofParameterGroup & label( std::string name );
    
    std::vector<Voice>      voices;

private: // -------------------------------------------------------------------  

    pdsp::Amp               ampL;
    pdsp::Amp               ampR;
    
    pdsp::Parameter         masterFader;
    pdsp::DBtoLin           dBtoLin;

    pdsp::Parameter     fbControl;
    pdsp::Parameter     dampingControl;
    
    pdsp::Parameter     pluckAttackControl;
    pdsp::Parameter     pluckDecayControl;
    
    pdsp::Parameter     filterModeControl;
    pdsp::Parameter     filterCutoffControl;
    pdsp::Parameter     filterResoControl;
    pdsp::Parameter     filterModControl;
    pdsp::Parameter     filterModDecayControl;
    pdsp::Parameter     filterModVeloControl;
    
    pdsp::Parameter     drift;

};

}} // end namespaces
