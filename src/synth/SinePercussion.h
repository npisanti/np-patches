
#pragma once

#include "ofParameterGroup.h"
#include "pdspCore.h"
#include "envelopes/AHR.h"
#include "ofx/Parameter.h"
#include "ofx/ParameterAmp.h"
#include "utility/DBtoLin.h"
#include "modules/oscillators/FMOperator.h"

namespace np { namespace synth {

class SinePercussion : public pdsp::Patchable{ 
public:
    SinePercussion(){ patch(); }
    SinePercussion(const SinePercussion & other){ patch(); }

    void longerEnvelopes();

    void enableDB( float minValue=-18.0f );
    void disableDB( );
    
    float meter() const ;

    pdsp::Patchable & in_trig();
    pdsp::Patchable & in_pitch();

    ofParameterGroup parameters;

    ofParameterGroup & label( string name );

private:
    void patch();

    pdsp::PatchNode     triggers;
    pdsp::FMOperator    osc;
    pdsp::AHR           aEnv;
    pdsp::AHR           pEnv;
    pdsp::Amp           amp;
    pdsp::Amp           pModAmt;

    pdsp::Parameter        fbModControl;
    pdsp::Parameter        fbControl;
    pdsp::Amp              fbModAmt;
    
    pdsp::Parameter        pitchControl;    
    pdsp::Parameter        veloToEnv;
    pdsp::Parameter        pEnvAmtControl;
    pdsp::Parameter        pEnvAttackControl;
    pdsp::Parameter        pEnvReleaseControl;

    pdsp::Parameter        envAttackControl;
    pdsp::Parameter        envHoldControl;
    pdsp::Parameter        envReleaseControl;

    pdsp::Amp              gain;
    pdsp::DBtoLin          dBtoLin;
    pdsp::Parameter        gainControl;  

};

}} // end namespaces
