
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

namespace np { namespace effect { 
    
class Chorus {

public:    
    Chorus() { patch(); }
    Chorus( const Chorus & other ) { patch (); }
    
    ofParameterGroup parameters;

    float meter_lfo() const;

    pdsp::Patchable & ch( size_t index );
    
    ofParameterGroup & label( std::string name );

private:    
    void patch();
        
    pdsp::DimensionChorus chorus;
    pdsp::Parameter    speedControl;
    pdsp::Parameter    depthControl;

};
    
}} // end namespaces 
