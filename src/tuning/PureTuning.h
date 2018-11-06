
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

// pure tuning using integer ratios

namespace np { namespace tuning {

class PureTuning {

private:

    // private internal classes ------------------------------------------------------------------
    struct RatioUI {

        RatioUI(){
            numerator.addListener(this, &RatioUI::ratioChange);
            octave.addListener(this, &RatioUI::ratioChange);
            denominator.addListener(this, &RatioUI::ratioChange);
        }

        void ratioChange( int & value ) {
            double ratio = (double) (numerator*octave) / (double) denominator;
            double bp = (double) (*basePitch);
            double freq = pdsp::p2f(bp);
            freq *= ratio;
            pitch.set( pdsp::f2p(freq) );
        }
        
        void setBasePitch( ofParameter<int> & bp) {
            basePitch = &bp;
        }
        
        ofParameter<int>*   basePitch;
        ofParameter<int>    numerator;
        ofParameter<int>    denominator;
        ofParameter<int>    octave;
        pdsp::ValueControl  pitch;
        
    };

public: // ------------------- PUBLIC API ------------------------------------------------------------

    PureTuning(){}
    PureTuning( const PureTuning & other ) { }
    
    ofParameterGroup & setup( int oscsNum, std::string name="integer ratio tuning" );
    ofParameterGroup & label( std::string name );  
    
    void smoothing( float ms );

    pdsp::Patchable & out_pitch(int voice);
    float pitch( int voice )const;

    ofParameterGroup parameters;    
    
private: // -------------------------------------------------------------------------------------------

    void updateAll( int & value );
    
    ofParameter<int>    masterPitchControl;
    vector<RatioUI>     ratios;
    
};

}} // end namespaces
