
#pragma once

#include "ofMain.h"

#include "ofxPDSP.h"

// SoftClip oversampled soft-clipper with adjustable threshold

namespace np { namespace dynamics {
    
class StereoSoftClip : public pdsp::Patchable {

public:    
    StereoSoftClip() { patch(); }
    StereoSoftClip( const StereoSoftClip & other ) { patch (); }

    pdsp::Patchable & ch( int i );
    
    ofParameterGroup & label( std::string name );
    
    ofParameterGroup parameters;

private:    

    void patch();

    pdsp::ChannelNode chan0;
    pdsp::ChannelNode chan1;
        
    pdsp::SoftClip clip0;
    pdsp::SoftClip clip1;
    
    pdsp::ParameterGain inputGain;
    pdsp::Parameter     clipThreshold;
    pdsp::ParameterGain outputGain;
    
};
    
}}
