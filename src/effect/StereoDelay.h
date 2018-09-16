
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

namespace np { namespace effect {

class StereoDelay : public pdsp::Patchable {

public:
    StereoDelay() { patch(); };
    StereoDelay(const StereoDelay & other) { patch(); };
    
    ofParameterGroup parameters;

    pdsp::ParameterAmp  lDelayTimeControl;
    pdsp::ParameterAmp  rDelayTimeControl;
    pdsp::Parameter     lFeedbackControl;
    pdsp::Parameter     rFeedbackControl;

    pdsp::Patchable & in_L();
    pdsp::Patchable & in_R();
    pdsp::Patchable & out_L();
    pdsp::Patchable & out_R();

    ofParameterGroup & label( string name );

private:
    void patch();

    pdsp::LFOPhazor         phazor;
    pdsp::CheapTri          LFO;
    pdsp::ParameterAmp      modAmt;
    pdsp::Parameter         speed;

    pdsp::Delay ldelay;
    pdsp::Delay rdelay;
    
    pdsp::OneBarTimeMs time;

    pdsp::Parameter dampingControl;
    
    pdsp::ParameterGain input;
    pdsp::ParameterGain output;
    
};

}} // end namespaces
