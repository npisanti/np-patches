
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

namespace np { namespace effect {
    
class Cruncher : public pdsp::Patchable {

public:    
    Cruncher() { patch(); }
    Cruncher( const Cruncher & other ) { patch (); }
    
    ofParameterGroup parameters;

    pdsp::Patchable& in_decimate_mod();

    void smoothing( float ms );

    float meter_decimate_pitch() const;

    pdsp::Patchable & in_L();
    pdsp::Patchable & in_R();
    pdsp::Patchable & out_L();
    pdsp::Patchable & out_R();
    
    ofParameterGroup & label( std::string name );

private:    
    void patch();

    pdsp::Decimator     decimate0;
    pdsp::Decimator     decimate1;
    pdsp::Bitcruncher   bitcrunch0;
    pdsp::Bitcruncher   bitcrunch1;
    pdsp::PitchToFreq   p2f;

    pdsp::Parameter     cutoffControl;
    pdsp::Parameter     cutoffModControl;
    pdsp::Amp           cutoffModAmt;
    pdsp::Parameter     bitsControl;

};
    
}}
