
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

// datatable based polysynth

namespace np { namespace synth {

class DataSynth {

public:
    // class to rapresent each synth voice ------------------------------------
    class Voice : public pdsp::Patchable {
    friend class DataSynth;
    
    public:
        Voice(){}
        Voice(const Voice& other){}
        
        pdsp::Patchable& in_trig();
        pdsp::Patchable& in_pitch();
        
        float meter_mod_env() const;
        float meter_pitch() const;

    private:
        void setup(DataSynth & m);

        pdsp::PatchNode     voiceTrigger;
        
        pdsp::DataOscillator    oscillator;
        pdsp::VAFilter          filter;
        pdsp::Amp               voiceAmp;

        pdsp::OnePole       leakDC;
        pdsp::ADSR          envelope;    
        pdsp::Amp           envToTable;
        pdsp::Amp           envToFilter;  
    }; // end voice class -----------------------------


    // synth public API -------------------------------------------------------

    ofParameterGroup & setup( int numVoice, float spread=0.0f, std::string name="datasynth" );
    
    pdsp::DataTable  datatable;

    pdsp::Patchable& out_L();
    pdsp::Patchable& out_R();

    std::vector<Voice>  voices;
    
    ofParameterGroup    parameters;
    
    ofParameterGroup & label (std::string name );

private: // --------------------------------------------------

    pdsp::ParameterGain gain;

    pdsp::Parameter     cutoff_ctrl;
    pdsp::Parameter     reso_ctrl;
    pdsp::Parameter     filter_mode_ctrl;

    pdsp::Parameter     env_attack_ctrl;
    pdsp::Parameter     env_decay_ctrl;
    pdsp::Parameter     env_sustain_ctrl;
    pdsp::Parameter     env_release_ctrl;
    pdsp::Parameter     env_filter_ctrl;

};

}} // end namespaces
