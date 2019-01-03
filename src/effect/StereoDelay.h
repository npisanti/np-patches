
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
    pdsp::ParameterAmp      modAmt;
    pdsp::Parameter         speed;
    pdsp::ParameterGain inputFader;
    pdsp::ParameterGain outputFader;

    pdsp::Patchable & ch( size_t index );

    ofParameterGroup & label( string name );

private:
    std::vector<pdsp::ChannelNode> channels;

    void patch();

    pdsp::LFOPhazor         phazor;
    pdsp::CheapTri          LFO;

    pdsp::Delay delay0;
    pdsp::Delay delay1;
    
    pdsp::OneBarTimeMs time;

    pdsp::Parameter dampingControl;
    

    
};

}} // end namespaces
