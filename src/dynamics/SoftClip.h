
#pragma once

#include "ofMain.h"

#include "ofxPDSP.h"

// SoftClip oversampled soft-clipper with adjustable threshold

namespace np { namespace dynamics {
    
class SoftClip : public pdsp::Patchable {

public:    
    SoftClip() { patch(); }
    SoftClip( const SoftClip & other ) { patch (); }
    
    ofParameterGroup & label( std::string name );
    
    ofParameterGroup parameters;

private:    
    void patch();
        
    pdsp::SoftClip clip;
    pdsp::IIRUpSampler2x upsampler;
    pdsp::IIRDownSampler2x downsampler; 
    
    pdsp::ParameterGain inputGain;
    pdsp::Parameter     clipThreshold;
    pdsp::ParameterGain outputGain;
    
};
    
}}
