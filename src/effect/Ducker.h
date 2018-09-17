
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

namespace np { namespace effect {
    
class Ducker : public pdsp::Patchable {

public:    
    Ducker() { patch(); }
    Ducker( const Ducker & other ) { patch (); }
    
    ofParameterGroup parameters;

    float meter_env() const;

    pdsp::Patchable & in_trig();

    pdsp::Patchable & ch( size_t index );

    ofParameterGroup & label( std::string name );

private:    
    void patch();
        
    pdsp::Ducker ducker;
    
    pdsp::Parameter    attackControl;
    pdsp::Parameter    holdControl;
    pdsp::Parameter    releaseControl;
    pdsp::Parameter    duckingControl;
    pdsp::Parameter    veloControl;
    
};
    
}}
