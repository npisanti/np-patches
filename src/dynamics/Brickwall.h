
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

    pdsp::Patchable & ch( size_t index );

    ofParameterGroup & label( std::string name );

    void enableScope( ofxPDSPEngine & engine );
    void drawScope( int x, int y, int w, int h );
    void drawMeter( int x, int y, int w, int h );

    void draw( int x, int y, int w, int h );

    pdsp::ParameterGain makeup;
    pdsp::Parameter    attackControl;
    pdsp::Parameter    releaseControl;
    pdsp::Parameter    thresholdControl;
    pdsp::Parameter    kneeControl;
    pdsp::Parameter    clipThreshold;

private:    
    struct Submodule : public pdsp::Patchable{
        Submodule();
        pdsp::PatchNode input;
        pdsp::SoftClip clip;
        pdsp::IIRUpSampler2x upsampler;
        pdsp::IIRDownSampler2x downsampler; 
    };

    void patch();
        
    pdsp::Compressor comp;
    
    Submodule submodule0;
    Submodule submodule1;
    
    
    float threshold(){ return thresholdControl.getOFParameterInt(); }
    
    pdsp::Scope                scope;
};
    
}}
