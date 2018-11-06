
#pragma once

#include "ofxPDSP.h"
#include "ofxGui.h"

namespace np {
    
class Analyzer {

public:

    void band( std::string name, int channel ); // add bands before init

    void setup( int id=0, int sampleRate=44100, int frames=512);
    float meter( int band=0 ) const;
    void draw( );
    
    ofxPanel gui;
    
private:
    class AudioAnalyzerBand {

    public:
        AudioAnalyzerBand(){ }
        AudioAnalyzerBand( const AudioAnalyzerBand & other ){ }
        
        void setup( ofxPDSPEngine & engine, string name, int input = 0 );
        
        ofParameterGroup        ui;

        float meter() const;
        
    private:
        void freqLoChanged(float & pitch);
        void freqHiChanged(float & pitch);

        pdsp::AbsoluteValue     absolute;
        pdsp::RMSDetector       rms;
        
        pdsp::EnvelopeFollower  envFollowerPeak;

        pdsp::HighCut           loCut;
        pdsp::LowCut            hiCut;

        ofxPDSPValue     hiFreqControl;

        ofxPDSPValue     peakAttackControl;
        ofxPDSPValue     peakReleaseControl;

        pdsp::Amp        bandGain;
        pdsp::DBtoLin    bandFader;
        ofxPDSPValue     gainControl;            
        
        pdsp::PitchToFreq       p2fLo;
        ofxPDSPValue            loPitchControl;
        ofParameter<float>      loFreqValue;

        pdsp::PitchToFreq       p2fHi;
        ofxPDSPValue            hiPitchControl;
        ofParameter<float>      hiFreqValue;
        
    };
    
    std::vector<AudioAnalyzerBand> bands;
    
    struct BandToInit {
        std::string name;
        int channel;
    };

    std::vector<BandToInit> toInit;
    ofxPDSPEngine engine;
    
};
    
}
