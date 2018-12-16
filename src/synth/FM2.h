
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

// wavetable based polysynth

namespace np { namespace synth {

class FM2 {

public:
    // class to rapresent each synth voice ------------
    class Voice : public pdsp::Patchable {
        friend class FM2;
    
    public:
        Voice(){}
        Voice(const Voice& other){}

        pdsp::Patchable& in_trig();
        pdsp::Patchable& in_pitch();
        pdsp::Patchable& in_fm();
        pdsp::Patchable& in_self();
        
        float meter_mod_env() const;
        float meter_pitch() const;

    private:
        void setup(FM2 & m, int i);

        pdsp::PatchNode     voiceTrigger;
        pdsp::PatchNode     pitchNode;
        
        pdsp::FMOperator    carrier;
        pdsp::FMOperator    modulator;

        pdsp::Amp           fmAmp;
        pdsp::Amp           voiceAmp;

        pdsp::ADSR          envelope;    
                
        pdsp::Amp               driftAmt;
        pdsp::LFOPhazor         phazorFree;
        pdsp::TriggeredRandom   rnd;
        pdsp::OnePole           randomSlew;
    }; // end voice class -----------------------------


    // synth public API --------------------------------------
    ofParameterGroup & setup( int numVoice, float spread=0.0f, std::string name="fm2" );

    pdsp::Patchable& ch( size_t index );

    vector<Voice>       voices;
    ofParameterGroup    parameters;

    ofParameterGroup & label (std::string name );

    // ------- parameters ------------------------------------

    pdsp::ParameterGain gain;

    pdsp::ParameterAmp  fm_mod;
    pdsp::ParameterAmp  self_mod;

    pdsp::Parameter     fm_ctrl;
    pdsp::Parameter     self_ctrl;
    pdsp::Parameter     ratio_ctrl;

    pdsp::Parameter     env_attack_ctrl;
    pdsp::Parameter     env_decay_ctrl;
    pdsp::Parameter     env_sustain_ctrl;
    pdsp::Parameter     env_release_ctrl;

    pdsp::Parameter     drift;
};

}} // end namspaces 
