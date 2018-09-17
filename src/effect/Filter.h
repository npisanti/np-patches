
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

namespace np { namespace effect {
    
class Filter : public pdsp::Patchable {

public:    
    Filter() { patch(); }
    Filter( const Filter & other ) { patch (); }
    
    ofParameterGroup parameters;

    pdsp::Patchable& in_cutoff();

    pdsp::Patchable & ch( size_t index );
    
    void smoothing( float ms );

    float meter_cutoff() const;

    ofParameterGroup & label( std::string name );
    
private:    
    void patch();
        
    pdsp::VAFilter  filter;
    pdsp::Parameter    cutoffControl;
    pdsp::Parameter    resoControl;
    pdsp::Parameter    modeControl;
    pdsp::Amp       cutoffModAmt;
    
};
    
}} // end namespaces 
