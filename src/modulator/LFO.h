
#pragma once

#include "ofMain.h"
#include "DSP/utility/PitchToFreq.h"
#include "DSP/core/Switch.h"
#include "ofx/ParameterAmp.h"
#include "modules/oscillators/LFO.h"

namespace np { namespace modulator { 
    
class LFO : public pdsp::Patchable {

public:    
    LFO() { patch(); }
    LFO( const LFO & other ) { patch (); }

    ofParameterGroup & label( std::string name );  
    
    float meter_output() const;

    ofParameterGroup parameters;
        pdsp::Parameter     pitchControl;
        pdsp::Parameter     shapeControl;    
        pdsp::ParameterAmp  amountControl;    
        
private:    
    void patch();
        
    pdsp::LFO lfo;
    pdsp::PitchToFreq   p2f;    
    pdsp::Switch        lfoSwitch;

};
    
}} // end namespaces 
