
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

namespace np { namespace effect {  
    
class BasiVerb : public pdsp::Patchable {

public:    
    BasiVerb() { patch(); }
    BasiVerb( const BasiVerb & other ) { patch (); }
    
    float meter_lfo() const;

    pdsp::Patchable & in_signal();

    pdsp::Patchable & ch( size_t index );

    ofParameterGroup & label( string name );

    ofParameterGroup parameters;
        pdsp::ParameterGain	gain;
        pdsp::Parameter     timeControl;
        pdsp::Parameter     densityControl;
        pdsp::Parameter     dampingControl;
        pdsp::Parameter     hiCutControl;
        pdsp::Parameter     modFreqControl;
        pdsp::Parameter     modAmountControl;
        pdsp::Parameter     lowCutControl;
    
private:    
    void patch();
        
    pdsp::BasiVerb 	    reverb;
    pdsp::LowCut        lowcut;
    
};
    
}}
