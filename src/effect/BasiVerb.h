
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

namespace np { namespace effect {  
    
class BasiVerb {

public:    
    BasiVerb() { patch(); }
    BasiVerb( const BasiVerb & other ) { patch (); }
    
    ofParameterGroup parameters;

    float meter_lfo() const;

    pdsp::Patchable & in();
    pdsp::Patchable & in_signal();
    pdsp::Patchable & out_L();
    pdsp::Patchable & out_R();

    ofParameterGroup & label( string name );
    
private:    
    void patch();
        
    pdsp::BasiVerb 	    reverb;
    
    pdsp::ParameterGain	gain;
    pdsp::Parameter     timeControl;
    pdsp::Parameter     densityControl;
    pdsp::Parameter     dampingControl;
    pdsp::Parameter     hiCutControl;
    pdsp::Parameter     modFreqControl;
    pdsp::Parameter     modAmountControl;
    
};
    
}}
