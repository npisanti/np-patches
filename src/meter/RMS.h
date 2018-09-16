
#pragma once

#include "ofxPDSP.h"

namespace np { namespace meter {
	
class RMS : public pdsp::Patchable {

public:
    RMS(){ patch(); }
    RMS(const RMS & other){ patch(); }

    void patch();
    
    ofParameterGroup & label( string name );

    ofParameterGroup parameters;

    float meter() const;

    pdsp::Parameter     attackControl;
    pdsp::Parameter     releaseControl;
    pdsp::ParameterGain gain;
    
private:
    pdsp::RMSDetector       rms;
    pdsp::EnvelopeFollower  envFollower;

    
};

}} // end namespaces
