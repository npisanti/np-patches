
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

    pdsp::Patchable & ch( size_t index );

    ofParameterGroup & label( std::string name );

    void enableScope( ofxPDSPEngine & engine );
    void drawScope( int x, int y, int w, int h );
    void drawMeter( int x, int y, int w, int h );

    void draw( int x, int y, int w, int h );

private:    
    struct Submodule : public pdsp::Patchable{
        Submodule();
        pdsp::PatchNode input;
        pdsp::PatchNode output;        
    };
    
    std::vector<Submodule> submodules;

    void patch();
        
    pdsp::Compressor comp;
    
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
