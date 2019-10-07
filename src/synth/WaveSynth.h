
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

// wavetable based polysynth

namespace np { namespace synth {

class WaveSynth {

public:
    // class to rapresent each synth voice ------------
    class Voice : public pdsp::Patchable {
        friend class WaveSynth;
    
    public:
        Voice(){}
        Voice(const Voice& other){}

        pdsp::Patchable& in_trig();
        pdsp::Patchable& in_pitch();
        pdsp::Patchable& in_table();
        pdsp::Patchable& in_cutoff();
        
        float meter_env() const;
        float meter_pitch() const;

    private:
        void setup(WaveSynth & m);

        pdsp::PatchNode     voiceTrigger;
        
        pdsp::TableOscillator   oscillator;
        pdsp::VAFilter          filter;
        pdsp::Amp               voiceAmp;


        pdsp::ADSR          envelope;    
        pdsp::Amp           envToTable;
        pdsp::Amp           envToFilter;  
                
        pdsp::Amp               driftAmt;
        pdsp::LFOPhazor         phazorFree;
        pdsp::TriggeredRandom   rnd;
        pdsp::OnePole           randomSlew;
    }; // end voice class -----------------------------


    // synth public API --------------------------------------
    ofParameterGroup & setup( int numVoice, float spread=0.0f, std::string name="wavesynth" );

    pdsp::Patchable& ch( size_t index );

    vector<Voice>       voices;
    ofParameterGroup    parameters;

    ofParameterGroup & label (std::string name );

    pdsp::WaveTable  wavetable;

private: // --------------------------------------------------

    pdsp::ParameterGain gain;

    pdsp::Parameter     table_ctrl;

    pdsp::Parameter     cutoff_ctrl;
    pdsp::Parameter     reso_ctrl;
    pdsp::Parameter     filter_mode_ctrl;

    pdsp::Parameter     env_attack_ctrl;
    pdsp::Parameter     env_decay_ctrl;
    pdsp::Parameter     env_sustain_ctrl;
    pdsp::Parameter     env_release_ctrl;
    pdsp::Parameter     env_filter_ctrl;
    pdsp::Parameter     env_table_ctrl;
    pdsp::Parameter     drift;
};

}} // end namspaces 
