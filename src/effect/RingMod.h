
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

namespace np { namespace effect {
    
class RingMod : public pdsp::Patchable {

public:    
    RingMod() { patch(); }
    RingMod( const RingMod & other ) { patch (); }
    
    ofParameterGroup parameters;

    pdsp::Patchable & in_pitch();

    pdsp::Patchable & ch( size_t index );
    
    void smoothing( float ms );

    ofParameterGroup & label( std::string name );
    
private:    
    struct Submodule : public pdsp::Patchable {
        Submodule();
        pdsp::PatchNode         input;
        pdsp::Amp               rm;
        pdsp::LinearCrossfader  mix;
    };

    void patch();
    
    std::vector<Submodule> channels;
        
    pdsp::FMOperator        sine;
    pdsp::BipolarToUnipolar b2u;
    
    pdsp::Parameter            pitchControl;
    pdsp::Parameter            wetControl;      

    
};
    
}} // end namespaces 
