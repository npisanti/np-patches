
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

namespace np { namespace modulator { 
    
class ClockedLFO : public pdsp::Patchable {

public:    
    ClockedLFO() { patch(); }
    ClockedLFO( const ClockedLFO & other ) { patch (); }
    
    ofParameterGroup parameters;

    ofParameterGroup & label( std::string name );  
    ofParameterGroup & setup( std::string name, float multiply );
    
    float meter_output() const;

    void setShape( int value );
    void setPulse( int value );
    void setDivide( int value );
    void setAmount( float value );
    
private:    
    void patch();
    void recalculateDivision( int & value );
        
    pdsp::ClockedLFO    lfo;
    ofParameter<int>    pulse;
    ofParameter<int>    divide;
    pdsp::Parameter     phaseControl;
    pdsp::Parameter     modeControl;

    pdsp::Switch        lfoSwitch;
    pdsp::ValueControl  divisionControl;
    
    pdsp::ParameterAmp multiplier;
    
};
    
}} // end namespaces 
