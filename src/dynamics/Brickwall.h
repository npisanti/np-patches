
#pragma once

#include "ofMain.h"

#include "ofxPDSP.h"

// Brickwall limiter followed by a oversampled soft-clipper with adjustable treshold

namespace np { namespace dynamics {
    
class Brickwall : public pdsp::Patchable {

public:    
    Brickwall() { patch(); }
    Brickwall( const Brickwall & other ) { patch (); }
    
    ofParameterGroup parameters;

    float meter_GR() const;

    pdsp::Patchable & in_L();
    pdsp::Patchable & in_R();
    pdsp::Patchable & out_L();
    pdsp::Patchable & out_R();

    ofParameterGroup & label( std::string name );

    void enableScope( ofxPDSPEngine & engine );
    void drawScope( int x, int y, int w, int h );
    void drawMeter( int x, int y, int w, int h );

    void draw( int x, int y, int w, int h );

private:    
    void patch();
        
    pdsp::Compressor comp;
    
    pdsp::SoftClip clip0;
    pdsp::SoftClip clip1;
    pdsp::IIRUpSampler2x upsampler0;
    pdsp::IIRUpSampler2x upsampler1;
    pdsp::IIRDownSampler2x downsampler0;
    pdsp::IIRDownSampler2x downsampler1;
    
    pdsp::PatchNode input0;
    
    pdsp::ParameterGain makeup;
    
    pdsp::Parameter    attackControl;
    pdsp::Parameter    releaseControl;
    pdsp::Parameter    thresholdControl;
    pdsp::Parameter    kneeControl;
    pdsp::Parameter    clipThreshold;
    
    float threshold(){ return thresholdControl.getOFParameterInt(); }
    
    pdsp::Scope                scope;
};
    
}}
