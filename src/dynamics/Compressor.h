
#pragma once

#include "ofMain.h"

#include "ofxPDSP.h"


namespace np{ namespace dynamics {
    
class Compressor : public pdsp::Patchable {

public:    
    Compressor() { patch(); }
    Compressor( const Compressor & other ) { patch (); }
    
    ofParameterGroup parameters;

    void peak(){ comp.peak(); }
    void RMS( float window ){ comp.RMS(window); }

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
    
    pdsp::PatchNode input0;
    
    pdsp::ParameterGain makeup;
    
    pdsp::Parameter    attackControl;
    pdsp::Parameter    releaseControl;
    pdsp::Parameter    thresholdControl;
    pdsp::Parameter    kneeControl;
    pdsp::Parameter    ratioControl;
    
    float threshold(){ return thresholdControl.getOFParameterInt(); }
    
    pdsp::Scope                scope;
};
    
}}
