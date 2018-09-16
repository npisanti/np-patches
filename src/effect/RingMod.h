
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

namespace np { namespace effect {
    
class RingMod : public pdsp::Patchable {

public:    
    RingMod() { patch(); }
    RingMod( const RingMod & other ) { patch (); }
    
    ofParameterGroup parameters;

    pdsp::Patchable& in_pitch();

    pdsp::Patchable & in_L();
    pdsp::Patchable & in_R();
    pdsp::Patchable & out_L();
    pdsp::Patchable & out_R();
    
    void smoothing( float ms );

    ofParameterGroup & label( std::string name );
    
private:    
    void patch();
        
    pdsp::FMOperator        sine;
    
    pdsp::Parameter            pitchControl;
    pdsp::Parameter            wetControl;      
    
    pdsp::Amp               rm0;
    pdsp::Amp               rm1;
    pdsp::LinearCrossfader  wet0;
    pdsp::LinearCrossfader  wet1;
    
};
    
}} // end namespaces 
