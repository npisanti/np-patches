
#pragma once

#include "ofxPDSP.h"
#include "ofxGui.h"

namespace np{
    
class SimpleAnalyzer {

public:
    void setup( int sampleRate=44100, int frames=512, int id=0 );
    float meter() const;
    void draw( );
    
    ofxPanel gui;
    
private:
    ofxPDSPEngine engine;
    
    class PeakMeter : public pdsp::Patchable{

    public:
        PeakMeter();
        PeakMeter( const PeakMeter & other ){ }
        
        void activate( ofxPDSPEngine & engine );
        
        ofParameterGroup        ui;
        ofxPDSPValue                peakAttackControl;
        ofxPDSPValue                peakReleaseControl;
        ofxPDSPValue                gainControl;     

        float meter_output() const;
        
    private:

        pdsp::AbsoluteValue     absolute;
        pdsp::EnvelopeFollower  envFollowerPeak;
        ofxPDSPMonoFader        gain;
        static int number;
    };

    PeakMeter pm;    
};
    
}
